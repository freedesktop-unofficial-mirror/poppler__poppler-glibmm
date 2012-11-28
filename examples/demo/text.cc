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

#include "text.h"
#include "util.h"

#include <glibmm/timer.h>
#include <iomanip>

namespace PGD
{

TextAttribView::TextAttribView(const Glib::RefPtr<Poppler::Document>& document) :
	m_Document(document),
	m_Alignment(0.5, 0.5, 1, 1)
{
	set_shadow_type(Gtk::SHADOW_NONE);
	m_Label.set_markup("<b>Text Attributes</b>");
	set_label_widget(m_Label);

	m_Alignment.set_padding(5, 5, 12, 5);
	add(m_Alignment);

	m_Grid.set_column_spacing(6);
	m_Grid.set_row_spacing(6);

	m_Grid.add_property_with_custom_widget("<b>Font Name:</b>", m_FontName);
	m_Grid.add_property_with_custom_widget("<b>Font Size:</b>", m_FontSize);
	m_Grid.add_property_with_custom_widget("<b>Underlined:</b>", m_IsUnderlined);
	m_Grid.add_property_with_custom_widget("<b>Color:</b>", m_TextColor);

	m_Alignment.add(m_Grid);
}

void TextAttribView::set_font_name(const Glib::ustring& name)
{
	m_FontName.set_text(name);
}

void TextAttribView::set_font_size(const double size)
{
	m_FontSize.set_text(Glib::ustring::format(std::fixed, std::setprecision(2), size));
}

void TextAttribView::set_is_underlined(const bool is_underlined)
{
   	m_IsUnderlined.set_text(is_underlined ? "Yes" : "No");
}

void TextAttribView::set_text_color(const Poppler::Color& color)
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf = pixbuf_new_for_color(color);
	m_TextColor.set(pixbuf);
}

Text::Text(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6), m_VBoxLeft(Gtk::ORIENTATION_VERTICAL, 12),
	m_PageLabel("Page:"),
	m_GetButton("Get Text"),
	m_HPaned(Gtk::ORIENTATION_HORIZONTAL),
	m_AttribView(document)
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Text::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));

	m_GetButton.signal_clicked().connect(sigc::mem_fun(*this, &Text::get_text));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);
	m_HBoxTop.pack_end(m_GetButton, false, false);

	m_TimerLabel.set_markup("<i>No text found</i>");
	m_TimerLabel.set_alignment(1.0, 0.5);

	m_TreeWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_TextStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.set_model(m_TextStore);

	m_TreeView.append_column_numeric("X1", m_StoreColumns.m_X1, "%.2f");
	m_TreeView.append_column_numeric("Y1", m_StoreColumns.m_Y1, "%.2f");
	m_TreeView.append_column_numeric("X2", m_StoreColumns.m_X2, "%.2f");
	m_TreeView.append_column_numeric("Y2", m_StoreColumns.m_Y2, "%.2f");
	m_TreeView.append_column_numeric("Offset", m_StoreColumns.m_Offset, "%d");

	m_TreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Text::selection_changed));

	m_TreeWin.add(m_TreeView);

	m_VBoxLeft.pack_start(m_TreeWin, true, true);
	m_VBoxLeft.pack_start(m_AttribView, false, false, 12);

	m_BuffWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_TextBuffer = Gtk::TextBuffer::create();
	m_TextView.set_has_tooltip();
	m_TextView.set_buffer(m_TextBuffer);
	m_BuffWin.add(m_TextView);
	m_TextView.signal_query_tooltip().connect(sigc::mem_fun(*this, &Text::view_query_tooltip));

	m_HPaned.add1(m_VBoxLeft);
	m_HPaned.add2(m_BuffWin);
	m_HPaned.set_position(300);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_TimerLabel, false, true);
	pack_start(m_HPaned, true, true);

	show_all();
}

void Text::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
}

void Text::get_text()
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	m_TextStore->clear();
	m_TextAttrs.clear();

	Glib::Timer timer;
	Glib::ustring text = page->get_text();
	timer.stop();

	std::vector<Poppler::Rectangle> recs;
	if (text.size())
	{
		double text_elapsed = timer.elapsed();

		timer.start();
		page->get_text_layout(recs);
		timer.stop();
		double layout_elapsed = timer.elapsed();

		timer.start();
		m_TextAttrs = page->get_text_attributes();
		timer.stop();

		m_TimerLabel.set_markup(Glib::ustring::format(std::fixed, std::setprecision(4),
					"<i>got text in ", text_elapsed, " seconds,  text layout in ",
					layout_elapsed, " seconds, text attrs in ", timer.elapsed()) + " seconds</i>");

		m_TextBuffer->set_text(text);
	}
	else m_TimerLabel.set_markup("<i>No text found</i>");

	for (std::vector<Poppler::Rectangle>::iterator rec = recs.begin();
			rec != recs.end(); ++rec)
	{
		Gtk::TreeRow tree_row = *(m_TextStore->append());
		tree_row[m_StoreColumns.m_X1] = rec->get_x1();
		tree_row[m_StoreColumns.m_Y1] = rec->get_y1();
		tree_row[m_StoreColumns.m_X2] = rec->get_x2();
		tree_row[m_StoreColumns.m_Y2] = rec->get_y2();
		tree_row[m_StoreColumns.m_Offset] = (rec - recs.begin());
	}
}

void Text::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter)
	{
		int offset = (*iter)[m_StoreColumns.m_Offset];
		Gtk::TextIter begin_iter = m_TextBuffer->get_iter_at_offset(offset);
		Gtk::TextIter end_iter = begin_iter;
		++end_iter;
		m_TextBuffer->select_range(begin_iter, end_iter);
		for (std::vector<Poppler::TextAttributes>::iterator attrs = m_TextAttrs.begin();
				attrs != m_TextAttrs.end(); ++attrs)
		{
			if (offset >= attrs->get_start_index() && offset <= attrs->get_end_index())
			{
				m_AttribView.set_font_name(attrs->get_font_name());
				m_AttribView.set_font_size(attrs->get_font_size());
				m_AttribView.set_is_underlined(attrs->get_is_underlined());
				m_AttribView.set_text_color(attrs->get_color());
			}
		}
	}
}

bool Text::view_query_tooltip(int, int, bool, const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter)
	{
		Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
		if (!page) return false;

		Poppler::Rectangle rect((*iter)[m_StoreColumns.m_X1], (*iter)[m_StoreColumns.m_Y1],
				(*iter)[m_StoreColumns.m_X2], (*iter)[m_StoreColumns.m_Y2]);
		tooltip->set_text(page->get_selected_text(Poppler::SELECTION_GLYPH, rect));

		return true;
	}
	return false;
}

}
