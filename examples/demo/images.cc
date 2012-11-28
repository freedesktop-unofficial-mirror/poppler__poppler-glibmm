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

#include "images.h"

#include <glibmm/timer.h>
#include <iomanip>

namespace PGD
{

ImageView::ImageView(const Glib::RefPtr<Poppler::Document>& document) :
	m_Document(document)
{
	m_DrawingArea.signal_draw().connect(sigc::mem_fun(*this, &ImageView::drawing_area_draw));
	set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	add(m_DrawingArea);
}

bool ImageView::drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!m_Image) return false;

	m_DrawingArea.set_size_request(m_Image->get_width(), m_Image->get_height());
	cr->set_source(m_Image, 0, 0);
	cr->paint();

	return true;
}

void ImageView::set_image(const Cairo::RefPtr<Cairo::ImageSurface>& image)
{
	m_Image = image;
	m_DrawingArea.queue_draw();
}

Images::Images(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:"),
	m_GetButton("Get Images"),
	m_HPaned(Gtk::ORIENTATION_HORIZONTAL),
	m_ImageView(document)
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Images::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));

	m_GetButton.signal_clicked().connect(sigc::mem_fun(*this, &Images::get_images));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);
	m_HBoxTop.pack_end(m_GetButton, false, false);

	m_TimerLabel.set_markup("<i>No images found</i>");
	m_TimerLabel.set_alignment(1.0, 0.5);

	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_ImageStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.set_model(m_ImageStore);

	m_TreeView.append_column_numeric("Image", m_StoreColumns.m_Id, "%d");
	m_TreeView.append_column_numeric("X1", m_StoreColumns.m_X1, "%.2f");
	m_TreeView.append_column_numeric("Y1", m_StoreColumns.m_Y1, "%.2f");
	m_TreeView.append_column_numeric("X2", m_StoreColumns.m_X2, "%.2f");
	m_TreeView.append_column_numeric("Y2", m_StoreColumns.m_Y2, "%.2f");

	m_TreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Images::selection_changed));

	m_ScrolledWin.add(m_TreeView);

	m_HPaned.add1(m_ScrolledWin);
	m_HPaned.add2(m_ImageView);
	m_HPaned.set_position(300);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_TimerLabel, false, true);
	pack_start(m_HPaned, true, true);

	show_all();
}

void Images::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
}

void Images::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter) m_ImageView.set_image(m_Document->get_page(m_Page)->get_image((*iter)[m_StoreColumns.m_Id]));
	else m_ImageView.set_image(Cairo::RefPtr<Cairo::ImageSurface>());
}

void Images::get_images()
{
	m_ImageStore->clear();

	m_ImageView.set_image(Cairo::RefPtr<Cairo::ImageSurface>());

	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	Glib::Timer timer;
	std::vector<Poppler::ImageMapping> mapping = page->get_image_mapping();
	timer.stop();

	int n_links = mapping.size();

	if (n_links > 0)
		m_TimerLabel.set_markup(Glib::ustring::format("<i>", n_links, " images found in ",
					std::fixed, std::setprecision(4), timer.elapsed(), " seconds</i>"));
	else
		m_TimerLabel.set_markup("<i>No images found</i>");

	for (std::vector<Poppler::ImageMapping>::iterator imapping = mapping.begin();
			imapping != mapping.end(); ++imapping)
	{
		Gtk::TreeRow tree_row = *(m_ImageStore->append());

		tree_row[m_StoreColumns.m_Id] = imapping->get_image_id();

		Poppler::Rectangle area = imapping->get_area();
		tree_row[m_StoreColumns.m_X1] = area.get_x1();
		tree_row[m_StoreColumns.m_Y1] = area.get_y1();
		tree_row[m_StoreColumns.m_X2] = area.get_x2();
		tree_row[m_StoreColumns.m_Y2] = area.get_y2();
	}
}

}
