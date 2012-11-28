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

#include "forms.h"
#include "property-grid.h"
#include "action-view.h"

#include <gtkmm/textview.h>
#include <glibmm/timer.h>
#include <iomanip>

namespace PGD
{

FormFieldView::FormFieldView(const Glib::RefPtr<Poppler::Document>& document) :
	m_Document(document),
	m_Alignment(0.5, 0.5, 1, 1)
{
	set_shadow_type(Gtk::SHADOW_NONE);
	m_Label.set_markup("<b>Form Field Properties</b>");
	set_label_widget(m_Label);

	m_Alignment.set_padding(5, 5, 12, 5);
	add(m_Alignment);
}

void FormFieldView::set_field(const Glib::RefPtr<Poppler::FormField>& field)
{
	PropertyGrid* grid = static_cast<PropertyGrid*>(m_Alignment.get_child());
	if (grid)
	{
		m_Alignment.remove();
		delete grid;
	}

	if (!field) return;

	m_Alignment.add(*Gtk::manage(grid = new PropertyGrid));

	grid->set_column_spacing(6);
	grid->set_row_spacing(6);

	if (field->get_name().size()) grid->add_property("<b>Name:</b>", field->get_name());
	if (field->get_partial_name().size()) grid->add_property("<b>Partial Name:</b>", field->get_partial_name());
	if (field->get_mapping_name().size()) grid->add_property("<b>Mapping Name:</b>", field->get_mapping_name());

	Poppler::Action action = field->get_action();
	if (action)
	{
		ActionView* action_view;
		grid->add_property_with_custom_widget("<b>Action:</b>", *Gtk::manage(action_view = new ActionView(m_Document)));
		action_view->set_action(action);
	}

	switch (field->get_field_type())
	{
		case Poppler::FORM_FIELD_BUTTON:
			{
				GEnumValue* enum_value = g_enum_get_value((GEnumClass*)g_type_class_ref(
							Glib::Value<Poppler::FormButtonType>::value_type()), field->button_get_button_type());
				grid->add_property("<b>Button Type:</b>", enum_value->value_name);
				grid->add_property("<b>Button State:</b>", field->button_get_state() ? "Active" : "Inactive");
			}
			break;
		case Poppler::FORM_FIELD_TEXT:
			{
				GEnumValue* enum_value = g_enum_get_value((GEnumClass*)g_type_class_ref(
							Glib::Value<Poppler::FormTextType>::value_type()), field->text_get_text_type());
				grid->add_property("<b>Text Type:</b>", enum_value->value_name);
				grid->add_property("<b>Contents:</b>", field->text_get_text());
				grid->add_property("<b>Max Length:</b>", Glib::ustring::format(field->text_get_max_len()));
				grid->add_property("<b>Do spellcheck:</b>", field->text_do_spell_check() ? "Yes" : "No");
				grid->add_property("<b>Do scroll:</b>", field->text_do_scroll() ? "Yes" : "No");
				grid->add_property("<b>Rich Text:</b>", field->text_is_rich_text() ? "Yes" : "No");
				grid->add_property("<b>Pasword type:</b>", field->text_is_password() ? "Yes" : "No");
			}
			break;
		case Poppler::FORM_FIELD_CHOICE:
			{
				GEnumValue* enum_value = g_enum_get_value((GEnumClass*)g_type_class_ref(
							Glib::Value<Poppler::FormChoiceType>::value_type()), field->choice_get_choice_type());
				grid->add_property("<b>Choice Type:</b>", enum_value->value_name);
				grid->add_property("<b>Editable:</b>", field->choice_is_editable() ? "Yes" : "No");
				grid->add_property("<b>Multiple Selection:</b>", field->choice_can_select_multiple() ? "Yes" : "No");
				grid->add_property("<b>Do spellcheck:</b>", field->choice_do_spell_check() ? "Yes" : "No");
				grid->add_property("<b>Commit on Change:</b>", field->choice_commit_on_change() ? "Yes" : "No");
				int n_items = field->choice_get_n_items();
				grid->add_property("<b>Number of items:</b>", Glib::ustring::format(n_items));

				Gtk::ScrolledWindow* swindow;
				grid->add_property_with_custom_widget("<b>Items:</b>", *Gtk::manage(swindow = new Gtk::ScrolledWindow));
				swindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
				Gtk::TextView* textview;
				swindow->add(*Gtk::manage(textview = new Gtk::TextView));
				textview->set_editable(false);
				Glib::RefPtr<Gtk::TextBuffer> buffer = textview->get_buffer();
				int selected = -1;
				for (int i = 0; i < n_items; ++i)
				{
					Glib::ustring item = field->choice_get_item(i);
					buffer->insert_at_cursor(item);
					buffer->insert_at_cursor("\n");
					if (field->choice_is_item_selected(i)) selected = i;
				}
				if (selected >= 0 && n_items > selected)
					grid->add_property("<b>Selected item:</b>",
							Glib::ustring::compose("%1 (%2)", selected, field->choice_get_item(selected)));

				grid->add_property("<b>Contents:</b>", field->choice_get_text());
			}
			break;
		case Poppler::FORM_FIELD_SIGNATURE:
		case Poppler::FORM_FIELD_UNKNOWN:
			break;
	}
	grid->show_all();
}

Forms::Forms(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
   	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:"),
	m_GetButton("Get Forms Fields"),
	m_HPaned(Gtk::ORIENTATION_HORIZONTAL),
	m_FormFieldView(document)
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Forms::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));

	m_GetButton.signal_clicked().connect(sigc::mem_fun(*this, &Forms::get_form_fields));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);
	m_HBoxTop.pack_end(m_GetButton, false, false);

	m_TimerLabel.set_markup("<i>No form fields found</i>");
	m_TimerLabel.set_alignment(1.0, 0.5);

	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_FieldStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.set_model(m_FieldStore);

	m_TreeView.append_column("Form Field Type", m_StoreColumns.m_FieldType);
	m_TreeView.append_column("Form Field Id", m_StoreColumns.m_Id);
	m_TreeView.append_column("Read Only", m_StoreColumns.m_ReadOnly);
	m_TreeView.append_column_numeric("X1", m_StoreColumns.m_X1, "%.2f");
	m_TreeView.append_column_numeric("Y1", m_StoreColumns.m_Y1, "%.2f");
	m_TreeView.append_column_numeric("X2", m_StoreColumns.m_X2, "%.2f");
	m_TreeView.append_column_numeric("Y2", m_StoreColumns.m_Y2, "%.2f");

	m_TreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Forms::selection_changed));

	m_ScrolledWin.add(m_TreeView);

	m_HPaned.add1(m_ScrolledWin);
	m_HPaned.add2(m_FormFieldView);
	m_HPaned.set_position(300);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_TimerLabel, false, true);
	pack_start(m_HPaned, true, true);

	show_all();
}

