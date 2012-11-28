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

#include "auth-dialog.h"

#include <gtkmm/stock.h>

AuthDialog::AuthDialog(const std::string& filename) :
	m_HBox(false, 12),
	m_VBox(false, 18),
	m_Icon(Gtk::Stock::DIALOG_AUTHENTICATION, Gtk::ICON_SIZE_DIALOG),
	m_PasswordLabel("_Password:", 0.0, 0.5, true)
{
	set_border_width(5);
	get_content_area()->set_spacing(2);
	get_action_area()->set_border_width(5);
	get_action_area()->set_spacing(6);

	set_title("Enter password");
	set_resizable(false);
	set_icon_name(Gtk::Stock::DIALOG_AUTHENTICATION.id);
	set_modal();

	add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	add_button("_Unlock Document", Gtk::RESPONSE_OK);
	set_response_sensitive(Gtk::RESPONSE_OK, false);
	set_default_response(Gtk::RESPONSE_OK);

	std::vector<int> button_order;
	button_order.push_back(Gtk::RESPONSE_OK);
	button_order.push_back(Gtk::RESPONSE_CANCEL);
	set_alternative_button_order_from_array(button_order);

	m_HBox.set_border_width(5);
	get_content_area()->pack_start(m_HBox, true, true);

	m_Icon.set_alignment(0.5, 0.0);
	m_HBox.pack_start(m_Icon, false, false);

	m_HBox.pack_start(m_VBox, true, true);

	m_ContentLabel.set_alignment(0.0, 0.5);
	m_ContentLabel.set_line_wrap();
	m_ContentLabel.set_max_width_chars(45);
	m_ContentLabel.set_justify(Gtk::JUSTIFY_FILL);

	m_ContentLabel.set_markup("<span size=\"larger\" weight=\"bold\">Password required</span>\n\n"
			"The document “" + filename + "” is locked and requires a password before it can be opened.");

	m_VBox.pack_start(m_ContentLabel, false, false);

	m_Grid.set_column_spacing(12);
	m_Grid.set_row_spacing(6);

	m_PasswordEntry.set_visibility(false);
	m_PasswordEntry.signal_changed().connect(sigc::mem_fun(*this, &AuthDialog::EntryChanged));
	m_PasswordEntry.set_activates_default();
	m_PasswordEntry.set_hexpand();
	m_PasswordLabel.set_mnemonic_widget(m_PasswordEntry);

	m_Grid.attach(m_PasswordLabel, 0, 0, 1, 1);
	m_Grid.attach(m_PasswordEntry, 1, 0, 1, 1);

	m_VBox.pack_start(m_Grid, false, false);

	show_all_children();
}

void AuthDialog::EntryChanged()
{
	m_Password = m_PasswordEntry.get_text();
	set_response_sensitive(Gtk::RESPONSE_OK, m_Password.size());
}
