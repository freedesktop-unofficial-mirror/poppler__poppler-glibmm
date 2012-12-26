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

#ifndef _OUTLINE_H_
#define _OUTLINE_H_

#include "action-view.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/paned.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>

namespace PGD
{

class Outline : public Gtk::Paned
{
	public:
		Outline(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void build_tree(const Gtk::TreeModel::Children& parent, Poppler::IndexIter& iter);
		void expand_open_links(const Gtk::TreeModel::Children& parent);
		void selection_changed();

		const Glib::RefPtr<Poppler::Document> m_Document;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_Title); add(m_ActionType); add(m_Expand); add(m_Action); }

				Gtk::TreeModelColumn<Glib::ustring> m_Title;
				Gtk::TreeModelColumn<Glib::ustring> m_ActionType;
				Gtk::TreeModelColumn<bool> m_Expand;
				Gtk::TreeModelColumn<Poppler::Action> m_Action;
		};

		ActionView m_ActionView;
		Gtk::ScrolledWindow m_ScrolledWin;
		Gtk::TreeView m_TreeView;
		ModelColumns m_StoreColumns;
		Glib::RefPtr<Gtk::TreeModel> m_ActionStore;
		Gtk::CellRendererText m_TitleCell;
		Gtk::TreeViewColumn m_TitleColumn;
};

}

#endif // _OUTLINE_H_
