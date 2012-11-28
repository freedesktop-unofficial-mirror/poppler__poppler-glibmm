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


#ifndef _PGD_GRID_H_
#define _PGD_GRID_H_

#include <gtkmm/grid.h>

namespace PGD
{

class PropertyGrid : public Gtk::Grid
{
	public:
		PropertyGrid();

		void add_property_with_custom_widget(const Glib::ustring& markup, Gtk::Widget& widget);
		void add_property_with_value_widget(const Glib::ustring& markup, Gtk::Label*& value_widget,
				const Glib::ustring& value);
		void add_property(const Glib::ustring& markup, const Glib::ustring& value);
		void add_row_widget(Gtk::Widget& widget);

	private:
		int m_Row;
};

}

#endif // _PGD_GRID_H_
