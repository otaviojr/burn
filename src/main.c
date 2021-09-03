#include <stdio.h>
#include <gtk/gtk.h>
#include <gdk/gdk.h>

#include "config.h"

#define APP_NAME "Burn"

typedef GtkApplication GtkBurnApp;
typedef GtkApplicationClass GtkBurnAppClass;

GType gtk_burn_app_get_type (void);
G_DEFINE_TYPE (GtkBurnApp, gtk_burn_app, GTK_TYPE_APPLICATION);

typedef struct
{
  GtkApplicationWindow parent;

  GPtrArray* g_players;

  GtkBox* main_area;
  GtkGrid* cameras_area;
  GtkWidget* null_layer;
  GtkWidget* group_list_title_widget;
  GtkWidget* main_popover_mennu_widget;
  guint cameras_count;
  guint cameras_total_group;

  //GtkIpcamPreferenceObj* preference;

} GtkBurnWindow;

typedef GtkApplicationWindowClass GtkBurnWindowClass;
G_DEFINE_TYPE (GtkBurnWindow, gtk_burn_window, GTK_TYPE_APPLICATION_WINDOW);

enum
{
  PROP_0,
//  PROP_URIS,
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

static void
create_ui(GtkBurnWindow * win)
{
    GtkWidget *button;
    button = gtk_button_new_with_label ("Hello World");

    gtk_window_set_child(GTK_WINDOW(win), button);
    gtk_widget_show (button);

    gtk_application_add_window (GTK_APPLICATION (g_application_get_default ()),
      GTK_WINDOW (win));
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
  gtk_widget_show(win);

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
