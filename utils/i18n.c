/**
 * @file    i18n.c
 * @brief   convertion and charset detection functions
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @license GPL 2.0/LGPL 2.1
 * @date    2009-04-13
 */
/* 支持 ICU Libary (International Components for Unicode, developed by IBM) */

#if USE_ICU
#include <stdio.h>
#include <string.h>
#include <assert.h>

#include <unicode/ucnv.h>
#include <unicode/ucnv_err.h>
#include <unicode/ucsdet.h>
#endif

//#include "pfall.h"
//#include "sdict_type.h"
#include "i18n.h"

int
chardet (const char *buffer, size_t size, char *result, size_t sz_result)
{
    chardet_t det;
    if (chardet_init (&det) < 0) {
        return -1;
    }
    if (chardet_parse (&det, buffer, size) < 0) {
        chardet_clear (&det);
        return -1;
    }
    chardet_end (&det);
    if (chardet_results (&det, result, sz_result) < 0) {
        chardet_clear (&det);
        return -1;
    }
    chardet_clear (&det);
    return 0;
}

#if USE_ICU
typedef struct _icudet_info_t {
    UCharsetDetector *csd;
    size_t szbuf;
    size_t szdata;
} icudet_info_t;

int
chardet_icu_init (chardet_icu_t * pdet)
{
    UErrorCode status = U_ZERO_ERROR;
    icudet_info_t * pinfo;
    UCharsetDetector *csd;
    assert (NULL != pdet);
    if (NULL == pdet) {
        return -1;
    }
    csd = ucsdet_open (&status);
    if (U_FAILURE(status)) {
        return -1;
    }
    pinfo = malloc (sizeof(icudet_info_t) + 10000);
    if (NULL == pinfo) {
        ucsdet_close (csd);
        return -1;
    }
    memset (pinfo, 0, sizeof (*pinfo));
    pinfo->szbuf = 10000;
    pinfo->szdata = 0;
    pinfo->csd = csd;
    *pdet = pinfo;
    return 0;
}

void
chardet_icu_clear (chardet_icu_t * det)
{
    icudet_info_t * pinfo;
    assert (NULL != det);
    pinfo = (icudet_info_t *) (*det);
    ucsdet_close (pinfo->csd);
    free (pinfo);
    *det = NULL;
}

int
chardet_icu_parse (chardet_icu_t * det, const char* data, size_t len)
{
    icudet_info_t * pinfo;
    assert (NULL != det);
    pinfo = (icudet_info_t *) (*det);

    if (pinfo->szbuf < pinfo->szdata + len) {
        size_t sznew = pinfo->szbuf * 2 + len;
        pinfo = realloc (pinfo, sznew);
        if (NULL == pinfo) {
            return -1;
        }
        pinfo->szbuf = sznew;
        *det = pinfo;
    }
    memmove ((char *)(pinfo + 1) + pinfo->szdata, data, len);
    pinfo->szdata += len;

    return 0;
}

int
chardet_icu_end (chardet_icu_t * det)
{
    UErrorCode status = U_ZERO_ERROR;
    icudet_info_t * pinfo;
    assert (NULL != det);
    pinfo = (icudet_info_t *) (*det);

    ucsdet_setText (pinfo->csd, pinfo + 1, pinfo->szdata, &status);
    if (U_FAILURE(status)) {
        return -1;
    }
    return 0;
}

int
chardet_icu_reset (chardet_icu_t * det)
{
    icudet_info_t * pinfo;
    assert (NULL != det);
    pinfo = (icudet_info_t *) (*det);
    pinfo->szdata = 0;
    return 0;
}

int
chardet_icu_results (chardet_icu_t * det, char* namebuf, size_t buflen)
{
    UErrorCode status = U_ZERO_ERROR;
    const char *name;
    const UCharsetMatch *match;
    icudet_info_t * pinfo;
    assert (NULL != det);
    pinfo = (icudet_info_t *) (*det);

#if 0
    int32_t match_count = 0;
    const UCharsetMatch **matches = ucsdet_detectAll(pinfo->csd, &match_count, &status);
    if ((NULL == matches) || (match_count < 1)) {
        return -1;
    }
    match = matches[0];
#else
    match = ucsdet_detect (pinfo->csd, &status);
#endif

    if (match == NULL) {
        return -1;
    }
    name = ucsdet_getName (match, &status);
    if (NULL == name) {
        return -1;
    }
    if (strlen (name) > buflen - 1) {
        return -1;
    }
    strcpy (namebuf, name);
    return 0;
}

