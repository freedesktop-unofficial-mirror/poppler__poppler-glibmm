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

#ifndef _FORMS_H_
#define _FORMS_H_

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

class FormFieldView : public Gtk::Frame
{
	public:
		FormFieldView(const Glib::RefPtr<Poppler::Document>& document);
		void set_field(const Glib::RefPtr<Poppler::FormField>& field);

	private:
		const Glib::RefPtr<Poppler::Document> m_Document;

		Gtk::Label m_Label;
		Gtk::Alignment m_Alignment;
};

class Forms : public Gtk::Box
{
	public:
		Forms(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void page_selector_changed();
		void get_form_fields();
		void selection_changed();

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_Page;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_FieldType); add(m_Id); add(m_ReadOnly);
					add(m_X1); add(m_Y1); add(m_X2); add(m_Y2); add(m_Field); }

				Gtk::TreeModelColumn<Glib::ustring> m_FieldType;
				Gtk::TreeModelColumn<int> m_Id;
				Gtk::TreeModelColumn<bool> m_ReadOnly;
				Gtk::TreeModelColumn<double> m_X1;
				Gtk::TreeModelColumn<double> m_Y1;
				Gtk::TreeModelColumn<double> m_X2;
				Gtk::TreeModelColumn<double> m_Y2;
				Gtk::TreeModelColumn< Glib::RefPtr<Poppler::FormField> > m_Field;
		};

		Gtk::Box m_HBoxTop;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_TimerLabel;
		Gtk::SpinButton m_PageSelector;
		Gtk::Button m_GetButton;
		Gtk::ScrolledWindow m_ScrolledWin;
		Gtk::Paned m_HPaned;
		FormFieldView m_FormFieldView;

		ModelColumns m_StoreColumns;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::ListStore> m_FieldStore;
};

}

#endif // _FORMS_H_
