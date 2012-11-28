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

#include "selections.h"

#include <glibmm/main.h>
#include <gdkmm/general.h>
#include <gtkmm/tooltip.h>

namespace PGD
{

Selections::Selections(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 6),
	m_Document(document),
	m_Cursor(Gdk::LAST_CURSOR),
	m_Scale(1.0), m_PageIndex(0),
	m_PropertiesVBox(Gtk::ORIENTATION_VERTICAL, 6),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 12), m_HBoxBottom(Gtk::ORIENTATION_HORIZONTAL, 12),
	m_PageHBox(Gtk::ORIENTATION_HORIZONTAL, 6), m_ScaleHBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_FGColorHBox(Gtk::ORIENTATION_HORIZONTAL, 6), m_BGColorHBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:"), m_ScaleLabel("Scale:"),
	m_FGColorLabel("Foreground Color:"), m_BGColorLabel("Background Color:"),
	m_ScaleSelector(0.1, 1),
	m_RenderButton("Render"),
	m_CopyButton("Copy")
{
	clear_selections();

	int n_pages = document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Selections::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));
	m_PageHBox.pack_start(m_PageLabel, true, true);
	m_PageHBox.pack_start(m_PageSelector, true, true);
	m_PageHBox.pack_start(m_PageNumLabel, true, true);
	m_HBoxTop.pack_start(m_PageHBox, false, true);

	m_ScaleSelector.set_range(0.0, 10.0);
	m_ScaleSelector.set_increments(0.1, 1.0);
	m_ScaleSelector.set_value(1.0);
	m_ScaleSelector.set_numeric();
	m_ScaleSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Selections::scale_selector_changed));
	m_ScaleHBox.pack_start(m_ScaleLabel, true, true);
	m_ScaleHBox.pack_start(m_ScaleSelector, true, true);
	m_HBoxTop.pack_start(m_ScaleHBox, false, true);

	m_FGColorButton.signal_color_set().connect(sigc::mem_fun(*this, &Selections::fg_color_changed));
	m_FGColorHBox.pack_start(m_FGColorLabel, true, true);
	m_FGColorHBox.pack_start(m_FGColorButton, true, true);
	m_HBoxBottom.pack_start(m_FGColorHBox, false, true);

	m_BGColorButton.signal_color_set().connect(sigc::mem_fun(*this, &Selections::bg_color_changed));
	m_BGColorHBox.pack_start(m_BGColorLabel, true, true);
	m_BGColorHBox.pack_start(m_BGColorButton, true, true);
	m_HBoxBottom.pack_start(m_BGColorHBox, false, true);

	m_CopyButton.signal_clicked().connect(sigc::mem_fun(*this, &Selections::copy));
	m_CopyButton.set_sensitive(false);
	m_HBoxBottom.pack_end(m_CopyButton, false, true);

	m_RenderButton.signal_clicked().connect(sigc::mem_fun(*this, &Selections::render));
	m_HBoxBottom.pack_end(m_RenderButton, false, true);

	m_PropertiesVBox.pack_start(m_HBoxTop, true, true);
	m_PropertiesVBox.pack_start(m_HBoxBottom, true, true);

	pack_start(m_PropertiesVBox, false, true, 6);

	m_DrawingArea.signal_draw().connect(sigc::mem_fun(*this, &Selections::drawing_area_draw));
	m_DrawingArea.signal_button_press_event().connect(sigc::mem_fun(*this, &Selections::drawing_area_button_press));
	m_DrawingArea.signal_button_release_event().connect(sigc::mem_fun(*this, &Selections::drawing_area_button_release));
	m_DrawingArea.signal_motion_notify_event().connect(sigc::mem_fun(*this, &Selections::drawing_area_motion_notify));
	m_DrawingArea.signal_query_tooltip().connect(sigc::mem_fun(*this, &Selections::drawing_area_query_tooltip));
	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_ScrolledWin.add(m_DrawingArea);

	m_DrawingArea.add_events(Gdk::POINTER_MOTION_HINT_MASK | Gdk::BUTTON1_MOTION_MASK |
			Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	m_DrawingArea.set_has_tooltip();

	Glib::RefPtr<Gtk::StyleContext> style = m_DrawingArea.get_style_context();
	m_FGColorButton.set_rgba(style->get_color(Gtk::STATE_FLAG_SELECTED));
	m_BGColorButton.set_rgba(style->get_background_color(Gtk::STATE_FLAG_SELECTED));
	fg_color_changed();
	bg_color_changed();

	pack_start(m_ScrolledWin, true, true);

	show_all();
}

void Selections::clear_selections()
{
	m_Start.set_x(-1);

	m_SelectionSurface.clear();
	m_SelectionRegion.clear();
	m_SelectedText.clear();
	m_SelectedRegion.clear();
	m_CopyButton.set_sensitive(false);
}

void Selections::page_selector_changed()
{
	m_PageIndex = m_PageSelector.get_value() - 1;
	if (m_Page) m_Page.reset();
}

void Selections::scale_selector_changed()
{
	m_Scale = m_ScaleSelector.get_value();
}

void Selections::fg_color_changed()
{
	Gdk::RGBA color = m_FGColorButton.get_rgba();
	m_GlyphColor.set_red(color.get_red_u());
	m_GlyphColor.set_green(color.get_green_u());
	m_GlyphColor.set_blue(color.get_blue_u());
}

void Selections::bg_color_changed()
{
	Gdk::RGBA color = m_BGColorButton.get_rgba();
	m_BackgroundColor.set_red(color.get_red_u());
	m_BackgroundColor.set_green(color.get_green_u());
	m_BackgroundColor.set_blue(color.get_blue_u());
}

