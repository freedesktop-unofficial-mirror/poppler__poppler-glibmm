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

#include <glibmm_generate_extra_defs/generate_extra_defs.h>
#include <poppler.h>
#include <iostream>

int main(int, char**)
{
	g_type_init();

	std::cout
		<< get_defs(POPPLER_TYPE_ACTION)
		<< get_defs(POPPLER_TYPE_ACTION_LAYER_ACTION)
		<< get_defs(POPPLER_TYPE_ACTION_MOVIE_OPERATION)
		<< get_defs(POPPLER_TYPE_ACTION_TYPE)
		<< get_defs(POPPLER_TYPE_ANNOT)
		<< get_defs(POPPLER_TYPE_ANNOT_CALLOUT_LINE)
		<< get_defs(POPPLER_TYPE_ANNOT_EXTERNAL_DATA_TYPE)
		<< get_defs(POPPLER_TYPE_ANNOT_FILE_ATTACHMENT)
		<< get_defs(POPPLER_TYPE_ANNOT_FLAG)
		<< get_defs(POPPLER_TYPE_ANNOT_FREE_TEXT)
		<< get_defs(POPPLER_TYPE_ANNOT_FREE_TEXT_QUADDING)
		<< get_defs(POPPLER_TYPE_ANNOT_MAPPING)
		<< get_defs(POPPLER_TYPE_ANNOT_MARKUP)
		<< get_defs(POPPLER_TYPE_ANNOT_MARKUP_REPLY_TYPE)
		<< get_defs(POPPLER_TYPE_ANNOT_MOVIE)
		<< get_defs(POPPLER_TYPE_ANNOT_SCREEN)
		<< get_defs(POPPLER_TYPE_ANNOT_TEXT)
		<< get_defs(POPPLER_TYPE_ANNOT_TEXT_STATE)
		<< get_defs(POPPLER_TYPE_ANNOT_TYPE)
		<< get_defs(POPPLER_TYPE_ATTACHMENT)
		<< get_defs(POPPLER_TYPE_BACKEND)
		<< get_defs(POPPLER_TYPE_COLOR)
		<< get_defs(POPPLER_TYPE_DEST)
		<< get_defs(POPPLER_TYPE_DEST_TYPE)
		<< get_defs(POPPLER_TYPE_DOCUMENT)
		<< get_defs(POPPLER_TYPE_ERROR)
		<< get_defs(POPPLER_TYPE_FIND_FLAGS)
		<< get_defs(POPPLER_TYPE_FONTS_ITER)
		<< get_defs(POPPLER_TYPE_FONT_INFO)
		<< get_defs(POPPLER_TYPE_FONT_TYPE)
		<< get_defs(POPPLER_TYPE_FORM_BUTTON_TYPE)
		<< get_defs(POPPLER_TYPE_FORM_CHOICE_TYPE)
		<< get_defs(POPPLER_TYPE_FORM_FIELD)
		<< get_defs(POPPLER_TYPE_FORM_FIELD_MAPPING)
		<< get_defs(POPPLER_TYPE_FORM_FIELD_TYPE)
		<< get_defs(POPPLER_TYPE_FORM_TEXT_TYPE)
		<< get_defs(POPPLER_TYPE_IMAGE_MAPPING)
		<< get_defs(POPPLER_TYPE_INDEX_ITER)
		<< get_defs(POPPLER_TYPE_LAYER)
		<< get_defs(POPPLER_TYPE_LAYERS_ITER)
		<< get_defs(POPPLER_TYPE_LINK_MAPPING)
		<< get_defs(POPPLER_TYPE_MEDIA)
		<< get_defs(POPPLER_TYPE_MOVIE)
		<< get_defs(POPPLER_TYPE_ORIENTATION)
		<< get_defs(POPPLER_TYPE_PAGE)
		<< get_defs(POPPLER_TYPE_PAGE_LAYOUT)
		<< get_defs(POPPLER_TYPE_PAGE_MODE)
		<< get_defs(POPPLER_TYPE_PAGE_TRANSITION)
		<< get_defs(POPPLER_TYPE_PAGE_TRANSITION_ALIGNMENT)
		<< get_defs(POPPLER_TYPE_PAGE_TRANSITION_DIRECTION)
		<< get_defs(POPPLER_TYPE_PAGE_TRANSITION_TYPE)
		<< get_defs(POPPLER_TYPE_PERMISSIONS)
		<< get_defs(POPPLER_TYPE_PRINT_FLAGS)
		<< get_defs(POPPLER_TYPE_PS_FILE)
		<< get_defs(POPPLER_TYPE_RECTANGLE)
		<< get_defs(POPPLER_TYPE_SELECTION_STYLE)
		<< get_defs(POPPLER_TYPE_TEXT_ATTRIBUTES)
		<< get_defs(POPPLER_TYPE_VIEWER_PREFERENCES)
		<< std::endl;

	return 0;
}
