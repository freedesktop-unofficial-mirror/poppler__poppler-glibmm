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

#ifndef _ACTION_VIEW_H_
#define _ACTION_VIEW_H_

#include "property-grid.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/frame.h>
#include <gtkmm/label.h>
#include <gtkmm/alignment.h>

namespace PGD
{

class ActionView : public Gtk::Frame
{
	public:
		ActionView(const Glib::RefPtr<Poppler::Document>& document);

		void set_action(const Poppler::Action& action);

	private:
		void add_destination(PropertyGrid* grid, const Poppler::Dest& dest, bool remote);
		void play_rendition(const Glib::RefPtr<const Poppler::Media>& media);
		void do_action_layer(const std::vector<Poppler::ActionLayer>& state_list);

		const Glib::RefPtr<Poppler::Document> m_Document;

		Gtk::Label m_Label;
		Gtk::Alignment m_Alignment;
};

class MovieView : public Gtk::Frame
{
	public:
		MovieView(const Glib::RefPtr<const Poppler::Movie>& movie);
		void play_movie();

	private:
		const Glib::RefPtr<const Poppler::Movie> m_Movie;

		Gtk::Label m_Label;
		Gtk::Alignment m_Alignment;
		PropertyGrid m_Grid;
};

}

#endif // _ACTION_VIEW_H_
