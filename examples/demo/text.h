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

#ifndef _TEXT_H_
#define _TEXT_H_

#include "property-grid.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>
#include <gtkmm/frame.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/alignment.h>
#include <gtkmm/textview.h>

namespace PGD
{

class TextAttribView : public Gtk::Frame
{
	public:
		TextAttribView(const Glib::RefPtr<Poppler::Document>& document);
		void set_font_name(const Glib::ustring& name);
		void set_font_size(const double size);
		void set_is_underlined(const bool is_underlined);
		void set_text_color(const Poppler::Color& color);

	private:
		const Glib::RefPtr<Poppler::Document> m_Document;

		Gtk::Label m_Label, m_FontName, m_FontSize, m_IsUnderlined;
		Gtk::Image m_TextColor;
		Gtk::Alignment m_Alignment;
		PropertyGrid m_Grid;
};

class Text : public Gtk::Box
{
	public:
		Text(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void page_selector_changed();
		void get_text();
		void selection_changed();
		bool view_query_tooltip(int x, int y, bool keyboard_tooltip, const Glib::RefPtr<Gtk::Tooltip>& tooltip);

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_Page;
		std::vector<Poppler::TextAttributes> m_TextAttrs;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_X1); add(m_Y1); add(m_X2); add(m_Y2); add(m_Offset); }

				Gtk::TreeModelColumn<double> m_X1;
				Gtk::TreeModelColumn<double> m_Y1;
				Gtk::TreeModelColumn<double> m_X2;
				Gtk::TreeModelColumn<double> m_Y2;
				Gtk::TreeModelColumn<int> m_Offset;
		};

		Gtk::Box m_HBoxTop, m_VBoxLeft;
		Gtk::Label m_PageLabel, m_PageNumLabel, m_TimerLabel, m_AttribFrameLabel;
		Gtk::SpinButton m_PageSelector;
		Gtk::Button m_GetButton;
		Gtk::ScrolledWindow m_TreeWin, m_BuffWin;
		Gtk::Paned m_HPaned;
		TextAttribView m_AttribView;
		Glib::RefPtr<Gtk::TextBuffer> m_TextBuffer;
		Gtk::TextView m_TextView;

		ModelColumns m_StoreColumns;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::ListStore> m_TextStore;
};

}

#endif // _TEXT_H_
