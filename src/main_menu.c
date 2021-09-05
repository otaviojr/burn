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

#include "main_menu.h"

struct _GtkMainMenu
{
    GtkWidget parent;
};

struct _GtkMainMenuClass
{
    GtkWidgetClass parent_class;
};

enum
{
  GTK_MAIN_MENU_PROP_0,
  GTK_MAIN_MENU_PROP_LAST
};

G_DEFINE_TYPE (GtkMainMenu, gtk_main_menu, GTK_TYPE_WIDGET);

static GParamSpec
* gtk_main_menu_param_specs[GTK_MAIN_MENU_PROP_LAST] = { NULL, };

static void
gtk_main_menu_get_property(GObject * object,
    guint prop_id, GValue * value, GParamSpec * pspec)
{
    GtkMainMenu *self = GTK_MAIN_MENU(object);

    switch (prop_id) {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gtk_main_menu_set_property(GObject * object, guint prop_id, const GValue * value,
    GParamSpec * pspec)
{
    GtkMainMenu *self = GTK_MAIN_MENU(object);

    switch (prop_id) {
      default:
        G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
        break;
    }
}

static void
gtk_main_menu_constructed(GObject* object)
{
    GtkMainMenu *self = GTK_MAIN_MENU(object);
}

static void
gtk_main_menu_finalize(GObject * object)
{
    GtkMainMenu *self = GTK_MAIN_MENU(object);
}

static void
gtk_main_menu_class_init(GtkMainMenuClass * klass)
{
    GObjectClass *gobject_class = G_OBJECT_CLASS (klass);

    gobject_class->set_property = gtk_main_menu_set_property;
    gobject_class->get_property = gtk_main_menu_get_property;
    gobject_class->finalize = gtk_main_menu_finalize;
    gobject_class->constructed = gtk_main_menu_constructed;
    
    g_object_class_install_properties (gobject_class,
        GTK_MAIN_MENU_PROP_LAST, gtk_main_menu_param_specs);
}

static void
gtk_main_menu_init(GtkMainMenu * self)
{
    printf("Initializing main menu object\n");
}

GtkMainMenu *
gtk_main_menu_new()
{
    return g_object_new(GTK_TYPE_MAIN_MENU, NULL);
}
