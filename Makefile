COLLECTD_SRC := collectd
COLLECTD_PREFIX := /Users/ifesdjeen/opt/collectd
LIBTOOL      := $(COLLECTD_SRC)/libtool
CFLAGS       ?= -DNDEBUG -O3

all: build write_nanomsg.la

build: build/write_nanomsg.lo

# write_nanomsg.so: src/write_nanomsg.c collectd/src/config.h
# 	gcc src/write_nanomsg.c -o write_nanomsg.so -shared -fPIC -lnanomsg -I$(COLLECTD_SRC)/src -DHAVE_CONFIG_H -Wall

collectd/configure:
	cd collectd; ./build.sh

collectd/src/config.h: collectd/configure
	cd collectd; ./configure $(CONFIGURE_ARGS)

write_nanomsg.la: build/write_nanomsg.lo
	$(LIBTOOL) --tag=CC --mode=link gcc -Wall -Werror $(CFLAGS) -module \
		-fPIC -lnanomsg -avoid-version -o $@ -rpath $(COLLECTD_PREFIX)/lib/collectd \
		-lpthread build/write_nanomsg.lo

build/utils_format_json.lo: src/utils_format_json.c
	$(LIBTOOL) --mode=compile gcc -DHAVE_CONFIG_H -I src \
		-I $(COLLECTD_SRC)/src -Wall -Werror $(CFLAGS) -MD -MP -c \
		-o $@ src/utils_format_json.c

build/write_nanomsg.lo: build/utils_format_json.lo src/write_nanomsg.c
	$(LIBTOOL) --mode=compile gcc -DHAVE_CONFIG_H -I src \
		-I $(COLLECTD_SRC)/src -Wall -Werror $(CFLAGS) -MD -MP -c \
		-o $@ src/write_nanomsg.c

.PHONY: clean
clean:
	rm -rf .libs
	rm -rf build
	rm -f write_graphite.la

install: all
	$(LIBTOOL) --mode=install /usr/bin/install -c write_nanomsg.la \
		$(COLLECTD_PREFIX)/lib/collectd
	$(LIBTOOL) --finish \
		$(COLLECTD_PREFIX)/lib/collectd
