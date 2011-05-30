#include "loader.h"



int elfopen(const char *elf)
{
    char *buffer;
    int size;
    int i=0;
    
    for(i=0; i<MAX_DLSO_BASE; ++i)
    {
      if( !__ex_table[i].elfSize )
	break;
    }
    
    if(i == MAX_DLSO_BASE-1) return -2;
    
    
    FILE *fp = fopen(elf, "rb");
    if(!fp){
        printf("Can`t open file '%s'\n", elf);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(size+1);

    fread(buffer, size, 1, fp);
    fclose(fp);
	
    buffer[size] = 0;

    if( elf_to_binary(buffer, size, &__ex_table[i]) )
      return -1;
    
    strcpy( __ex_table[i]._file, elf);
    
    return i;
}


void *elf_entry(int op)
{
  if( !__ex_table[op].elfSize ) return 0;
  return entryPoint(&__ex_table[op]);
}


int elfclose(int op)
{
  if( !__ex_table[op].elfSize ) return -1;
  
  free(__ex_table[op].physAdr);
  free(__ex_table[op].bin);
  memset(&__ex_table[op], 0, sizeof(Elf32_Exec));
  return 0;
}



