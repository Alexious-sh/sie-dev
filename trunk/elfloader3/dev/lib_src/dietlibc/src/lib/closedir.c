#include "dietdirent.h"
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>


int closedir (DIR* d)
{
    int res = FindClose(&d->dir, 0);
    free(d);
    return res;
}
