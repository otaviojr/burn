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

#include "gerber/gerbv.h"

gboolean check_file_ext ( const gchar *filename, const gchar *fileext );

struct _GtkPresentation
{
    GtkWidget parent;

    GValue filename;
    GValue frames;
    GValue current_frame;
    GValue state;
    GValue presentation_type;
};

struct _GtkPresentationClass
{
    GtkWidgetClass parent_class;
};

enum
{
  GTK_PRESENTATION_PROP_0,
  GTK_PRESENTATION_PROP_FILENAME,
  GTK_PRESENTATION_PROP_FRAMES,
  GTK_PRESENTATION_PROP_CURRENT_FRAME,
  GTK_PRESENTATION_PROP_STATE,
  GTK_PRESENTATION_PROP_PRESENTATION_TYPE,
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
      GTK_PRESENTATION_PROP_FILENAME:
        g_value_set_string(value, g_value_get_string(&self->filename));
        break;

      GTK_PRESENTATION_PROP_FRAMES:
        g_value_set_int(value, g_value_get_int(&self->frames));
        break;

      GTK_PRESENTATION_PROP_CURRENT_FRAME:
        g_value_set_int(value, g_value_get_int(&self->current_frame));
        break;

      GTK_PRESENTATION_PROP_STATE:
        g_value_set_int(value, g_value_get_int(&self->state));
        break;

      GTK_PRESENTATION_PROP_PRESENTATION_TYPE:
        g_value_set_int(value, g_value_get_int(&self->presentation_type));
        break;

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
  cairo_t* cr;
  float width, height;

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

  width = gtk_widget_get_width (widget);
  height = gtk_widget_get_height (widget);
  cr = gtk_snapshot_append_cairo(snapshot, &GRAPHENE_RECT_INIT(0,0,width,height));
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

    g_value_unset(&self->filename);
    g_value_unset(&self->frames);
    g_value_unset(&self->current_frame);
    g_value_unset(&self->state);
    g_value_unset(&self->presentation_type);
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
        [GTK_PRESENTATION_PROP_FILENAME] =
        g_param_spec_string("file", "File",
        "File currently being presented", NULL,
        G_PARAM_READWRITE);

    gtk_presentation_param_specs
        [GTK_PRESENTATION_PROP_CURRENT_FRAME] =
        g_param_spec_object ("current_frame", "Current Frame",
        "Current frame", G_TYPE_INT,
        G_PARAM_READABLE);

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

    gtk_presentation_param_specs
        [GTK_PRESENTATION_PROP_PRESENTATION_TYPE] =
        g_param_spec_object ("presentation_type", "Presentation Type",
        "Current presentation type based on the filename", G_TYPE_INT,
        G_PARAM_READABLE);

    g_object_class_install_properties (gobject_class,
        GTK_PRESENTATION_PROP_LAST, gtk_presentation_param_specs);
}

static void
gtk_presentation_init(GtkPresentation * self)
{
    printf("Initializing presentation object\n");

    g_value_init(&self->filename, G_TYPE_STRING);
    g_value_init(&self->frames, G_TYPE_INT);
    g_value_init(&self->current_frame, G_TYPE_INT);
    g_value_init(&self->state, G_TYPE_INT);
    g_value_init(&self->presentation_type, G_TYPE_INT);

    g_value_set_int(&self->state, PRESENTATION_STATE_IDLE);
    g_value_set_int(&self->frames, 0);
    g_value_set_int(&self->current_frame, 0);
    g_value_set_int(&self->presentation_type, PRESENTATION_TYPE_NO_CONTENT);
}

gboolean
gtk_presentation_open_from_file(GtkPresentation * self, const gchar* val)
{
    g_return_val_if_fail (GTK_IS_PRESENTATION(self), FALSE);

    g_value_set_string(&self->filename, val);

    /* clear any current presentation */
    if(g_value_get_int(&self->state) != PRESENTATION_STATE_IDLE){
        //TODO: clear current presentation
    }

    /* check the type of this file to choose the correct processor */

    if(check_file_ext(val,"gbr") || check_file_ext(val, "xln")){
        //TODO: process greber files
        g_value_set_int(&self->presentation_type, PRESENTATION_TYPE_GERBER);
        g_value_set_int(&self->state, PRESENTATION_STATE_STILL);

        gerbv_project_t * gerber_project = gerbv_create_project();

    } else if(check_file_ext(val,"svg")) {
        //TODO: process svg files
    } else if(check_file_ext(val,"jpg") || check_file_ext(val,"jpeg")) {
        //TODO: process jpg files
    } else if(check_file_ext(val,"png")) {
        //TODO: process png files
    } else {
        return FALSE;
    }

    return TRUE;
}

GtkPresentation *
gtk_presentation_new()
{
    return g_object_new(GTK_TYPE_PRESENTATION, NULL);
}

gboolean check_file_ext ( const gchar *filename, const gchar *fileext )
{
    gboolean ret = false;
    const gchar *basename = g_path_get_basename(filename);
    g_assert( filename );
    g_assert( fileext && fileext[0]=='.' );
    if (!basename) return FALSE;

    const char * dot = strrchr(basename, '.');
    if (dot && !strcmp(dot, fileext))
        ret = true;

    g_free((gpointer)basename);
    return ret;
}
