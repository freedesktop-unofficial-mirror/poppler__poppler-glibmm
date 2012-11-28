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

#include "attachments.h"
#include "util.h"

#include <gtkmm/main.h>
#include <gtkmm/stock.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/filechooserdialog.h>
#include <glibmm/main.h>
#include <glibmm/markup.h>

namespace PGD
{

Attachments::Attachments(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_ButtonBox(Gtk::ORIENTATION_HORIZONTAL),
	m_SaveButton("Save"), m_ValidateButton("Validate"),
	m_DescriptionCell(), m_NameCell(),
	m_DescriptionColumn("Description", m_DescriptionCell), m_NameColumn("Name", m_NameCell)
{
	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	bool has_attachments = document->has_attachments();

	m_AttachmentsStore = Gtk::ListStore::create(m_StoreColumns);

	m_TreeView.set_model(m_AttachmentsStore);
	m_TreeView.append_column(m_NameColumn);

	if (has_attachments)
	{
		fill_model();

		m_TreeView.set_headers_visible();
		m_TreeView.set_rules_hint();

		m_NameColumn.add_attribute(m_NameCell.property_text(), m_StoreColumns.m_Name);

		m_DescriptionCell.property_ellipsize() = Pango::ELLIPSIZE_END;
		m_DescriptionColumn.add_attribute(m_DescriptionCell.property_text(), m_StoreColumns.m_Description);
		m_DescriptionColumn.set_expand();
		m_TreeView.append_column(m_DescriptionColumn);

		m_TreeView.append_column_numeric("Size", m_StoreColumns.m_Size, "%" G_GSIZE_FORMAT);
		m_TreeView.append_column("Creation Date", m_StoreColumns.m_CTime);
		m_TreeView.append_column("Modification Date", m_StoreColumns.m_MTime);
	}
	else
	{
		m_NameColumn.add_attribute(m_NameCell.property_markup(), m_StoreColumns.m_Name);
		Gtk::TreeRow tree_row = *(m_AttachmentsStore->append());
		tree_row[m_StoreColumns.m_Name] =
			"<span size=\"larger\" style=\"italic\">Document does not contain attachments</span>";
	}

	m_ScrolledWin.add(m_TreeView);

	pack_start(m_ScrolledWin, true, true);

	if (!has_attachments) return;

	m_ButtonBox.set_layout(Gtk::BUTTONBOX_SPREAD);
	m_ButtonBox.pack_start(m_SaveButton, false, false);
	m_ButtonBox.pack_start(m_ValidateButton, false, false);

	pack_start(m_ButtonBox, false, true, 6);

	m_SaveButton.signal_clicked().connect(sigc::mem_fun(*this, &Attachments::save_button_clicked));
	m_ValidateButton.signal_clicked().connect(sigc::mem_fun(*this, &Attachments::validate_button_clicked));
	
	show_all();
}

void Attachments::fill_model()
{
	std::vector< Glib::RefPtr<Poppler::Attachment> > attachments = m_Document->get_attachments();
	for (std::vector< Glib::RefPtr<Poppler::Attachment> >::iterator attachment = attachments.begin();
			attachment != attachments.end(); ++attachment)
	{
		Gtk::TreeRow tree_row = *(m_AttachmentsStore->append());

		Glib::ustring name = (*attachment)->get_name();
		tree_row[m_StoreColumns.m_Name] = name.empty() ? "Unknown" : name;

		Glib::ustring description = (*attachment)->get_description();
		tree_row[m_StoreColumns.m_Description] = description.empty() ? "Unknown" : description;

		tree_row[m_StoreColumns.m_Size] = (*attachment)->get_size();

		Glib::ustring ctime = format_date((*attachment)->get_ctime());
		tree_row[m_StoreColumns.m_CTime] = ctime.empty() ? "Unknown" : ctime;

		Glib::ustring mtime = format_date((*attachment)->get_mtime());
		tree_row[m_StoreColumns.m_MTime] = mtime.empty() ? "Unknown" : mtime;

		tree_row[m_StoreColumns.m_Attachment] = *attachment;
	}
}

void Attachments::save_button_clicked()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (!iter) return;

	Glib::RefPtr<Poppler::Attachment> attachment = (*iter)[m_StoreColumns.m_Attachment];
	if (!attachment) return;

	Gtk::FileChooserDialog file_chooser("Save attachment", Gtk::FILE_CHOOSER_ACTION_SAVE);
	file_chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	file_chooser.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_ACCEPT);
	file_chooser.set_current_name(attachment->get_name());

	if (file_chooser.run() == Gtk::RESPONSE_ACCEPT)
	{
		std::string filename = file_chooser.get_filename();
		try
		{
			attachment->save(filename);
		}
		catch (Glib::Error exception)
		{
		}
	}
}

static void message_dialog_run(const Glib::ustring& message)
{
	Gtk::MessageDialog dialog(message, false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_CLOSE);
	dialog.run();
}

bool Attachments::save_slot(const gchar* buf, gsize count, Glib::Checksum& cs)
{
	cs.update((const guchar*)buf, count);
	return true;
}

void Attachments::validate_button_clicked()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (!iter) return;

	Glib::RefPtr<Poppler::Attachment> attachment = (*iter)[m_StoreColumns.m_Attachment];
	if (!attachment) return;

	std::string checksum = attachment->get_checksum();
	if (checksum.empty()) message_dialog_run("Impossible to validate attachment: checksum is not available");

	Glib::Checksum cs(Glib::Checksum::CHECKSUM_MD5);
	attachment->save(sigc::bind(sigc::mem_fun(*this, &Attachments::save_slot), cs));

	gsize digest_len = Glib::Checksum::get_length(Glib::Checksum::CHECKSUM_MD5);
	std::string digest(digest_len, 0);
	cs.get_digest((guint8*)&digest[0], &digest_len);

	if (checksum.size() == digest_len)
	{
		for (unsigned int i = 0; i < digest_len; ++i)
			if (checksum[i] != digest[i])
			{
				message_dialog_run("Attachment is not valid: the checksum does not match");
				return;
			}
	}
	message_dialog_run("Attachment is valid");
}

}
