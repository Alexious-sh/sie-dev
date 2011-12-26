#include <sys/shm.h>
#include <swilib.h>
#include <dirent.h>


struct __dirstream {
    DIR_ENTRY dir;
    struct dirent de;
};				/* stream data from opendir() */
