

#include <unistd.h>
#include <errno.h>


int fchmod(int filedes, mode_t mode) {
  errno = ENOSYS;
  return -1;
}

int chmod(const char * filedes, mode_t mode) {
  errno = ENOSYS;
  return -1;
}
