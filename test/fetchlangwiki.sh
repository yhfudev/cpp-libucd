#!/bin/sh

# This is a "run once" script, kept for reference. The pages were manually 
# edited after being fetched, so don't run again
echo dont run this if you dont know what you are doing
#exit 1

# Fetch wikipedia index pages for a number of langages and transcode them
# to different charsets for testing language/charset recognition modules

topdir=localefiles

# List of country codes + charsets, country_cs1_cs2...
langcharsets="
cz_ISO-8859-2
de_WINDOWS-1252
dk_WINDOWS-1252
en_WINDOWS-1252
es_WINDOWS-1252
fi_WINDOWS-1252
fr_WINDOWS-1252
el_ISO-8859-7
hu_ISO-8859-2
it_WINDOWS-1252
nl_WINDOWS-1252
no_WINDOWS-1252
pl_ISO-8859-2
pt_WINDOWS-1252
ru_WINDOWS-1251_KOI8-R
se_WINDOWS-1252
tr_ISO-8859-9
zh_GB18030_BIG5
"

maybemkdir()
{
    if test ! -d $1; then
       mkdir -p $1 || exit 1
    fi
}

dirforlangcs()
{
    echo $topdir/$1/$2
}

for ccs in $langcharsets;do
    set `echo $ccs | sed -e 's/_/ /g'`
    country=$1;shift

    udir=`dirforlangcs ${country} UTF-8`
    maybemkdir $udir
    ufile=$udir/wikitop_${country}_UTF-8.txt

    lynx -crawl -dump -display_charset=utf-8 \
         http://${country}.wikipedia.org/ \
         > $ufile

    for code in $*;do
        dir=`dirforlangcs ${country} ${code}`
        maybemkdir $dir
        file=$dir/wikitop_${country}_${code}.txt
        # Using -c here because there are utf characters on the index page
        # (the other languages list part) which can't be represented in the
        # target charset.
        iconv -c -f UTF-8  -t ${code} $ufile > $file
    done
done
