
# public headers
# it will be moved to include/Makefile.am
include_HEADERS = \
    $(top_srcdir)/include/libucd.h \
    $(NULL)

#EXTRA_DIST = libucd.pc.in
#pkgconfigdir = $(libdir)/pkgconfig
#pkgconfig_DATA = libucd.pc

lib_LTLIBRARIES=libucd.la

libucd_la_SOURCES = \
	ucdapi.cpp \
	CharDistribution.cpp \
	JpCntx.cpp \
	LangBulgarianModel.cpp \
	LangCyrillicModel.cpp \
	LangGreekModel.cpp \
	LangHebrewModel.cpp \
	LangHungarianModel.cpp \
	LangThaiModel.cpp \
	nsBig5Prober.cpp \
	nsCharSetProber.cpp \
	nsEUCJPProber.cpp \
	nsEUCKRProber.cpp \
	nsEUCTWProber.cpp \
	nsEscCharsetProber.cpp \
	nsEscSM.cpp \
	nsGB2312Prober.cpp \
	nsHebrewProber.cpp \
	nsLatin1Prober.cpp \
	nsMBCSGroupProber.cpp \
	nsMBCSSM.cpp \
	nsSBCSGroupProber.cpp \
	nsSBCharSetProber.cpp \
	nsSJISProber.cpp \
	nsUTF8Prober.cpp \
	nsUniversalDetector.cpp \
	$(NULL)

libucd_la_CPPFLAGS = -I$(top_srcdir)/src/ -I$(top_srcdir)/include/

#DEFS+= -I$(top_srcdir)/src/ -I$(top_srcdir)/include/

#DEFS+=-DMEMWATCH

if DEBUG
DEFS+= -DDEBUG=1
endif

AM_CPPFLAGS=-g -Wall