bool Selections::drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!m_Surface) return false;

	cr->set_source(m_Surface, 0, 0);
	cr->paint();

	if (m_SelectionSurface)
	{
		cr->set_source(m_SelectionSurface, 0, 0);
		cr->paint();
	}

	return true;
}

bool Selections::render_selections()
{
	if (!m_Page || m_Start.get_x() == -1) return false;

	double page_width, page_height;
	m_Page->get_size(page_width, page_height);
	page_width *= m_Scale;
	page_height *= m_Scale;

	Poppler::Rectangle doc_area(m_Start.get_x() / m_Scale, m_Start.get_y() / m_Scale,
			m_Stop.get_x() / m_Scale, m_Stop.get_y() / m_Scale);

	m_SelectionSurface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, page_width, page_height);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(m_SelectionSurface);

	if (m_Scale != 1.0) cr->scale(m_Scale, m_Scale);

	m_Page->render_selection(cr, doc_area, m_DocArea, m_Style, m_GlyphColor, m_BackgroundColor);
	m_DocArea = doc_area;

	m_DrawingArea.queue_draw();

	return false;
}

bool Selections::drawing_area_button_press(GdkEventButton* event)
{
	if (!m_Page || event->button != 1) return false;

	m_Start.set_x(event->x);
	m_Start.set_y(event->y);
	m_Stop = m_Start;

	switch (event->type)
   	{
		case Gdk::DOUBLE_BUTTON_PRESS:
			m_Style = Poppler::SELECTION_WORD;
			break;
		case Gdk::TRIPLE_BUTTON_PRESS:
			m_Style = Poppler::SELECTION_LINE;
			break;
		default:
			m_Style = Poppler::SELECTION_GLYPH;
	}

	render_selections();

	return true;
}

bool Selections::drawing_area_button_release(GdkEventButton* event)
{
	if (!m_Page || event->button != 1) return false;

	if (m_Start.get_x() != -1)
	{
		m_SelectedRegion = m_Page->get_selected_region(1.0, m_Style, m_DocArea);
		m_SelectedText.clear();
		Glib::ustring text = m_Page->get_selected_text(m_Style, m_DocArea);
		if (text.size()) m_SelectedText = text.normalize(Glib::NORMALIZE_NFKC);
		m_CopyButton.set_sensitive(m_SelectedText.size());
	}

	m_Start.set_x(-1);

	if (m_SelectionsIdle.connected()) m_SelectionsIdle.disconnect();

	return true;
}

bool Selections::drawing_area_motion_notify(GdkEventMotion* event)
{
	if (!m_Page) return false;

	if (m_Start.get_x() != -1)
	{
		m_Stop.set_x(event->x);
		m_Stop.set_y(event->y);
		if (m_SelectionsIdle.empty())
			m_SelectionsIdle = Glib::signal_idle().connect(sigc::mem_fun(*this, &Selections::render_selections));
	}
	else
	{
		bool over_text = m_SelectionRegion->contains_point(event->x / m_Scale, event->y / m_Scale);
		Gdk::CursorType cursor_type = over_text ? Gdk::XTERM : Gdk::LAST_CURSOR;
		if (cursor_type != m_Cursor)
		{
			Glib::RefPtr<Gdk::Cursor> cursor;

			if (cursor_type != Gdk::LAST_CURSOR)
				cursor = Gdk::Cursor::create(m_DrawingArea.get_display(), cursor_type);

			m_Cursor = cursor_type;
			m_DrawingArea.get_window()->set_cursor(cursor);
			Gdk::flush();
		}
	}

	return true;
}

bool Selections::drawing_area_query_tooltip(int x, int y, bool, const Glib::RefPtr<Gtk::Tooltip>& tooltip)
{
	if (m_SelectedText.empty() || !m_SelectedRegion || !m_SelectedRegion->contains_point(x / m_Scale, y / m_Scale))
		return false;

	tooltip->set_text(m_SelectedText);

	Cairo::RectangleInt selection_area = m_SelectedRegion->get_extents();
	tooltip->set_tip_area(Gdk::Rectangle(selection_area.x * m_Scale, selection_area.y * m_Scale,
				selection_area.width * m_Scale, selection_area.height * m_Scale));

	return true;
}

void Selections::render()
{
	if (!m_Page) m_Page = m_Document->get_page(m_PageIndex);
	if (!m_Page) return;

	clear_selections();

	double page_width, page_height;
	m_Page->get_size(page_width, page_height);

	m_SelectionRegion =
		m_Page->get_selected_region(1.0, Poppler::SELECTION_GLYPH, Poppler::Rectangle(0, 0, page_width, page_height));

	page_width *= m_Scale;
	page_height *= m_Scale;

	m_Surface = Cairo::ImageSurface::create(Cairo::FORMAT_ARGB32, page_width, page_height);
	Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(m_Surface);

	cr->save();

	if (m_Scale != 1.0) cr->scale(m_Scale, m_Scale);
	m_Page->render(cr);

	cr->restore();

	cr->set_operator(Cairo::OPERATOR_DEST_OVER);
	cr->set_source_rgb(1.0, 1.0, 1.0);
	cr->paint();

	m_DrawingArea.set_size_request(page_width, page_height);
	m_DrawingArea.queue_draw();
}

void Selections::copy()
{
	Gtk::Clipboard::get_for_display(Gdk::Display::get_default())->set_text(m_SelectedText);
}

}
