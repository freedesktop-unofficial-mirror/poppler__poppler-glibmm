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

#ifndef _LINKS_H_
#define _LINKS_H_

#include "action-view.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>

namespace PGD
{

class Links : public Gtk::Box
{
	public:
		Links(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void page_selector_changed();
		void get_links();
		void selection_changed();

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_Page;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_ActionType); add(m_X1); add(m_Y1); add(m_X2); add(m_Y2); add(m_Action); }

				Gtk::TreeModelColumn<Glib::ustring> m_ActionType;
				Gtk::TreeModelColumn<double> m_X1;
				Gtk::TreeModelColumn<double> m_Y1;
				Gtk::TreeModelColumn<double> m_X2;
				Gtk::TreeModelColumn<double> m_Y2;
				Gtk::TreeModelColumn<Poppler::Action> m_Action;
		};

		Gtk::Box m_HBoxTop;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_TimerLabel;
		Gtk::SpinButton m_PageSelector;
		Gtk::Button m_GetButton;
		Gtk::ScrolledWindow m_ScrolledWin;
		Gtk::Paned m_HPaned;
		ActionView m_ActionView;

		ModelColumns m_StoreColumns;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::ListStore> m_LinkStore;
};

}

#endif // _LINKS_H_
