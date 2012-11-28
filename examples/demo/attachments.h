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

#ifndef _ATTACHMENTS_H_
#define _ATTACHMENTS_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/buttonbox.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <glibmm/checksum.h>

namespace PGD
{

class Attachments : public Gtk::Box
{
	public:
		Attachments(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void fill_model();
		void save_button_clicked();
		bool save_slot(const gchar* buf, gsize count, Glib::Checksum& cs);
		void validate_button_clicked();

		const Glib::RefPtr<Poppler::Document> m_Document;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_Name); add(m_Description); add(m_Size);
					add(m_CTime); add(m_MTime); add(m_Attachment); }

				Gtk::TreeModelColumn<Glib::ustring> m_Name;
				Gtk::TreeModelColumn<Glib::ustring> m_Description;
				Gtk::TreeModelColumn<gsize> m_Size;
				Gtk::TreeModelColumn<Glib::ustring> m_CTime;
				Gtk::TreeModelColumn<Glib::ustring> m_MTime;
				Gtk::TreeModelColumn< Glib::RefPtr<Poppler::Attachment> > m_Attachment;
		};

		Gtk::ButtonBox m_ButtonBox;
		Gtk::Button m_SaveButton, m_ValidateButton;
		Gtk::ScrolledWindow m_ScrolledWin;
		Gtk::TreeView m_TreeView;
		ModelColumns m_StoreColumns;
		Glib::RefPtr<Gtk::ListStore> m_AttachmentsStore;
		Gtk::CellRendererText m_DescriptionCell, m_NameCell;
		Gtk::TreeViewColumn m_DescriptionColumn, m_NameColumn;
};

}

#endif // _ATTACHMENTS_H_
