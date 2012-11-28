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

#ifndef _INFO_H_
#define _INFO_H_

#include "property-grid.h"

#include <poppler-glibmm/document.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/frame.h>
#include <gtkmm/alignment.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>

namespace PGD
{

class Info : public Gtk::Box
{
	public:
		Info(const Glib::RefPtr<Poppler::Document>& document);

	private:
		Gtk::Label m_VersionLabel, m_FrameLabel;
		Gtk::Frame m_Frame;
		Gtk::Alignment m_Alignment;
		PropertyGrid m_Grid;

		Gtk::Box m_PermissionsHBoxTop, m_PermissionsHBoxBottom, m_PermissionsVBox;
		Gtk::CheckButton m_PrintCB, m_CopyCB, m_ModifyCB, m_AddNotesCB, m_FillFormsCB,
			m_ExtractContentsCB, m_AssembleCB, m_HighResPrintCB;

		Gtk::ScrolledWindow m_MetadataWin;
		Gtk::TextView m_MetadataView;
};

}

#endif // _INFO_H_
