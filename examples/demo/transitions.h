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

#include <gtk/gtk.h>
#include <poppler.h>

#ifndef _TRANSITIONS_H_
#define _TRANSITIONS_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/progressbar.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>

namespace PGD
{

class Transitions : public Gtk::Box
{
	public:
		Transitions(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void scan_button_clicked();
		void fill_model();
		void update_progress(int n_pages, int scanned);

		const Glib::RefPtr<Poppler::Document> m_Document;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_Page); add(m_Type); add(m_Alignment); add(m_Direction);
					add(m_Duration); add(m_Angle); add(m_Scale); add(m_Rectangular); }

				Gtk::TreeModelColumn<int> m_Page;
				Gtk::TreeModelColumn<Glib::ustring> m_Type;
				Gtk::TreeModelColumn<Glib::ustring> m_Alignment;
				Gtk::TreeModelColumn<Glib::ustring> m_Direction;
				Gtk::TreeModelColumn<int> m_Duration;
				Gtk::TreeModelColumn<int> m_Angle;
				Gtk::TreeModelColumn<float> m_Scale;
				Gtk::TreeModelColumn<Glib::ustring> m_Rectangular;
		};

		Gtk::Box m_HBoxTop;
		Gtk::ProgressBar m_ProgressBar;
		Gtk::Button m_ScanButton;
		Gtk::ScrolledWindow m_ScrolledWin;

		ModelColumns m_StoreColumns;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::ListStore> m_TransitionStore;
};

}

#endif // _TRANSITIONS_H_
