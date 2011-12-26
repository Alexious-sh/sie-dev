

#include <stdio.h>
#include <swilib.h>
#include <time.h>
#include <fcntl.h>

ssize_t my_write(int fd, const void *buf, size_t sz)
{
    ShowMSG(1, (int)buf);
    return sz;
}


void std()
{
    //int fd = open("0:\\Misc\\elf3\\stdout.txt", O_CREAT | O_TRUNC | O_WRONLY);
    //__setup_stdout_fd(fd);
    stdout = fsetopen(-1, O_WRONLY, 0, my_write, 0, 0);
    //fsetwrite(stdout, my_write);
}


int main()
{
    std();

    struct tm *ptr;
    time_t lt;
    char str[80];

    lt = time(NULL);
    ptr = localtime(&lt);

    strftime(str, 100, "Сейчас %H %p.", ptr);
    printf("O_O: %s", str);

    fflush(stdout);
    //close(fileno(stdout));
    fclose(stdout);
    kill_elf();
    return 0;
}
