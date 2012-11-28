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

ACLOCAL_AMFLAGS = -I build ${ACLOCAL_FLAGS}
DISTCHECK_CONFIGURE_FLAGS = --enable-warnings=fatal

if MAINTAINER_MODE
src_subdirs = codegen poppler-glib/src
else
src_subdirs =
endif
if ENABLE_DOCUMENTATION
doc_subdirs = doc
else
doc_subdirs =
endif
SUBDIRS = $(src_subdirs) poppler-glib/poppler-glibmm examples $(doc_subdirs)

poppler_glibmm_includedir = $(includedir)/$(POPPLER_GLIBMM_MODULE_NAME)
poppler_glibmm_include_HEADERS = poppler-glib/poppler-glibmm.h

poppler_glibmm_libincludedir = $(libdir)/$(POPPLER_GLIBMM_MODULE_NAME)/include
nodist_poppler_glibmm_libinclude_HEADERS = poppler-glib/poppler-glibmmconfig.h

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = poppler-glib/$(POPPLER_GLIBMM_MODULE_NAME).pc

dist_noinst_SCRIPTS = autogen.sh

# Optional: auto-generate the ChangeLog file from the git log on make dist
#include $(top_srcdir)/build/dist-changelog.am
