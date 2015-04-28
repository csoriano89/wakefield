/* gtkpreviewerprovider.c
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
#include "gtkpreviewerprovider.h"
#include "gtkpreviewerproviderimage.h"

typedef struct
{

} GtkPreviewerProviderPrivate;

G_DEFINE_TYPE (GtkPreviewerProvider, gtk_previewer_provider, G_TYPE_OBJECT)

static void
gtk_previewer_provider_class_init (GtkPreviewerProviderClass *klass)
{

}

static void
gtk_previewer_provider_init (GtkPreviewerProvider *self)
{

}

GtkWidget*
gtk_previewer_provider_create_widget_for_file (GtkPreviewerProvider *self,
                                               GFile                *file)
{
   g_return_val_if_fail (G_IS_FILE (file), NULL);

  return GTK_PREVIEWER_PROVIDER_GET_CLASS (self)->create_widget_for_file (self, file);
}

GtkWidget*
gkt_previewer_provider_create_widget_for_stream (GtkPreviewerProvider *self,
                                                 GInputStream         *stream,
                                                 const gchar          *content_type)
{
   g_return_val_if_fail (G_IS_INPUT_STREAM (stream), NULL);

  return GTK_PREVIEWER_PROVIDER_GET_CLASS (self)->create_widget_for_stream (self, stream, content_type);
}

gboolean
gtk_previewer_provider_supports_type (GtkPreviewerProvider *self,
                                      const gchar          *type)
{
  return GTK_PREVIEWER_PROVIDER_GET_CLASS (self)->supports_type (self, type);
}

void
gtk_previewer_provider_ensure_extensions_registered (void)
{
  static gsize once_init_value = 0;

  if (g_once_init_enter (&once_init_value))
    {
      GIOExtensionPoint *extension_point;

      extension_point = g_io_extension_point_register (GTK_PREVIEWER_PROVIDER_EXTENSION_POINT_NAME);
      g_io_extension_point_set_required_type (extension_point, GTK_TYPE_PREVIEWER_PROVIDER);

      /* Ensure built-ins */
      g_type_ensure (GTK_TYPE_PREVIEWER_PROVIDER_IMAGE);

      g_once_init_leave (&once_init_value, 1);
    }
}

GList*
gtk_previewer_provider_get_all (void)
{
  GIOExtensionPoint *extension_point;
  GList *extensions;

  gtk_previewer_provider_ensure_extensions_registered ();

  extension_point = g_io_extension_point_lookup (GTK_PREVIEWER_PROVIDER_EXTENSION_POINT_NAME);
  extensions = g_io_extension_point_get_extensions (extension_point);

  return extensions;
}
