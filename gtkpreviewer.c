/* gtkpreviewer.c
 *
 * Copyright (C) 2015 Carlos Soriano <csoriano@gnome.org>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "gtkpreviewer.h"
#include "gtkpreviewerprovider.h"

typedef struct
{
  GFile *file;
  GtkWidget *current_preview;
  GInputStream *stream;
  GtkWidget *loading_spinner;

} GtkPreviewerPrivate;

G_DEFINE_TYPE_WITH_PRIVATE (GtkPreviewer, gtk_previewer, GTK_TYPE_BIN)

enum {
  PROP_0,
  PROP_FILE,
  PROP_STREAM,
  LAST_PROP
};

static void mark_as_loading (GtkPreviewer *self,
                             gboolean      loading);

static void
set_preview_from_file_info_callback (GObject      *source,
                                     GAsyncResult *result,
                                     gpointer      user_data)
{
  GFileInfo *info;
  GFile *file;
  GError *error;
  GtkPreviewer *self;
  GtkPreviewerPrivate *priv;

  self = GTK_PREVIEWER (user_data);
  priv = gtk_previewer_get_instance_private (self);
  file = G_FILE (source);
  info = g_file_query_info_finish (file, result, &error);

  if (info)
    {
      const gchar *type;
      GList *extensions;
      GList *l;
      GIOExtension *gio_extension;
      GtkPreviewerProvider *current_provider;
      GtkPreviewerProvider *handling_provider;

      handling_provider = NULL;
      type = g_file_info_get_content_type (info);
      g_type_ensure (GTK_TYPE_PREVIEWER_PROVIDER);
      extensions = gtk_previewer_provider_get_all ();

      g_print ("extensions %d\n", g_list_length (extensions));
      for (l = extensions; l != NULL; l = l->next)
        {
          gio_extension = l->data;
          current_provider = GTK_PREVIEWER_PROVIDER (g_object_new (g_io_extension_get_type (gio_extension), NULL));
          if (gtk_previewer_provider_supports_type (current_provider, type))
            {
              handling_provider = current_provider;
              break;
            }
         else
           {
             gchar *file_uri;

             file_uri = g_file_get_path (file);
             g_warning ("No previewer provider found for file %s of type %s", file_uri, type);

             g_object_unref (current_provider);
             g_free (file_uri);
           }
        }

      if (handling_provider != NULL)
        {
          priv->current_preview = gtk_previewer_provider_create_widget_for_file (handling_provider, file);

          mark_as_loading (self, FALSE);
          gtk_container_add (GTK_CONTAINER (self), priv->current_preview);
          gtk_widget_show_all (priv->current_preview);

          g_object_unref (current_provider);
        }
    }
}

static void
set_preview_from_file (GtkPreviewer *self,
                       GFile        *file)
{
  g_file_query_info_async (file,
                           G_FILE_ATTRIBUTE_STANDARD_CONTENT_TYPE,
                           G_FILE_QUERY_INFO_NONE,
                           0,
                           NULL,
                           (GAsyncReadyCallback) set_preview_from_file_info_callback,
                           self);
}

static void
update_preview (GtkPreviewer *self)
{
  GtkPreviewerPrivate *priv = gtk_previewer_get_instance_private (self);

  if (priv->current_preview != NULL)
    {
      gtk_widget_destroy (priv->current_preview);
    }
  mark_as_loading (self, TRUE);

  if (priv->file != NULL)
    {
      set_preview_from_file (self, priv->file);
    }
  else if (priv->stream != NULL)
    {
    }
  else
    {
      priv->current_preview = NULL;
    }
}

static GParamSpec *gParamSpecs [LAST_PROP];

GtkPreviewer *
gtk_previewer_item_new (void)
{
  return g_object_new (GTK_TYPE_PREVIEWER, NULL);
}

static void
gtk_previewer_item_finalize (GObject *object)
{
  G_OBJECT_CLASS (gtk_previewer_parent_class)->finalize (object);
}

static void
gtk_previewer_item_get_property (GObject    *object,
                                 guint       prop_id,
                                 GValue     *value,
                                 GParamSpec *pspec)
{
  GtkPreviewer *self = GTK_PREVIEWER (object);
  GtkPreviewerPrivate *priv = gtk_previewer_get_instance_private (self);

  switch (prop_id)
    {
    case PROP_FILE:
      g_value_set_object (value, priv->file);
      break;
    case PROP_STREAM:
      g_value_set_object (value, priv->stream);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtk_previewer_set_property (GObject      *object,
                            guint         prop_id,
                            const GValue *value,
                            GParamSpec   *pspec)
{
  GtkPreviewer *self = GTK_PREVIEWER (object);
  GtkPreviewerPrivate *priv = gtk_previewer_get_instance_private (self);

  switch (prop_id)
    {
    case PROP_FILE:
      if (priv->file != NULL)
        {
          g_object_unref (priv->file);
          priv->file = NULL;
        }
      if (priv->stream != NULL)
        {
          g_object_unref (priv->stream);
          priv->stream = NULL;
        }
      priv->file = g_object_ref (G_FILE (g_value_get_object (value)));
      update_preview (self);
      break;
    case PROP_STREAM:
      if (priv->file != NULL)
        {
          g_object_unref (priv->file);
          priv->file = NULL;
        }
      if (priv->stream != NULL)
        {
          g_object_unref (priv->stream);
          priv->stream = NULL;
        }
      priv->stream = g_object_ref (G_INPUT_STREAM (g_value_get_object (value)));
      update_preview (self);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtk_previewer_class_init (GtkPreviewerClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gtk_previewer_finalize;
  object_class->get_property = gtk_previewer_get_property;
  object_class->set_property = gtk_previewer_set_property;

  gParamSpecs [PROP_FILE] =
    g_param_spec_object ("file",
                         "File",
                         "File",
                         G_TYPE_FILE,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (object_class, PROP_FILE,
                                   gParamSpecs [PROP_FILE]);

  gParamSpecs [PROP_STREAM] =
    g_param_spec_object ("stream",
                         "Stream",
                         "Stream",
                         G_TYPE_INPUT_STREAM,
                         (G_PARAM_READWRITE |
                          G_PARAM_STATIC_STRINGS));
  g_object_class_install_property (object_class, PROP_STREAM,
                                   gParamSpecs [PROP_STREAM]);
}

static void
mark_as_loading (GtkPreviewer *self,
                 gboolean loading)
{
  GtkPreviewerPrivate *priv = gtk_previewer_get_instance_private (self);

  if (loading)
    {
      if (priv->loading_spinner == NULL)
        {
          priv->loading_spinner = gtk_spinner_new ();
          gtk_container_add (GTK_CONTAINER (self), priv->loading_spinner);
          gtk_widget_show (priv->loading_spinner);
          gtk_spinner_start (GTK_SPINNER (priv->loading_spinner));
        }
    }
  else
    {
      gtk_widget_destroy (priv->loading_spinner);
      priv->loading_spinner = NULL;
    }
}

void
gtk_previewer_set_file (GtkPreviewer *self,
                        GFile        *file)
{
  g_object_set (self, "file", file, NULL);
}

static void
gtk_previewer_init (GtkPreviewer *self)
{
  GtkPreviewerPrivate *priv = gtk_previewer_get_instance_private (self);

  gtk_widget_set_hexpand (GTK_WIDGET (self), TRUE);
  gtk_widget_set_vexpand (GTK_WIDGET (self), TRUE);

  mark_as_loading (self, TRUE);

  priv->file = NULL;
  priv->stream = NULL;
  priv->current_preview = NULL;
}
