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
#include <gdkmm/general.h>
#include <iomanip>

namespace PGD
{

Find::Find(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_PageIndex(0), m_SelectedPage(-1),
	m_Options(Poppler::FIND_DEFAULT),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_HBoxOptions(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_FindButton("Find"),
	m_CaseSensitiveCButton("Case sensitive"),
	m_BackwardsCButton("Backwards"),
	m_WholeWordsCButton("Whole words only"),
	m_HPaned(Gtk::ORIENTATION_HORIZONTAL)
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

	m_CaseSensitiveCButton.signal_toggled().connect(sigc::mem_fun(*this, &Find::case_sensitive_toggled));
	m_BackwardsCButton.signal_toggled().connect(sigc::mem_fun(*this, &Find::backwards_toggled));
	m_WholeWordsCButton.signal_toggled().connect(sigc::mem_fun(*this, &Find::whole_words_toggled));

	m_HBoxOptions.pack_start(m_CaseSensitiveCButton, false, false);
	m_HBoxOptions.pack_start(m_BackwardsCButton, false, false);
	m_HBoxOptions.pack_start(m_WholeWordsCButton, false, false);

	m_MatchScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_MatchStore = Gtk::TreeStore::create(m_StoreColumns);
	m_TreeView.set_model(m_MatchStore);
	m_TreeView.set_rules_hint();
	Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();
	selection->set_mode(Gtk::SELECTION_SINGLE);
	selection->signal_changed().connect(sigc::mem_fun(*this, &Find::selection_changed));

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

	m_MatchScrolledWin.add(m_TreeView);

	m_DrawingArea.signal_draw().connect(sigc::mem_fun(*this, &Find::drawing_area_draw));
	m_DAScrolledWin.add(m_DrawingArea);

	m_HPaned.add1(m_MatchScrolledWin);
	m_HPaned.add2(m_DAScrolledWin);
	m_HPaned.set_position(300);

	pack_start(m_HBoxTop, false, true, 6);
	pack_start(m_HBoxOptions, false, false);
	pack_start(m_HPaned, true, true);

	show_all();
}

void Find::queue_redraw()
{
	m_Surface.clear();
	m_DrawingArea.queue_draw();
}

void Find::find_button_clicked()
{
	m_MatchStore->clear();
	m_PageIndex = 0;
	m_SelectedPage = -1;
	queue_redraw();

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
	std::vector<Poppler::Rectangle> matches = page->find_text_with_options(m_TextEntry.get_text(), m_Options);
	timer.stop();

	if (matches.size())
	{
		Gtk::TreeRow row = *(m_MatchStore->append());
		row[m_StoreColumns.m_Matches] = Glib::ustring::format(matches.size(), " matches found on page ", m_PageIndex + 1,
				" in ", std::fixed, std::setprecision(4), timer.elapsed(), " seconds");
		row[m_StoreColumns.m_Visible] = false;
		row[m_StoreColumns.m_PageColumn] = m_PageIndex;

		double width, height;
		page->get_size(width, height);

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
			match_row[m_StoreColumns.m_PageColumn] = m_PageIndex;

			double tmp = match->get_y1();
			match->set_y1(height - match->get_y2());
			match->set_y2(height - tmp);

			match_row[m_StoreColumns.m_Rect] = *match;
		}
	}

	update_progress(m_NPages, ++m_PageIndex);
	return m_PageIndex < m_NPages;
}

bool Find::drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (m_SelectedPage == -1) return false;

	if (!m_Surface)
	{
		Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_SelectedPage);
		if (!page) return false;

		double width, height;
		page->get_size(width, height);
		m_DrawingArea.set_size_request(width, height);

		m_Surface = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, width, height);
		Cairo::RefPtr<Cairo::Context> cr_tmp = Cairo::Context::create(m_Surface);

		cr_tmp->save();
		cr_tmp->set_source_rgb(1.0, 1.0, 1.0);
		cr_tmp->rectangle(0.0, 0.0, width, height); 
		cr_tmp->fill();
		cr_tmp->restore();

		cr_tmp->save();
		page->render(cr_tmp);
		cr_tmp->restore();

		if (!m_Surface) return false;
	}

	cr->set_source(m_Surface, 0, 0);
	cr->paint();

	if (m_SelectedMatch.get_width() > 0 && m_SelectedMatch.get_height() > 0)
	{
		cr->set_source_rgb(1.0, 1.0, 0.0);
		// TODO: The following operator should be Cairo::OPERATOR_MULTIPLY but that is not in cairomm yet.
		cr->set_operator((Cairo::Operator)CAIRO_OPERATOR_MULTIPLY);
		Gdk::Cairo::add_rectangle_to_path(cr, m_SelectedMatch);
		cr->fill();
	}

	return true;
}

void Find::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter)
	{
		if ((*iter)[m_StoreColumns.m_Visible])
		{
			Poppler::Rectangle rect = (*iter)[m_StoreColumns.m_Rect];
			m_SelectedMatch.set_x(rect.get_x1());
			m_SelectedMatch.set_y(rect.get_y1());
			m_SelectedMatch.set_width(rect.get_x2() - rect.get_x1());
			m_SelectedMatch.set_height(rect.get_y2() - rect.get_y1());
		}
		else
		{
			m_SelectedMatch.set_width(0);
			m_SelectedMatch.set_height(0);
		}

		if (m_SelectedPage != (*iter)[m_StoreColumns.m_PageColumn])
		{
			m_SelectedPage = (*iter)[m_StoreColumns.m_PageColumn];
			queue_redraw();
		}
		else
			m_DrawingArea.queue_draw();
	}
}

void Find::case_sensitive_toggled()
{
	if (m_CaseSensitiveCButton.get_active())
		m_Options |= Poppler::FIND_CASE_SENSITIVE;
	else
		m_Options &= ~Poppler::FIND_CASE_SENSITIVE;
}

void Find::backwards_toggled()
{
	if (m_BackwardsCButton.get_active())
		m_Options |= Poppler::FIND_BACKWARDS;
	else
		m_Options &= ~Poppler::FIND_BACKWARDS;
}

void Find::whole_words_toggled()
{
	if (m_WholeWordsCButton.get_active())
		m_Options |= Poppler::FIND_WHOLE_WORDS_ONLY;
	else
		m_Options &= ~Poppler::FIND_WHOLE_WORDS_ONLY;
}

}
