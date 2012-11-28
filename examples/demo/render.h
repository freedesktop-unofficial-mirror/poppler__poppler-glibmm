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

#ifndef _RENDER_H_
#define _RENDER_H_

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/scrolledwindow.h>

namespace PGD
{

class Render : public Gtk::Box
{
	public:
		Render(const Glib::RefPtr<Poppler::Document>& document);

	private:
		bool drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr);
		void page_selector_changed();
		void scale_selector_changed();
		void rotate_selector_changed();
		void printing_selector_changed();
		void slice_selector_changed();
		void start();
		void slice_selector_setup();

		const Glib::RefPtr<Poppler::Document> m_Document;
		double m_Scale;
		int m_Page, m_Rotate;
		Gdk::Rectangle m_Slice;
		bool m_Printing;
		Cairo::RefPtr<Cairo::Surface> m_Surface;

		Gtk::Box m_PropertiesVBox, m_HBoxTop, m_HBoxBottom, m_PageHBox, m_ScaleHBox, m_RotateHBox,
			m_SliceXHBox, m_SliceYHBox, m_SliceWidthHBox, m_SliceHeightHBox;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_ScaleLabel, m_RotateLabel,
			m_SliceXLabel, m_SliceYLabel, m_SliceWidthLabel, m_SliceHeightLabel,
			m_TimerLabel;
		Gtk::SpinButton m_PageSelector, m_ScaleSelector,
			m_SliceXSelector, m_SliceYSelector, m_SliceWidthSelector, m_SliceHeightSelector;
		Gtk::ComboBoxText m_RotateSelector;
		Gtk::CheckButton m_PrintingSelector;
		Gtk::Button m_RenderButton;
		Gtk::DrawingArea m_DrawingArea;
		Gtk::ScrolledWindow m_ScrolledWin;
};

}

#endif // _RENDER_H_
