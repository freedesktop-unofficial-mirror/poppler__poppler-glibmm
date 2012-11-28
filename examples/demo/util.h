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


#ifndef _UTIL_H_
#define _UTIL_H_

#include <glibmm/ustring.h>
#include <gdkmm/pixbuf.h>
#include <poppler-glibmm/color.h>

namespace PGD
{

Glib::ustring format_date(time_t utime);
Glib::RefPtr<Gdk::Pixbuf> pixbuf_new_for_color(const Poppler::Color& poppler_color);

}

#endif // _UTIL_H_
