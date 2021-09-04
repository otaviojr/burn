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

#include <gtk/gtk.h>

#include "presentation.h"

struct _GtkPresentation
{
    GtkWidget parent;
};

struct _GtkPresentationClass
{
    GtkWidgetClass parent_class;
};

enum
{
  GTK_PRESENTATION_PROP_0,
  GTK_PRESENTATION_PROP_FILE,
  GTK_PRESENTATION_PROP_FRAMES,
  GTK_PRESENTATION_PROP_STATE,
  GTK_PRESENTATION_PROP_LAST
};

G_DEFINE_TYPE (GtkPresentation, gtk_presentation, GTK_TYPE_WIDGET);

static GParamSpec
* gtk_presentation_param_specs[GTK_PRESENTATION_PROP_LAST] = { NULL, };

static void
gtk_presentation_get_property(GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec)
{
    GtkPresentation *self = GTK_PRESENTATION(object);

    switch (prop_id) {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gtk_presentation_set_property(GObject * object, guint prop_id, const GValue * value,
    GParamSpec * pspec)
{
    GtkPresentation *self = GTK_PRESENTATION(object);

    switch (prop_id) {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gtk_presentation_snapshot (GtkWidget *widget, GtkSnapshot *snapshot)
{
  GdkRGBA red, green, yellow, blue;
  float w, h;

  gdk_rgba_parse (&red, "red");
  gdk_rgba_parse (&green, "green");
  gdk_rgba_parse (&yellow, "yellow");
  gdk_rgba_parse (&blue, "blue");

  w = gtk_widget_get_width (widget) / 2.0;
  h = gtk_widget_get_height (widget) / 2.0;

  gtk_snapshot_append_color (snapshot, &red,
                             &GRAPHENE_RECT_INIT(0, 0, w, h));
  gtk_snapshot_append_color (snapshot, &green,
                             &GRAPHENE_RECT_INIT(w, 0, w, h));
  gtk_snapshot_append_color (snapshot, &yellow,
                             &GRAPHENE_RECT_INIT(0, h, w, h));
  gtk_snapshot_append_color (snapshot, &blue,
                             &GRAPHENE_RECT_INIT(w, h, w, h));
}

static void
gtk_presentation_constructed(GObject* object)
{
    GtkPresentation *self = GTK_PRESENTATION(object);
}

static void
gtk_presentation_finalize(GObject * object)
{
    GtkPresentation *self = GTK_PRESENTATION(object);
}

static void
gtk_presentation_class_init(GtkPresentationClass * klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gobject_class->set_property = gtk_presentation_set_property;
    gobject_class->get_property = gtk_presentation_get_property;
    gobject_class->finalize = gtk_presentation_finalize;
    gobject_class->constructed = gtk_presentation_constructed;
    
    widget_class->snapshot = gtk_presentation_snapshot;

    gtk_presentation_param_specs
        [GTK_PRESENTATION_PROP_FILE] =
        g_param_spec_object ("file", "File",
        "File to read the frame from", G_TYPE_STRING,
        G_PARAM_READABLE | G_PARAM_WRITABLE);

    gtk_presentation_param_specs
        [GTK_PRESENTATION_PROP_FRAMES] =
        g_param_spec_object ("frames", "Frames",
        "Total Number of frames available do render", G_TYPE_INT,
        G_PARAM_READABLE);

    gtk_presentation_param_specs
        [GTK_PRESENTATION_PROP_STATE] =
        g_param_spec_object ("state", "State",
        "Current state of the presentation", G_TYPE_INT,
        G_PARAM_READABLE);

    g_object_class_install_properties (gobject_class,
        GTK_PRESENTATION_PROP_LAST, gtk_presentation_param_specs);
}

static void
gtk_presentation_init(GtkPresentation * self)
{
    printf("Initializing presentation object\n");
}

GtkPresentation *
gtk_presentation_new()
{
    return g_object_new(GTK_TYPE_PRESENTATION, NULL);
}
