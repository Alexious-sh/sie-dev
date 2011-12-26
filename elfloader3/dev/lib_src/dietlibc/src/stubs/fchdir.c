

#include <unistd.h>
#include <errno.h>


int fchdir(int fd) {
  errno = ENOSYS;
  return -1;
}
