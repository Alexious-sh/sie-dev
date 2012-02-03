
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include <unistd.h>
#include "loader.h"
#include <stdlib.h>


int main(int argc, char **argv)
{
  if(argc < 2) return 0;
  
  const char *NAME = argv[1];

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
      
#undef fopen
#undef fread
#undef fwrite
#undef fclose
      FILE *fp = fopen("elfdump.bin", "w+");
      if(fp)
      {
	fwrite(ex->body, 1, ex->bin_size, fp);
	fclose(fp);
      }
  }
  
    unsetenv("sie_test");
    elfclose(ex);
    return 0;
}




