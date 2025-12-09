
ACLOCAL_AMFLAGS= -I m4
NULL =

SUBDIRS = 		\
	svlock	\
	python 		\
	utils		\
	tests		\
	$(NULL)

if HAVE_DOXYGEN
SUBDIRS += 		\
	doc
endif


confexecdir=$(libdir)

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA=svlock.pc

bin_SCRIPTS = svlock-config

DIST_COMMON =

EXTRA_DIST = 			\
	configure		\
	configure.ac		\
	Makefile.am		\
	svlock-config.in		\
	svlock.pc.in		\
	$(NULL)
