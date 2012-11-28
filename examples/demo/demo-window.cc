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

DemoWindow::DemoWindow(const Glib::RefPtr<Poppler::Document>& document) :
	m_HBox(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_Info(document),
	m_Fonts(document),
	m_Render(document),
	m_Selections(document),
	m_PageInfo(document),
	m_Outline(document),
	m_Links(document),
	m_Forms(document),
	m_Transitions(document),
	m_Images(document),
	m_Annots(document),
	m_Attachments(document),
	m_Layers(document),
	m_Text(document),
	m_Find(document),
	m_Print(document)
{
	// Set up the treeview that displays the list of available demos
	m_DemoStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.append_column("Demos", m_StoreColumns.m_DemoName);
	m_TreeView.set_model(m_DemoStore);
	m_TreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &DemoWindow::demo_changed));

	int index = 0;

	Gtk::TreeRow row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Info";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Fonts";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Render";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Selections";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Page Info";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Outline";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Links";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Forms";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Page Transitions";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Images";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Annots";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Attachments";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Layers";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Text";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Find";
	row[m_StoreColumns.m_Index] = index++;

	row = *(m_DemoStore->append());
	row[m_StoreColumns.m_DemoName] = "Print";
	row[m_StoreColumns.m_Index] = index++;

	// Set up the notebook that will show the demos in action
	m_Notebook.set_show_tabs(false);
	m_Notebook.set_show_border(false);

	// Add the demo widgets as the pages
	m_Notebook.append_page(m_Info);
	m_Notebook.append_page(m_Fonts);
	m_Notebook.append_page(m_Render);
	m_Notebook.append_page(m_Selections);
	m_Notebook.append_page(m_PageInfo);
	m_Notebook.append_page(m_Outline);
	m_Notebook.append_page(m_Links);
	m_Notebook.append_page(m_Forms);
	m_Notebook.append_page(m_Transitions);
	m_Notebook.append_page(m_Images);
	m_Notebook.append_page(m_Annots);
	m_Notebook.append_page(m_Attachments);
	m_Notebook.append_page(m_Layers);
	m_Notebook.append_page(m_Text);
	m_Notebook.append_page(m_Find);
	m_Notebook.append_page(m_Print);

	m_HBox.pack_start(m_TreeView, false, true);
	m_HBox.pack_start(m_Notebook, true, true);
	add(m_HBox);

	set_default_size(660, 600);
	set_title("Poppler GLib Demo");

	show_all();
}

void DemoWindow::demo_changed()
{
	Gtk::TreeModel::iterator iter = m_TreeView.get_selection()->get_selected();
	if (iter) m_Notebook.set_current_page((*iter)[m_StoreColumns.m_Index]);
}

bool DemoWindow::on_key_press_event(GdkEventKey* event)
{
	if (event->keyval == GDK_KEY_q && (event->state & GDK_CONTROL_MASK)) hide();
	return false;
}
