#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>


typedef void (*function)(void);

#define NUM_ATEXIT	32

static function __atexitlist[NUM_ATEXIT];
static int atexit_counter;

int atexit(function t) {
  if (atexit_counter<NUM_ATEXIT) {
    __atexitlist[atexit_counter]=t;
    ++atexit_counter;
    return 0;
  }
  return -1;
}

extern void __thread_doexit(int doexit);

void __libc_exit(int code);
void __libc_exit(int code) {
  register int i=atexit_counter;
  //__thread_doexit(code);
  while(i) {
    __atexitlist[--i]();
  }

  //setvbuf(stdout, 0, _IONBF, 0);
  //setvbuf(stderr, 0, _IONBF, 0);
  fclose(stdout);
  fclose(stderr);
  //_exit(code);
}

void exit(int code) __attribute__((alias("__libc_exit")));
