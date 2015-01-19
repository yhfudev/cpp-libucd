#include <stdio.h>
#include <string.h>

#include <libucd.h>

int main (void) {
    int ret;
    ucd_t det;
    char buffer[100] = "test data";

    if ( ucd_init (&det) < 0 ) {
        fprintf (stderr, "libucd initialize failed\\n");
        return -1;
    }

    ret = ucd_parse (&det, buffer, strlen(buffer));
    switch (ret) {
    case UCD_RESULT_NOMEMORY :
        fprintf (stderr, "On handle processing, occured out of memory\\n");
        break;
    case UCD_RESULT_INVALID_DETECTOR :
        fprintf (stderr, "error in parsing\\n");
        break;
    default:
        break;
    }
    memset (buffer, 0, sizeof(buffer));
    if (ret >= 0) {
        ucd_end (&det);
        ucd_results (&det, buffer, sizeof(buffer));
    }
    ucd_clear (&det);

    printf ("encoding: %s\\n", buffer);

    return 0;
}
