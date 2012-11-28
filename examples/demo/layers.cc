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

#include "layers.h"

#include <gtkmm/treestore.h>
#include <gtkmm/liststore.h>
#include <glibmm/markup.h>

namespace PGD
{

LayersViewer::LayersViewer(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 6),
	m_Document(document),
	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:")
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &LayersViewer::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);

	m_DrawingArea.signal_draw().connect(sigc::mem_fun(*this, &LayersViewer::drawing_area_draw));
	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	m_ScrolledWin.add(m_DrawingArea);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_ScrolledWin, true, true);
}

void LayersViewer::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
	queue_redraw();
}

void LayersViewer::queue_redraw()
{
	m_Surface.clear();
	m_DrawingArea.queue_draw();
}

bool LayersViewer::drawing_area_draw(const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!m_Surface)
	{
		Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
		if (!page) return false;

		double width, height;
		page->get_size(width, height);
		m_DrawingArea.set_size_request(width, height);


		m_Surface = Cairo::ImageSurface::create(Cairo::FORMAT_RGB24, width, height);
		Cairo::RefPtr<Cairo::Context> cr_tmp = Cairo::Context::create(m_Surface);

		cr_tmp->save();
		cr_tmp->set_source_rgb(1.0, 1.0, 1.0);
		cr_tmp->rectangle(0.0, 0.0, width, height); 
		cr_tmp->fill();
		cr_tmp->restore();

		cr_tmp->save();
		page->render(cr_tmp);
		cr_tmp->restore();
	}

	cr->set_source(m_Surface, 0, 0);
	cr->paint();

	return true;
}

Layers::Layers(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Paned(Gtk::ORIENTATION_HORIZONTAL),
	m_Document(document),
	m_Viewer(document),
	m_TitleCell(),
	m_TitleColumn("Title", m_TitleCell)
{
	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Poppler::LayersIter iter = m_Document->layers_iter_new();
	if (iter)
	{
		m_LayerStore = Gtk::TreeStore::create(m_StoreColumns);
		build_tree(m_LayerStore->children(), iter);
	}
	else
   	{
		m_LayerStore = Gtk::ListStore::create(m_StoreColumns);
		Gtk::TreeRow tree_row = *(Glib::RefPtr<Gtk::ListStore>::cast_static(m_LayerStore)->append());
		tree_row[m_StoreColumns.m_Title] =
			"<span size=\"larger\" style=\"italic\">Document does not contain layers</span>";
	}

	m_TreeView.set_model(m_LayerStore);
	m_TitleCell.property_ellipsize() = Pango::ELLIPSIZE_END;
	m_TitleColumn.add_attribute(m_TitleCell.property_markup(), m_StoreColumns.m_Title);
	m_TitleColumn.set_expand();
	m_TreeView.append_column(m_TitleColumn);

	if (iter)
	{
		m_TreeView.insert_column("Show/Hide", m_StoreColumns.m_Visibility, 1);
		Gtk::TreeViewColumn* sh_column = m_TreeView.get_column(1);
		Gtk::CellRendererToggle* sh_cell = static_cast<Gtk::CellRendererToggle*>(m_TreeView.get_column_cell_renderer(1));
		sh_column->add_attribute(sh_cell->property_activatable(), m_StoreColumns.m_Enable);
		sh_column->add_attribute(sh_cell->property_visible(), m_StoreColumns.m_ShowToggle);
		sh_cell->signal_toggled().connect(sigc::mem_fun(*this, &Layers::visibility_changed));
	}

	m_TreeView.get_selection()->set_mode(Gtk::SELECTION_NONE);

	m_ScrolledWin.add(m_TreeView);

	add1(m_ScrolledWin);
	add2(m_Viewer);

	set_position(150);

	show_all();
}

void Layers::build_tree(const Gtk::TreeModel::Children& parent, const Poppler::LayersIter &iter)
{
	do
	{
		Glib::ustring markup;
		bool visible;
		int rb_group = 0;

		Glib::RefPtr<Poppler::Layer> layer = iter.get_layer();
		if (layer)
	   	{
			markup = Glib::Markup::escape_text(layer->get_title());
			visible = layer->is_visible();
			rb_group = layer->get_radio_button_group_id();
		}
	   	else
	   	{
			markup = Glib::Markup::escape_text(iter.get_title());
			visible = false;
		}

		Gtk::TreeRow tree_row = *(Glib::RefPtr<Gtk::TreeStore>::cast_static(m_LayerStore)->append(parent));
		tree_row[m_StoreColumns.m_Title] = markup;
		tree_row[m_StoreColumns.m_Visibility] = visible;
		tree_row[m_StoreColumns.m_Enable] = true; // FIXME
		tree_row[m_StoreColumns.m_ShowToggle] = layer;
		tree_row[m_StoreColumns.m_RBGroup] = rb_group;
		tree_row[m_StoreColumns.m_Layer] = layer;

		Poppler::LayersIter child = iter.get_child();
		if (child) build_tree(tree_row.children(), child);
	} while (iter.next());
}

void Layers::visibility_changed(const Glib::ustring& path)
{
	Gtk::TreeIter iter = m_LayerStore->get_iter(path);
	(*iter)[m_StoreColumns.m_Visibility] = !bool((*iter)[m_StoreColumns.m_Visibility]);
	Glib::RefPtr<Poppler::Layer> layer = (*iter)[m_StoreColumns.m_Layer];
	if ((*iter)[m_StoreColumns.m_Visibility])
	{
		layer->show();
		int rb_group = layer->get_radio_button_group_id();
		if (rb_group) m_LayerStore->foreach_iter(sigc::bind(sigc::mem_fun(*this, &Layers::clear_rb_group), rb_group));

	}
	else layer->hide();

	if (layer->is_parent()) m_LayerStore->foreach_iter(sigc::bind(sigc::mem_fun(*this, &Layers::update_kids), iter));
	m_Viewer.queue_redraw();
}

bool Layers::clear_rb_group(const Gtk::TreeIter& iter, int rb_group)
{
	if ((*iter)[m_StoreColumns.m_RBGroup] == rb_group) (*iter)[m_StoreColumns.m_Visibility] = false;
	return false;
}

bool Layers::update_kids(const Gtk::TreeIter& iter, const Gtk::TreeIter& parent)
{
	if (Glib::RefPtr<Gtk::TreeStore>::cast_static(m_LayerStore)->is_ancestor(parent, iter))
		(*iter)[m_StoreColumns.m_Visibility] = bool((*parent)[m_StoreColumns.m_Visibility]);
	return false;
}

}
