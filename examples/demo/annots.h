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

#ifndef _ANNOTS_H_
#define _ANNOTS_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>
#include <gtkmm/frame.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/alignment.h>

namespace PGD
{

class AnnotView : public Gtk::Frame
{
	public:
		AnnotView(const Glib::RefPtr<Poppler::Document>& document, Gtk::Button* remove_button);
		void set_annot(const Glib::RefPtr<Poppler::Annot>& annot);
		void save_file_attachment();

	private:
		const Glib::RefPtr<Poppler::Document> m_Document;
		Glib::RefPtr<Poppler::Annot> m_Annot;

		Gtk::Label m_Label;
		Gtk::Alignment m_Alignment;
		Gtk::Button* m_RemoveButton;
};

class Annots : public Gtk::Box
{
	public:
		Annots(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void page_selector_changed();
		void add_annot();
		void remove_annot();
		void get_annots();
		void selection_changed();

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_Page;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_X1); add(m_Y1); add(m_X2); add(m_Y2); add(m_AnnotType);
					add(m_Color); add(m_FlagInvisible); add(m_FlagHidden); add(m_FlagPrint); add(m_Annot); }

				Gtk::TreeModelColumn<double> m_X1;
				Gtk::TreeModelColumn<double> m_Y1;
				Gtk::TreeModelColumn<double> m_X2;
				Gtk::TreeModelColumn<double> m_Y2;
				Gtk::TreeModelColumn<Glib::ustring> m_AnnotType;
				Gtk::TreeModelColumn< Glib::RefPtr<Gdk::Pixbuf> > m_Color;
				Gtk::TreeModelColumn<bool> m_FlagInvisible;
				Gtk::TreeModelColumn<bool> m_FlagHidden;
				Gtk::TreeModelColumn<bool> m_FlagPrint;
				Gtk::TreeModelColumn< Glib::RefPtr<Poppler::Annot> > m_Annot;
		};

		Gtk::Box m_HBoxTop;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_TimerLabel;
		Gtk::SpinButton m_PageSelector;
		Gtk::Button m_AddButton;
		Gtk::Button m_GetButton;
		Gtk::Button m_RemoveButton;
		Gtk::ScrolledWindow m_ScrolledWin;
		Gtk::Paned m_HPaned;
		AnnotView m_AnnotView;

		ModelColumns m_StoreColumns;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::ListStore> m_AnnotStore;
};

}

#endif // _ANNOTS_H_
