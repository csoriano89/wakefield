/* gtkpreviewerproviderimage.c
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
#include "gtkpreviewerproviderimage.h"

typedef struct
{

} GtkPreviewerProviderImagePrivate;

G_DEFINE_TYPE_WITH_CODE (GtkPreviewerProviderImage, gtk_previewer_provider_image, GTK_TYPE_PREVIEWER_PROVIDER,
                         g_io_extension_point_implement (GTK_PREVIEWER_PROVIDER_EXTENSION_POINT_NAME,
                                                         g_define_type_id,
                                                         GTK_PREVIEWER_PROVIDER_IMAGE_NAME,
                                                         0));

enum {
  PROP_0,
  LAST_PROP
};

static GParamSpec *gParamSpecs [LAST_PROP];

GtkPreviewerProviderImage *
gtk_previewer_provider_image_new (void)
{
  return g_object_new (GTK_TYPE_PREVIEWER_PROVIDER_IMAGE, NULL);
}

static void
gtk_previewer_provider_image_finalize (GObject *object)
{
  G_OBJECT_CLASS (gtk_previewer_provider_image_parent_class)->finalize (object);
}

static void
gtk_previewer_provider_image_get_property (GObject    *object,
                                           guint       prop_id,
                                           GValue     *value,
                                           GParamSpec *pspec)
{
  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static void
gtk_previewer_provider_image_set_property (GObject      *object,
                                           guint         prop_id,
                                           const GValue *value,
                                           GParamSpec   *pspec)
{
  switch (prop_id)
    {
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
    }
}

static GtkWidget*
gtk_previewer_provider_image_create_widget_for_file (GtkPreviewerProvider *self,
                                                     GFile                *file)
{
  const gchar *file_path;

  file_path = g_file_get_path (file);
  return gtk_image_new_from_file (file_path);
}

static gboolean
gtk_previewer_provider_image_supports_type (GtkPreviewerProvider *self,
                                            const gchar          *type)
{
  return g_strcmp0 (type, "image/png") == 0;
}

static void
gtk_previewer_provider_image_class_init (GtkPreviewerProviderImageClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GtkPreviewerProviderClass *provider_class = GTK_PREVIEWER_PROVIDER_CLASS (klass);

  object_class->finalize = gtk_previewer_provider_image_finalize;
  object_class->get_property = gtk_previewer_provider_image_get_property;
  object_class->set_property = gtk_previewer_provider_image_set_property;

  provider_class->create_widget_for_file = gtk_previewer_provider_image_create_widget_for_file;
  provider_class->supports_type = gtk_previewer_provider_image_supports_type;
}

static void
gtk_previewer_provider_image_init (GtkPreviewerProviderImage *self)
{
}
