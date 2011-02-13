

#ifndef __LOADER_H__
#define __LOADER_H__

/** includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "elf.h"


typedef struct
{
	unsigned int elfSize;
	char *bin;
	Elf32_Ehdr *ehdr;
	char *physAdr;
	unsigned long virtAdr;
	unsigned int binarySize;
	char* needed[10];
	int libs_count;
} Elf32_Exec;

#define VIRT2PHYS(phys, virt, addr) ( phys + addr - virt )

char isElfValid(Elf32_Ehdr *ehdr);
int elf_to_binary(char *data, int size);
void *readSegments(Elf32_Exec *ex);
int relocDynamicSection(Elf32_Exec *ex, Elf32_Phdr *EPH);
int calculateBinarySize(Elf32_Exec *ex);


void dump(Elf32_Ehdr *ehdr);
void dump(Elf32_Phdr *p);
void dump(char *data, int sz);

#endif
