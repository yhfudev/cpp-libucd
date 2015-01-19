/*
 * This programs is fed text in a given encoding and language and
 * produces an ordered table of octet frequency. This is then used to manually
 * select the 64 characters which will participate in the
 * pair-frequency analysis (done by a python program).
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>

static char *thisprog;

static char usage [] =
"  \n\n"
;
static void
Usage(void)
{
    fprintf(stderr, "%s: usage:\n%s", thisprog, usage);
    exit(1);
}

static int     op_flags;
#define OPT_MOINS 0x1
#define OPT_k	  0x4 

int main(int argc, char **argv)
{
  int bskb = 8;
    
  thisprog = argv[0];
  argc--; argv++;

  while (argc > 0 && **argv == '-') {
    (*argv)++;
    if (!(**argv))
      /* Cas du "adb - core" */
      Usage();
    while (**argv)
      switch (*(*argv)++) {
      case 'k':	op_flags |= OPT_k; if (argc < 2)  Usage();
	if ((sscanf(*(++argv), "%d", &bskb)) != 1) Usage(); 
	argc--; goto b1;
      default: Usage();	break;
      }
  b1: argc--; argv++;
  }

  if (argc != 1) Usage();
  char *filename = *argv++;argc--;
  
  int fd = open(filename, 0);
  if (fd < 0) {
      perror("open");
      exit(1);
  }
  const int rbs = 8192;
  unsigned char buf[rbs];
  int tot = 0;
  int stats[256];
  memset(stats, 0, 256* sizeof(int));
  for (;;) {
    int nr = read(fd, buf, rbs);
    if (nr < 0) {
	perror("read");
	exit(0);
    }
    if (nr == 0)
      break;
    for (unsigned char *cp = (unsigned char *)buf; cp < buf+nr; cp++) {
	stats[*cp]++;
    }
    tot += nr;
  }
  for (unsigned int i = 0; i < 16; i++) {
      for (unsigned int j = 0; j < 16; j++) {
	  unsigned int ch = i*16+j;
	  printf("0x%x %c %d\n", ch, ch, stats[ch]);
      }
  }
  printf("Ok. Total: %d\n", tot);
  
  exit(0);
}

