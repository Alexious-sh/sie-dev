
#include <signal.h>
#include <errno.h>


sighandler_t signal(int signo, sighandler_t func) {
    errno = ENOSYS;
    return 0;
}
