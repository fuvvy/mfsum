AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS} -I m4
AM_CFLAGS = -I${top_srcdir}/inc

TESTS = quick_test
check_PROGRAMS = quick_test
quick_test_SOURCES = test/quick_test.c \
        src/mfsum.c \
        inc/mfsum.h \
        src/stack.c \
        inc/stack.h \
        src/memory.c \
        inc/memory.h \
        inc/uthash.h \
        config.h
quick_test_LDADD = ${lib_LTLIBRARIES}
quick_test_CFLAGS = $(AM_CFLAGS)

bin_PROGRAMS = mfsum
mfsum_SOURCES = src/main.c \
		src/mfsum.c \
		inc/mfsum.h \
		src/stack.c \
		inc/stack.h \
		src/memory.c \
		inc/memory.h \
		inc/uthash.h \
		config.h
mfsum_CFLAGS = $(AM_CFLAGS)

lib_LTLIBRARIES = libmfsum-@API_VERSION@.la
libmfsum_@API_VERSION@_la_SOURCES = src/mfsum.c \
				    inc/mfsum.h \
				    src/stack.c \
				    inc/stack.h \
				    src/memory.c \
				    inc/memory.h \
				    inc/uthash.h

libmfsum_@API_VERSION@_la_LDFLAGS = -version-info $(SO_VERSION)
mfsum_includedir = $(includedir)/mfsum-$(API_VERSION)
nobase_mfsum_include_HEADERS = inc/mfsum.h
mfsum_libincludedir = $(libdir)/mfsum-$(API_VERSION)/include
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = mfsum-$(API_VERSION).pc
