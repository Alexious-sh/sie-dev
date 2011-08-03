
/*
 * ���� ���� �������� ������ ��������� ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include "loader.h"



__arch void dump1(Elf32_Ehdr *ehdr)
{
	printf("Elf32_Ehdr dump\n");
	printf("e_ehsize: %d\n", ehdr->e_ehsize);
	printf("e_entry: %d\n", ehdr->e_entry);
	printf("e_flags: %d\n", ehdr->e_flags);
	printf("e_ident: %s\n", ehdr->e_ident);
	printf("e_machine: %d\n", ehdr->e_machine);
	printf("e_phentsize: %d\n", ehdr->e_phentsize);
	printf("e_phnum: %d\n", ehdr->e_phnum);
	printf("e_phoff: %d\n", ehdr->e_phoff);
	printf("e_shentsize: %d\n", ehdr->e_shentsize);
	printf("e_shnum: %d\n", ehdr->e_shnum);
	printf("e_shoff: %d\n", ehdr->e_shoff);
	printf("e_shstrndx: %d\n", ehdr->e_shstrndx);
	printf("e_type: %d\n", ehdr->e_type);
	printf("e_version: %d\n", ehdr->e_version);
}


__arch void dump2(Elf32_Phdr *p)
{
	printf("Elf32_Phdr dump\n");
	printf("p_align: %d\n", p->p_align);
	printf("p_filesz: %d\n", p->p_filesz);
	printf("p_flags: %d\n", p->p_flags);
	printf("p_memsz: %d\n", p->p_memsz);
	printf("p_offset: %d\n", p->p_offset);
	printf("p_paddr: %d\n", p->p_paddr);
	printf("p_type: %d\n", p->p_type);
	printf("p_vaddr: %d\n", p->p_vaddr);
}


__arch void dump3(Elf32_Sym *p)
{
	printf("Elf32_Sym dump\n");
	printf("st_name: %d\n", p->st_name);
	printf("st_value: %d\n", p->st_value);
	printf("st_size: %d\n", p->st_size);
	printf("st_info: %d\n", p->st_info);
	printf("st_other: %d\n", p->st_other);
	printf("st_shndx: %d\n", p->st_shndx);
}


__arch void dump(char *data, int sz)
{
	for(int i=0,a=0; i<sz; i++,a++)
	{
		if(a>5){
			a=0;
			printf("\n");
		}
		printf("%02x ", data[i]);
	}
}
