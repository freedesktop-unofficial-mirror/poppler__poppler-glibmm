// -*- c++ -*-
/* $Id: init.cc 89 2012-09-20 04:46:54Z glennricster $ */

/* Copyright (c) 2010  Glenn Rice <glennricster@gmail.com>
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

#include <glibmm/init.h>
#include <poppler-glibmm/wrap_init.h>

namespace Poppler
{

void init()
{
	Glib::init();
	Poppler::wrap_init();  
}

} // namespace Poppler

