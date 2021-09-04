/*
 * Copyright (c) 2020 - Otávio Ribeiro <otavio.ribeiro@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __BURN_PRESENTATION_H__
#define __BURN_PRESENTATION_H__

enum
{
    PRESENTATION_STATE_IDLE,
    PRESENTATION_STATE_STILL,
    PRESENTATION_STATE_PLAYING,
};

enum
{
    PRESENTATION_TYPE_NO_CONTENT,
    PRESENTATION_TYPE_GERBER,
    PRESENTATION_TYPE_SVG,
    PRESENTATION_TYPE_PNG,
    PRESENTATION_TYPE_JPEG
};

typedef struct _GtkPresentation GtkPresentation;
typedef struct _GtkPresentationClass GtkPresentationClass;

#define GTK_TYPE_PRESENTATION                         (gtk_presentation_get_type ())
#define GTK_IS_PRESENTATION(obj)                      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PRESENTATION))
#define GTK_IS_PRESENTATION_CLASS(klass)              (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PRESENTATION))
#define GTK_PRESENTATION_GET_CLASS(obj)               (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PRESENTATION, GtkPresentationClass))
#define GTK_PRESENTATION(obj)                         (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRESENTATION, GtkPresentation))
#define GTK_PRESENTATION_CLASS(klass)                 (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_PRESENTATION, GtkPresentationClass))
#define GTK_PRESENTATION_CAST(obj)                    ((GtkPresentation*)(obj))

G_BEGIN_DECLS

GType gtk_presentation_get_type(void);
GtkPresentation* gtk_presentation_new();
gboolean gtk_presentation_open_from_file(GtkPresentation * self, const gchar* val);

G_END_DECLS

#endif //__BURN_PRESENTATION_H__
