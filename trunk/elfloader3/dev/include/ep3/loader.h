
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */


#ifndef __LOADER_H__
#define __LOADER_H__

#include <swilib.h>
#include "elf.h"

enum ERROR{

    E_NO_ERROR = 0x0,
    E_RELOCATION,
    E_READ,
    E_SHARED,
    E_HEADER,
    E_SECTION,
    E_RAM,
    E_EMPTY,
    E_FILE,
    E_MACHINE,
    E_ALIGN,
    E_UNDEF,
    E_SYMTAB,
    E_STRTAB,
    E_PHDR,
    E_HASTAB
};

typedef struct
{
  void *lib;
  void *next;
  void *prev;
} Global_Queue;

typedef struct
{
  void *lib;
  void *next;
} Libs_Queue;

typedef enum elf32_type
{
  EXEC_NONE,
  EXEC_ELF,
  EXEC_LIB,
} Elf32_Type;

typedef struct
{
  char* body;
  unsigned int bin_size;
  Elf32_Ehdr ehdr;
  unsigned int v_addr;
  Elf32_Type type;
  Elf32_Word dyn[DT_FLAGS+1];
  Elf32_Sym* symtab;
  Elf32_Rel* jmprel;
  Elf32_Word* hashtab;
  char* strtab;
  Libs_Queue* libs;
  int fp;
  char complete, __is_ex_import;
  void *meloaded;
  int *switab;
} Elf32_Exec;

typedef struct
{
  char soname[64];
  Elf32_Exec* ex;
  int users_cnt;
  void *glob_queue;
} Elf32_Lib;



#endif
