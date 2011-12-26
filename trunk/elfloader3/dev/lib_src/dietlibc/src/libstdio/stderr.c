#include <dietstdio.h>
#include <unistd.h>

static FILE __stderr = {
  .fd=-1,
  .flags= NOBUF | CANWRITE,
  .bs=0, .bm=0,
  .buflen=0,
  .buf=0,
  .next=0,
  .popen_kludge=0,
  .ungetbuf=0,
  .ungotten=0,
  ._read = read,
  ._write = write,
  ._seek = lseek,
  ._close = close,
#ifdef WANT_THREAD_SAFE
  .m=PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP,
#endif
};

FILE *stderr=&__stderr;

int __fflush_stderr(void) {
  return fflush(stderr);
}


int __setup_stderr_fd(int fd)
{
    stderr->fd = fd;
    return fd;
}
