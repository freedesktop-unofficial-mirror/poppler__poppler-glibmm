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

#include "annots.h"
#include "property-grid.h"
#include "action-view.h"
#include "util.h"

#include <gtkmm/textview.h>
#include <gtkmm/filechooserdialog.h>
#include <gtkmm/comboboxtext.h>
#include <gtkmm/stock.h>
#include <glibmm/timer.h>
#include <poppler-glibmm/utility.h>
#include <iomanip>

namespace PGD
{

AnnotView::AnnotView(const Glib::RefPtr<Poppler::Document>& document) :
	m_Document(document),
	m_Alignment(0.5, 0.5, 1, 1)
{
	set_shadow_type(Gtk::SHADOW_NONE);
	m_Label.set_markup("<b>Annot Properties</b>");
	set_label_widget(m_Label);

	m_Alignment.set_padding(5, 5, 12, 5);
	add(m_Alignment);
}

static Glib::ustring get_markup_date(const Glib::RefPtr<Poppler::AnnotMarkup>& poppler_annot)
{
	Glib::Date date = poppler_annot->get_date();
	if (date.valid()) return date.format_string("%c");
	else return "";
}

static Glib::ustring get_markup_reply_to(const Glib::RefPtr<Poppler::AnnotMarkup>& poppler_annot)
{
	switch (poppler_annot->get_reply_to())
	{
		case Poppler::ANNOT_MARKUP_REPLY_TYPE_R:
			return "Type R";
		case Poppler::ANNOT_MARKUP_REPLY_TYPE_GROUP:
			return "Type Group";
		default:
			break;
	}
	return "Unknown";
}

static Glib::ustring get_markup_external_data(const Glib::RefPtr<Poppler::AnnotMarkup>& poppler_annot)
{
	switch (poppler_annot->get_external_data())
	{
		case Poppler::ANNOT_EXTERNAL_DATA_MARKUP_3D:
			return "Markup 3D";
		default:
			break;
	}
	return "Unknown";
}

static Glib::ustring get_text_state(const Poppler::AnnotTextState state)
{
	switch (state)
	{
		case Poppler::ANNOT_TEXT_STATE_MARKED:
			return "Marked";
		case Poppler::ANNOT_TEXT_STATE_UNMARKED:
			return "Unmarked";
		case Poppler::ANNOT_TEXT_STATE_ACCEPTED:
			return "Accepted";
		case Poppler::ANNOT_TEXT_STATE_REJECTED:
			return "Rejected";
		case Poppler::ANNOT_TEXT_STATE_CANCELLED:
			return "Cancelled";
		case Poppler::ANNOT_TEXT_STATE_COMPLETED:
			return "Completed";
		case Poppler::ANNOT_TEXT_STATE_NONE:
			return "None";
		case Poppler::ANNOT_TEXT_STATE_UNKNOWN:
			return "Unknown";
		default:
			break;
	}
	return "Unknown";
}

static Glib::ustring get_free_text_quadding(const Poppler::AnnotFreeTextQuadding quadding)
{
	switch (quadding)
	{
		case Poppler::ANNOT_FREE_TEXT_QUADDING_LEFT_JUSTIFIED:
			return "Left Justified";
		case Poppler::ANNOT_FREE_TEXT_QUADDING_CENTERED:
			return "Centered";
		case Poppler::ANNOT_FREE_TEXT_QUADDING_RIGHT_JUSTIFIED:
			return "Right Justified";
		default:
			break;
	}
	return "Unknown";
}

static Glib::ustring get_free_text_callout_line(const Poppler::AnnotCalloutLine& callout)
{
	if (callout)
	{
		Glib::ustring text = Glib::ustring::format(std::fixed, callout.get_x1(), ",", callout.get_y1(), ",") +
			Glib::ustring::format(std::fixed, callout.get_x2(), ",", callout.get_y2());
		if (callout.get_multiline())
			text += Glib::ustring::format(std::fixed, ",", callout.get_x3(), ",", callout.get_y3());
		return text;
	}
	return "";
}

void AnnotView::save_file_attachment()
{
	Glib::RefPtr<Poppler::Attachment> attachment =
		Glib::RefPtr<Poppler::AnnotFileAttachment>::cast_static(m_Annot)->get_attachment();
	if (!attachment) return;

	Gtk::FileChooserDialog file_chooser("Save attachment", Gtk::FILE_CHOOSER_ACTION_SAVE);
	file_chooser.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	file_chooser.add_button(Gtk::Stock::SAVE, Gtk::RESPONSE_ACCEPT);
	file_chooser.set_current_name(attachment->get_name());

	if (file_chooser.run() == Gtk::RESPONSE_ACCEPT)
	{
		std::string filename = file_chooser.get_filename();
		try
		{
			attachment->save(filename);
		}
		catch (Glib::Error exception)
		{
		}
	}
}

void AnnotView::set_annot(const Glib::RefPtr<Poppler::Annot>& annot)
{
	PropertyGrid* grid = static_cast<PropertyGrid*>(m_Alignment.get_child());
	if (grid)
	{
		m_Alignment.remove();
		delete grid;
	}

	m_Annot = annot;
	if (!annot) return;

	m_Alignment.add(*Gtk::manage(grid = new PropertyGrid));

	grid->set_column_spacing(6);
	grid->set_row_spacing(6);

    grid->add_property("<b>Contents:</b>", annot->get_contents());
	grid->add_property("<b>Name:</b>", annot->get_name());
	
	time_t timet;
	if (Poppler::date_parse(annot->get_modified(), &timet))
		grid->add_property("<b>Modified:</b>", format_date(timet));
	else
		grid->add_property("<b>Modified:</b>", annot->get_modified());

	grid->add_property("<b>Flags:</b>", Glib::ustring::format(annot->get_flags()));
	grid->add_property("<b>Page:</b>", Glib::ustring::format(annot->get_page_index()));

	const Glib::RefPtr<Poppler::AnnotMarkup>& annot_markup = Glib::RefPtr<Poppler::AnnotMarkup>::cast_dynamic(annot);
	if (annot_markup)
	{
		grid->add_property("<b>Label:</b>", annot_markup->get_label());

		if (annot_markup->has_popup())
		{
			grid->add_property("<b>Popup is open:</b>", annot_markup->get_popup_is_open() ? "Yes" : "No");

			Poppler::Rectangle rect;
			annot_markup->get_popup_rectangle(rect);
			grid->add_property("<b>Popup Rectangle:</b>",
					Glib::ustring::format(std::fixed, std::setprecision(2),
						"X1: ", rect.get_x1(), ", Y1: ", rect.get_y1()) +
					Glib::ustring::format(std::fixed, std::setprecision(2),
						", X2: ", rect.get_x2(), ", Y2: ", rect.get_y2()));
		}

		grid->add_property("<b>Opacity:</b>", Glib::ustring::format(std::fixed, annot_markup->get_opacity()));
		grid->add_property("<b>Date:</b>", get_markup_date(annot_markup));
		grid->add_property("<b>Subject:</b>", annot_markup->get_subject());
		grid->add_property("<b>Reply To:</b>", get_markup_reply_to(annot_markup));
		grid->add_property("<b>External Data:</b>", get_markup_external_data(annot_markup));
	}

    switch (annot->get_annot_type())
	{
		case Poppler::ANNOT_TEXT:
			{
				const Glib::RefPtr<Poppler::AnnotText>& annot_text = Glib::RefPtr<Poppler::AnnotText>::cast_static(annot);
				grid->add_property("<b>Is open:</b>", annot_text->get_is_open() ? "Yes" : "No");
				grid->add_property("<b>Icon:</b>", annot_text->get_icon());
				grid->add_property("<b>State:</b>", get_text_state(annot_text->get_state()));
			}
			break;
		case Poppler::ANNOT_FREE_TEXT:
			{
				const Glib::RefPtr<Poppler::AnnotFreeText>& annot_free_text =
					Glib::RefPtr<Poppler::AnnotFreeText>::cast_static(annot);
				grid->add_property("<b>Quadding:</b>", get_free_text_quadding(annot_free_text->get_quadding()));
				grid->add_property("<b>Callout:</b>", get_free_text_callout_line(annot_free_text->get_callout_line()));
			}
			break;
		case Poppler::ANNOT_FILE_ATTACHMENT:
			{
				const Glib::RefPtr<Poppler::AnnotFileAttachment>& annot_file_attachment =
					Glib::RefPtr<Poppler::AnnotFileAttachment>::cast_static(annot);

				grid->add_property("<b>Attachment Name:</b>", annot_file_attachment->get_name());

				Gtk::Button* button;
				grid->add_property_with_custom_widget("<b>File Attachment:</b>",
						*Gtk::manage(button = new Gtk::Button("Save Attachment")));
				button->signal_clicked().connect(sigc::mem_fun(*this, &AnnotView::save_file_attachment));
			}
			break;
		case Poppler::ANNOT_MOVIE:
			{
				const Glib::RefPtr<Poppler::AnnotMovie>& annot_movie =
					Glib::RefPtr<Poppler::AnnotMovie>::cast_static(annot);

				grid->add_property("<b>Movie Title:</b>", annot_movie->get_title());
				MovieView* movie_view;
				grid->add_property_with_custom_widget("<b>Movie:</b>",
						*Gtk::manage(movie_view = new MovieView(annot_movie->get_movie())));
			}
			break;
		case Poppler::ANNOT_SCREEN:
			{
				const Glib::RefPtr<Poppler::AnnotScreen>& annot_screen =
					Glib::RefPtr<Poppler::AnnotScreen>::cast_static(annot);

				ActionView* action_view;
				grid->add_property_with_custom_widget("<b>Action:</b>",
						*Gtk::manage(action_view = new ActionView(m_Document)));
				action_view->set_action(annot_screen->get_action());
			}
			break;
		default:
			break;
	}
	grid->show_all();
}

Annots::Annots(const Glib::RefPtr<Poppler::Document>& document) :
	Gtk::Box(Gtk::ORIENTATION_VERTICAL, 12),
	m_Document(document),
	m_Page(0),
	m_HBoxTop(Gtk::ORIENTATION_HORIZONTAL, 6),
	m_PageLabel("Page:"),
	m_AddButton("Add Annot"),
	m_GetButton("Get Annots"),
	m_HPaned(Gtk::ORIENTATION_HORIZONTAL),
	m_AnnotView(document)
{
	int n_pages = m_Document->get_n_pages();
	m_PageSelector.set_range(1, n_pages);
	m_PageSelector.set_increments(1, 10);
	m_PageSelector.set_numeric();
	m_PageSelector.signal_value_changed().connect(sigc::mem_fun(*this, &Annots::page_selector_changed));
	m_PageNumLabel.set_text(Glib::ustring::compose("of %1", n_pages));

	m_AddButton.signal_clicked().connect(sigc::mem_fun(*this, &Annots::add_annot));
	m_GetButton.signal_clicked().connect(sigc::mem_fun(*this, &Annots::get_annots));

	m_HBoxTop.pack_start(m_PageLabel, false, true);
	m_HBoxTop.pack_start(m_PageSelector, false, true);
	m_HBoxTop.pack_start(m_PageNumLabel, false, true);
	m_HBoxTop.pack_end(m_GetButton, false, false);
	m_HBoxTop.pack_end(m_AddButton, false, false);

	m_TimerLabel.set_markup("<i>No annots found</i>");
	m_TimerLabel.set_alignment(1.0, 0.5);

	m_ScrolledWin.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	m_AnnotStore = Gtk::ListStore::create(m_StoreColumns);
	m_TreeView.set_model(m_AnnotStore);

	m_TreeView.append_column_numeric("X1", m_StoreColumns.m_X1, "%.2f");
	m_TreeView.append_column_numeric("Y1", m_StoreColumns.m_Y1, "%.2f");
	m_TreeView.append_column_numeric("X2", m_StoreColumns.m_X2, "%.2f");
	m_TreeView.append_column_numeric("Y2", m_StoreColumns.m_Y2, "%.2f");
	m_TreeView.append_column("Type", m_StoreColumns.m_AnnotType);
	m_TreeView.append_column("Color", m_StoreColumns.m_Color);
	m_TreeView.append_column("Invisible", m_StoreColumns.m_FlagInvisible);
	m_TreeView.append_column("Hidden", m_StoreColumns.m_FlagHidden);
	m_TreeView.append_column("Print", m_StoreColumns.m_FlagPrint);

	m_TreeView.get_selection()->signal_changed().connect(sigc::mem_fun(*this, &Annots::selection_changed));

	m_ScrolledWin.add(m_TreeView);

	m_HPaned.add1(m_ScrolledWin);
	m_HPaned.add2(m_AnnotView);
	m_HPaned.set_position(300);

	pack_start(m_HBoxTop, false, true);
	pack_start(m_TimerLabel, false, true);
	pack_start(m_HPaned, true, true);

	show_all();
}

void Annots::page_selector_changed()
{
	m_Page = m_PageSelector.get_value() - 1;
}

void Annots::add_annot()
{
	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	double width, height;
	page->get_size(width, height);

	Gtk::Dialog dialog("Add new annotation", true);
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button("Add annotation", Gtk::RESPONSE_ACCEPT);

	Gtk::Box* vbox = dialog.get_content_area();

	Gtk::ComboBoxText type_selector;
	type_selector.append("POPPLER_ANNOT_UNKNOWN");
	type_selector.append("POPPLER_ANNOT_TEXT");
	type_selector.set_active(1);
	vbox->pack_start(type_selector, true, true);

	Gtk::Box hbox(Gtk::ORIENTATION_HORIZONTAL, 6);

	Gtk::Label x1_label("x1:");
	Gtk::SpinButton rect_x1;
	rect_x1.set_range(0, width);
	rect_x1.set_increments(1, 10);
	rect_x1.set_numeric();

	Gtk::Box x1_hbox(Gtk::ORIENTATION_HORIZONTAL, 6);
	x1_hbox.pack_start(x1_label, true, true);
	x1_hbox.pack_start(rect_x1, true, true);
	hbox.pack_start(x1_hbox, false, true);

	Gtk::Label x2_label("x2:");
	Gtk::SpinButton rect_x2;
	rect_x2.set_range(0, width);
	rect_x2.set_increments(1, 10);
	rect_x2.set_numeric();

	Gtk::Box x2_hbox(Gtk::ORIENTATION_HORIZONTAL, 6);
	x2_hbox.pack_start(x2_label, true, true);
	x2_hbox.pack_start(rect_x2, true, true);
	hbox.pack_start(x2_hbox, false, true);

	Gtk::Label y1_label("y1:");
	Gtk::SpinButton rect_y1;
	rect_y1.set_range(0, height);
	rect_y1.set_increments(1, 10);
	rect_y1.set_numeric();

	Gtk::Box y1_hbox(Gtk::ORIENTATION_HORIZONTAL, 6);
	y1_hbox.pack_start(y1_label, true, true);
	y1_hbox.pack_start(rect_y1, true, true);
	hbox.pack_start(y1_hbox, false, true);

	Gtk::Label y2_label("y2:");
	Gtk::SpinButton rect_y2;
	rect_y2.set_range(0, height);
	rect_y2.set_increments(1, 10);
	rect_y2.set_numeric();

	Gtk::Box y2_hbox(Gtk::ORIENTATION_HORIZONTAL, 6);
	y2_hbox.pack_start(y2_label, true, true);
	y2_hbox.pack_start(rect_y2, true, true);
	hbox.pack_start(y2_hbox, false, true);

	vbox->pack_start(hbox, true, true);

	dialog.show_all();

	if (dialog.run() != Gtk::RESPONSE_ACCEPT) return;

	Poppler::Rectangle rect(rect_x1.get_value(), height - rect_y1.get_value(),
			rect_x2.get_value(), height - rect_y2.get_value());

	Glib::RefPtr<Poppler::AnnotText> annot = Poppler::AnnotText::create(m_Document, rect);
	page->add_annot(annot);
}

static Glib::ustring get_annot_type_string(Poppler::AnnotType type)
{
	switch (type)
	{
		case Poppler::ANNOT_TEXT:
			return "Text";
		case Poppler::ANNOT_LINK:
			return "Link";
		case Poppler::ANNOT_FREE_TEXT:
			return "Free Text";
		case Poppler::ANNOT_LINE:
			return "Line";
		case Poppler::ANNOT_SQUARE:
			return "Square";
		case Poppler::ANNOT_CIRCLE:
			return "Circle";
		case Poppler::ANNOT_POLYGON:
			return "Polygon";
		case Poppler::ANNOT_POLY_LINE:
			return "Poly Line";
		case Poppler::ANNOT_HIGHLIGHT:
			return "Highlight";
		case Poppler::ANNOT_UNDERLINE:
			return "Underline";
		case Poppler::ANNOT_SQUIGGLY:
			return "Squiggly";
		case Poppler::ANNOT_STRIKE_OUT:
			return "Strike Out";
		case Poppler::ANNOT_STAMP:
			return "Stamp";
		case Poppler::ANNOT_CARET:
			return "Caret";
		case Poppler::ANNOT_INK:
			return "Ink";
		case Poppler::ANNOT_POPUP:
			return "Popup";
		case Poppler::ANNOT_FILE_ATTACHMENT:
			return "File Attachment";
		case Poppler::ANNOT_SOUND:
			return "Sound";
		case Poppler::ANNOT_MOVIE:
			return "Movie";
		case Poppler::ANNOT_WIDGET:
			return "Widget";
		case Poppler::ANNOT_SCREEN:
			return "Screen";
		case Poppler::ANNOT_PRINTER_MARK:
			return "Printer Mark";
		case Poppler::ANNOT_TRAP_NET:
			return "Trap Net";
		case Poppler::ANNOT_WATERMARK:
			return "Watermark";
		case Poppler::ANNOT_3D:
			return "3D";
		default:
			break;
	}
  return "Unknown";
}


void Annots::get_annots()
{
	m_AnnotStore->clear();

	m_AnnotView.set_annot(Glib::RefPtr<Poppler::Annot>());

	Glib::RefPtr<Poppler::Page> page = m_Document->get_page(m_Page);
	if (!page) return;

	Glib::Timer timer;
	std::vector<Poppler::AnnotMapping> mapping = page->get_annot_mapping();
	timer.stop();
	
	int n_annots = mapping.size();

	if (n_annots > 0)
		m_TimerLabel.set_markup(Glib::ustring::format("<i>", n_annots, " annots found in ",
					std::fixed, std::setprecision(4), timer.elapsed(), " seconds</i>"));
	else
		m_TimerLabel.set_markup("<i>No annots found</i>");

	for (std::vector<Poppler::AnnotMapping>::iterator amapping = mapping.begin();
			amapping != mapping.end(); ++amapping)
	{
		Gtk::TreeRow tree_row = *(m_AnnotStore->append());

		Poppler::Rectangle area = amapping->get_area();
		tree_row[m_StoreColumns.m_X1] = area.get_x1();
		tree_row[m_StoreColumns.m_Y1] = area.get_y1();
		tree_row[m_StoreColumns.m_X2] = area.get_x2();
		tree_row[m_StoreColumns.m_Y2] = area.get_y2();

		Glib::RefPtr<Poppler::Annot> annot = amapping->get_annot();
		tree_row[m_StoreColumns.m_AnnotType] = get_annot_type_string(annot->get_annot_type());
		tree_row[m_StoreColumns.m_Color] = pixbuf_new_for_color(annot->get_color());

		Poppler::AnnotFlag flags = annot->get_flags();
		tree_row[m_StoreColumns.m_FlagInvisible] = flags & Poppler::ANNOT_FLAG_INVISIBLE;
		tree_row[m_StoreColumns.m_FlagHidden] = flags & Poppler::ANNOT_FLAG_HIDDEN;
		tree_row[m_StoreColumns.m_FlagPrint] = flags & Poppler::ANNOT_FLAG_PRINT;

		tree_row[m_StoreColumns.m_Annot] = annot;
	}
}

void Annots::selection_changed()
{
	Gtk::TreeIter iter = m_TreeView.get_selection()->get_selected();
	if (iter) m_AnnotView.set_annot((*iter)[m_StoreColumns.m_Annot]);
	else m_AnnotView.set_annot(Glib::RefPtr<Poppler::Annot>());
}

}
