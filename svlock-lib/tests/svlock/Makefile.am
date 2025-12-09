NULL =

bindir = ${exec_prefix}/bin/svlock-ut

SUBDIRS = 						\
	$(NULL)

AM_CPPFLAGS =						\
	-DLOCALDIR=\""$(srcdir)"\"			\
	-I$(srcdir) 					\
	-I$(top_srcdir)					\
	-Wall -Werror -std=c99				\
	$(NULL)

bin_PROGRAMS =						\
	svlock_test					\
	$(NULL)

LDADDS = $(top_builddir)/svlock/libsvlock.la

svlock_test_LDADD = $(LDADDS)

EXTRA_DIST =						\
	$(NULL)

TESTS_ENVIRONMENT = 					\
        $(NULL)

TESTS =	 						\
	$(NULL)


TEST_EXTENSIONS = .py

if HAVE_PYTHON
TESTS_ENVIRONMENT += \
	PYTHONPATH="`$(top_builddir)/tests/svlock/build-python-path.sh $(top_builddir)`"   \
	LD_PRELOAD=		\
	LD_LIBRARY_PATH="$(top_builddir)/svlock/.libs"          \
	$(NULL)
PY_LOG_COMPILER=$(PYTHON)

TESTS +=			\
	touch-svlock-apis.py		\
	$(NULL)
endif
