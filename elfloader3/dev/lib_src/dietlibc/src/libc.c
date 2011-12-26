#include "dietstdio.h"
#include <fcntl.h>


int __libc_open(const char*fn,int flags,...)
{
    return open(fn, flags);
}

int __libc_close(int fd)
{
    return close(fd);
}

/*
ssize_t __libc_read(int fd,void*buf,size_t len)
{
    return read(fd, buf, len);
}


ssize_t __libc_write(int fd,const void*buf,size_t len)
{
    return write(fd, buf, len);
}
*/
