
#include <dietstdio.h>
#include <unistd.h>


FILE *fsetopen( int fd, int flags,
               ssize_t (*_read)(int fd, void *buf, size_t sz),
               ssize_t (*_write)(int fd, const void *buf, size_t sz),
               ssize_t (*_seek)(int fd, long offset, int whence),
               int (*_close)(int fd) )
{
    FILE *fp = __stdio_init_file(fd, 1, flags);
    if(!fp) return 0;
    fsetread(fp, _read);
    fsetwrite(fp, _write);
    fsetseek(fp, _seek);
    fsetclose(fp, _close);
    return fp;
}


FILE *fsetread(FILE *stream, ssize_t (*_read)(int fd, void *buf, size_t sz))
{
    if(!stream) return 0;
    stream->_read = _read;
    return stream;
}


FILE *fsetwrite(FILE *stream, ssize_t (*_write)(int fd, const void *buf, size_t sz))
{
    if(!stream) return 0;
    stream->_write = _write;
    return stream;
}


FILE *fsetseek(FILE *stream, ssize_t (*_seek)(int fd, long offset, int whence))
{
    if(!stream) return 0;
    stream->_seek = _seek;
    return stream;
}


FILE *fsetclose(FILE *stream, int (*_close)(int fd))
{
    if(!stream) return 0;
    stream->_close = _close;
    return stream;
}

