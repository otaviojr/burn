#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "main_overlay.h"
#include "main_menu.h"
#include "presentation.h"

#include "config.h"

#define APP_NAME "Burn"

typedef GtkApplication GtkBurnApp;
typedef GtkApplicationClass GtkBurnAppClass;

GType gtk_burn_app_get_type (void);
G_DEFINE_TYPE (GtkBurnApp, gtk_burn_app, GTK_TYPE_APPLICATION);

typedef struct
{
    GtkApplicationWindow parent;

    GtkWidget* main_overlay;
    GtkWidget* main_container;
    GtkMainMenu* main_menu;
    GtkPresentation* presentation;

    GtkButton* open_file;
    GtkWidget *open_file_image;
    GtkButton* play;

} GtkBurnWindow;

typedef GtkApplicationWindowClass GtkBurnWindowClass;
G_DEFINE_TYPE (GtkBurnWindow, gtk_burn_window, GTK_TYPE_APPLICATION_WINDOW);

enum
{
  PROP_0,
  LAST_PROP
};

static GParamSpec *gtk_burn_properties[LAST_PROP] = { NULL, };

static void
gtk_burn_window_set_property (GObject * object, guint prop_id, const GValue * value,
    GParamSpec * pspec)
{
  switch (prop_id) {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}


static gboolean overlay_callback(GtkOverlay *overlay, 
				 GtkWidget *widget, 
                                 GdkRectangle *allocation,
                                 gpointer data )
{
    GtkBurnWindow *win = (GtkBurnWindow *)data;

    if ( GTK_IS_OVERLAY( overlay ) )
    {
        allocation->height = 100;
        allocation->width = 100;
        if(strcmp(gtk_widget_get_name(widget),"open_file_button") == 0){
            allocation->x     = gtk_widget_get_width(overlay) - 120;
            allocation->y     = gtk_widget_get_height(overlay) - 120;
        } else {
           allocation->x     = gtk_widget_get_width(overlay) - 120;
           allocation->y     = gtk_widget_get_height(overlay) - 240;
        }

        return TRUE;
    }

    return FALSE;
}

static void open_file_press_callback(GtkButton *btn, gpointer data)
{
  GtkBurnWindow *win = (GtkBurnWindow*)data;
  gtk_burn_file_chooser_dialog_new(GTK_WINDOW(win));
}

static void
create_ui(GtkBurnWindow * win)
{
    gchar* open_file_icon_filename = g_build_filename(PREFIX,DATADIR,"burn","resources","icons","open_file.png",NULL);

    win->main_overlay = gtk_overlay_new();
    g_signal_connect( win->main_overlay, "get-child-position", G_CALLBACK( overlay_callback ), win );

    win->main_container = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

    win->main_menu = gtk_main_menu_new();
    gtk_box_prepend(GTK_BOX(win->main_container), GTK_WIDGET(win->main_menu));

    win->presentation = gtk_presentation_new();
    gtk_box_append(GTK_BOX(win->main_container), GTK_WIDGET(win->presentation));

    gtk_overlay_set_child(GTK_OVERLAY(win->main_overlay), win->main_container);
    gtk_window_set_child(GTK_WINDOW(win), GTK_WIDGET(win->main_overlay));

    gtk_widget_set_vexpand(GTK_WIDGET(win->presentation), true);
    gtk_widget_set_hexpand(GTK_WIDGET(win->presentation), true);

    gtk_widget_show(GTK_WIDGET(win->main_overlay));
    gtk_widget_show(win->main_container);
    gtk_widget_show(GTK_WIDGET(win->main_menu));
    gtk_widget_show(GTK_WIDGET(win->presentation));

    win->open_file = gtk_button_new();
    gtk_widget_set_halign(GTK_WIDGET(win->open_file), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(win->open_file), GTK_ALIGN_END);
    gtk_widget_set_size_request(GTK_WIDGET(win->open_file), 100, 100);
    gtk_widget_set_name(GTK_WIDGET(win->open_file), "open_file_button");
    gtk_widget_add_css_class(GTK_WIDGET(win->open_file), "presentation-button");
    gtk_overlay_add_overlay(GTK_OVERLAY(win->main_overlay), GTK_WIDGET(win->open_file));
    win->open_file_image = gtk_image_new_from_file(open_file_icon_filename);
    gtk_button_set_child(GTK_WIDGET(win->open_file), win->open_file_image);
    g_signal_connect(G_OBJECT(win->open_file), "clicked", G_CALLBACK(open_file_press_callback), win);
    gtk_widget_show(GTK_WIDGET(win->open_file));

    win->play = gtk_button_new();
    gtk_widget_set_halign(GTK_WIDGET(win->play), GTK_ALIGN_END);
    gtk_widget_set_valign(GTK_WIDGET(win->play), GTK_ALIGN_END);
    gtk_widget_set_size_request(GTK_WIDGET(win->play), 100, 100);
    gtk_widget_set_name(GTK_WIDGET(win->play), "play_button");
    gtk_widget_add_css_class(GTK_WIDGET(win->play), "presentation-button");
    gtk_overlay_add_overlay(GTK_OVERLAY(win->main_overlay), GTK_WIDGET(win->play));
    gtk_widget_show(GTK_WIDGET(win->play));

    gtk_application_add_window (GTK_APPLICATION (g_application_get_default ()),
      GTK_WINDOW (win));

    //gtk_presentation_open_from_file(win->presentation, "/home/cage/bottom.gbr");

    g_free(open_file_icon_filename);
}

static void
attach_style(GtkBurnWindow * win)
{
  GtkCssProvider *cssProvider = gtk_css_provider_new();
  gchar* filename = g_build_filename(PREFIX,DATADIR,"burn","resources","css","main.css",NULL);
  gtk_css_provider_load_from_path(cssProvider, filename);
  gtk_style_context_add_provider_for_display(gdk_display_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_free(filename);
  g_object_unref (cssProvider);
}

static GObject *
gtk_burn_window_constructor (GType type, guint n_construct_params,
    GObjectConstructParam * construct_params)
{
  GtkBurnWindow *self;

  self =
      (GtkBurnWindow *) G_OBJECT_CLASS (gtk_burn_window_parent_class)->constructor (type,
      n_construct_params, construct_params);

  create_ui(self);
  attach_style(self);

  return G_OBJECT (self);
}

static void
gtk_burn_window_dispose (GObject * object)
{
  GtkBurnWindow *self = (GtkBurnWindow *) object;

  G_OBJECT_CLASS (gtk_burn_window_parent_class)->dispose(object);
}

static void
gtk_burn_window_class_init (GtkBurnWindowClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->constructor = gtk_burn_window_constructor;
  object_class->dispose = gtk_burn_window_dispose;
  object_class->set_property = gtk_burn_window_set_property;

  g_object_class_install_properties (object_class, LAST_PROP,
      gtk_burn_properties);
}

static void
gtk_burn_window_init(GtkBurnWindow * win)
{
}

static gint
gtk_burn_app_command_line (GApplication * application,
    GApplicationCommandLine * command_line)
{
  GtkBurnWindow *win;

  win =
      g_object_new (gtk_burn_window_get_type (), NULL);
  gtk_widget_show(GTK_WIDGET(win));

  return
      G_APPLICATION_CLASS (gtk_burn_app_parent_class)->command_line
      (application, command_line);
}

static void
gtk_burn_app_class_init (GtkBurnAppClass * klass)
{
  GApplicationClass *application_class = G_APPLICATION_CLASS (klass);
  application_class->command_line = gtk_burn_app_command_line;
}

static GtkBurnApp*
gtk_burn_app_new(void)
{
  GtkBurnApp* self;

  self = g_object_new (gtk_burn_app_get_type(),
      "application-id", "org.gtk.delta3d.burn",
      "flags", G_APPLICATION_HANDLES_COMMAND_LINE,
      "register-session", TRUE, NULL);

  g_application_set_default (G_APPLICATION (self));

  return self;
}

static void
gtk_burn_app_init (GtkBurnApp * self)
{
}

int main(int argc, char *argv[]){
  GtkBurnApp *app;
  gint status;

  app = gtk_burn_app_new ();
  status = g_application_run (G_APPLICATION (app), argc, argv);
  g_object_unref (app);

  return status;
}
