#include "dietdirent.h"
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>


struct dirent* readdir(DIR *d) {

  if( !FindNextFile(&d->dir, 0) )
  {
    return 0;
  }

  struct dirent *de = &d->de;

  de->d_ino = -1;
  de->d_off = -1;
  strcpy(de->d_name, (char*)d->dir.wsfile_name);
  return de;
}