/************************************************************************/
typedef struct _iconv_item_t {
    UConverter * targetCnv;
    UConverter * sourceCnv;
} iconv_item_t;

iconv_icu_t
iconv_icu_open (const char *__tocode, const char *__fromcode)
{
    /* ./source/test/cintltst/ccapitst.c:2065: ucnv_convert */
    UErrorCode status = U_ZERO_ERROR;
    iconv_item_t *reticonv;

    assert (NULL != __tocode);
    assert (NULL != __fromcode);

    reticonv = (iconv_item_t *) malloc (sizeof (*reticonv));
    if (NULL == reticonv) {
        return NULL;
    }
    reticonv->targetCnv = ucnv_open(__tocode, &status);
    if (U_FAILURE(status)) {
        DBGMSG (PFDBG_CATLOG_USR_UTILS, PFDBG_LEVEL_ERROR, "Error in ucnv_open('%s') 1!\n", __tocode);
        DBGMSG (PFDBG_CATLOG_USR_UTILS, PFDBG_LEVEL_ERROR, "Please refer to the Charset Alias: http://www.unicode.org/reports/tr22/\n");
        free (reticonv);
        assert (0);
        return NULL;
    }
    reticonv->sourceCnv = ucnv_open(__fromcode, &status);
    if (U_FAILURE(status)) {
        DBGMSG (PFDBG_CATLOG_USR_UTILS, PFDBG_LEVEL_ERROR, "Error in ucnv_open('%s') 2!\n", __fromcode);
        DBGMSG (PFDBG_CATLOG_USR_UTILS, PFDBG_LEVEL_ERROR, "Please refer to the Charset Alias: http://www.unicode.org/reports/tr22/\n");
        ucnv_close (reticonv->targetCnv);
        free (reticonv);
        assert (0);
        return NULL;
    }
    return (iconv_icu_t)reticonv;
}

int
iconv_icu_close (iconv_icu_t __cd)
{
    iconv_item_t *reticonv = (iconv_item_t *)__cd;

    assert (NULL != reticonv);
    ucnv_close (reticonv->targetCnv);
    ucnv_close (reticonv->sourceCnv);
    free (reticonv);
    return 0;
}

size_t
iconv_icu (iconv_icu_t __cd, char ** __inbuf, size_t * __inbytesleft, char ** __outbuf, size_t * __outbytesleft)
{
    UErrorCode status = U_ZERO_ERROR;
    char *target;
    const char *source;
    iconv_item_t *reticonv = (iconv_item_t *)__cd;

    assert (NULL != __cd);
    assert (NULL != __inbuf);
    assert (NULL != *__inbuf);
    assert (NULL != __outbuf);
    assert (NULL != *__outbuf);
    assert (NULL != __inbytesleft);
    assert (NULL != __outbytesleft);

    target = *__outbuf;
    source = (const char *)(*__inbuf);
    reticonv = (iconv_item_t *)__cd;
    assert (NULL != reticonv->targetCnv);
    assert (NULL != reticonv->sourceCnv);

    ucnv_convertEx (reticonv->targetCnv, reticonv->sourceCnv, &target, target + *__outbytesleft,
        &source, source + *__inbytesleft, NULL, NULL, NULL, NULL, TRUE, TRUE, &status);
    if (U_FAILURE(status)) {
        DBGMSG (PFDBG_CATLOG_USR_UTILS, PFDBG_LEVEL_WARNING, "Some error in ucnv_convertEx()\n");
        /*return 0; */
    }
    *__inbytesleft -= (source - *__inbuf);
    *__inbuf = (char *)source;
    *__outbytesleft -= (target - *__outbuf);
    *__outbuf = target;

    return (target - *__outbuf);
}
#endif

/*
#define CHKRET_NAME(name,typ)    if (0 == strcasecmp (encname, name)) return typ;
int
chardet_cstr2val (char * encname)
{
    CHKRET_NAME ("utf-8",    ENCTYP_UTF8);
    CHKRET_NAME ("UTF-16LE", ENCTYP_UTF16LE);
    CHKRET_NAME ("UTF-16BE", ENCTYP_UTF16BE);
    CHKRET_NAME ("UTF-32LE", ENCTYP_UTF32LE);
    CHKRET_NAME ("UTF-32BE", ENCTYP_UTF32BE);
    CHKRET_NAME ("GB18030",  ENCTYP_GB18030);
    CHKRET_NAME ("GB2312",   ENCTYP_GB2312);
    CHKRET_NAME ("BIG5",     ENCTYP_BIG5);
    return "ISO8859-1"
}
*/
