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

#include "action-view.h"

#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <glibmm/fileutils.h>
#include <gtkmm/stock.h>
#include <gtkmm/button.h>
#include <gtkmm/textview.h>
#include <gtkmm/scrolledwindow.h>
#include <iomanip>

namespace PGD
{

ActionView::ActionView(const Glib::RefPtr<Poppler::Document>& document) :
	m_Document(document),
	m_Alignment(0.5, 0.5, 1, 1)
{
	set_shadow_type(Gtk::SHADOW_NONE);
	m_Label.set_markup("<b>Action Properties</b>");
	set_label_widget(m_Label);

	m_Alignment.set_padding(5, 5, 12, 5);
	add(m_Alignment);
}

void ActionView::add_destination(PropertyGrid* grid, const Poppler::Dest& dest, bool remote)
{
	grid->add_property("<b>Type:</b>", "Destination");

	GEnumValue* enum_value =
		g_enum_get_value((GEnumClass*)g_type_class_ref(Glib::Value<Poppler::DestType>::value_type()),
				dest.get_dest_type());
	grid->add_property("<b>Destination Type:</b>", enum_value->value_name);

	if (dest.get_dest_type() != Poppler::DEST_NAMED)
	{
		Glib::ustring str;
		if (m_Document && !remote)
		{
			Glib::RefPtr<Poppler::Page> poppler_page = m_Document->get_page(std::max(0, dest.get_page_num() - 1));
			Glib::ustring page_label = poppler_page->get_label();
			if (page_label.size()) str = Glib::ustring::compose("%1 (%2)", dest.get_page_num(), page_label);
		}
		if (str.empty()) str = Glib::ustring::format(dest.get_page_num());
		grid->add_property("<b>Page:</b>", str);

		grid->add_property("<b>Left:</b>", Glib::ustring::format(std::fixed, std::setprecision(2), dest.get_left()));
		grid->add_property("<b>Right:</b>", Glib::ustring::format(std::fixed, std::setprecision(2), dest.get_right()));
		grid->add_property("<b>Top:</b>", Glib::ustring::format(std::fixed, std::setprecision(2), dest.get_top()));
		grid->add_property("<b>Bottom:</b>", Glib::ustring::format(std::fixed, std::setprecision(2), dest.get_bottom()));
		grid->add_property("<b>Zoom:</b>", Glib::ustring::format(std::fixed, std::setprecision(2), dest.get_zoom()));
	}
	else
	{
		grid->add_property("<b>Named Dest:</b>", dest.get_named_dest());

		if (m_Document && !remote)
		{
			Poppler::Dest new_dest = m_Document->find_dest(dest.get_named_dest());
			if (new_dest)
			{
				Gtk::Alignment* alignment;
				grid->add_row_widget(*Gtk::manage(alignment = new Gtk::Alignment(0.5, 0.5, 1, 1)));
				alignment->set_padding(5, 5, 12, 5);

				PropertyGrid* new_grid;
				alignment->add(*Gtk::manage(new_grid = new PropertyGrid));
				new_grid->set_column_spacing(6);
				new_grid->set_row_spacing(6);

				add_destination(new_grid, new_dest, false);
				alignment->show_all();
			}
		}
	}
}

static Glib::ustring get_movie_op(Poppler::ActionMovieOperation op)
{
	switch (op)
	{
		case Poppler::ACTION_MOVIE_PLAY:
			return "Play";
		case Poppler::ACTION_MOVIE_PAUSE:
			return "Pause";
		case Poppler::ACTION_MOVIE_RESUME:
			return "Resume";
		case Poppler::ACTION_MOVIE_STOP:
			return "Stop";
	}
	return "";
}

void ActionView::set_action(const Poppler::Action& action)
{
	PropertyGrid* grid = static_cast<PropertyGrid*>(m_Alignment.get_child());
	if (grid)
	{
		m_Alignment.remove();
		delete grid;
	}

	if (!action) return;

	m_Alignment.add(*Gtk::manage(grid = new PropertyGrid));

	grid->set_column_spacing(6);
	grid->set_row_spacing(6);

	grid->add_property("<b>Title:</b>", action.get_title());

	switch (action.get_action_type())
	{
		case Poppler::ACTION_UNKNOWN:
			grid->add_property("<b>Type:</b>", "Unknown");
			break;
		case Poppler::ACTION_NONE:
			grid->add_property("<b>Type:</b>", "None");
			break;
		case Poppler::ACTION_GOTO_DEST:
			add_destination(grid, action.get_goto_dest_dest(), false);
			break;
		case Poppler::ACTION_GOTO_REMOTE:
			grid->add_property("<b>Type:</b>", "Remote Destination");
			grid->add_property("<b>Filename:</b>", action.get_goto_remote_file_name());
			add_destination(grid, action.get_goto_remote_dest(), true);
			break;
		case Poppler::ACTION_LAUNCH:
			grid->add_property("<b>Type:</b>", "Launch");
			grid->add_property("<b>Filename:</b>", action.get_launch_file_name());
			grid->add_property("<b>Params:</b>", action.get_launch_params());
			break;
		case Poppler::ACTION_URI:
			grid->add_property("<b>Type:</b>", "External URI");
			grid->add_property("<b>URI</b>", action.get_uri());
			break;
		case Poppler::ACTION_NAMED:
			grid->add_property("<b>Type:</b>", "Named Action");
			grid->add_property("<b>Name:</b>", action.get_named_dest());
			break;
		case Poppler::ACTION_MOVIE:
			grid->add_property("<b>Type:</b>", "Movie");
			grid->add_property("<b>Operation:</b>", get_movie_op(action.get_movie_operation()));
			grid->add_property_with_custom_widget("<b>Movie:</b>",
					*Gtk::manage(new MovieView(action.get_movie_movie())));
			break;
		case Poppler::ACTION_RENDITION:
			grid->add_property("<b>Type:</b>", "Rendition");
			grid->add_property("<b>Operation:</b>", Glib::ustring::format(action.get_rendition_op()));

			if (action.get_rendition_media())
			{
				bool embedded = action.get_rendition_media()->is_embedded();
				grid->add_property("<b>Embedded:</b>", embedded ? "Yes": "No");

				if (embedded)
					grid->add_property("<b>Mime type:</b>", action.get_rendition_media()->get_mime_type());
				else
					grid->add_property("<b>Filename:</b>", action.get_rendition_media()->get_filename());

				Gtk::Button* button;
				grid->add_property_with_custom_widget("",
						*Gtk::manage(button = new Gtk::Button(Gtk::Stock::MEDIA_PLAY)));
				button->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &ActionView::play_rendition),
							action.get_rendition_media()));
			}
			break;
		case Poppler::ACTION_OCG_STATE:
			{
				grid->add_property("<b>Type:</b>", "OCGState");

				std::vector<Poppler::ActionLayer> action_layer_list = action.get_ocg_state_list();
				for (std::vector<Poppler::ActionLayer>::iterator action_layer = action_layer_list.begin();
						action_layer != action_layer_list.begin(); ++action_layer)
				{
					Glib::ustring text;
					int n_layers = action_layer->get_layers().size();

					switch (action_layer->get_action())
					{
						case Poppler::ACTION_LAYER_ON:
							text = Glib::ustring::format(n_layers, " layers On");
							break;
						case Poppler::ACTION_LAYER_OFF:
							text = Glib::ustring::format(n_layers, " layers Off");
							break;
						case Poppler::ACTION_LAYER_TOGGLE:
							text = Glib::ustring::format(n_layers, " layers Toggle");
							break;
					}
					grid->add_property("<b>Action:</b>", text);
				}
				Gtk::Button* button;
				grid->add_property_with_custom_widget("",
						*Gtk::manage(button = new Gtk::Button("Do action")));
				button->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &ActionView::do_action_layer),
							action_layer_list));
			}
			break;
		case Poppler::ACTION_JAVASCRIPT:
			{
				grid->add_property("<b>Type:</b>", "JavaScript");

				Glib::RefPtr<Gtk::TextBuffer> buffer = Gtk::TextBuffer::create();
				buffer->set_text(action.get_javascript());

				Gtk::ScrolledWindow* swindow;
				grid->add_property_with_custom_widget("", *Gtk::manage(swindow = new Gtk::ScrolledWindow));
				swindow->set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
				swindow->set_hexpand();

				Gtk::TextView* textview;
				swindow->add(*Gtk::manage(textview = new Gtk::TextView(buffer)));
				textview->set_editable(false);
			}
			break;
	}

	grid->show_all();
}

