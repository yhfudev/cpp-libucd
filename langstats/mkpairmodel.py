#!/usr/bin/env python

# Generate the C++ code for the pair frequency analysis. We produce 2
# tables:
#
# First table: CharToOrder is a 256 entries table to translate between
# a 8byte value and the order by frequency of the characters. It is
# created from the charstats input file which basically contains a
# list of all characters sorted by frequency (charstats is created by
# another program).
#
# The first 64 (1-64) values are further used to compute pair
# frequencies and as an index in the character pair frequency table.
#
# Values from 64 to 250 are considered as "other character"
# positions. They are used during identication to weight down the
# result of the pair-frequency analysis (ie: if there his a very high
# proportion of high frequency pairs, but very few in absolute numbers
# and a lot of other chars, the result is not significant).
#
# Values above 250 are for characters that are control or punctuation
# in all the considered encodings and should not influence the result
# at all.
#
# Second table: LangModel has the frequencies for the 4096 resulting
# pairs of characters.  The frequencies are not exact but
# characterized as 0,1,2,3, from rare to frequent

import sys
import os

maxrank = 64

def Usage():
    print "Usage: mkchartoorder.py <charstats file> <text reference file>"
    sys.exit(1)
    
if len(sys.argv) != 3:
    Usage()

charstats = sys.argv[1]
reftext = sys.argv[2]

# print "Charstats file:", charstats, "Ref text:", reftext

# The reference text name is like french_cp1252.txt
langcode = os.path.splitext(os.path.basename(reftext))[0]
lang,encoding = langcode.split('_')
# print "lang:", lang, "encoding:", encoding

# Encoding name for use in C variable names
cencoding = encoding.replace("-", "_")

# Read the charstats file and populate the order table

chartoorder = 256 * [255]
f = open(charstats, "r")
order = 1
for line in f:
    l = line.split()
    bytevalue = int(l[0], 16)
    # Eliminate the common control/punctuation areas. Note that this is only
    # the ascii control / punctuation because the winxxx encodings have
    # lexical characters in the 80-a0 area
    if bytevalue <= 0x40 or \
       (bytevalue >= 0x5b and bytevalue <= 0x60) or \
       (bytevalue >= 0x7b and bytevalue <= 0x7f):
        continue

    #print "bytevalue ", bytevalue, " -> order", order
    chartoorder[bytevalue] = order
    order += 1

f.close()

def ordertoichar(order):
    for i in range(256):
        if chartoorder[i] == order:
            return i
    return 0

############ Uninteresting C++ file header
prolog = """/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Communicator client code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

#include "nsSBCharSetProber.h"
"""
print prolog
############ End Uninteresting C++ file header

# Output the CharToOrder table
ordermapname = "%s_%sCharToOrderMap" % (lang, cencoding)
print "static const unsigned char %s[] = " % ordermapname
print "{"
for i in range(16):
    for j in range(16):
        sys.stdout.write("%3d," % chartoorder[16*i+j])
    print
print "};\n"


# Compute the char pair frequency stats

f = open(reftext, "r")
text = f.read()

pairfreqs = (maxrank*maxrank) * [0]

totalpairs = 0
prevorder = 255
for i in range(len(text)):
    ichar = ord(text[i])
    order = chartoorder[ichar]
    #print "char ", text[i], "order", order
    if order > maxrank:
        prevorder = 255
        continue
    if prevorder <= maxrank:
        pairfreqs[(prevorder-1) * maxrank + (order-1)] += 1
        totalpairs += 1
    prevorder = order

# We now have a 4096 entries array indexed by the 64x64 possible
# pairs of frequent characters, listing their count of occurences.
# We want to transform this array so that the values are just 0, 1, 2, 3
# 3 is for the 512 most frequent sequences
# 2 for the 512 next
# 1 for all having more than 3 occurences
# 0 for negative sequences 0 to 2 occurences
# So sort by order of occurences, and compute the 512th,and 1024th ranks,
# then use comparisons to these values to classify the characters.
byocc = sorted(pairfreqs, reverse=1)
t512 = byocc[512]
t1024 = byocc[1024]
oc512 = 0
for i in range(512):
    oc512 += byocc[i]
prop512 = float(oc512) / float(totalpairs)

# print "/* Threshold 512 is ", t512, "threshold 1024 is ", t1024, "*/"
modelname = "%sLangModel" % lang
ctext = "static const PRUint8 %s[] = " % modelname + "\n{\n"
linecounter = 0
for i in range(maxrank*maxrank):
    order1 = i / maxrank + 1
    order2 = i % maxrank + 1
    ichar1 = ordertoichar(order1)
    if ichar1 == 0:
        print "ordertoichar returned 0 for order", order1
    ichar2 = ordertoichar(order2)
    if ichar2 == 0:
        print "ordertoichar returned 0 for order", order2
#   print "order1", order1, "order2", order2, "ichar1", ichar1, "ichar2", ichar2

    cntocs = pairfreqs[(order1-1) * maxrank + order2-1]
    if cntocs > 0 and cntocs >= t512:
        fclass = 3
        print "Seq 3", chr(ichar1), chr(ichar2)
    elif cntocs > 0 and cntocs >= t1024:
        fclass = 2
    elif cntocs >= 2:
        fclass = 1
    else:
        fclass = 0
        
    #print chr(ichar1), chr(ichar2), fclass, cntocs
    ctext += "%d%s" % (fclass, ",")
    linecounter += 1
    if linecounter == 32:
        linecounter = 0
        ctext += "\n"

ctext += "};\n"
print ctext

print "const SequenceModel %s%sModel = " % (cencoding, lang)
print "{"
print "  %s," % ordermapname
print "  %s," % modelname
print "  (float)%f," % prop512
print "  PR_TRUE,"
print "  \"%s\"," % encoding
print "  \"%s\"" % lang
print "};"

