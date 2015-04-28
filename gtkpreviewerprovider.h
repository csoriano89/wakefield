/* gtkpreviewerprovider.h
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
#ifndef GTK_PREVIEWER_PROVIDER_H
#define GTK_PREVIEWER_PROVIDER_H

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GTK_PREVIEWER_PROVIDER_EXTENSION_POINT_NAME "gtk-previewer-provider"

#define GTK_TYPE_PREVIEWER_PROVIDER (gtk_previewer_provider_get_type())

G_DECLARE_DERIVABLE_TYPE (GtkPreviewerProvider, gtk_previewer_provider, GTK, PREVIEWER_PROVIDER, GObject)

struct _GtkPreviewerProviderClass
{
  GObjectClass parent;

  GtkWidget*           (* create_widget_for_file)         (GtkPreviewerProvider *self,
                                                           GFile                *file);
  GtkWidget*           (* create_widget_for_stream)       (GtkPreviewerProvider *self,
                                                           GInputStream         *stream,
                                                           const gchar          *content_type);
  gboolean             (* supports_type)                  (GtkPreviewerProvider *self,
                                                           const gchar          *type);
};

GtkWidget*            gkt_previewer_provider_create_widget_for_stream        (GtkPreviewerProvider *self,
                                                                              GInputStream         *stream,
                                                                              const gchar          *content_type);
GtkWidget*            gtk_previewer_provider_create_widget_for_file          (GtkPreviewerProvider *self,
                                                                              GFile                *file);

gboolean              gtk_previewer_provider_supports_type                   (GtkPreviewerProvider *self,
                                                                              const gchar          *type);
void                  gtk_previewer_provider_ensure_extensions_registered    (void);
GList*                gtk_previewer_provider_get_all                         (void);

G_END_DECLS

#endif /* GTK_PREVIEWER_PROVIDER_H */
