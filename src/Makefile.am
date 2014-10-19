pkglib_LTLIBRARIES = write_nanomsg.la
write_nanomsg_la_SOURCES = write_nanomsg.c utils_format_json.c
write_nanomsg_la_LDFLAGS = -module -avoid-version -shared -lnanomsg
write_nanomsg_la_CFLAGS = -I${abs_top_builddir}/collectd/src -Wall -g -O3
EXTRA_DIST = utils_format_json.c
