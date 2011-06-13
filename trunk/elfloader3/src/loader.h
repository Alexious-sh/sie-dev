
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */


#ifndef __LOADER_H__
#define __LOADER_H__

#include <inc/swilib.h>
#include "elf.h"
#include <stdio.h>

static const unsigned char elf_magic_header[] =
{
  0x7f, 0x45, 0x4c, 0x46,  /* 0x7f, 'E', 'L', 'F' */
  0x01,                    /* Only 32-bit objects. */
  0x01,                    /* Only LSB data. */
  0x01,                    /* Only ELF version 1. */
};

#define __arch

enum ERROR{

    E_NO_ERROR = 0x0,
    E_RELOCATION,
    E_READ,
    E_SHARED,
    E_HEADER,
    E_SECTION,
    E_RAM,
    E_EMPTY
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
} Elf32_Exec;

typedef struct
{
  char soname[64];
  Elf32_Exec* ex;
  int users_cnt;
  void *glob_queue;
} Elf32_Lib;

typedef int ELF_ENTRY(const char *, void *);
typedef int LIB_FUNC();

extern unsigned int ferr;

int CheckElf(Elf32_Ehdr *ehdr);
unsigned int GetBinSize(Elf32_Exec *ex, Elf32_Phdr* phdrs);
int LoadSections(Elf32_Exec* ex);
int DoRelocation(Elf32_Exec* ex, Elf32_Dyn* dyn_sect, Elf32_Phdr* phdr);
unsigned long elfhash(const char* name);
Elf32_Word findExport(Elf32_Exec* ex, const char* name);

/* shared support */
Elf32_Lib* dlopen(const char *name);
Elf32_Word dlsym(Elf32_Lib* lib, const char *name);
int dlclose(Elf32_Lib* lib);

/* executable support */
Elf32_Exec* elfopen(const char* filenam);
int elfclose(Elf32_Exec* ex);
void *elf_entry(Elf32_Exec *);

/* init/fini arrays support */
void run_INIT_Array(Elf32_Exec *ex);
void run_FINI_Array(Elf32_Exec *ex);

#endif
