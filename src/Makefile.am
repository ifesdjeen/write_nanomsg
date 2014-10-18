AM_CFLAGS = -I/Users/ifesdjeen/foss/c/collectd/src/daemon -Wall -g -O3

# AM_LDFLAGS = -module -avoid-version

# pkglib_LTLIBRARIES = write_nanomsg.la
# write_nanomsg_la_SOURCES = write_nanomsg.c
# write_nanomsg_la_LDFLAGS = -module -avoid-version -shared
# write_nanomsg_la_CFLAGS = $(COLLECTD_CFLAGS)

# EXTRA_DIST = utils_format_json.c utils_format_json.h

# # AM_* rules are common to all programs
# AM_CPPFLAGS = -g -O2 -Wall
# #AM_LDFLAGS = -lm

# # pkglib_LTLIBRARIES = writenanomsg.la
# # writenanomsg_la_SOURCES = write_nanomsg.c
# # writenanomsg_la_LDFLAGS = -module -avoid-version -shared

# EXTRA_DIST = utils_format_json.c utils_format_json.h

# # Program specific rules
# #write_nanomsg_CPPFLAGS =
# #write_nanomsg_LDADD =




# pkglibdir=/Users/ifesdjeen/opt/collectd/collectd
pkglib_LTLIBRARIES = write_nanomsg.la
write_nanomsg_la_SOURCES = write_nanomsg.c utils_format_json.c
write_nanomsg_la_LDFLAGS = -module -avoid-version -shared -I/Users/ifesdjeen/foss/c/collectd/src
write_nanomsg_la_CFLAGS = -I/Users/ifesdjeen/hackage/write_nanomsg/collectd/src -lnanomsg -avoid-version -Wall -g -O3
EXTRA_DIST = utils_format_json.c
