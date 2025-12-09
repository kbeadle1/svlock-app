AM_CPPFLAGS =			\
	-I$(top_srcdir)		\
	-I$(top_srcdir)/svlock	\
	$(LIBXML_CFLAGS)	\
	-Wall 			\
	-std=c99		\
	$(NULL)

AM_LDFLAGS= -L../svlock/.libs

SOURCES=svlock_wrap.c svlock.py

all-local:
	[ -d $(srcdir)/svlock ] || mkdir $(srcdir)/svlock
	[ -d $(abs_builddir)/svlock ] || mkdir $(abs_builddir)/svlock
	cp $(srcdir)/svlock.py $(srcdir)/svlock/__init__.py
	(cd $(srcdir);CPPFLAGS="$(AM_CPPFLAGS)" LDFLAGS="$(AM_LDFLAGS)" $(PYTHON) $(abs_builddir)/setup.py build --build-base $(abs_builddir)/build)

install-exec-local:
	if [ -d /etc/apt ]; then \
		(cd $(srcdir);CPPFLAGS="$(AM_CPPFLAGS)" LDFLAGS="$(AM_LDFLAGS)" $(PYTHON) $(abs_builddir)/setup.py install --root=$(DESTDIR) --install-layout=deb ) \
	else \
		(cd $(srcdir);CPPFLAGS="$(AM_CPPFLAGS)" LDFLAGS="$(AM_LDFLAGS)" $(PYTHON) $(abs_builddir)/setup.py install --root=$(DESTDIR) ) \
	fi

clean-local:
#	$(PYTHON) setup.py clean
	rm -rf build
