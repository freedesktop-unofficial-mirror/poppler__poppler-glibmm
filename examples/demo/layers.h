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

#ifndef _LAYERS_H_
#define _LAYERS_H_

#include "action-view.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/label.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/paned.h>
#include <gtkmm/treeview.h>

namespace PGD
{

class LayersViewer : public Gtk::Box
{
	public:
		LayersViewer(const Glib::RefPtr<Poppler::Document>& document);
		void queue_redraw();

	private:
		void page_selector_changed();
		bool drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr);

		const Glib::RefPtr<Poppler::Document> m_Document;
		int m_Page;
		Cairo::RefPtr<Cairo::ImageSurface> m_Surface;
		bool m_RecreateSurface;

		Gtk::Box m_HBoxTop;
		Gtk::Label m_PageLabel, m_PageNumLabel;
		Gtk::SpinButton m_PageSelector;
		Gtk::DrawingArea m_DrawingArea;
		Gtk::ScrolledWindow m_ScrolledWin;
};

class Layers : public Gtk::Paned
{
	public:
		Layers(const Glib::RefPtr<Poppler::Document>& document);

	private:
		void build_tree(const Gtk::TreeModel::Children& parent, const Poppler::LayersIter &iter);
		void selection_changed();
		void visibility_changed(const Glib::ustring& path);
		bool clear_rb_group(const Gtk::TreeIter& iter, int rb_group);
		bool update_kids(const Gtk::TreeIter& iter, const Gtk::TreeIter& parent);

		const Glib::RefPtr<Poppler::Document> m_Document;

		class ModelColumns : public Gtk::TreeModel::ColumnRecord
		{
			public:
				ModelColumns() { add(m_Title); add(m_Visibility); add(m_Enable);
				   	add(m_ShowToggle); add(m_RBGroup); add(m_Layer); }

				Gtk::TreeModelColumn<Glib::ustring> m_Title;
				Gtk::TreeModelColumn<bool> m_Visibility;
				Gtk::TreeModelColumn<bool> m_Enable;
				Gtk::TreeModelColumn<bool> m_ShowToggle;
				Gtk::TreeModelColumn<int> m_RBGroup;
				Gtk::TreeModelColumn< Glib::RefPtr<Poppler::Layer> > m_Layer;
		};

		Gtk::ScrolledWindow m_ScrolledWin;
		LayersViewer m_Viewer;

		ModelColumns m_StoreColumns;
		Gtk::TreeView m_TreeView;
		Glib::RefPtr<Gtk::TreeModel> m_LayerStore;
		Gtk::CellRendererText m_TitleCell;
		Gtk::TreeViewColumn m_TitleColumn;
};

}

#endif // _LAYERS_H_
