
#include <stdio.h>
#include <errno.h>


int pclose(FILE *stream) {
  errno = ENOSYS;
  return -1;
}
