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

#include "page-info.h"

#include <gtkmm/stock.h>
#include <iomanip>

namespace PGD
{

PageInfo::PageInfo(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6), m_HBoxBottom(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_ThumbnailVBox(Gtk::ORIENTATION_VERTICAL, 6),
	m_PropertiesAlignment(0.5, 0.5, 1, 1), m_ThumbnailAlignment(0.5, 0.5, 1, 1),
	m_PageLabel("Page:"),
	m_GetInfoButton("Get Info")
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &PageInfo::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));
	m_GetInfoButton.signal_clicked().connect(sigc::mem_fun(*this, &PageInfo::get_info));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);
	m_HBoxTop.pack_end(m_GetInfoButton, false, false);

	// Page Properties
	m_PropertiesFrame.set_shadow_type(Gtk::SHADOW_NONE);
	m_PropertiesFrameLabel.set_markup("<b>Page properties</b>");
	m_PropertiesFrame.set_label_widget(m_PropertiesFrameLabel);

	m_Grid.set_column_spacing(6);
	m_Grid.set_row_spacing(6);

	m_Grid.add_property_with_value_widget("<b>Page Index:</b>", m_Index, "");
	m_Grid.add_property_with_value_widget("<b>Page Label:</b>", m_Label, "");
	m_Grid.add_property_with_value_widget("<b>Page Size:</b>", m_Size, "");
	m_Grid.add_property_with_value_widget("<b>Page Duration:</b>", m_Duration, "");

	// Page Thumbnail
	m_ThumbnailFrame.set_shadow_type(Gtk::SHADOW_NONE);
	m_ThumbnailFrame.set_label_align(0.5, 0.5);
	m_ThumbnailFrameLabel.set_markup("<b>Page Thumbnail</b>");
	m_ThumbnailFrame.set_label_widget(m_ThumbnailFrameLabel);

	m_ThumbnailSize.set_alignment(0.5, 0.5);
	m_ThumbnailVBox.pack_start(m_Thumbnail, true, true);
	m_ThumbnailVBox.pack_start(m_ThumbnailSize, true, true);

	m_PropertiesAlignment.set_padding(5, 5, 12, 5);
	m_PropertiesAlignment.add(m_Grid);
	m_PropertiesFrame.add(m_PropertiesAlignment);
	m_HBoxBottom.pack_start(m_PropertiesFrame, true, true);

	m_ThumbnailAlignment.set_padding(5, 5, 12, 5);
	m_ThumbnailAlignment.add(m_ThumbnailVBox);
	m_ThumbnailFrame.add(m_ThumbnailAlignment);
	m_HBoxBottom.pack_start(m_ThumbnailFrame, true, true);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_HBoxBottom, false, true);

	show_all();
}

void PageInfo::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
}

void PageInfo::get_info()
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);

	if (page)
	{
		m_Index->set_text(Glib::ustring::format(page->get_index()));
		m_Label->set_text(page->get_label());

		double width, height;
		page->get_size(width, height);
		m_Size->set_text(Glib::ustring::format(std::fixed, std::setprecision(2), width, " x ", height));

		m_Duration->set_text(Glib::ustring::format(std::fixed, std::setprecision(2), page->get_duration(), " seconds"));
	}
	else
	{
		m_Index->set_text("");
		m_Label->set_text("");
		m_Size->set_text("");
		m_Duration->set_text("");
	}

	Cairo::RefPtr<Cairo::ImageSurface> thumbnail = page ? page->get_thumbnail() : Cairo::RefPtr<Cairo::ImageSurface>();
	if (thumbnail)
	{
		int width, height;
		page->get_thumbnail_size(width, height);
		m_ThumbnailSize.set_text(Glib::ustring::format(width, " x ", height));

		width = thumbnail->get_width();
		height = thumbnail->get_height();
		Cairo::Format surface_format = thumbnail->get_format();
		bool has_alpha = (surface_format == Cairo::FORMAT_ARGB32);

		Glib::RefPtr<Gdk::Pixbuf> pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, true, 8, width, height);
		int pixbuf_n_channels = pixbuf->get_n_channels();
		int pixbuf_rowstride = pixbuf->get_rowstride();
		guint8* pixbuf_pixels = pixbuf->get_pixels();

		Cairo::RefPtr<Cairo::ImageSurface> image = Cairo::ImageSurface::create(pixbuf_pixels,
				surface_format, width, height, pixbuf_rowstride);
		Cairo::RefPtr<Cairo::Context> cr = Cairo::Context::create(image);
		cr->set_source(thumbnail, 0, 0);

		if (has_alpha) cr->mask(thumbnail, 0, 0);
		else cr->paint();

		for (int y = 0; y < height; ++y)
		{
			guint8* p = pixbuf_pixels + y * pixbuf_rowstride;

			for (int x = 0; x < width; ++x)
			{
#if G_BYTE_ORDER == G_LITTLE_ENDIAN
				std::swap(p[0], p[2]);
				p[3] = (has_alpha) ? p[3] : 0xff;
#else
				guint8 tmp = p[0];
				p[0] = (has_alpha) ? p[3] : 0xff;
				p[3] = p[2];
				p[2] = p[1];
				p[1] = tmp;
#endif			
				p += pixbuf_n_channels;
			}
		}

		m_Thumbnail.set(pixbuf);
	}
	else
	{
		m_ThumbnailSize.set_markup("<i>No thumbnail found</i>");
		m_Thumbnail.set(Gtk::Stock::MISSING_IMAGE, Gtk::ICON_SIZE_DIALOG);
	}
}

}
