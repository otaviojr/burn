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

#ifndef __BURN_MAIN_OVERLAY_H__
#define __BURN_MAIN_OVERLAY_H__

typedef struct _GtkMainOverlay GtkMainOverlay;
typedef struct _GtkMainOverlayClass GtkMainOverlayClass;

#define GTK_TYPE_MAIN_OVERLAY                         (gtk_main_overlay_get_type ())
#define GTK_IS_MAIN_OVERLAY(obj)                      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MAIN_OVERLAY))
#define GTK_IS_MAIN_OVERLAY_CLASS(klass)              (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MAIN_OVERLAY))
#define GTK_MAIN_OVERLAY_GET_CLASS(obj)               (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MAIN_OVERLAY, GtkMainOverlayClass))
#define GTK_MAIN_OVERLAY(obj)                         (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MAIN_OVERLAY, GtkMainOverlay))
#define GTK_MAIN_OVERLAY_CLASS(klass)                 (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_MAIN_OVERLAY, GtkMainOverlayClass))
#define GTK_MAIN_OVERLAY_CAST(obj)                    ((GtkMainOverlay*)(obj))

G_BEGIN_DECLS

GType gtk_main_overlay_get_type(void);
GtkMainOverlay* gtk_main_overlay_new();

G_END_DECLS

#endif //__BURN_MAIN_OVERLAY_H__
