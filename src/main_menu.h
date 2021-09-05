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

#ifndef __BURN_MAIN_MENU_H__
#define __BURN_MAIN_MENU_H__

typedef struct _GtkMainMenu GtkMainMenu;
typedef struct _GtkMainMenuClass GtkMainMenuClass;

#define GTK_TYPE_MAIN_MENU                         (gtk_main_menu_get_type ())
#define GTK_IS_MAIN_MENU(obj)                      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MAIN_MENU))
#define GTK_IS_MAIN_MENU_CLASS(klass)              (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MAIN_MENU))
#define GTK_MAIN_MENU_GET_CLASS(obj)               (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MAIN_MENU, GtkMainMenuClass))
#define GTK_MAIN_MENU(obj)                         (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MAIN_MENU, GtkMainMenu))
#define GTK_MAIN_MENU_CLASS(klass)                 (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_MAIN_MENU, GtkMainMenuClass))
#define GTK_MAIN_MENU_CAST(obj)                    ((GtkMainMenu*)(obj))

G_BEGIN_DECLS

GType gtk_main_menu_get_type(void);
GtkMainMenu* gtk_main_menu_new();

G_END_DECLS

#endif //__BURN_PRESENTATION_H__
