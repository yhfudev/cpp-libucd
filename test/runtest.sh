#!/bin/sh

PATH="../utils:$PATH"

EXEC_UCD=$(which ucd)

FN_FAIL="/dev/stderr"
FN_FAIL="runtest.fail"
FN_PASS="runtest.pass"

rm -f "${FN_FAIL}" "${FN_PASS}"
touch "${FN_FAIL}" "${FN_PASS}"

#detect if ucd supports icu
ARG_ICU=
${EXEC_UCD} -c $0
if [ "$?" = "0" ]; then
    ARG_ICU="-c"
fi

tail -f "${FN_FAIL}" &
tail -f "${FN_PASS}" &
for LANG in ?? ; do 
  for CODE in ${LANG}/* ; do 

    CODE=$(basename ${CODE})
    TESTFILE=${LANG}/${CODE}/wikitop_${LANG}_${CODE}.txt

    CODEKEY="`echo ${CODE} | tr a-z A-Z`"
    CODEKEY="`echo ${CODEKEY} | sed -e 's/CP1/WINDOWS-1/'`"

    #echo "DEBUG: ${EXEC_UCD} ${ARG_ICU} ${TESTFILE} 2TO/dev/null PIPE tail -1"
    CODEANS=$( ${EXEC_UCD} ${ARG_ICU} < ${TESTFILE} 2>/dev/null | tail -1 )
    #echo "DEBUG: codeans=${CODEANS}"
    CODEANS="`echo ${CODEANS} | tr a-z A-Z`"
    CODEANS="`echo ${CODEANS} | sed -e 's/WINDOWS-874/TIS-620/'`"
    CODEANS="`echo ${CODEANS} | sed -e 's/WINDOWS-28592/ISO-8859-2/'`"
    C1=${CODEANS}
    if [ ! "${ARG_ICU}" = "" ]; then
        C1=$( echo ${CODEANS} | awk -F, '{print $1}' )
        C2=$( echo ${CODEANS} | awk -F, '{print $2}' )
    fi

    if [ "${CODEKEY}" = "${C1}" ]; then
       echo "Pass: libucd ${LANG} ${CODEKEY} in file ${TESTFILE}." >> "${FN_PASS}"
    else
       echo "Fail: Wrong libucd('${C1}') when detecting ${LANG} ${CODEKEY} in file ${TESTFILE}." >> "${FN_FAIL}"
    fi
    if [ ! "${ARG_ICU}" = "" ]; then
        if [ "${CODEKEY}" = "${C2}" ]; then
        echo "Pass: libicu ${LANG} ${CODEKEY} in file ${TESTFILE}." >> "${FN_PASS}"
        else
        echo "Fail: Wrong libicu('${C2}') when detecting ${LANG} ${CODEKEY} in file ${TESTFILE}." >> "${FN_FAIL}"
        fi
    fi

  done
done

killall tail > /dev/null 2>&1
killall tail > /dev/null 2>&1
