/**
 * @file    ucdet.c
 * @brief   Universal charset detection use the library by Mozilla.
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @license GPL 2.0/LGPL 2.1
 * @date    2014-09-07
 */

#include <stdint.h>    /* uint8_t */
#include <stdlib.h>    /* size_t */
#include <unistd.h> // write()
#include <sys/types.h> /* ssize_t */

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <getopt.h>

#include "getline.h"
#include "i18n.h"

/**********************************************************************************/
#define VER_MAJOR 0
#define VER_MINOR 1
#define VER_MOD   1

static void
version (void)
{
    fprintf (stderr, "Universal charset detection use the library by Mozilla.\n");
    fprintf (stderr, "Version %d.%d.%d\n", VER_MAJOR, VER_MINOR, VER_MOD);
    fprintf (stderr, "Copyright (c) 2014 Y. Fu. All rights reserved.\n\n");
}

static void
help (char *progname)
{
    fprintf (stderr, "Usage: \n"
        "\t%s [files ...]\n"
        , basename(progname));
    fprintf (stderr, "\nOptions:\n");
    fprintf (stderr, "\tfiles...\tThe list of files group, if none, read from STDIN.\n");
    fprintf (stderr, "\t-c\tComparision mode. Output of the results seperated by semicolumn for libucd and libicu\n");
    fprintf (stderr, "\t-h\tPrint this message.\n");
    fprintf (stderr, "\t-v\tVerbose information.\n");
}

static void
usage (char *progname)
{
    version ();
    help (progname);
}

/**********************************************************************************/

typedef struct {
    char *modname;
    size_t bfsize;
    int (* init) (void * pdet);
    int (* clear) (void * pdet);
    int (* parse) (void * pdet, const char* data, size_t len);
    int (* end) (void * pdet);
    int (* results) (void * pdet, char* namebuf, size_t buflen);
    void *ptr;
} chardet_mod_t;

int
load_file (chardet_mod_t * mod, void * det, FILE *fp)
{
    char * buffer = NULL;
    size_t szbuf = 0;
    //off_t pos;

    szbuf = 10000;
    buffer = (char *) malloc (szbuf);
    if (NULL == buffer) {
        return -1;
    }
    //pos = ftell (fp);
    while ( getline ( &buffer, &szbuf, fp ) > 0 ) {
        mod->parse (det, (const char*)buffer, (unsigned int)strlen ((char *)buffer));
        //pos = ftell (fp);
    }

    free (buffer);
    return 0;
}

int
load_filename (chardet_mod_t * mod, void * det, char * filename)
{
    FILE *fp = NULL;
    fp = fopen (filename, "r");

    load_file (mod, det, fp);

    fclose (fp);
    return 0;
}

chardet_mod_t chardet_funcs[] = {
  {
    "ucd",
    sizeof(chardet_ucd_t),
    chardet_ucd_init,
    chardet_ucd_clear,
    chardet_ucd_parse,
    chardet_ucd_end,
    chardet_ucd_results,
    NULL,
  },
#if USE_ICU
  {
    "icu",
    sizeof(chardet_icu_t),
    chardet_icu_init,
    chardet_icu_clear,
    chardet_icu_parse,
    chardet_icu_end,
    chardet_icu_results,
    NULL,
  },
#endif
};

#define NUM_ARRAY(a) (sizeof(a)/sizeof((a)[0]))

int
main (int argc, char * argv[])
{
    char encname[UCD_MAX_ENCODING_NAME];
    char flg_useicu = 0;
    void * det[NUM_ARRAY(chardet_funcs)];
    size_t maxmod = 1;
    int i;
    int j;

    int c;
    struct option longopts[]  = {
        { "help",         0, 0, 'h' },
        { "verbose",      0, 0, 'v' },
        { 0,              0, 0,  0  },
    };

    while ((c = getopt_long( argc, argv, "cvh", longopts, NULL )) != EOF) {
        switch (c) {
        case 'c':
#if USE_ICU
            flg_useicu = 1;
#else
            fprintf (stderr, "Error: This option won't be supported without compiled with ICU!\n");
            exit (-1);
#endif
            break;
        case 'v':
            break;

        case 'h':
            usage (argv[0]);
            exit (0);
            break;
        default:
            fprintf (stderr, "%s: Unknown parameter: '%c'.\n", argv[0], c);
            fprintf (stderr, "Use '%s -h' for more information.\n", basename(argv[0]));
            exit (-1);
            break;
        }
    }
    memset (det, 0, sizeof(det));
    maxmod = 1;
    i = 0;
    assert (sizeof(void *) >= chardet_funcs[i].bfsize);
    chardet_funcs[i].init (&det[i]);
#if USE_ICU
    if(1 == flg_useicu) {
        for (i = 1; i < NUM_ARRAY(chardet_funcs); i ++) {
            assert (sizeof(void *) >= chardet_funcs[i].bfsize);
            chardet_funcs[i].init (&det[i]);
            maxmod ++;
        }
    }
#endif

    c = optind;
    if (argc > 1) {
        for (i = 0; i < maxmod; i ++) {
            for (j = c; j < argc; j ++) {
                load_filename (&chardet_funcs[i], &det[i], argv[j]);
            }
        }
    } else {
        for (i = 0; i < maxmod; i ++) {
            load_file (&chardet_funcs[i], &det[i], stdin);
        }
    }

    for (i = 0; i < maxmod; i ++) {
        chardet_funcs[i].end(&det[i]);
        if (chardet_funcs[i].results (&det[i], encname, UCD_MAX_ENCODING_NAME) < 0) {
            fprintf (stderr, "Error in detect charset encoding!\n");
        } else {
            fprintf (stderr, "'%s' detected charset encoding: '%s'!\n", chardet_funcs[i].modname, encname);
        }
        chardet_funcs[i].clear (&det[i]);
        if (i > 0) fprintf (stdout, ",");
        fprintf (stdout, "%s", encname);
    }
    fprintf (stdout, "\n");
    return 0;
}
