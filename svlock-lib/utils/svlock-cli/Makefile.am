NULL =

bin_PROGRAMS =		\
	svlock-cli	\
	$(NULL)

AM_CPPFLAGS = 					\
	-I$(top_srcdir) 			\
	-I$(top_srcdir)/utils/svlock-cli 	\
	-Wall -Werror -std=c99			\
	-DHIDE_DEPRECATED			\
	-D_POSIX_C_SOURCE=200112L		\
	$(NULL)


svlock_cli_SOURCES =		\
	svlock-cli.c		\
	args.c			\
	$(NULL)

svlock_cli_LDADD = $(top_builddir)/svlock/libsvlock.la

EXTRA_DIST = \
	args.h		\
	$(NULL)
