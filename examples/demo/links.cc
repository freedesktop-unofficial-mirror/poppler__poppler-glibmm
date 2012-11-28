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

#include "links.h"
#include "property-grid.h"

#include <gtkmm/textview.h>
#include <glibmm/timer.h>
#include <iomanip>

namespace PGD
{

Links::Links(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:"),
	m_GetButton("Get Links"),
	m_HPaned(Gtk::ORIENTATION_HORIZONTAL),
	m_ActionView(document)
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Links::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));

	m_GetButton.signal_clicked().connect(sigc::mem_fun(*this, &Links::get_links));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);
	m_HBoxTop.pack_end(m_GetButton, false, false);

	m_TimerLabel.set_markup("<i>No links found</i>");
	m_TimerLabel.set_alignment(1.0, 0.5);

	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_LinkStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.set_model(m_LinkStore);

	m_TreeView.append_column("Action Type", m_StoreColumns.m_ActionType);
	m_TreeView.append_column_numeric("X1", m_StoreColumns.m_X1, "%.2f");
	m_TreeView.append_column_numeric("Y1", m_StoreColumns.m_Y1, "%.2f");
	m_TreeView.append_column_numeric("X2", m_StoreColumns.m_X2, "%.2f");
	m_TreeView.append_column_numeric("Y2", m_StoreColumns.m_Y2, "%.2f");

	m_TreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Links::selection_changed));

	m_ScrolledWin.add(m_TreeView);

	m_HPaned.add1(m_ScrolledWin);
	m_HPaned.add2(m_ActionView);
	m_HPaned.set_position(300);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_TimerLabel, false, true);
	pack_start(m_HPaned, true, true);

	show_all();
}

void Links::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
}

void Links::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter) m_ActionView.set_action((*iter)[m_StoreColumns.m_Action]);
	else m_ActionView.set_action(Poppler::Action());
}

void Links::get_links()
{
	m_LinkStore->clear();

	m_ActionView.set_action(Poppler::Action());

	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	Glib::Timer timer;
	std::vector<Poppler::LinkMapping> mapping = page->get_link_mapping();
	timer.stop();

	int n_links = mapping.size();

	if (n_links > 0)
		m_TimerLabel.set_markup(Glib::ustring::format("<i>", n_links, " links found in ",
					std::fixed, std::setprecision(4), timer.elapsed(), " seconds</i>"));
	else
		m_TimerLabel.set_markup("<i>No links found</i>");

	for (std::vector<Poppler::LinkMapping>::iterator lmapping = mapping.begin();
			lmapping != mapping.end(); ++lmapping)
	{
		Gtk::TreeRow tree_row = *(m_LinkStore->append());

		Poppler::Action action = lmapping->get_action();
		GEnumValue* enum_value = g_enum_get_value((GEnumClass*)g_type_class_ref(
					Glib::Value<Poppler::ActionType>::value_type()), action.get_action_type());
		tree_row[m_StoreColumns.m_ActionType] = enum_value->value_name;

		Poppler::Rectangle area = lmapping->get_area();
		tree_row[m_StoreColumns.m_X1] = area.get_x1();
		tree_row[m_StoreColumns.m_Y1] = area.get_y1();
		tree_row[m_StoreColumns.m_X2] = area.get_x2();
		tree_row[m_StoreColumns.m_Y2] = area.get_y2();

		tree_row[m_StoreColumns.m_Action] = action;
	}
}

}
