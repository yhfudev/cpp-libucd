/**
 * @file    getline.c
 * @brief   getline() header
 * @author  Yunhui Fu (yhfudev@gmail.com)
 * @version 1.0
 * @license GPL 2.0/LGPL 2.1
 * @date    2014-09-07
 */

#ifndef MYGETLINE_H
#define MYGETLINE_H 1

#include "config.h"

#if ! HAVE_GETLINE
#include <stdio.h>
ssize_t getdelim (char **lineptr, size_t *n, int delimiter, FILE *fp);
#define getline(lineptr, n, stream) getdelim (lineptr, n, '\n', stream)
#endif

#endif // MYGETLINE_H
