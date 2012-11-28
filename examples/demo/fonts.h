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

#ifndef _FONTS_H_
#define _FONTS_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>

namespace PGD
{

class Fonts : public Gtk::Box
{
	public:
		Fonts(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void scan_button_clicked();
		void fonts_cell_data_func(Gtk::CellRenderer* cell, const Gtk::TreeModel::iterator& iter);
		void fill_model();
		void update_progress(int n_pages, int scanned);

		const Glib::RefPtr<Poppler::Document> m_Document;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_FontName); add(m_FontDetails); }

				Gtk::TreeModelColumn<Glib::ustring> m_FontName;
				Gtk::TreeModelColumn<Glib::ustring> m_FontDetails;
		};

		Gtk::Box m_HBox;
		Gtk::ProgressBar m_ProgressBar;
		Gtk::Button m_ScanButton;
		Gtk::ScrolledWindow m_FontsWin;
		Gtk::TreeView m_TreeView;
		ModelColumns m_StoreColumns;
		Glib::RefPtr<Gtk::ListStore> m_FontsStore;
		Gtk::TreeViewColumn m_Column;
		Gtk::CellRendererText m_FontCell;
};

}

#endif // _FONTS_H_
