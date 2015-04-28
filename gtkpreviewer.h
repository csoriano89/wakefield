/* gtkpreviewer.h
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
#ifndef GTK_PREVIEWER_H
#define GTK_PREVIEWER_H

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define GTK_TYPE_PREVIEWER (gtk_previewer_get_type())

G_DECLARE_DERIVABLE_TYPE (GtkPreviewer, gtk_previewer, GTK, PREVIEWER, GtkBin)

struct _GtkPreviewerClass
{
  GtkBinClass parent;
};

GtkPreviewer *gtk_previewer_new (void);
void          gtk_previewer_set_file (GtkPreviewer *self,
                                      GFile        *file);

G_END_DECLS

#endif /* GTK_PREVIEWER_H */
