#!/bin/sh

PATH="$PATH:../utils"
EXEC_UCD=$(which ucd)

FN_FAIL="/dev/stderr"
FN_FAIL="runtest.fail"
FN_PASS="runtest.pass"

rm -f "${FN_FAIL}" "${FN_PASS}"
touch "${FN_FAIL}" "${FN_PASS}"

tail -f "${FN_FAIL}" &
tail -f "${FN_PASS}" &
for LANG in ?? ; do 
  for CODE in ${LANG}/* ; do 
    CODE=$(basename ${CODE})
    TESTFILE=${LANG}/${CODE}/wikitop_${LANG}_${CODE}.txt

    CODEKEY="`echo ${CODE} | tr a-z A-Z`"
    CODEKEY="`echo ${CODEKEY} | sed -e 's/CP1/WINDOWS-1/'`"

    CODEANS=$( ${EXEC_UCD} < ${TESTFILE} 2>/dev/null | tail -1 )
    CODEANS="`echo ${CODEANS} | tr a-z A-Z`"
    CODEANS="`echo ${CODEANS} | sed -e 's/WINDOWS-874/TIS-620/'`"
    CODEANS="`echo ${CODEANS} | sed -e 's/WINDOWS-28592/ISO-8859-2/'`"

    if [ "${CODEKEY}" = "${CODEANS}" ]; then
       echo "Pass: ${LANG} ${CODEKEY} in file ${TESTFILE}." >> "${FN_PASS}"
    else
       echo "Fail: Wrong '${CODEANS}' when detecting ${LANG} ${CODEKEY} in file ${TESTFILE}." >> "${FN_FAIL}"
    fi

  done
done

killall tail > /dev/null 2>&1
killall tail > /dev/null 2>&1
