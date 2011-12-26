

#include <sys/stat.h>
#include <stdio.h>
#include <swilib.h>
#include <errno.h>
#include <fcntl.h>


int    stat(const char *file, struct stat *_s)
{
    if(_s) zeromem(_s, sizeof(struct stat));
    if(!file || !_s) return -1;
    FSTATS fs;
    if( GetFileStats(file, &fs, (unsigned int*)__errno_location()) )
    {
        __set_errno(ENOENT);
        return -1;
    }

    zeromem(_s, sizeof(struct stat));

    _s->st_size = fs.size;

    return 0;
}



int    fstat(int _fd, struct stat *_s)
{
    if(_s) zeromem(_s, sizeof(struct stat));
    if(_fd < 0 || !_s) return -1;

    ssize_t ls = lseek(_fd, 0, SEEK_CUR);

    zeromem(_s, sizeof(struct stat));

    _s->st_size = lseek(_fd, 0, SEEK_END);

    lseek(_fd, ls, SEEK_SET);
    return 0;
}


int    lstat(const char *f, struct stat *s)
{
    return stat(f, s);
}


