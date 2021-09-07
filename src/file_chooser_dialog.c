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

#include "config.h"
#include "file_chooser_dialog.h"

static void
gtk_file_chooser_dialog_response(GtkDialog *dialog,
                    int        response,
                    gpointer   user_data)
{
    if (response == GTK_RESPONSE_OK){
    }

    gtk_window_destroy (GTK_WINDOW (dialog));
}

void gtk_burn_file_chooser_dialog_new(GtkWidget* parent)
{
    GtkWidget* content_area;
    GtkWidget* file_chooser_widget;

    GtkWidget* file_chooser_dialog = gtk_dialog_new_with_buttons(gettext("File Chooser"),
                                                                 GTK_WINDOW(parent),
                                                                 GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_MODAL,
                                                                 "OK", GTK_RESPONSE_OK,
                                                                 "Cancel", GTK_RESPONSE_CANCEL,
                                                                 NULL);

    gtk_window_set_default_size(GTK_WINDOW(file_chooser_dialog), gtk_widget_get_width(GTK_WIDGET(parent)), gtk_widget_get_height(GTK_WIDGET(parent)));
    g_signal_connect (file_chooser_dialog, "response", G_CALLBACK (gtk_file_chooser_dialog_response), NULL);

    content_area = gtk_dialog_get_content_area (GTK_DIALOG (file_chooser_dialog));
    file_chooser_widget = gtk_file_chooser_widget_new ( GTK_FILE_CHOOSER_ACTION_OPEN );
    gtk_widget_set_hexpand (file_chooser_widget, TRUE);
    gtk_widget_set_vexpand (file_chooser_widget, TRUE);
    gtk_box_append (GTK_BOX (content_area), file_chooser_widget);

    gtk_widget_show(file_chooser_dialog);
}
