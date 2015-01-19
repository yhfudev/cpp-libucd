
EXTRA_DIST = mkpairmodel.py README.txt
EXTRA_DIST += \
    french/french_WINDOWS-1252.txt \
    french/charstats_french_WINDOWS-1252.txt \
    $(NULL)

noinst_PROGRAMS=mkcharstats
#bin_PROGRAMS=mkcharstats

noinst_HEADERS = \
    $(NULL)

mkcharstats_SOURCES= \
    mkcharstats.cpp \
    $(NULL)

# force the autoconf use g++ to link the c program with c++ lib
#dummy.cpp:
#	touch $@

mkcharstats_LDADD=

# add the required lib to the end of link command line
#mkcharstats_LDADD+=$(LIBUCD_CFLAGS) $(LIBUCD_LIBS)

#mkcharstats_CPPFLAGS = $(LIBUCD_CFLAGS) $(AM_CPPFLAGS)
#mkcharstats_LDFLAGS = $(LIBUCD_LIBS) $(AM_LDFLAGS)

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

#AM_CPPFLAGS+= \
    -I$(top_srcdir)/src \
    -I$(top_builddir)/include/ \
    $(NULL)

#AM_LDFLAGS += \
    -L$(top_builddir)/src/ \
    `getconf LFS_LDFLAGS` \
    `getconf LFS64_LDFLAGS` \
    $(NULL)

if DEBUG
# use "valgrind --tool=memcheck --leak-check=yes" to check memory leak, MemWatch will drag the program.
#DEFS+=-DMEMWATCH
DEFS+= -DDEBUG=1
AM_LDFLAGS += -lbfd
AM_CPPFLAGS+=-g -Wall

else
AM_CPPFLAGS+=-O3 -Wall
endif

