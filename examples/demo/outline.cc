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

#include "outline.h"

#include <glibmm/markup.h>

namespace PGD
{

Outline::Outline(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Paned(Gtk::ORIENTATION_HORIZONTAL),
	m_Document(document),
	m_ActionView(document),
	m_TitleCell(),
	m_TitleColumn("Title", m_TitleCell)
{
	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	Poppler::IndexIter iter = m_Document->index_iter_new();
	
	if (iter)
	{
		m_ActionStore = Gtk::TreeStore::create(m_StoreColumns);
		build_tree(m_ActionStore->children(), iter);
	}
	else
	{
		m_ActionStore = Gtk::ListStore::create(m_StoreColumns);
		Gtk::TreeRow tree_row = *(Glib::RefPtr<Gtk::ListStore>::cast_static(m_ActionStore)->append());
		tree_row[m_StoreColumns.m_Title] =
			"<span size=\"larger\" style=\"italic\">Document does not contain an outline</span>";
	}

	m_TreeView.set_model(m_ActionStore);
	m_TitleCell.property_ellipsize() = Pango::ELLIPSIZE_END;
	m_TitleColumn.add_attribute(m_TitleCell.property_markup(), m_StoreColumns.m_Title);
	m_TitleColumn.set_expand();
	m_TreeView.append_column(m_TitleColumn);

	Glib::RefPtr<Gtk::TreeSelection> selection = m_TreeView.get_selection();

	if (iter)
	{
		m_TreeView.append_column("Action Type", m_StoreColumns.m_ActionType);
		expand_open_links(m_ActionStore->children());
		selection->signal_changed().connect(sigc::mem_fun(*this, &Outline::selection_changed));

	}
	else selection->set_mode(Gtk::SELECTION_NONE);

	m_ScrolledWin.add(m_TreeView);

	add1(m_ScrolledWin);
	add2(m_ActionView);

	set_position(300);

	show_all();
}

void Outline::build_tree(const Gtk::TreeModel::Children& parent, Poppler::IndexIter& iter)
{
	do
	{
		Poppler::Action action = iter.get_action();
		if (!action) continue;

		if (action.get_action_type() == Poppler::ACTION_GOTO_DEST &&
				action.get_goto_dest_dest().get_dest_type() == Poppler::DEST_NAMED)
		{
			// TODO
		}			

		GEnumValue* enum_value =
			g_enum_get_value((GEnumClass*)g_type_class_ref(Glib::Value<Poppler::ActionType>::value_type()),
					action.get_action_type());

		Gtk::TreeRow tree_row = *(Glib::RefPtr<Gtk::TreeStore>::cast_static(m_ActionStore)->append(parent));
		tree_row[m_StoreColumns.m_Title] = Glib::Markup::escape_text(action.get_title());
		tree_row[m_StoreColumns.m_ActionType] = enum_value->value_name;
		tree_row[m_StoreColumns.m_Expand] = iter.is_open();
		tree_row[m_StoreColumns.m_Action] = action;

		Poppler::IndexIter child = iter.get_child();
		if (child) build_tree(tree_row.children(), child);
	} while (iter.next());
}

void Outline::expand_open_links(const Gtk::TreeModel::Children& parent)
{
	for (Gtk::TreeIter iter = parent.begin(); iter != parent.end(); ++iter)
	{
		if ((*iter)[m_StoreColumns.m_Expand])
			m_TreeView.expand_row(m_ActionStore->get_path(iter), false);

		expand_open_links((*iter).children());
	}
}

void Outline::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter) m_ActionView.set_action((*iter)[m_StoreColumns.m_Action]);
	else m_ActionView.set_action(Poppler::Action());
}

}
