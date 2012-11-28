## Copyright (c) 2010  Glenn Rice <glennrister@gmail.com>
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

include $(top_srcdir)/poppler-glib/src/filelist.am
include $(top_srcdir)/poppler-glib/poppler-glibmm/filelist.am

poppler_glibmm_files_h = $(files_built_h) $(filter-out wrap_init.h,$(files_extra_h))

book_name = $(POPPLER_GLIBMM_MODULE_NAME)
doc_input = $(top_srcdir)/poppler-glib/poppler-glibmm.h	$(addprefix\
            $(top_srcdir)/poppler-glib/poppler-glibmm/,$(poppler_glibmm_files_h))

include $(top_srcdir)/build/doc-reference.am
