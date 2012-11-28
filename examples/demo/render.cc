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

#include "render.h"

#include <glibmm/timer.h>

namespace PGD
{

Render::Render(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 6),
	m_Document(document),
	m_Scale(1.0), m_Page(0), m_Rotate(0),
	m_PropertiesVBox(Gtk::ORIENTATION_VERTICAL, 6),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 12), m_HBoxBottom(Gtk::ORIENTATION_HORIZONTAL, 12),
   	m_PageHBox(Gtk::ORIENTATION_HORIZONTAL, 6), m_ScaleHBox(Gtk::ORIENTATION_HORIZONTAL, 6),
   	m_RotateHBox(Gtk::ORIENTATION_HORIZONTAL, 6),
   	m_SliceXHBox(Gtk::ORIENTATION_HORIZONTAL, 6), m_SliceYHBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_SliceWidthHBox(Gtk::ORIENTATION_HORIZONTAL, 6), m_SliceHeightHBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:"), m_ScaleLabel("Scale:"), m_RotateLabel("Rotate:"),
	m_SliceXLabel("x:"), m_SliceYLabel("y:"),
	m_SliceWidthLabel("width:"), m_SliceHeightLabel("height:"),
	m_ScaleSelector(0.1, 1),
	m_PrintingSelector("Printing"),
	m_RenderButton("Render")
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Render::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));
	m_PageHBox.pack_start(m_PageLabel, true, true);
	m_PageHBox.pack_start(m_PageSelector, true, true);
	m_PageHBox.pack_start(m_PageNumLabel, true, true);
	m_HBoxTop.pack_start(m_PageHBox, false, true);

	m_ScaleSelector.set_range(0.0, 10.0);
	m_ScaleSelector.set_increments(0.1, 1.0);
	m_ScaleSelector.set_value(1.0);
	m_ScaleSelector.set_numeric();
	m_ScaleSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Render::scale_selector_changed));
	m_ScaleHBox.pack_start(m_ScaleLabel, true, true);
	m_ScaleHBox.pack_start(m_ScaleSelector, true, true);
	m_HBoxTop.pack_start(m_ScaleHBox, false, true);

	m_RotateSelector.append("0");
	m_RotateSelector.append("90");
	m_RotateSelector.append("180");
	m_RotateSelector.append("270");
	m_RotateSelector.set_active(0);
	m_RotateSelector.signal_changed().connect(sigc::mem_fun(*this, &Render::rotate_selector_changed));
	m_RotateHBox.pack_start(m_RotateLabel, true, true);
	m_RotateHBox.pack_start(m_RotateSelector, true, true);
	m_HBoxTop.pack_start(m_RotateHBox, false, true);

	m_PrintingSelector.signal_toggled().connect(sigc::mem_fun(*this, &Render::printing_selector_changed));
	m_HBoxTop.pack_start(m_PrintingSelector, false, true);

	m_SliceXSelector.set_increments(1, 10);
	m_SliceXSelector.set_numeric();
	m_SliceXSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Render::slice_selector_changed));
	m_SliceXHBox.pack_start(m_SliceXLabel, true, true);
	m_SliceXHBox.pack_start(m_SliceXSelector, true, true);
	m_HBoxBottom.pack_start(m_SliceXHBox, false, true);

	m_SliceYSelector.set_increments(1, 10);
	m_SliceYSelector.set_numeric();
	m_SliceYSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Render::slice_selector_changed));
	m_SliceYHBox.pack_start(m_SliceYLabel, true, true);
	m_SliceYHBox.pack_start(m_SliceYSelector, true, true);
	m_HBoxBottom.pack_start(m_SliceYHBox, false, true);

	m_SliceWidthSelector.set_increments(1, 10);
	m_SliceWidthSelector.set_numeric();
	m_SliceWidthSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Render::slice_selector_changed));
	m_SliceWidthHBox.pack_start(m_SliceWidthLabel, true, true);
	m_SliceWidthHBox.pack_start(m_SliceWidthSelector, true, true);
	m_HBoxBottom.pack_start(m_SliceWidthHBox, false, true);

	m_SliceHeightSelector.set_increments(1, 10);
	m_SliceHeightSelector.set_numeric();
	m_SliceHeightSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Render::slice_selector_changed));
	m_SliceHeightHBox.pack_start(m_SliceHeightLabel, true, true);
	m_SliceHeightHBox.pack_start(m_SliceHeightSelector, true, true);
	m_HBoxBottom.pack_start(m_SliceHeightHBox, false, true);

	slice_selector_setup();

	m_RenderButton.signal_clicked().connect(sigc::mem_fun(*this, &Render::start));
	m_HBoxBottom.pack_end(m_RenderButton, false, true);

	m_TimerLabel.set_markup("<i>No page rendered</i>");
	m_TimerLabel.set_alignment(1.0, 0.5);

	m_PropertiesVBox.pack_start(m_HBoxTop, true, true);
	m_PropertiesVBox.pack_start(m_HBoxBottom, true, true);
	m_PropertiesVBox.pack_start(m_TimerLabel, true, true);

	pack_start(m_PropertiesVBox, false, true, 6);

	m_DrawingArea.signal_draw().connect(sigc::mem_fun(*this, &Render::drawing_area_draw));
	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_ScrolledWin.add(m_DrawingArea);

	pack_start(m_ScrolledWin, true, true);

	show_all();
}

