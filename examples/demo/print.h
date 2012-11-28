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

#ifndef _PRINT_H_
#define _PRINT_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/printoperation.h>

namespace PGD
{

typedef enum
{
	PRINT_DOCUMENT,
	PRINT_DOCUMENT_MARKUPS,
	PRINT_DOCUMENT_STAMPS
} PrintOptions;

class Print : public Gtk::Box
{
	public:
		Print(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void print();
		void begin_print(const Glib::RefPtr<Gtk::PrintContext>& context);
		void draw_page(const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr);
		Gtk::Widget* create_custom_widget();
		void custom_widget_apply(Gtk::Widget* widget);

		const Glib::RefPtr<Poppler::Document> m_Document;

		Gtk::Box m_HBox;
		Gtk::Button m_PrintButton;
		Glib::RefPtr<Gtk::PrintOperation> m_PrintOp;

		static const char* m_PrintOptions;
};

}

#endif // _PRINT_H_
