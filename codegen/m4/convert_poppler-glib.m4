dnl Copyright (c) 2010  Glenn Rice <glennrister@gmail.com>
dnl This file is part of poppler-glibmm.

_CONV_ENUM(Poppler,ActionLayerAction)
_CONV_ENUM(Poppler,ActionMovieOperation)
_CONV_ENUM(Poppler,ActionType)
_CONV_ENUM(Poppler,AnnotExternalDataType)
_CONV_ENUM(Poppler,AnnotFlag)
_CONV_ENUM(Poppler,AnnotFreeTextQuadding)
_CONV_ENUM(Poppler,AnnotMarkupReplyType)
_CONV_ENUM(Poppler,AnnotTextState)
_CONV_ENUM(Poppler,AnnotType)
_CONV_ENUM(Poppler,Backend)
_CONV_ENUM(Poppler,DestType)
_CONV_ENUM(Poppler,FontType)
_CONV_ENUM(Poppler,FormButtonType)
_CONV_ENUM(Poppler,FormChoiceType)
_CONV_ENUM(Poppler,FormFieldType)
_CONV_ENUM(Poppler,FormTextType)
_CONV_ENUM(Poppler,PageLayout)
_CONV_ENUM(Poppler,PageMode)
_CONV_ENUM(Poppler,PageTransitionAlignment)
_CONV_ENUM(Poppler,PageTransitionDirection)
_CONV_ENUM(Poppler,PageTransitionType)
_CONV_ENUM(Poppler,Permissions)
_CONV_ENUM(Poppler,PrintFlags)
_CONV_ENUM(Poppler,SelectionStyle)
_CONV_ENUM(Poppler,ViewerPreferences)

_CONVERSION(`PopplerDocument*',`Glib::RefPtr<Document>',`Glib::wrap($3)')
_CONVERSION(`const Glib::RefPtr<Document>&',`PopplerDocument*',`($3)->gobj()')
_CONVERSION(`PopplerDest*',`Dest',`Glib::wrap($3)')
_CONVERSION(`const PopplerDest*',`Dest',`Glib::wrap($3, true)')
_CONVERSION(`PopplerFontInfo*',`Glib::RefPtr<FontInfo>',`Glib::wrap($3)')
_CONVERSION(`PopplerPSFile*',`Glib::RefPtr<PSFile>',`Glib::wrap($3)')
_CONVERSION(`const Glib::RefPtr<PSFile>&',`PopplerPSFile*',`($3)->gobj()')
_CONVERSION(`PopplerFormField*',`Glib::RefPtr<FormField>',`Glib::wrap($3)')
_CONVERSION(`PopplerIndexIter*',`IndexIter',`Glib::wrap($3)')
_CONVERSION(`PopplerLayersIter*',`LayersIter',`Glib::wrap($3)')
_CONVERSION(`PopplerAction*',`Action',`Glib::wrap($3)')
_CONVERSION(`const PopplerAction*',`Action',`Action($3)')
_CONVERSION(`PopplerLayer*',`Glib::RefPtr<Layer>',`Glib::wrap($3)')
_CONVERSION(`PopplerMedia*',`Glib::RefPtr<Media>',`Glib::wrap($3)')
_CONVERSION(`PopplerMedia*',`Glib::RefPtr<const Media>',`Glib::wrap($3)')
_CONVERSION(`PopplerMovie*',`Glib::RefPtr<Movie>',`Glib::wrap($3)')
_CONVERSION(`PopplerMovie*',`Glib::RefPtr<const Movie>',`Glib::wrap($3)')
_CONVERSION(`PopplerFormField*',`Glib::RefPtr<const FormField>',`Glib::wrap($3)')

_CONVERSION(`PopplerAnnot*',`Glib::RefPtr<AnnotText>',`Glib::wrap((PopplerAnnotText*)($3))')
_CONVERSION(`const Glib::RefPtr<Annot>&',`PopplerAnnot*',`($3)->gobj()')
_CONVERSION(`PopplerAnnotCalloutLine*',`AnnotCalloutLine',`Glib::wrap($3)')
_CONVERSION(`PopplerAnnot*',`Glib::RefPtr<const Annot>',`Glib::wrap($3)')
_CONVERSION(`PopplerAnnot*',`Glib::RefPtr<Annot>',`Glib::wrap($3)')

_CONVERSION(`PopplerPage*',`Glib::RefPtr<Page>',`Glib::wrap($3)')
_CONVERSION(`PopplerPageTransition*',`PageTransition',`Glib::wrap($3)')

_CONVERSION(`const Cairo::RefPtr<Cairo::Context>&',`cairo_t*',`($3)->cobj()')
_CONVERSION(`const Cairo::RefPtr<const Cairo::Context>&',`cairo_t*',`const_cast<cairo_t*>(($3)->cobj())')
_CONVERSION(`const Cairo::RefPtr<Cairo::Surface>&',`cairo_surface_t*',`($3)->cobj()')
_CONVERSION(`const Cairo::RefPtr<const Cairo::Surface>&',`cairo_surface_t*',`const_cast<cairo_surface_t*>(($3)->cobj())')
_CONVERSION(`cairo_surface_t*',`Cairo::RefPtr<Cairo::ImageSurface>',`Cairo::RefPtr<Cairo::ImageSurface>(new Cairo::ImageSurface($3))')
_CONVERSION(`cairo_region_t*',`Cairo::RefPtr<Cairo::Region>',`Cairo::RefPtr<Cairo::Region>(new Cairo::Region($3))')

_CONVERSION(`PopplerAttachment*',`Glib::RefPtr<Attachment>',`Glib::wrap($3)')
_CONVERSION(`GList*',`Glib::ListHandle< Glib::RefPtr<Attachment> >',`$2($3, Glib::OWNERSHIP_NONE)')
_CONVERSION(`GList*',`Glib::ListHandle< Glib::RefPtr<const Attachment> >',`$2($3, Glib::OWNERSHIP_NONE)')

_CONVERSION(`Rectangle&',`PopplerRectangle*',($3).gobj())
_CONVERSION(`Poppler::Rectangle&',`PopplerRectangle*',($3).gobj())
_CONVERSION(`const Rectangle&',`const PopplerRectangle*',($3).gobj())
_CONVERSION(`const Rectangle&',`PopplerRectangle*',`const_cast<PopplerRectangle*>(($3).gobj())')
_CONVERSION(`const Poppler::Rectangle&',`const PopplerRectangle*',($3).gobj())
_CONVERSION(`PopplerRectangle*',`Rectangle',`Glib::wrap($3)')
_CONVERSION(`PopplerRectangle',`Rectangle',`Rectangle(const_cast<PopplerRectangle*>(&($3)))')

_CONVERSION(`Color&',`PopplerColor*',($3).gobj())
_CONVERSION(`Poppler::Color&',`PopplerColor*',($3).gobj())
_CONVERSION(`const Color&',`const PopplerColor*',($3).gobj())
_CONVERSION(`const Color&',`PopplerColor*',`const_cast<PopplerColor*>(($3).gobj())')
_CONVERSION(`const Poppler::Color&',`const PopplerColor*',($3).gobj())
_CONVERSION(`PopplerColor',`Color',`Color(const_cast<PopplerColor*>(&($3)))')
_CONVERSION(`Color',`PopplerColor',`*(($3).gobj())')
_CONVERSION(`PopplerColor*',`Color',`Glib::wrap($3)')
