

#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>


int access(const char *file, int mode)
{
    size_t m = 0;

    switch(mode)
    {
        case 0: case 4:
        m |= O_RDONLY;
        break;

        case 2:
        m |= O_WRONLY;
        break;

        case 6:
        m |= O_RDWR;
        break;
    }

    int fd = open(file, m);
    if(fd == -1)
    {
        // попробуем только риадонли
        if( m != O_RDONLY)
        {
            fd = open(file, O_RDONLY);
            if(fd != -1) // получилось открыть, значит нету доступа
            {
                __set_errno(EACCES);
                close(fd);
                return -1;
            }
            // не получилось значит нету файла
            __set_errno(ENOENT);
            return -1;
        }
        // в режиме реад онли не открыло
        return -1;
    }

    // открыло
    close(fd);
    return 0;
}


