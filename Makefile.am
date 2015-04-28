NULL=
lib_LTLIBRARIES = libwakefield.la

EXTRA_DIST = \
	protocol/xdg-shell.xml		\
	$(NULL)

BUILT_SOURCES =				\
	xdg-shell-server-protocol.h	\
	xdg-shell-protocol.c		\
	$(NULL)

DISTCLEANFILES =			\
	$(BUILT_SOURCES)		\
	$(NULL)

libwakefieldincludedir = $(includedir)/wakefield

libwakefieldinclude_HEADERS =		\
	wakefield-compositor.h		\
	$(NULL)

nodist_libwakefield_la_SOURCES =	\
	$(BUILT_SOURCES)		\
	$(NULL)

libwakefield_la_SOURCES =		\
	wakefield-private.h		\
	wakefield-compositor.h		\
	wakefield-compositor.c		\
	wakefield-surface.c		\
	wakefield-data-device.c		\
	$(NULL)

libwakefield_la_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)
libwakefield_la_LIBADD = $(GTK_LIBS)

%-protocol.c : $(srcdir)/protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) code < $< > $@
	$(AM_V_GEN)$(SED) -i -e 's/WL_EXPORT //' $@

%-server-protocol.h : $(srcdir)/protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) server-header < $< > $@

%-client-protocol.h : $(srcdir)/protocol/%.xml
	$(AM_V_GEN)$(WAYLAND_SCANNER) client-header < $< > $@

noinst_PROGRAMS =			\
	test-compositor			\
	test-embedding			\
	test-embedded			\
	test-previewer-basic			\
	$(NULL)

test_compositor_LDADD = libwakefield.la
test_compositor_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)

test_compositor_SOURCES =	\
	test-compositor.c	\
	$(NULL)

test_compositor_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)

test_embedding_LDADD = libwakefield.la

test_embedding_SOURCES =	\
	test-embedding.c	\
	$(NULL)

test_embedding_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)

test_embedded_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)

test_embedded_LDADD = libwakefield.la

test_embedded_SOURCES =	\
	test-embedded.c	\
	$(NULL)

test_embedded_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)

test_previewer_basic_CFLAGS = $(AM_CFLAGS) $(GTK_CFLAGS)
test_previewer_basic_LDADD = $(GTK_LIBS);

test_previewer_basic_SOURCES =	\
	gtkpreviewerprovider.c \
	gtkpreviewerprovider.h \
	gtkpreviewerproviderimage.c \
	gtkpreviewerproviderimage.h \
	gtkpreviewer.c	\
	gtkpreviewer.h	\
	test-previewer-basic.c	\
	$(NULL)
