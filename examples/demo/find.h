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

#ifndef _FIND_H_
#define _FIND_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/treestore.h>

namespace PGD
{

class Find : public Gtk::Box
{
	public:
		Find(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void find_button_clicked();
		void set_button_sensitivity();
		void update_progress(int n_pages, int scanned);
		bool find_text();

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_NPages;
		int m_PageIndex;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_Matches); add(m_X1); add(m_Y1); add(m_X2); add(m_Y2); add(m_Visible); }

				Gtk::TreeModelColumn<Glib::ustring> m_Matches;
				Gtk::TreeModelColumn<double> m_X1;
				Gtk::TreeModelColumn<double> m_Y1;
				Gtk::TreeModelColumn<double> m_X2;
				Gtk::TreeModelColumn<double> m_Y2;
				Gtk::TreeModelColumn<bool> m_Visible;
		};

		Gtk::Box m_HBoxTop;
		Gtk::ProgressBar m_ProgressBar;
		Gtk::Entry m_TextEntry;
		Gtk::Button m_FindButton;
		Gtk::ScrolledWindow m_ScrolledWin;
		Gtk::TreeView m_TreeView;
		ModelColumns m_StoreColumns;
		Glib::RefPtr<Gtk::TreeStore> m_MatchStore;
		sigc::connection m_FindIdle;
};

}

#endif // _FIND_H_
