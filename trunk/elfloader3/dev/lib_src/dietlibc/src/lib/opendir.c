#include "../dietdirent.h"
#include <sys/mman.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <fcntl.h>


DIR*  opendir ( const char* name ) {

    DIR *dir = malloc(sizeof(DIR));

    if( !FindFirstFile(&dir->dir, name, 0) )
    {
        free(dir);
        dir = 0;
    }

    return dir;
}
