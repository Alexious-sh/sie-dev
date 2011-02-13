#include "loader.h"
#define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES 1
#define _CRT_SECURE_NO_DEPRECATE

static const unsigned char elf_magic_header[] =
  {0x7f, 0x45, 0x4c, 0x46,  /* 0x7f, 'E', 'L', 'F' */
   0x01,                    /* Only 32-bit objects. */
   0x01,                    /* Only LSB data. */
   0x01,                    /* Only ELF version 1. */
  };


/** Ельф ли это? И для нашей ли платформы? */
char isElfValid(Elf32_Ehdr *ehdr)
{
	if(memcmp(ehdr, elf_magic_header, sizeof(elf_magic_header)))
	{
		// о_О
		return 0;
	}
	return 1;
}



int elf_to_binary(char *data, int size)
{
	char *s = data;
	void *adr;
	Elf32_Ehdr ehdr;
	Elf32_Exec ex;

	ex.ehdr = &ehdr;
	ex.bin = data;
	ex.elfSize = size;
	ex.virtAdr = (unsigned long)-1;
	ex.libs_count = 0;

	/** хедер ельфа */
	memcpy(&ehdr, s, sizeof(Elf32_Ehdr));
	s += sizeof(Elf32_Ehdr);

	/** проверка */
	if( !isElfValid(&ehdr) )
	{
		printf("Bad elf\n");
		goto bad_rez;
	}

	dump(&ehdr);

	/** сегментация*/
	adr = readSegments(&ex);


	//((int (*)())adr)();


	return 0;
	bad_rez:
	return 1;
}



void * readSegments(Elf32_Exec *ex)
{
	Elf32_Phdr *EPH = ( Elf32_Phdr *)(ex->bin + ex->ehdr->e_phoff);
    Elf32_Shdr *ESH = ( Elf32_Shdr *)(ex->bin + ex->ehdr->e_shoff);

	printf("Elf32_Ehdr: %x\n", sizeof(Elf32_Ehdr));
	printf("Program header table is found at %X\n", EPH);
	printf("Sections header table is found at %X\n", ESH);

	printf("Number of sections: %i/%i\n",  ex->ehdr->e_phnum,  ex->ehdr->e_shnum);

	printf(" [+] Program header table struct dump\n");
	dump(EPH);
	printf(" [+] Sections header table struct dump\n");
// !!! Тут другая структура же
	//dump(ESH);

	ex->binarySize = calculateBinarySize(ex);
	printf("Raw binary output size: %d\n", ex->binarySize);

	if( !(ex->physAdr = (char*)malloc(ex->binarySize)) ){
		printf("Out of memory\n");
		return 0;
	}

	printf("physic addres: %08X\n", ex->physAdr);

	// Перебор программных сегментов
	int scnt = ex->ehdr->e_phnum;
	while (scnt--) {
 
		switch (EPH->p_type)
		{
            case PT_LOAD:
	 			if(EPH->p_filesz == 0) break; // Skip empty segments
				memcpy ((char*)VIRT2PHYS(ex->physAdr, ex->virtAdr, EPH->p_vaddr), (ex->bin + EPH->p_offset), EPH->p_filesz);
				printf("Loading CODE segment (%X, size %i)\n", ex->bin + EPH->p_offset, EPH->p_filesz);
				break;
            case PT_DYNAMIC:
				if(EPH->p_filesz == 0) break; // Skip empty segments
                printf("Loading Dynamic segment (%X, size %i):\n", ex->bin + EPH->p_offset, EPH->p_filesz);
			    relocDynamicSection(ex, EPH);
                break;
            default:
                if(EPH->p_filesz != 0) printf("Unknown segment (%i)\n", EPH->p_type);
        }
		EPH = ( Elf32_Phdr *)((unsigned char *)EPH + ex->ehdr->e_phentsize);
	}

	// Перебор каких-то там секций =)
	scnt = ex->ehdr->e_shnum;
	printf("\n----- Load sections -----\n");
	while (scnt--) {
 
		switch (ESH->sh_type)
		{
            default:
                printf("-->Section type %X\n", ESH->sh_type);
				printf(" sh_name: %X\n", ESH->sh_name);
				printf(" sh_flags: %X\n", ESH->sh_flags);
				printf(" sh_addr: %X\n", ESH->sh_addr);
				printf(" sh_offset: %X\n", ESH->sh_offset);
				printf(" sh_size: %X\n", ESH->sh_size);
				printf(" sh_link: %X\n", ESH->sh_link);
				printf(" sh_info: %X\n", ESH->sh_info);
				printf(" sh_addralign: %X\n", ESH->sh_addralign);
				printf(" sh_entsize: %X\n", ESH->sh_entsize);

        }
		ESH = ( Elf32_Shdr *)((unsigned char *)ESH + ex->ehdr->e_shentsize);
	}
	printf("----- Load sections end -----\n\n");

	printf(" [+] EntryPoint: %X %X\n", VIRT2PHYS(ex->physAdr, ex->virtAdr, ex->ehdr->e_entry), ex->ehdr->e_entry-ex->virtAdr);
	return (void*)VIRT2PHYS(ex->physAdr, ex->virtAdr, ex->ehdr->e_entry);
}

