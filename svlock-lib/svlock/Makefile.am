NULL =

LIB_MAJOR = 1
LIB_MINOR = 0

SUBDIRS = 		\
	$(NULL)


AM_CPPFLAGS = 					\
	-I$(top_srcdir) 			\
	-I$(top_srcdir)/svlock		\
	 $(LIBXML_CFLAGS) 			\
	-Wall -Werror -std=c99 -D_XOPEN_SOURCE=500	\
	$(NULL)

lib_LTLIBRARIES = libsvlock.la

libsvlock_la_SOURCES = 			\
	libsvlock.c				\
	$(NULL)


libsvlock_la_LIBADD = 						\
	-lpthread						\
	$(NULL)

libsvlock_la_LDFLAGS = $(CYGWIN_EXTRA_LDFLAGS) $(WIN32_EXTRA_LDFLAGS) \
                     $(MODULE_PLATFORM_LIBS) 				\
		     -version-number $(LIB_MAJOR):$(LIB_MINOR):0

svlockincdir = $(includedir)/svlock

svlockinc_HEADERS = 			\
	svlock.h			\
	$(NULL)
