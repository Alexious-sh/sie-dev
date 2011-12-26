
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */


#ifndef __LOADER_H__
#define __LOADER_H__

//#define _test_linux
//#define __ZVOVA
//#define __GANSTER


#include "elf.h"

#ifdef __GANSTER
#include <swilib.h>
#endif

#ifdef __ZVOVA
#include <inc/swilib.h>
#define fopen open
#define fread read
#define fwrite write
#define fclose close
#endif

#ifdef _test_linux
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define A_ReadOnly O_RDONLY
#define A_BIN 0
#define A_TXT O_TXT

#define mfree free

#define fopen(p, t, m, er) open(p, t)
#define fread(f, b, s, er) read(f, b, s)
#define fwrite(f, b, s, er) write(f, b, s)
#define fclose(f, er) close(f)
#define lseek(f, s, s1, er, er1) lseek(f, s, s1)

#define S_SET SEEK_SET
#define S_END SEEK_END
#define P_READ 0

#define zeromem_a(d, s) memset(d, 0, s)
#define zeromem(d, s) memset(d, 0, s)

#define l_msg(x, y) printf("MESSAGE: %s\n", (char*)y);
#define __e_div(a, b) (b % a)
#endif


static const unsigned char elf_magic_header[] =
{
  0x7f, 0x45, 0x4c, 0x46,  /* 0x7f, 'E', 'L', 'F' */
  0x01,                    /* Only 32-bit objects. */
  0x01,                    /* Only LSB data. */
  0x01,                    /* Only ELF version 1. */
};

#ifndef _test_linux
  #define printf(...) 
  #define __arch __arm
#else
  #define __arch 
#endif

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

#define NO_FILEORDIR	"no such file or directory"
#define BADFILE		"bad file type"
#define OUTOFMEM	"out of memory"

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

typedef int ELF_ENTRY(const char *, void *);
typedef int LIB_FUNC();

extern unsigned int ferr;

#ifndef _test_linux
 #ifdef ARM
 #define zeromem_a(a,b) zeromem(a,b)
 #define l_msg(a,b) ShowMSG(a,b)
 #else
 void zeromem_a(void *d, int l);
 void l_msg(int a, int b);
 #endif
#endif

int CheckElf(Elf32_Ehdr *ehdr);
unsigned int GetBinSize(Elf32_Exec *ex, Elf32_Phdr* phdrs);
int LoadSections(Elf32_Exec* ex);
int DoRelocation(Elf32_Exec* ex, Elf32_Dyn* dyn_sect, Elf32_Phdr* phdr);
unsigned long elfhash(const char* name);
Elf32_Word findExport(Elf32_Exec* ex, const char* name);
Elf32_Word FindFunction(Elf32_Lib* lib, const char* name);

/* shared support */
Elf32_Lib* OpenLib(const char *name, Elf32_Exec *ex);
int CloseLib(Elf32_Lib* lib, int immediate);
int dlopen(const char *name);
int dlclose(int handle);
Elf32_Word dlsym(int handle, const char *name);

/* executable support */
Elf32_Exec* elfopen(const char* filenam);
int elfclose(Elf32_Exec* ex);
void *elf_entry(Elf32_Exec *);

__arch void sub_clients(Elf32_Lib* lib);

/* init/fini arrays support */
void run_INIT_Array(Elf32_Exec *ex);
void run_FINI_Array(Elf32_Exec *ex);

#endif
