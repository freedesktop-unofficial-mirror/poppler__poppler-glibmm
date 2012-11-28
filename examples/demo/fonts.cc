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

#include "fonts.h"

#include <glibmm/main.h>
#include <glibmm/markup.h>
#include <gtkmm/main.h>

namespace PGD
{

Fonts::Fonts(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_HBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_ScanButton("Scan"),
	m_Column("Fonts")
{
	m_ProgressBar.set_ellipsize(Pango::ELLIPSIZE_END);
	m_ProgressBar.set_show_text();
	m_ProgressBar.set_text("");
	m_ScanButton.signal_clicked().connect(sigc::mem_fun(*this, &Fonts::scan_button_clicked));

	m_HBox.pack_start(m_ProgressBar, true, true);
	m_HBox.pack_end(m_ScanButton, false, false);
	pack_start(m_HBox, false, true, 6);

	// Set up the treeview that displays the fonts
	m_FontsWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_FontsStore = Gtk::ListStore::create(m_StoreColumns);

	m_Column.pack_start(m_FontCell, true);
	m_Column.set_cell_data_func(m_FontCell, sigc::mem_fun(*this, &Fonts::fonts_cell_data_func));

	m_TreeView.append_column(m_Column);
	m_TreeView.set_model(m_FontsStore);
	m_TreeView.set_headers_visible(false);
	m_TreeView.set_rules_hint();
	m_TreeView.get_selection()->set_mode(Gtk::SELECTION_NONE);

	m_FontsWin.add(m_TreeView);
	pack_start(m_FontsWin, true, true);

	show_all();
}

void Fonts::scan_button_clicked()
{
	Glib::signal_idle().connect_once(sigc::mem_fun(*this, &Fonts::fill_model));
}

void Fonts::fonts_cell_data_func(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter)
{
	Glib::ustring font_name = (*iter)[m_StoreColumns.m_FontName];
	Glib::ustring details = (*iter)[m_StoreColumns.m_FontDetails];

	if (details.empty())
		((Gtk::CellRendererText*)cell)->property_markup() = "<b><big>" + font_name + "</big></b>";
	else
		((Gtk::CellRendererText*)cell)->property_markup() =
			"<b><big>" + font_name + "</big></b>\n<small>" + details + "</small>";
}

static Glib::ustring font_type_to_string(Poppler::FontType type)
{
	switch (type)
	{
		case Poppler::FONT_TYPE_TYPE1:
			return "Type 1";
		case Poppler::FONT_TYPE_TYPE1C:
			return "Type 1C";
		case Poppler::FONT_TYPE_TYPE3:
			return "Type 3";
		case Poppler::FONT_TYPE_TRUETYPE:
			return "TrueType";
		case Poppler::FONT_TYPE_CID_TYPE0:
			return "Type 1 (CID)";
		case Poppler::FONT_TYPE_CID_TYPE0C:
			return "Type 1C (CID)";
		case Poppler::FONT_TYPE_CID_TYPE2:
			return "TrueType (CID)";
		default:
			return "Unknown font type";
	}
}

void Fonts::fill_model()
{
	int n_pages = m_Document->get_n_pages(), scanned = 0;
	m_FontsStore->clear();

	Glib::RefPtr<Poppler::FontInfo> font_info = m_Document->get_font_info();
	Poppler::FontsIter fonts_iter;
	while (font_info->scan(20, fonts_iter))
	{
		update_progress(n_pages, scanned);
		while (Gtk::Main::events_pending())
			Gtk::Main::iteration();

		scanned += 20;

		if (!fonts_iter) continue; // No fonts found in these 20 pages
		do
		{
			Gtk::TreeRow row = *(m_FontsStore->append());

			Glib::ustring name = fonts_iter.get_name();
			row[m_StoreColumns.m_FontName] = name.empty() ? "No name" : name;

			Glib::ustring encoding = fonts_iter.get_encoding();
			if (encoding.empty()) encoding = "None";

			Glib::ustring type = font_type_to_string(fonts_iter.get_font_type());

			Glib::ustring embedded;
			if (fonts_iter.is_embedded())
			{
				if (fonts_iter.is_subset()) embedded = "Embedded subset";
				else embedded = "Embedded";
			}
			else embedded = "Not embedded";

			Glib::ustring substitute = fonts_iter.get_substitute_name();
			Glib::ustring filename = fonts_iter.get_file_name();

			Glib::ustring details = type + "\nEncoding: " + Glib::Markup::escape_text(encoding) + "\n" + embedded;
			if (substitute.size() && filename.size())
				details += ", substituting with <b>" + Glib::Markup::escape_text(substitute) + "</b>\n(" +
					Glib::Markup::escape_text(filename) + ")";

			row[m_StoreColumns.m_FontDetails] = details;

		} while (fonts_iter.next());
	}
	update_progress(n_pages, scanned);
}

void Fonts::update_progress(int n_pages, int scanned)
{
	m_ProgressBar.set_text(Glib::ustring::compose("Scanning fonts (%1%%)", std::min(scanned * 100 / n_pages, 100)));
	m_ProgressBar.set_fraction(std::min((double)scanned / n_pages, 1.0));
}

}
