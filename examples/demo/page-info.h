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

#ifndef _PAGE_H_
#define _PAGE_H_

#include "property-grid.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/frame.h>
#include <gtkmm/alignment.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/button.h>

namespace PGD
{

class PageInfo : public Gtk::Box
{
	public:
		PageInfo(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void page_selector_changed();
		void get_info();

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_Page;
		Gtk::Label* m_Index;
		Gtk::Label* m_Label;
		Gtk::Label* m_Size;
		Gtk::Label* m_Duration;
		Gtk::Image m_Thumbnail;
		Gtk::Label m_ThumbnailSize;

		Gtk::Box m_HBoxTop, m_HBoxBottom, m_ThumbnailVBox;
		Gtk::Frame m_PropertiesFrame, m_ThumbnailFrame;
		PGD::PropertyGrid m_Grid;
		Gtk::Alignment m_PropertiesAlignment, m_ThumbnailAlignment;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_PropertiesFrameLabel, m_ThumbnailFrameLabel;
		Gtk::SpinButton m_PageSelector;
		Gtk::Button m_GetInfoButton;
};

}

#endif // _PAGE_H_
