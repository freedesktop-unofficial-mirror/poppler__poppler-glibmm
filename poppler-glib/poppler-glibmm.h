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

#ifndef POPPLER_GLIBMM_H_INCLUDED
#define POPPLER_GLIBMM_H_INCLUDED

/** @mainpage poppler-glibmm Reference Manual
 *
 * @section description Description
 *
 * The poppler-glibmm C++ binding provides a C++ interface on top of the poppler-glib
 * C library.
 *
 * @section overview Overview
 *
 * poppler-glibmm is a C++ interface for the poppler-glib library.  poppler-glib is the Glib
 * interface for Poppler, which is a PDF rendering library based on Xpdf PDF viewer.
 *
 * @section use Use
 *
 * To use poppler-glibmm in your C++ application, include the central header file
 * <tt>\<poppler-glibmm.h\></tt>.  The poppler-glibmm package ships a @c pkg-config
 * file with the correct include path and link command-line for the compiler.
 */

#include <poppler-glibmm/action.h>
#include <poppler-glibmm/annot.h>
#include <poppler-glibmm/annot_callout_line.h>
#include <poppler-glibmm/annot_mapping.h>
#include <poppler-glibmm/attachment.h>
#include <poppler-glibmm/color.h>
#include <poppler-glibmm/document.h>
#include <poppler-glibmm/error.h>
#include <poppler-glibmm/font_info.h>
#include <poppler-glibmm/fonts_iter.h>
#include <poppler-glibmm/form_field.h>
#include <poppler-glibmm/form_field_mapping.h>
#include <poppler-glibmm/image_mapping.h>
#include <poppler-glibmm/index_iter.h>
#include <poppler-glibmm/init.h>
#include <poppler-glibmm/layer.h>
#include <poppler-glibmm/layers_iter.h>
#include <poppler-glibmm/link_mapping.h>
#include <poppler-glibmm/media.h>
#include <poppler-glibmm/movie.h>
#include <poppler-glibmm/page.h>
#include <poppler-glibmm/page_transition.h>
#include <poppler-glibmm/ps_file.h>
#include <poppler-glibmm/rectangle.h>
#include <poppler-glibmm/text_attributes.h>
#include <poppler-glibmm/utility.h>

/** @example demo/main.cc
 */

/** @example demo/demo-window.cc
 */

/** @example demo/info.cc
 */

/** @example demo/fonts.cc
 */

/** @example demo/render.cc
 */

/** @example demo/selections.cc
 */

/** @example demo/page-info.cc
 */

/** @example demo/outline.cc
 */

/** @example demo/links.cc
 */

/** @example demo/links.cc
 */

/** @example demo/forms.cc
 */

/** @example demo/transitions.cc
 */

/** @example demo/images.cc
 */

/** @example demo/annots.cc
 */

/** @example demo/attachments.cc
 */

/** @example demo/layers.cc
 */

/** @example demo/text.cc
 */

/** @example demo/find.cc
 */

/** @example demo/print.cc
 */

#endif /* !POPPLER_GLIBMM_H_INCLUDED */