static bool save_helper(const gchar *buf, gsize count, const Glib::RefPtr<Gio::FileOutputStream>& stream)
{
	return stream->write(buf, count) == (unsigned int)count;
}

void ActionView::play_rendition(const Glib::RefPtr<const Poppler::Media>& media)
{
	Glib::RefPtr<Gio::File> file;

	if (media->is_embedded())
	{
		file = Gio::File::create_for_path(tmpnam(NULL));

		Glib::RefPtr<Gio::FileOutputStream> stream;
		if(file->query_exists()) stream = file->replace();
		else stream = file->create_file();

		if (!stream || !media->save(sigc::bind(&save_helper, stream)))
			file.reset();
	}
	else
	{
		std::string filename = media->get_filename();
		if (Glib::path_is_absolute(filename))
			file = Gio::File::create_for_path(filename);
		else if (filename.find("://") != std::string::npos)
			file = Gio::File::create_for_uri(filename);
		else
			// FIXME: Make path relative to the document uri, not the cwd
			file = Gio::File::create_for_path(Glib::build_filename(Glib::get_current_dir(), filename));
	}

	if (file)
	{
		try
		{
			Gio::AppInfo::launch_default_for_uri(file->get_uri());
		}
		catch (const Glib::Error& ex)
		{
		}
	}
}

