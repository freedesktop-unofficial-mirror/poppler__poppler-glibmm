## Copyright (c) 2010  Glenn Rice <glennricster@gmail.com>
##
## This file is part of poppler-glibmm.
##
## poppler-glibmm is free software: you can redistribute it and/or modify it
## under the terms of the GNU Lesser General Public License as published
## by the Free Software Foundation, either version 2.1 of the License,
## or (at your option) any later version.
##
## poppler-glibmm is distributed in the hope that it will be useful, but
## WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU Lesser General Public License for more details.
##
## You should have received a copy of the GNU Lesser General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

AUTOMAKE_OPTIONS = subdir-objects

check_PROGRAMS = demo/poppler-glibmm-demo

local_includes = -I$(top_builddir)/poppler-glib $(if $(srcdir:.=),-I$(top_srcdir)/poppler-glib)
local_libs = $(top_builddir)/poppler-glib/poppler-glibmm/libpoppler-glibmm-$(POPPLER_GLIBMM_API_VERSION).la

AM_CPPFLAGS = -I$(top_builddir) $(local_includes) $(GTHREAD_CFLAGS) $(POPPLER_GLIBMM_CFLAGS) $(GTKMM_CFLAGS)
AM_CXXFLAGS = $(POPPLER_GLIBMM_WXXFLAGS)

LDADD = $(POPPLER_GLIBMM_LIBS) $(GTKMM_LIBS) $(local_libs)

demo_poppler_glibmm_demo_SOURCES = \
								   demo/action-view.cc		\
								   demo/action-view.h		\
								   demo/annots.cc			\
								   demo/annots.h			\
								   demo/attachments.cc		\
								   demo/attachments.h		\
								   demo/auth-dialog.cc		\
								   demo/auth-dialog.h		\
								   demo/demo-window.cc		\
								   demo/demo-window.h		\
								   demo/find.cc				\
								   demo/find.h				\
								   demo/fonts.cc			\
								   demo/fonts.h				\
								   demo/forms.cc			\
								   demo/forms.h				\
								   demo/images.cc			\
								   demo/images.h			\
								   demo/info.cc				\
								   demo/info.h				\
								   demo/layers.cc			\
								   demo/layers.h			\
								   demo/links.cc			\
								   demo/links.h				\
								   demo/main.cc				\
								   demo/outline.cc			\
								   demo/outline.h			\
								   demo/page-info.cc		\
								   demo/page-info.h			\
								   demo/print.cc		\
								   demo/print.h			\
								   demo/property-grid.cc	\
								   demo/property-grid.h		\
								   demo/render.cc			\
								   demo/render.h			\
								   demo/selections.cc		\
								   demo/selections.h		\
								   demo/text.cc				\
								   demo/text.h				\
								   demo/transitions.cc		\
								   demo/transitions.h		\
								   demo/util.cc				\
								   demo/util.h
