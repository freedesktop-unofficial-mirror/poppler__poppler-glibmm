// Generated by generate_wrap_init.pl -- DO NOT MODIFY!

#include <glibmm.h>

// Disable the 'const' function attribute of the get_type() functions.
// GCC would optimize them out because we don't use the return value.
#undef  G_GNUC_CONST
#define G_GNUC_CONST /* empty */

#include <poppler-glibmm/wrap_init.h>
#include <glibmm/error.h>
#include <glibmm/object.h>

// #include the widget headers so that we can call the get_type() static methods:
#include "action.h"
#include "annot.h"
#include "annot_callout_line.h"
#include "annot_mapping.h"
#include "attachment.h"
#include "color.h"
#include "document.h"
#include "error.h"
#include "font_info.h"
#include "fonts_iter.h"
#include "form_field.h"
#include "form_field_mapping.h"
#include "image_mapping.h"
#include "index_iter.h"
#include "layer.h"
#include "layers_iter.h"
#include "link_mapping.h"
#include "media.h"
#include "movie.h"
#include "page.h"
#include "page_transition.h"
#include "ps_file.h"
#include "rectangle.h"
#include "text_attributes.h"
#include "utility.h"

extern "C"
{
//Declarations of the *_get_type() functions:

GType poppler_annot_get_type(void);
GType poppler_annot_markup_get_type(void);
GType poppler_annot_text_get_type(void);
GType poppler_annot_free_text_get_type(void);
GType poppler_annot_file_attachment_get_type(void);
GType poppler_annot_movie_get_type(void);
GType poppler_annot_screen_get_type(void);
GType poppler_attachment_get_type(void);
GType poppler_document_get_type(void);
GType poppler_font_info_get_type(void);
GType poppler_form_field_get_type(void);
GType poppler_layer_get_type(void);
GType poppler_media_get_type(void);
GType poppler_movie_get_type(void);
GType poppler_page_get_type(void);
GType poppler_ps_file_get_type(void);

//Declarations of the *_error_quark() functions:

GQuark poppler_error_quark(void);
} // extern "C"

namespace Poppler {

//Declarations of the *_Class::wrap_new() methods, instead of including all the private headers:

class Annot_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class AnnotMarkup_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class AnnotText_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class AnnotFreeText_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class AnnotFileAttachment_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class AnnotMovie_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class AnnotScreen_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class Attachment_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class Document_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class FontInfo_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class FormField_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class Layer_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class Media_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class Movie_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class Page_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };
class PSFile_Class { public: static Glib::ObjectBase* wrap_new(GObject*); };

void wrap_init()
{
  // Register Error domains in the main namespace:
  Glib::Error::register_domain(poppler_error_quark(), &Error::throw_func);

  // Map gtypes to gtkmm wrapper-creation functions:
  Glib::wrap_register(poppler_annot_get_type(), &Annot_Class::wrap_new);
  Glib::wrap_register(poppler_annot_markup_get_type(), &AnnotMarkup_Class::wrap_new);
  Glib::wrap_register(poppler_annot_text_get_type(), &AnnotText_Class::wrap_new);
  Glib::wrap_register(poppler_annot_free_text_get_type(), &AnnotFreeText_Class::wrap_new);
  Glib::wrap_register(poppler_annot_file_attachment_get_type(), &AnnotFileAttachment_Class::wrap_new);
  Glib::wrap_register(poppler_annot_movie_get_type(), &AnnotMovie_Class::wrap_new);
  Glib::wrap_register(poppler_annot_screen_get_type(), &AnnotScreen_Class::wrap_new);
  Glib::wrap_register(poppler_attachment_get_type(), &Attachment_Class::wrap_new);
  Glib::wrap_register(poppler_document_get_type(), &Document_Class::wrap_new);
  Glib::wrap_register(poppler_font_info_get_type(), &FontInfo_Class::wrap_new);
  Glib::wrap_register(poppler_form_field_get_type(), &FormField_Class::wrap_new);
  Glib::wrap_register(poppler_layer_get_type(), &Layer_Class::wrap_new);
  Glib::wrap_register(poppler_media_get_type(), &Media_Class::wrap_new);
  Glib::wrap_register(poppler_movie_get_type(), &Movie_Class::wrap_new);
  Glib::wrap_register(poppler_page_get_type(), &Page_Class::wrap_new);
  Glib::wrap_register(poppler_ps_file_get_type(), &PSFile_Class::wrap_new);

  // Register the gtkmm gtypes:
  Annot::get_type();
  AnnotMarkup::get_type();
  AnnotText::get_type();
  AnnotFreeText::get_type();
  AnnotFileAttachment::get_type();
  AnnotMovie::get_type();
  AnnotScreen::get_type();
  Attachment::get_type();
  Document::get_type();
  FontInfo::get_type();
  FormField::get_type();
  Layer::get_type();
  Media::get_type();
  Movie::get_type();
  Page::get_type();
  PSFile::get_type();

} // wrap_init()

} // Poppler

