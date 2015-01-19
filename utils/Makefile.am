
noinst_PROGRAMS=sample

noinst_HEADERS = \
    getline.h \
    i18n.h \
    $(NULL)

bin_PROGRAMS=ucd

ucd_SOURCES= \
    dummy.cpp \
    getline.c \
    i18n.c \
    ucd.c \
    $(NULL)

sample_SOURCES= \
    dummy.cpp \
    sample.c \
    $(NULL)

# force the autoconf use g++ to link the c program with c++ lib
dummy.cpp:
	touch $@

sample_LDADD=../src/libucd.la

ucd_LDADD=

# add the required lib to the end of link command line
#ucd_LDADD+=$(LIBUCD_CFLAGS) $(LIBUCD_LIBS)

#ucd_LDADD += /usr/lib/x86_64-linux-gnu/libicui18n.a /usr/lib/x86_64-linux-gnu/libicuuc.a /usr/lib/x86_64-linux-gnu/libicudata.a -ldl
#ucd_LDADD +=/usr/lib/libucd.a
ucd_LDADD +=../src/libucd.la

#ucd_CPPFLAGS = $(LIBUCD_CFLAGS) $(AM_CPPFLAGS)
#ucd_LDFLAGS = $(LIBUCD_LIBS) $(AM_LDFLAGS)



AM_LDFLAGS=$(OPTS)
AM_CPPFLAGS=$(OPTS)

DEFS+= \
    `getconf LFS_CFLAGS` \
    `getconf LFS64_CFLAGS` \
    -D_GNU_SOURCE \
    -D_FILE_OFFSET_BITS=64 \
    -DHAVE_MMAP64=1 \
    -DUSE_FINDHTML=1 \
    $(NULL)

AM_CPPFLAGS+= \
    -I$(top_srcdir)/src \
    -I$(top_builddir)/include/ \
    $(NULL)

AM_LDFLAGS += \
    -L$(top_builddir)/src/ \
    `getconf LFS_LDFLAGS` \
    `getconf LFS64_LDFLAGS` \
    $(NULL)

if USE_ICONV
DEFS+= -DUSE_ICONV=1
#AM_LDFLAGS +=-L/usr/lib/x86_64-linux-gnu/
#AM_LDFLAGS += -liconv # iconv is a part of glib now, yum install glibc-common
#AM_CPPFLAGS += `pkg-config --cflags libucd`
#AM_LDFLAGS += `pkg-config --libs libucd`
#AM_CPPFLAGS += $(LIBUCD_CFLAGS)
#AM_LDFLAGS += $(LIBUCD_LIBS)
endif

if USE_ICU
DEFS+= -DUSE_ICU=1
#AM_LDFLAGS+=-L/usr/lib/x86_64-linux-gnu/
#AM_CPPFLAGS += `pkg-config --cflags icu`
#AM_LDFLAGS+= `pkg-config --libs icu`
AM_CPPFLAGS += `pkg-config --cflags icu-i18n`
AM_LDFLAGS+= `pkg-config --libs icu-i18n`
#libseederdict_la_CPPFLAGS += $(LIBICU_CFLAGS)
#libseederdict_la_LDFLAGS+= $(LIBICU_LIBS)
endif

#AM_CPPFLAGS += `pkg-config --cflags zlib`
#AM_LDFLAGS += `pkg-config --libs zlib`
#AM_CPPFLAGS += $(ZLIB_CFLAGS)
#AM_LDFLAGS += $(ZLIB_LIBS)

#AM_LDFLAGS += -pthread


if DEBUG
# use "valgrind --tool=memcheck --leak-check=yes" to check memory leak, MemWatch will drag the program.
#DEFS+=-DMEMWATCH
DEFS+= -DDEBUG=1
AM_LDFLAGS += -lbfd
AM_CPPFLAGS+=-g -Wall

else
AM_CPPFLAGS+=-O3 -Wall
endif

