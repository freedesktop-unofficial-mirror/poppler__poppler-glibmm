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

#ifndef _SELECTIONS_H_
#define _SELECTIONS_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/colorbutton.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/scrolledwindow.h>

namespace PGD
{

class Selections : public Gtk::Box
{
	public:
		Selections(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void clear_selections();
		void page_selector_changed();
		void scale_selector_changed();
		void fg_color_changed();
		void bg_color_changed();
		bool drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr);
		bool drawing_area_button_press(GdkEventButton* event);
		bool drawing_area_button_release(GdkEventButton* event);
		bool drawing_area_motion_notify(GdkEventMotion* event);
		bool drawing_area_query_tooltip(int x, int y, bool keyboard_tip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);
		void render();
		bool render_selections();
		void copy();

		const Glib::RefPtr<Poppler::Document> m_Document;
		Glib::RefPtr<Poppler::Page> m_Page;
		Poppler::Color m_GlyphColor, m_BackgroundColor;
		Poppler::Rectangle m_DocArea;
		Poppler::SelectionStyle m_Style;
		Gdk::CursorType m_Cursor;
		Gdk::Point m_Start, m_Stop;
		Cairo::RefPtr<Cairo::Surface> m_Surface, m_SelectionSurface;
		Cairo::RefPtr<Cairo::Region> m_SelectionRegion, m_SelectedRegion;
		double m_Scale;
		int m_PageIndex;
		sigc::connection m_SelectionsIdle;
		Glib::ustring m_SelectedText;

		Gtk::Box m_PropertiesVBox, m_HBoxTop, m_HBoxBottom, m_PageHBox, m_ScaleHBox,
			m_FGColorHBox, m_BGColorHBox;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_ScaleLabel,
			m_FGColorLabel, m_BGColorLabel;
		Gtk::SpinButton m_PageSelector, m_ScaleSelector;
		Gtk::Button m_RenderButton, m_CopyButton;
		Gtk::ColorButton m_FGColorButton, m_BGColorButton;
		Gtk::DrawingArea m_DrawingArea;
		Gtk::ScrolledWindow m_ScrolledWin;
};

}

#endif // _SELECTIONS_H_
