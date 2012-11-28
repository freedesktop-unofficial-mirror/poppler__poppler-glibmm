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


#ifndef _AUTH_DIALOG_H_
#define _AUTH_DIALOG_H_

#include <gtkmm/dialog.h>
#include <gtkmm/grid.h>
#include <gtkmm/entry.h>
#include <gtkmm/image.h>

class AuthDialog : public Gtk::Dialog
{
	public:
		AuthDialog(const std::string& filename);

		inline Glib::ustring GetPassword() const { return m_Password; }

	private:
		Glib::ustring m_Password;

		Gtk::HBox m_HBox;
		Gtk::VBox m_VBox;
		Gtk::Image m_Icon;
		Gtk::Label m_ContentLabel, m_PasswordLabel;
		Gtk::Grid m_Grid;
		Gtk::Entry m_PasswordEntry;

		void EntryChanged();
};


#endif // _AUTH_DIALOG_H_
