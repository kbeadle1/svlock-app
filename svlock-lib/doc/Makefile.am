CLEANFILES = rm -rf html man

if HAVE_DOXYGEN
MANUALS = html/index.html
else
MANUALS =
endif

SRCFILES = $(top_srcdir)/svlock/*.c \
	   $(top_srcdir)/svlock/*.h \
	   $(top_srcdir)/utils/*/*.[ch] \
	   Doxyfile

all: $(MANUALS)

html/index.html: $(SRCFILES)
	$(DOXYGEN)

HTML_DIR=${datarootdir}/doc/svlock
MAN_DIR=${datarootdir}/man

install-data-local:
	@MKDIR_P@ $(DESTDIR)$(HTML_DIR)/html
	-@INSTALL@ -m 0644 $(srcdir)/html/* $(DESTDIR)$(HTML_DIR)/html
	@MKDIR_P@ $(DESTDIR)$(MAN_DIR)/man3
	-@INSTALL@ -m 0644 $(srcdir)/man/man3/* $(DESTDIR)$(MAN_DIR)/man3
