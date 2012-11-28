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

#include "property-grid.h"

#include <gtkmm/label.h>

namespace PGD
{

PropertyGrid::PropertyGrid() :
	m_Row(0)
{
}

void PropertyGrid::add_property_with_custom_widget(const Glib::ustring& markup, Gtk::Widget& widget)
{
	Gtk::Label *label = Gtk::manage(new Gtk::Label());
	label->set_alignment(0.0, 0.5);
	label->set_markup(markup);
	attach(*label, 0, m_Row, 1, 1);
	label->show();

	attach(widget, 1, m_Row, 1, 1);
	widget.show();

	++m_Row;
}

void PropertyGrid::add_property_with_value_widget(const Glib::ustring& markup, Gtk::Label*& value_widget,
		const Glib::ustring& value)
{
	Gtk::Label *label = value_widget = Gtk::manage(new Gtk::Label(value));
	label->set_alignment(0.0, 0.5);
	label->set_selectable();
	label->set_ellipsize(Pango::ELLIPSIZE_END);

	add_property_with_custom_widget(markup, *label);
}

void PropertyGrid::add_property(const Glib::ustring& markup, const Glib::ustring& value)
{
	Gtk::Label *label;
	add_property_with_value_widget(markup, label, value);
}

void PropertyGrid::add_row_widget(Gtk::Widget& widget)
{
	attach(widget, 0, m_Row, 2, 1);
	widget.show();

	++m_Row;
}

}
