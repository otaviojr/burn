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
#include <gtk/gtkoverlay.h>

#include "main_overlay.h"

struct _GtkMainOverlay
{
    GtkWidget parent;
};

struct _GtkMainOverlayClass
{
    GtkWidgetClass parent_class;
};

G_DEFINE_TYPE (GtkMainOverlay, gtk_main_overlay, GTK_TYPE_OVERLAY);

static void
gtk_main_overlay_snapshot (GtkWidget *widget, GtkSnapshot *snapshot)
{
    GtkMainOverlay *self = GTK_MAIN_OVERLAY(widget);
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
gtk_main_overlay_finalize(GObject * object)
{
    G_OBJECT_CLASS (gtk_main_overlay_parent_class)->finalize (object);
}

static void
gtk_main_overlay_class_init(GtkMainOverlayClass * klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);
    GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

    gobject_class->finalize = gtk_main_overlay_finalize;
    widget_class->snapshot = gtk_main_overlay_snapshot;
}

static void
gtk_main_overlay_init(GtkMainOverlay * self)
{
    printf("Initializing main overlay object\n");
}

GtkMainOverlay *
gtk_main_overlay_new()
{
    return g_object_new(GTK_TYPE_MAIN_OVERLAY, NULL);
}
