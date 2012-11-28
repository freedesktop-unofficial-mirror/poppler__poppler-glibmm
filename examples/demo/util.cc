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

#include "util.h"

#include <glibmm/convert.h>
#include <glibmm/date.h>
#include <time.h>

namespace PGD
{

Glib::ustring format_date(time_t utime)
{
	Glib::Date date;
	date.set_time(utime);
	return date.format_string("%c");
}

Glib::RefPtr<Gdk::Pixbuf> pixbuf_new_for_color(const Poppler::Color& poppler_color)
{
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;

	if (!poppler_color) return pixbuf;

	pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, 64, 16);

	guint8* pixels = pixbuf->get_pixels();
	int num = pixbuf->get_width() * pixbuf->get_height();

	for (int i = 0; i < num; ++i)
   	{
		pixels[0] = poppler_color.get_red();
		pixels[1] = poppler_color.get_green();
		pixels[2] = poppler_color.get_blue();
		pixels += 3;
	}

	return pixbuf;
}

}
