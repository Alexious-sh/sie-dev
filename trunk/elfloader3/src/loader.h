

#ifndef __LOADER_H__
#define __LOADER_H__

/** includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"

#define MAX_DLSO_BASE 1024
#define MAX_LOAD_LIBS 54

typedef struct
{
	unsigned int elfSize;
	char *bin;
	Elf32_Ehdr ehdr;
	char *physAdr;
	unsigned long virtAdr;
	unsigned int binarySize;
	char* needed[MAX_LOAD_LIBS];
	int libs_count;
	Elf32_Word dyn[DT_FLAGS+1];
	int used_lib[MAX_LOAD_LIBS];
	int used_lib_cnt;
	char _file[256];
} Elf32_Exec;

extern Elf32_Exec __ex_so_table[MAX_DLSO_BASE];
extern Elf32_Exec __ex_table[MAX_DLSO_BASE];

#define VIRT2PHYS(phys, virt, addr) ( phys + addr - virt )

char isElfValid(Elf32_Ehdr *ehdr);
void *entryPoint(Elf32_Exec *ex);
int elf_to_binary(char *data, int size, Elf32_Exec *ex);
int readSegments(Elf32_Exec *ex);
int relocDynamicSection(Elf32_Exec *ex, Elf32_Phdr *EPH);
int calculateBinarySize(Elf32_Exec *ex);

/* shared library */
int lib_prepare(char *data, int data_size, Elf32_Exec *ex);
void *findFunctionAddres(Elf32_Exec *ex, const char *name);
int dlopen(const char *name);
void *dlsym(int dlopen_id, const char *name);
int dlclose(int dlopen_id);
void clear_all_libs();

/* executable */
int elfopen(const char *elf);
void *elf_entry(int op);
int elfclose(int op);


void dump1(Elf32_Ehdr *ehdr);
void dump2(Elf32_Phdr *p);
void dump3(Elf32_Sym *p);
void dump(char *data, int sz);

#endif