int relocDynamicSection(Elf32_Exec *ex, Elf32_Phdr *EPH)
{
 	Elf32_Dyn *dyn_sect = (Elf32_Dyn*)(ex->bin + EPH->p_offset);
	Elf32_Word dyn[DT_BINDNOW+1];
	memset(dyn, 0, sizeof(dyn));
 	int m = 0;
 	
 	printf(" phdr.p_type:%i\n", EPH->p_type);
    printf(" phdr.p_offset:%X\n", EPH->p_offset);
    printf(" phdr.p_vaddr:%X\n", EPH->p_vaddr);
    printf(" phdr.p_paddr:%X\n", EPH->p_paddr);
    printf(" phdr.p_filesz:%X\n", EPH->p_filesz);
    printf(" phdr.p_memsz:%X\n", EPH->p_memsz);
 	
 	printf("\n----- Parse Dynamic Section -----\n");
	
    while (dyn_sect[m].d_tag!=DT_NULL)
	{
		if (dyn_sect[m].d_tag<=DT_BINDNOW)
		{
			if(dyn_sect[m].d_tag == DT_NEEDED)
			{
				// Сохраняем смещения в .symtab на имена либ
				ex->needed[ex->libs_count++] = (char*)dyn_sect[m].d_un.d_val;
			}
			else
				dyn[dyn_sect[m].d_tag]=dyn_sect[m].d_un.d_val;
		}
		m++;
    }
	
	// Выводим список DT_NEEDED либ(пример получения)
	char *strtab = ex->bin + dyn[DT_STRTAB] - ex->virtAdr;
	int i = ex->libs_count;
	while(i--)
		printf(" --> I need the lib! It's name: %s\n", strtab + (int)ex->needed[i]);

    for (m = 0; m <= DT_BINDNOW; m++)
	{
		printf("%i = %X\n", m, dyn[m]);
    }
    // Do Relocation
    if (dyn[DT_RELSZ]!=0)
	{
 	    m=0;
 	    long* addr;
 	    Elf32_Word r_type;
 	    Elf32_Rel *relTable = (Elf32_Rel*)((char*)dyn_sect + dyn[DT_REL] - EPH->p_vaddr);
 	    printf("relTable offset=%X, realAddr=%X, sectionAddr=%X\n", dyn[DT_REL] - EPH->p_vaddr, (char*)dyn_sect + dyn[DT_REL] - EPH->p_vaddr, dyn_sect);
		
 	    while (m*sizeof(Elf32_Rel)<dyn[DT_RELSZ])
		{
		 	printf("rel: of=%X, sym_idx=%X, rel_type=%i\n",
			 relTable[m].r_offset,
			 ELF32_R_SYM(relTable[m].r_info),
			 (int)ELF32_R_TYPE(relTable[m].r_info));
  			
  		 	r_type = ELF32_R_TYPE(relTable[m].r_info);
  		 	switch(r_type)
  		 	{
 			  	case R_ARM_NONE: break;
			 	case R_ARM_RABS32:
					 printf("R_ARM_RABS32: ");
					 addr = (long*)(ex->physAdr + ((Elf32_Rel*)((char*)dyn_sect+dyn[DT_REL] - EPH->p_vaddr))[m].r_offset);
					 printf("from %X to %X\n", *addr, *addr + (long)(ex->physAdr - ex->virtAdr));
					 *addr+=(long)(ex->physAdr - ex->virtAdr);
					 break;
				case R_ARM_ABS32:
					 printf("R_ARM_ABS32: ");
					 addr = (long*)(ex->physAdr + ((Elf32_Rel *)((char*)dyn_sect+dyn[DT_REL] - EPH->p_vaddr))[m].r_offset - ex->virtAdr);
					 printf("from %X to %X\n", *addr, *addr + (long)ex->physAdr);
					 *addr+=(long)ex->physAdr;
					 break;
				case R_ARM_RELATIVE:
					 printf("R_ARM_RELATIVE: ");
					 addr = (long*)(ex->physAdr + ((Elf32_Rel *)((char*)dyn_sect+dyn[DT_REL] - EPH->p_vaddr))[m].r_offset-ex->virtAdr);
					 printf("from %X to %X\n", *addr, *addr + (long)(ex->physAdr - ex->virtAdr));
					 *addr+=(long)(ex->physAdr - ex->virtAdr);
					 break;
			 	default:
					 printf("Unknown REL type (%i)!\n", r_type);
			}
			m++;
 		}
	}
    printf("----- Parse Dynamic Section End -----\n\n");
 	return 0;
}


/** расчет размера бинарного файла */
int calculateBinarySize(Elf32_Exec *ex)
{
	unsigned int scnt = ex->ehdr->e_phnum, binsz = 0;
	unsigned long maxadr=0;
	unsigned int adr;
	Elf32_Phdr *ph = ( Elf32_Phdr *)(ex->bin + ex->ehdr->e_phoff);

	while (scnt--)
	{
		if (ph->p_type == PT_LOAD)
		{
			if (ex->virtAdr > ph->p_vaddr)
					ex->virtAdr = ph->p_vaddr;

			adr = (ph->p_vaddr+ph->p_memsz);
		    if (maxadr < adr)
				maxadr = adr;
		}
		ph = ( Elf32_Phdr *)((unsigned char *)ph + ex->ehdr->e_phentsize);
	}

	return maxadr-ex->virtAdr;
}
