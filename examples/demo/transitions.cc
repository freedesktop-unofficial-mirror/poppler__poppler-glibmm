/* Copyright (c) 2010  Glenn Rice <glennrister@gmail.com>
 *
 * This file is part of poppler-glibmm.
 *
 * poppler-glibmm is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 2.1 of the License,
 * or (at your option) any later version.
 *
 * poppler-glibmm is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "transitions.h"

#include <glibmm/main.h>
#include <gtkmm/main.h>

namespace PGD
{

Transitions::Transitions(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_ScanButton("Scan")
{
	m_ProgressBar.set_ellipsize(Pango::ELLIPSIZE_END);
	m_ProgressBar.set_show_text();
	m_ProgressBar.set_text("");
	m_ScanButton.signal_clicked().connect(sigc::mem_fun(*this, &Transitions::scan_button_clicked));
	m_HBoxTop.pack_start(m_ProgressBar, true, true);
	m_HBoxTop.pack_end(m_ScanButton, false, false);

	pack_start(m_HBoxTop, false, true, 6);

	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_TransitionStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.set_model(m_TransitionStore);
	m_TreeView.set_headers_visible();
	m_TreeView.set_rules_hint();
	m_TreeView.get_selection()->set_mode(Gtk::SELECTION_NONE);

	m_TreeView.append_column_numeric("Page", m_StoreColumns.m_Page, "%d");
	m_TreeView.append_column("Type", m_StoreColumns.m_Type);
	m_TreeView.append_column("Alignment", m_StoreColumns.m_Alignment);
	m_TreeView.append_column("Direction", m_StoreColumns.m_Direction);
	m_TreeView.append_column_numeric("Duration", m_StoreColumns.m_Duration, "%d");
	m_TreeView.append_column_numeric("Scale", m_StoreColumns.m_Scale, "%.2f");
	m_TreeView.append_column("Rectangular", m_StoreColumns.m_Rectangular);

	m_ScrolledWin.add(m_TreeView);
	pack_start(m_ScrolledWin, true, true);

	show_all();
}

void Transitions::scan_button_clicked()
{
	Glib::signal_idle().connect_once(sigc::mem_fun(*this, &Transitions::fill_model));
}

static Glib::ustring transition_type_to_string(Poppler::PageTransitionType type)
{
	switch (type)
	{
		case Poppler::PAGE_TRANSITION_REPLACE:
			return "Replace";
		case Poppler::PAGE_TRANSITION_SPLIT:
			return "Split";
		case Poppler::PAGE_TRANSITION_BLINDS:
			return "Blinds";
		case Poppler::PAGE_TRANSITION_BOX:
			return "Box";
		case Poppler::PAGE_TRANSITION_WIPE:
			return "Wipe";
		case Poppler::PAGE_TRANSITION_DISSOLVE:
			return "Dissolve";
		case Poppler::PAGE_TRANSITION_GLITTER:
			return "Glitter";
		case Poppler::PAGE_TRANSITION_FLY:
			return "Fly";
		case Poppler::PAGE_TRANSITION_PUSH:
			return "Push";
		case Poppler::PAGE_TRANSITION_COVER:
			return "Cover";
		case Poppler::PAGE_TRANSITION_UNCOVER:
			return "Uncover";
		case Poppler::PAGE_TRANSITION_FADE:
			return "Fade";
	}
	return "Unknown";
}

static Glib::ustring transition_alignment_to_string (Poppler::PageTransitionAlignment alignment)
{
	return alignment == Poppler::PAGE_TRANSITION_HORIZONTAL ? "Horizontal" : "Vertical";
}

static Glib::ustring transition_direction_to_string (Poppler::PageTransitionDirection direction)
{
	return direction == Poppler::PAGE_TRANSITION_INWARD ? "Inward" : "Outward";
}

void Transitions::fill_model()
{
	int n_pages = m_Document->get_n_pages();

	for (int i = 0; i < n_pages; ++i)
	{
		update_progress(n_pages, i);
		while (Gtk::Main::events_pending())
			Gtk::Main::iteration();

		Glib::RefPtr<Poppler::Page> page = m_Document->get_page(i);
		if (!page) continue;

		Poppler::PageTransition transition = page->get_transition();
		if (transition)
		{
			Gtk::TreeRow row = *(m_TransitionStore->append());
			row[m_StoreColumns.m_Page] = i + 1;
			row[m_StoreColumns.m_Type] = transition_type_to_string(transition.get_transition_type());
			row[m_StoreColumns.m_Alignment] = transition_alignment_to_string(transition.get_alignment());
			row[m_StoreColumns.m_Direction] = transition_direction_to_string(transition.get_direction());
			row[m_StoreColumns.m_Duration] = transition.get_duration();
			row[m_StoreColumns.m_Angle] = transition.get_angle();
			row[m_StoreColumns.m_Scale] = transition.get_scale();
			row[m_StoreColumns.m_Rectangular] = transition.get_rectangular() ? "Yes" : "No";

		}

	}
	update_progress(n_pages, n_pages);
}

void Transitions::update_progress(int n_pages, int scanned)
{
	m_ProgressBar.set_text(Glib::ustring::compose("Scanning transitions (%1%%)", std::min(scanned * 100 / n_pages, 100)));
	m_ProgressBar.set_fraction(std::min((double)scanned / n_pages, 1.0));
}

}
