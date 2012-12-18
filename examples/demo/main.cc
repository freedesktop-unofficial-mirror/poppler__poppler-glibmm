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

#include "demo-window.h"
#include "auth-dialog.h"

#include <gtkmm/application.h>
#include <glibmm/timer.h>
#include <poppler-glibmm/init.h>
#include <poppler-glibmm/document.h>
#include <poppler-glibmm/error.h>
#include <iostream>
#include <iomanip>

int main (int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Usage: poppler-glibmm-demo file" << std::endl;
		return 1;
	}

	Poppler::init();

	// Hack to work around a bug in recent versions of Gtk/Gtkmm
	int _argc = 1;
	char** _argv = &argv[0];
	Glib::RefPtr<Gtk::Application> app = Gtk::Application::create(_argc, _argv,
			"org.poppler-glibmm.demo", Gio::APPLICATION_HANDLES_OPEN);

	Glib::RefPtr<Gio::File> file = Gio::File::create_for_commandline_arg(argv[1]);

	Glib::RefPtr<Poppler::Document> document;
	{
		std::string password;
		AuthDialog dialog(file->get_basename());
		do
		{
			password = dialog.GetPassword();
			Glib::Timer timer;
			try
			{
				document = Poppler::Document::new_from_gio_file(file, password);
			}
			catch (Poppler::Error exception)
			{
				continue;
			}
			catch (Glib::Error exception)
			{
				return 1;
			}
			timer.stop();
			std::cout << "Document successfully loaded in "
				<< std::fixed << std::setprecision(4) << timer.elapsed() << " seconds\n";
		} while (!document && dialog.run() == Gtk::RESPONSE_OK);
	}

	if (!document) return 1;

	// Main window
	DemoWindow window(document);
	app->run(window);

	return 0;
}