void ActionView::do_action_layer(const std::vector<Poppler::ActionLayer>& state_list)
{

	for (std::vector<Poppler::ActionLayer>::const_iterator action_layer = state_list.begin();
			action_layer != state_list.begin(); ++action_layer)
	{
		std::vector< Glib::RefPtr<Poppler::Layer> > layers = action_layer->get_layers();
		for (std::vector< Glib::RefPtr<Poppler::Layer> >::iterator layer = layers.begin();
				layer != layers.end(); ++layer)
		{
			switch (action_layer->get_action())
			{
				case Poppler::ACTION_LAYER_ON:
					(*layer)->show();
					break;
				case Poppler::ACTION_LAYER_OFF:
					(*layer)->hide();
					break;
				case Poppler::ACTION_LAYER_TOGGLE:
					if ((*layer)->is_visible()) (*layer)->hide();
					else (*layer)->show();
					break;
			}
		}
	}
}

MovieView::MovieView(const Glib::RefPtr<const Poppler::Movie>& movie) :
	m_Movie(movie),
	m_Alignment(0.5, 0.5, 1, 1)
{
	set_shadow_type(Gtk::SHADOW_NONE);
	m_Label.set_markup("<b>Movie Properties</b>");
	set_label_widget(m_Label);

	m_Alignment.set_padding(5, 5, 12, 5);
	add(m_Alignment);

	m_Alignment.add(m_Grid);

	m_Grid.set_column_spacing(6);
	m_Grid.set_row_spacing(6);

	if (movie)
	{
		m_Grid.add_property("<b>Filename:</b>", movie->get_filename());
		m_Grid.add_property("<b>Need Poster:</b>", movie->need_poster() ? "Yes" : "No");
		m_Grid.add_property("<b>Show Controls:</b>", movie->show_controls() ? "Yes" : "No");

		Gtk::Button* button;
		m_Grid.add_property_with_custom_widget("", *Gtk::manage(button = new Gtk::Button(Gtk::Stock::MEDIA_PLAY)));
		button->signal_clicked().connect(sigc::mem_fun(*this, &MovieView::play_movie));
	}

	show_all();
}

void MovieView::play_movie()
{
	Glib::RefPtr<Gio::File> file;

	std::string filename = m_Movie->get_filename();

	if (Glib::path_is_absolute(filename))
		file = Gio::File::create_for_path(filename);
	else if (filename.find("://") != std::string::npos)
		file = Gio::File::create_for_uri(filename);
	else
		// FIXME: Make path relative to the document uri, not the cwd
		file = Gio::File::create_for_path(Glib::build_filename(Glib::get_current_dir(), filename));

	try
	{
		Gio::AppInfo::launch_default_for_uri(file->get_uri());
	}
	catch (const Glib::Error& ex)
	{
	}
}

}
