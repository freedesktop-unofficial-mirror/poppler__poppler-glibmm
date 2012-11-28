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

#include "find.h"

#include <glibmm/main.h>
#include <glibmm/timer.h>
#include <iomanip>

namespace PGD
{

Find::Find(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_PageIndex(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_FindButton("Find")
{
	m_NPages = document->get_n_pages();

	m_ProgressBar.set_ellipsize(Pango::ELLIPSIZE_END);
	m_ProgressBar.set_show_text();
	m_ProgressBar.set_text("");
	m_FindButton.set_sensitive(false);
	m_FindButton.signal_clicked().connect(sigc::mem_fun(*this, &Find::find_button_clicked));
	m_TextEntry.signal_changed().connect(sigc::mem_fun(*this, &Find::set_button_sensitivity));

	m_HBoxTop.pack_start(m_TextEntry, false, true);
	m_HBoxTop.pack_start(m_ProgressBar, true, true);
	m_HBoxTop.pack_end(m_FindButton, false, false);

	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_MatchStore = Gtk::TreeStore::create(m_StoreColumns);
	m_TreeView.set_model(m_MatchStore);
	m_TreeView.set_rules_hint();
	m_TreeView.get_selection()->set_mode(Gtk::SELECTION_NONE);

	m_TreeView.append_column("Matches", m_StoreColumns.m_Matches);
	int column_num = m_TreeView.append_column_numeric("X1", m_StoreColumns.m_X1, "%.2f");
	m_TreeView.get_column(column_num - 1)->add_attribute(
			m_TreeView.get_column_cell_renderer(column_num - 1)->property_visible(), m_StoreColumns.m_Visible);
	column_num = m_TreeView.append_column_numeric("Y1", m_StoreColumns.m_Y1, "%.2f");
	m_TreeView.get_column(column_num - 1)->add_attribute(
			m_TreeView.get_column_cell_renderer(column_num - 1)->property_visible(), m_StoreColumns.m_Visible);
	column_num = m_TreeView.append_column_numeric("X2", m_StoreColumns.m_X2, "%.2f");
	m_TreeView.get_column(column_num - 1)->add_attribute(
			m_TreeView.get_column_cell_renderer(column_num - 1)->property_visible(), m_StoreColumns.m_Visible);
	column_num = m_TreeView.append_column_numeric("Y2", m_StoreColumns.m_Y2, "%.2f");
	m_TreeView.get_column(column_num - 1)->add_attribute(
			m_TreeView.get_column_cell_renderer(column_num - 1)->property_visible(), m_StoreColumns.m_Visible);

	m_ScrolledWin.add(m_TreeView);

	pack_start(m_HBoxTop, false, true, 6);
	pack_start(m_ScrolledWin, true, true);

	show_all();
}

void Find::find_button_clicked()
{
	m_MatchStore->clear();
	m_PageIndex = 0;

	update_progress(m_NPages, m_PageIndex);
	if (m_FindIdle.connected()) m_FindIdle.disconnect();
	m_FindIdle = Glib::signal_idle().connect(sigc::mem_fun(*this, &Find::find_text));
}

void Find::set_button_sensitivity()
{
	m_FindButton.set_sensitive(m_TextEntry.get_text().size());
}

void Find::update_progress(int n_pages, int scanned)
{
	m_ProgressBar.set_text(Glib::ustring::compose("Searching ... (%1%%)", std::min(scanned * 100 / n_pages, 100)));
	m_ProgressBar.set_fraction(std::min((double)scanned / n_pages, 1.0));
}

bool Find::find_text()
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_PageIndex);
	if (!page) return ++m_PageIndex < m_NPages;

	Glib::Timer timer;
	std::vector<Poppler::Rectangle> matches = page->find_text(m_TextEntry.get_text());
	timer.stop();

	if (matches.size())
	{
		Gtk::TreeRow row = *(m_MatchStore->append());
		row[m_StoreColumns.m_Matches] = Glib::ustring::format(matches.size(), " matches found on page ", m_PageIndex + 1,
				" in ", std::fixed, std::setprecision(4), timer.elapsed(), " seconds");

		int n_match = 0;
		for (std::vector<Poppler::Rectangle>::iterator match = matches.begin();
				match != matches.end(); ++match)
		{
			Gtk::TreeRow match_row = *(m_MatchStore->append(row.children()));
			match_row[m_StoreColumns.m_Matches] = Glib::ustring::format("Match ", ++n_match);
			match_row[m_StoreColumns.m_X1] = match->get_x1();
			match_row[m_StoreColumns.m_Y1] = match->get_y1();
			match_row[m_StoreColumns.m_X2] = match->get_x2();
			match_row[m_StoreColumns.m_Y2] = match->get_y2();
			match_row[m_StoreColumns.m_Visible] = true;
		}
	}

	update_progress(m_NPages, ++m_PageIndex);
	return m_PageIndex < m_NPages;
}

}
