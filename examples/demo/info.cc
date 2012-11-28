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

#include "info.h"
#include "util.h"

#include <poppler-glibmm/utility.h>

namespace PGD
{

Info::Info(const Glib::RefPtr<Poppler::Document>& document) :
   	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Alignment(0.5, 0.5, 1, 1),
	m_PermissionsHBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PermissionsHBoxBottom(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PermissionsVBox(Gtk::ORIENTATION_VERTICAL, 0),
	m_PrintCB("Print"),
   	m_CopyCB("Copy"),
   	m_ModifyCB("Modify"),
   	m_AddNotesCB("Add notes"),
	m_FillFormsCB("Fill forms"),
	m_ExtractContentsCB("Extract contents"),
   	m_AssembleCB("Assemble"),
   	m_HighResPrintCB("Print at high resolution")
{
	GEnumValue* enum_value = g_enum_get_value((GEnumClass*)g_type_class_ref(Glib::Value<Poppler::Backend>::value_type()),
			Poppler::get_backend());
	m_VersionLabel.set_markup("<span weight='bold' size='larger'>Poppler " + Poppler::get_version()
			+ " (" + enum_value->value_name + ")</span>");
	pack_start(m_VersionLabel, false, false, 12);

	m_Frame.set_shadow_type(Gtk::SHADOW_NONE);
	m_FrameLabel.set_markup("<b>Document properties</b>");
	m_Frame.set_label_widget(m_FrameLabel);

	m_Grid.set_column_spacing(6);
	m_Grid.set_row_spacing(6);

	m_Grid.add_property("<b>Format:</b>", document->get_pdf_version_string());
	m_Grid.add_property("<b>Title:</b>", document->get_title());
	m_Grid.add_property("<b>Author:</b>", document->get_author());
	m_Grid.add_property("<b>Subject:</b>", document->get_subject());
	m_Grid.add_property("<b>Keywords:</b>", document->get_keywords());
	m_Grid.add_property("<b>Creator:</b>", document->get_creator());
	m_Grid.add_property("<b>Producer:</b>", document->get_producer());
	m_Grid.add_property("<b>Linearized:</b>", document->is_linearized() ? "Yes" : "No");
	m_Grid.add_property("<b>Creation Date:</b>", format_date(document->get_creation_date()));
	m_Grid.add_property("<b>Modification Date:</b>", format_date(document->get_modification_date()));

	enum_value = g_enum_get_value((GEnumClass*)g_type_class_peek(Glib::Value<Poppler::PageMode>::value_type()),
			document->get_page_mode());
	m_Grid.add_property("<b>Page Mode:</b>", enum_value->value_name);
	enum_value = g_enum_get_value((GEnumClass*)g_type_class_peek(Glib::Value<Poppler::PageLayout>::value_type()),
			document->get_page_layout());
	m_Grid.add_property("<b>Page Layout:</b>", enum_value->value_name);

	Glib::ustring perm_id, up_id;
	if (document->get_id(perm_id, up_id))
	{
		m_Grid.add_property("<b>Permanent ID:</b>", perm_id);
		m_Grid.add_property("<b>Update ID:</b>", up_id);
	}

	// Permissions
	Poppler::Permissions permissions = document->get_permissions();

	m_PrintCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_PRINT);
	m_PermissionsHBoxTop.pack_start(m_PrintCB, false, true);

	m_CopyCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_COPY);
	m_PermissionsHBoxTop.pack_start(m_CopyCB, false, true);

	m_ModifyCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_MODIFY);
	m_PermissionsHBoxTop.pack_start(m_ModifyCB, false, true);

	m_AddNotesCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_ADD_NOTES);
	m_PermissionsHBoxTop.pack_start(m_AddNotesCB, false, true);

	m_FillFormsCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_FILL_FORM);
	m_PermissionsHBoxTop.pack_start(m_FillFormsCB, false, true);

	m_PermissionsVBox.pack_start(m_PermissionsHBoxTop, false, true);

	m_ExtractContentsCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_EXTRACT_CONTENTS);
	m_PermissionsHBoxBottom.pack_start(m_ExtractContentsCB, false, true);

	m_AssembleCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_ASSEMBLE);
	m_PermissionsHBoxBottom.pack_start(m_AssembleCB, false, true);

	m_HighResPrintCB.set_active(permissions & Poppler::PERMISSIONS_OK_TO_PRINT_HIGH_RESOLUTION);
	m_PermissionsHBoxBottom.pack_start(m_HighResPrintCB, false, true);

	m_PermissionsVBox.pack_start(m_PermissionsHBoxBottom, false, true);

	m_Grid.add_property_with_custom_widget("<b>Permissions:</b>", m_PermissionsVBox);

	// Metadata
	m_MetadataWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_MetadataView.set_editable(false);
	m_MetadataView.get_buffer()->set_text(document->get_metadata());
	m_MetadataWin.add(m_MetadataView);
	m_MetadataWin.set_hexpand();
	m_MetadataWin.set_vexpand();
	m_Grid.add_property_with_custom_widget("<b>Metadata:</b>", m_MetadataWin);

	// TODO: view_prefs

	m_Alignment.set_padding(5, 5, 12, 5);
	m_Alignment.add(m_Grid);
	m_Frame.add(m_Alignment);
	pack_start(m_Frame, true, true, 0);

	show_all();
}

}
