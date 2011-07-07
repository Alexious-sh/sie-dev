
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */


#include "loader.h"

// Загрузка эльфа
__arch Elf32_Exec* elfopen(const char* filename)
{
  int fp;
  Elf32_Ehdr ehdr;
  Elf32_Exec* ex;

  if((fp = open(filename,A_ReadOnly+A_BIN,P_READ,&ferr)) == -1) return 0;

  if(read(fp, &ehdr, sizeof(Elf32_Ehdr), &ferr) == sizeof(Elf32_Ehdr))
  {
    if( !CheckElf(&ehdr) )
    {
      ex = malloc(sizeof(Elf32_Exec));

      if(ex)
      {
        memcpy(&ex->ehdr, &ehdr, sizeof(Elf32_Ehdr));
        ex->v_addr = (unsigned int)-1;
        ex->fp = fp;
        ex->body = 0;
        ex->type = EXEC_ELF;
        ex->libs = 0;
        ex->hashtab = 0;
        ex->complete = 0;
	ex->__is_ex_import = 0;

        if(!LoadSections(ex))
        {
          ex->complete = 1;
          close(fp, &ferr);
          return ex;
        }
        else
            elfclose(ex);
      }
    }
  }

  close(fp, &ferr);
  return 0;
}


__arch void *elf_entry(Elf32_Exec *ex)
{
    if(!ex) return 0;
    return ( ex->body + ex->ehdr.e_entry - ex->v_addr );
}


__arch int elfclose(Elf32_Exec* ex)
{
  if(!ex) return E_EMPTY;

  if(ex->complete)
    run_FINI_Array(ex);
  // Закрываем либы
  while(ex->libs)
  {
    Libs_Queue* lib = ex->libs;
    dlclose(lib->lib);
    ex->libs = lib->next;
    mfree(lib);
  }

  if(ex->hashtab) mfree(ex->hashtab);
  if(ex->body) mfree(ex->body);
  mfree(ex);
  return E_NO_ERROR;
}

