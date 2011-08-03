
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include <unistd.h>
#include "loader.h"
#include <stdlib.h>


int main(char *exename, char *fname)
{

  #define NAME "test.elf"

   // l_setenv("LD_LIBRARY_PATH", "0:\\Misc\\elf3\\");
   char buf[256]={0};
   
   setenv("sie_test", strcat(getcwd(buf, 256), "/"), 1);
   printf("env: %s\n", getenv("sie_test"));
   
   printf("loading...\n");
  Elf32_Exec *ex = elfopen( NAME );
  if( ex == 0 ){
    printf("Error\n");
    return 1;
  }

  void (*addr)(const char * argc, char *argv) = (void(*)(const char* argc, char *argv))elf_entry(ex);
  
  run_INIT_Array(ex);
  run_FINI_Array(ex);
  
  if( !addr )
  {
    printf("entry not found\n");
  }else
  {
    printf("entry: 0x%X\n", addr);
      fflush(stdout);
     //addr( NAME, (char*)0);
  }
  
  unsetenv("sie_test");
    elfclose(ex);
    return 0;
}




