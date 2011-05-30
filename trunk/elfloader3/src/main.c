
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "loader.h"


int main(char *exename, char *fname)
{
  /*const char *name = "test";
  int i = dlopen("libtest.so");
  if(i==-1){
    printf("error\n");
    return 1;
  }
  
  /*void (*addr)() = (void(*)())dlsym(i, name);
  
  if( !addr )
  {
    printf("function '%s' not found\n", name);
  }else
  {
    printf("function '%s' found at adress 0x%X\n", name, addr);
  }*/
  //addr();
  
  //dlclose(i);
  
  int i = elfopen("test.elf");
  if(i==-1){
    printf("error\n");
    return 1;
  }
  
  void (*addr)(int argc, char *argv[]) = (void(*)(int argc, char *argv[]))elf_entry(i);
  
  if( !addr )
  {
    printf("entry not found\n");
  }else
  {
    printf("entry found at adress 0x%X\n", addr);
  }
  //addr();
  
  elfclose(i);
  
  //clear_all_libs();
  return 0;
}




