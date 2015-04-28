/* test-previewer-basic.c
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

#include <glib.h>
#include "gtkpreviewer.h"

gint
main (gint   argc,
      gchar *argv[])
{
  GtkWidget *window;
  GtkWidget *previewer;
  GFile *file;

  gtk_init (&argc, &argv);

  g_set_prgname ("my-program");
  g_set_application_name ("My-program");

  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_widget_set_size_request (window, 200, 200);
  previewer = GTK_WIDGET (gtk_previewer_new ());
  gtk_container_add (GTK_CONTAINER (window), previewer);

  file = g_file_new_for_path ("/home/csoriano/Pictures/preview.png");
  g_assert (file != NULL);
  gtk_previewer_set_file (GTK_PREVIEWER (previewer), file);

  gtk_widget_show_all (window);

  gtk_main ();

  return 0;
}
