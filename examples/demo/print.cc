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

#include "print.h"

#include <gtkmm/messagedialog.h>

namespace PGD
{

const char* Print::m_PrintOptions = "pgd-print-options";

Print::Print(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_HBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PrintButton("Print...")
{
	m_PrintButton.signal_clicked().connect(sigc::mem_fun(*this, &Print::print));
	m_HBox.pack_end(m_PrintButton, false, false);
	pack_start(m_HBox, false, true);
	show_all();
}

void Print::print()
{
	m_PrintOp = Gtk::PrintOperation::create();
	m_PrintOp->set_custom_tab_label("PDF Options");
	m_PrintOp->signal_begin_print().connect(sigc::mem_fun(*this, &Print::begin_print));
	m_PrintOp->signal_draw_page().connect(sigc::mem_fun(*this, &Print::draw_page));
	m_PrintOp->signal_create_custom_widget().connect(sigc::mem_fun(*this, &Print::create_custom_widget));
	m_PrintOp->signal_custom_widget_apply().connect(sigc::mem_fun(*this, &Print::custom_widget_apply));
	try
	{
		m_PrintOp->run();
	}
	catch (Glib::Error exception)
	{
		Gtk::MessageDialog dialog(exception.what(), false, Gtk::MESSAGE_ERROR, Gtk::BUTTONS_CLOSE);
		dialog.run();
	}
}

void Print::begin_print(const Glib::RefPtr<Gtk::PrintContext>& /* context */)
{
	m_PrintOp->set_n_pages(m_Document->get_n_pages());
}

void Print::draw_page(const Glib::RefPtr<Gtk::PrintContext>& context, int page_nr)
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(page_nr);
	if (!page) return;

	Poppler::PrintFlags flags = Poppler::PrintFlags(0);
	int options = m_PrintOp->get_print_settings()->get_int_with_default(m_PrintOptions, PGD::PRINT_DOCUMENT_MARKUPS);
	switch (options)
	{
		case PGD::PRINT_DOCUMENT:
			flags |= Poppler::PRINT_DOCUMENT;
			break;
		case PGD::PRINT_DOCUMENT_MARKUPS:
			flags |= Poppler::PRINT_MARKUP_ANNOTS;
			break;
		case PGD::PRINT_DOCUMENT_STAMPS:
			flags |= Poppler::PRINT_STAMP_ANNOTS_ONLY;
			break;
	}

	Cairo::RefPtr<Cairo::Context> cr = context->get_cairo_context();
	page->render_for_printing_with_options(cr, flags);
}

Gtk::Widget* Print::create_custom_widget()
{
	Gtk::Box* hbox = Gtk::manage(new Gtk::Box(Gtk::ORIENTATION_HORIZONTAL));
	hbox->set_border_width(12);
	hbox->pack_start(*Gtk::manage(new Gtk::Label("Print: ")), false, false);

	Gtk::ComboBoxText* combo;
	hbox->pack_start(*Gtk::manage(combo = new Gtk::ComboBoxText), false, false);
	combo->append("Document");
	combo->append("Document and markup");
	combo->append("Document and stamps");

	int options = m_PrintOp->get_print_settings()->get_int_with_default(m_PrintOptions, PGD::PRINT_DOCUMENT_MARKUPS);
	combo->set_active(options);

	hbox->show_all();

	return hbox;
}

void Print::custom_widget_apply(Gtk::Widget* widget)
{
	Gtk::ComboBoxText* combo = static_cast<Gtk::ComboBoxText*>(widget);
	m_PrintOp->get_print_settings()->set_int(m_PrintOptions, combo->get_active_row_number());
}

}
