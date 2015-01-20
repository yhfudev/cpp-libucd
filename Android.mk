# Makefile for the libucd in Android

LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := libucd
LOCAL_MODULE_TAGS := debug
LOCAL_PRELINK_MODULE := false

LOCAL_SRC_FILES := \
    src/ucdapi.cpp \
    src/CharDistribution.cpp \
    src/JpCntx.cpp \
    src/LangBulgarianModel.cpp \
    src/LangCyrillicModel.cpp \
    src/LangGreekModel.cpp \
    src/LangHebrewModel.cpp \
    src/LangHungarianModel.cpp \
    src/LangThaiModel.cpp \
    src/LangCzechModel.cpp \
    src/LangFinnishModel.cpp \
    src/LangFrenchModel.cpp \
    src/LangGermanModel.cpp \
    src/LangPolishModel.cpp \
    src/LangSpanishModel.cpp \
    src/LangSwedishModel.cpp \
    src/LangTurkishModel.cpp \
    src/nsBig5Prober.cpp \
    src/nsCharSetProber.cpp \
    src/nsEUCJPProber.cpp \
    src/nsEUCKRProber.cpp \
    src/nsEUCTWProber.cpp \
    src/nsEscCharsetProber.cpp \
    src/nsEscSM.cpp \
    src/nsGB2312Prober.cpp \
    src/nsHebrewProber.cpp \
    src/nsLatin1Prober.cpp \
    src/nsMBCSGroupProber.cpp \
    src/nsMBCSSM.cpp \
    src/nsSBCSGroupProber.cpp \
    src/nsSBCharSetProber.cpp \
    src/nsSJISProber.cpp \
    src/nsUTF8Prober.cpp \
    src/nsUniversalDetector.cpp \
    $(NULL)

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH)/include \
    $(LOCAL_PATH)/src/ \
    $(NULL)

LOCAL_CFLAGS += $(LOCAL_C_INCLUDES:%=-I%)
LOCAL_CXXFLAGS += ${LOCAL_CFLAGS}

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_C_INCLUDES)

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

