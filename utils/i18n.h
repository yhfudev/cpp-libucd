/**
 * @file    i18n.h
 * @brief   convertion and charset detection functions
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @license GPL 2.0/LGPL 2.1
 * @date    2009-04-13
 */
#ifndef __MY_I18N_H
#define __MY_I18N_H

/*#include <locale.h> // setlocale() */
#include <stdlib.h>

#ifndef TRACE
#define TRACE(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
//#define TRACE(...)
#endif
#ifndef DBGMSG
#define DBGMSG(catlog, level, fmt, ...) fprintf (stderr, "[%s()]\t" fmt "\t{%d," __FILE__ "}\n", __func__, ##__VA_ARGS__, __LINE__)
//#define DBGMSG(...)
#endif

#ifdef __cplusplus
extern "C" {
#endif /*__cplusplus*/

#if USE_ICONV
#include <iconv.h>
//#define iconv_ucd_t     iconv_t
//#define iconv_ucd_open  iconv_open
//#define iconv_ucd_close iconv_close
//#define iconv_ucd       iconv
#endif

#if 1
#include <libucd.h>

#define chardet_ucd_t       ucd_t
#define chardet_ucd_init    ucd_init
#define chardet_ucd_clear   ucd_clear
#define chardet_ucd_parse   ucd_parse
#define chardet_ucd_end     ucd_end
#define chardet_ucd_reset   ucd_reset
#define chardet_ucd_results ucd_results

#define chardet_t       chardet_ucd_t
#define chardet_init    chardet_ucd_init
#define chardet_clear   chardet_ucd_clear
#define chardet_parse   chardet_ucd_parse
#define chardet_end     chardet_ucd_end
#define chardet_reset   chardet_ucd_reset
#define chardet_results chardet_ucd_results
#define CHARDET_MAX_ENCODING_NAME UCD_MAX_ENCODING_NAME
#endif


#if USE_ICU
//#define UCD_MAX_ENCODING_NAME 30
typedef void * chardet_icu_t;
extern int  chardet_icu_init (chardet_icu_t* pdet);
extern void chardet_icu_clear (chardet_icu_t* det);
extern int  chardet_icu_parse (chardet_icu_t* det, const char* data, size_t len);
extern int  chardet_icu_end (chardet_icu_t* det);
extern int chardet_icu_reset (chardet_icu_t* det);
//#define chardet_icu_reset(a) (0)
extern int  chardet_icu_results (chardet_icu_t* det, char* namebuf, size_t buflen);

#ifndef chardet_init
#define chardet_t       chardet_icu_t
#define chardet_init    chardet_icu_init
#define chardet_clear   chardet_icu_clear
#define chardet_parse   chardet_icu_parse
#define chardet_end     chardet_icu_end
#define chardet_reset   chardet_icu_reset
#define chardet_results chardet_icu_results
#define CHARDET_MAX_ENCODING_NAME 50
#endif

typedef void *iconv_icu_t;
extern iconv_icu_t iconv_icu_open (const char *__tocode, const char *__fromcode);
extern int iconv_icu_close (iconv_icu_t __cd);
extern size_t iconv_icu (iconv_icu_t __cd, char ** __inbuf,
                     size_t * __inbytesleft,
                     char ** __outbuf,
                     size_t * __outbytesleft);
#define iconv_open  iconv_icu_open
#define iconv_close iconv_icu_close
#define iconv       iconv_icu
#define iconv_t     iconv_icu_t
#endif

int chardet (const char *buffer, size_t size, char *result, size_t sz_result);

#ifdef __cplusplus
}
#endif /*__cplusplus*/
#endif /* __MY_I18N_H */
