

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>


gid_t getgid(void) {
  errno = ENOSYS;
  return -1;
}


pid_t getpid(void)
{
    errno = ENOSYS;
    return -1;
}