void Forms::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
}

static Glib::ustring get_form_field_type(const Glib::RefPtr<Poppler::FormField>& field)
{
	switch (field->get_field_type())
	{
		case Poppler::FORM_FIELD_TEXT:
			return "Text";
		case Poppler::FORM_FIELD_BUTTON:
			return "Button";
		case Poppler::FORM_FIELD_CHOICE:
			return "Choice";
		case Poppler::FORM_FIELD_SIGNATURE:
			return "Signature";
		case Poppler::FORM_FIELD_UNKNOWN:
			break;
	}
	return "Unknown";
}

void Forms::get_form_fields()
{
	m_FieldStore->clear();

	m_FormFieldView.set_field(Glib::RefPtr<Poppler::FormField>());

	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	Glib::Timer timer;
	std::vector<Poppler::FormFieldMapping> mapping = page->get_form_field_mapping();
	timer.stop();
	
	int n_fields = mapping.size();

	if (n_fields > 0)
		m_TimerLabel.set_markup(Glib::ustring::format("<i>", n_fields, " form fields found in ",
					std::fixed, std::setprecision(4), timer.elapsed(), " seconds</i>"));
	else
		m_TimerLabel.set_markup("<i>No form fields found</i>");

	for (std::vector<Poppler::FormFieldMapping>::iterator fmapping = mapping.begin();
			fmapping != mapping.end(); ++fmapping)
	{
		Gtk::TreeRow tree_row = *(m_FieldStore->append());

		Glib::RefPtr<Poppler::FormField> field = fmapping->get_field();
		tree_row[m_StoreColumns.m_FieldType] = get_form_field_type(field);
		tree_row[m_StoreColumns.m_Id] = field->get_id();
		tree_row[m_StoreColumns.m_ReadOnly] = field->is_read_only();

		Poppler::Rectangle area = fmapping->get_area();
		tree_row[m_StoreColumns.m_X1] = area.get_x1();
		tree_row[m_StoreColumns.m_Y1] = area.get_y1();
		tree_row[m_StoreColumns.m_X2] = area.get_x2();
		tree_row[m_StoreColumns.m_Y2] = area.get_y2();

		tree_row[m_StoreColumns.m_Field] = field;
	}
}

void Forms::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter) m_FormFieldView.set_field((*iter)[m_StoreColumns.m_Field]);
	else m_FormFieldView.set_field(Glib::RefPtr<Poppler::FormField>());
}

}
