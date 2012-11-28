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

#ifndef _DEMO_WINDOW_H_
#define _DEMO_WINDOW_H_

#include "info.h"
#include "fonts.h"
#include "render.h"
#include "selections.h"
#include "page-info.h"
#include "outline.h"
#include "links.h"
#include "forms.h"
#include "transitions.h"
#include "images.h"
#include "annots.h"
#include "attachments.h"
#include "layers.h"
#include "text.h"
#include "find.h"
#include "print.h"

#include <gtkmm/window.h>
#include <gtkmm/treeview.h>
#include <gtkmm/liststore.h>
#include <gtkmm/notebook.h>
#include <gtkmm/box.h>
#include <poppler-glibmm/document.h>

class DemoWindow : public Gtk::Window
{
	public:
		DemoWindow(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void demo_changed();
		bool on_key_press_event(GdkEventKey* event);

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_DemoName); add(m_Index); }

				Gtk::TreeModelColumn<std::string> m_DemoName;
				Gtk::TreeModelColumn<int> m_Index;
		};

		Gtk::TreeView m_TreeView;
		ModelColumns m_StoreColumns;
		Glib::RefPtr<Gtk::ListStore> m_DemoStore;

		Gtk::Box m_HBox;
		Gtk::Notebook m_Notebook;
		Glib::RefPtr<Gtk::AccelGroup> m_AccelGroup;

		// Demo widgets
		PGD::Info m_Info;
		PGD::Fonts m_Fonts;
		PGD::Render m_Render;
		PGD::Selections m_Selections;
		PGD::PageInfo m_PageInfo;
		PGD::Outline m_Outline;
		PGD::Links m_Links;
		PGD::Forms m_Forms;
		PGD::Transitions m_Transitions;
		PGD::Images m_Images;
		PGD::Annots m_Annots;
		PGD::Attachments m_Attachments;
		PGD::Layers m_Layers;
		PGD::Text m_Text;
		PGD::Find m_Find;
		PGD::Print m_Print;
};

#endif // _DEMO_WINDOW_H_
