#include <dietstdio.h>
#include <unistd.h>

static char __stdout_buf[BUFSIZE];
static FILE __stdout = {
  .fd=-1,
  .flags = NOBUF | CANWRITE,
  .bs=0, .bm=0,
  .buflen = 0,
  .buf= 0,
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

FILE *stdout=&__stdout;

int __fflush_stdout(void) {
  return fflush(stdout);
}

int __setup_stdout_fd(int fd)
{
    stdout->fd = fd;
}