void Render::slice_selector_setup()
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	double width, height;
	page->get_size(width, height);

	m_SliceXSelector.set_range(0, width);
	m_SliceYSelector.set_range(0, height);
	m_SliceWidthSelector.set_range(0, width);
	m_SliceHeightSelector.set_range(0, height);

	m_SliceXSelector.set_value(0);
	m_SliceYSelector.set_value(0);
	m_SliceWidthSelector.set_value(width);
	m_SliceHeightSelector.set_value(height);
}

void Render::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
	slice_selector_setup();
}

void Render::scale_selector_changed()
{
	m_Scale = m_ScaleSelector.get_value();
}

void Render::rotate_selector_changed()
{
	m_Rotate = m_RotateSelector.get_active_row_number() * 90;
}

void Render::printing_selector_changed()
{
	m_Printing = m_PrintingSelector.get_active();
}

void Render::slice_selector_changed()
{
	m_Slice.set_x(m_SliceXSelector.get_value());
	m_Slice.set_y(m_SliceYSelector.get_value());
	m_Slice.set_width(m_SliceWidthSelector.get_value());
	m_Slice.set_height(m_SliceHeightSelector.get_value());
}

void Render::start()
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	double page_width, page_height;
	page->get_size(page_width, page_height);

	double width, height;
	int x, y;
	if (m_Rotate == 0 || m_Rotate == 180)
	{
		width = m_Slice.get_width() * m_Scale;
		height = m_Slice.get_height() * m_Scale;
		x = m_Slice.get_x() * m_Scale;
		y = m_Slice.get_y() * m_Scale;
	}
	else
	{
		width = m_Slice.get_height() * m_Scale;
		height = m_Slice.get_width() * m_Scale;
		x = m_Slice.get_y() * m_Scale;
		y = m_Slice.get_x() * m_Scale;
	}

	Glib::Timer timer;
	m_Surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, width, height);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(m_Surface);

	cr->save();

	switch (m_Rotate)
   	{
		case 90:
			cr->translate(x + width, -y);
			break;
		case 180:
			cr->translate(x + width, y + height);
			break;
		case 270:
			cr->translate(-x, y + height);
			break;
		default:
			cr->translate(-x, -y);
	}

	if (m_Scale != 1.0) cr->scale(m_Scale, m_Scale);
	if (m_Rotate != 0) cr->rotate(m_Rotate * G_PI / 180.0);

	if (m_Printing) page->render_for_printing(cr);
	else page->render(cr);

	cr->restore();

	cr->set_operator(Cairo::OPERATOR_DEST_OVER);
	cr->set_source_rgb(1.0, 1.0, 1.0);
	cr->paint();

	timer.stop();

	m_TimerLabel.set_markup(Glib::ustring::compose("<i>Page rendered in %1 seconds</i>", timer.elapsed()));

	m_DrawingArea.set_size_request(width, height);
	m_DrawingArea.queue_draw();
}

bool Render::drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!m_Surface) return false;

	cr->set_source(m_Surface, 0, 0);
	cr->paint();

	return true;
}

}
