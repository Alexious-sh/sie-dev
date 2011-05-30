#include "loader.h"

char __env[256] = {0};
Elf32_Exec __ex_table[MAX_DLSO_BASE] = {0};

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
	
	if( ehdr->e_type != ET_EXEC && ehdr->e_type != ET_DYN )
	  return 0;
	
	if( ehdr->e_machine != EM_ARM )
	  return 0;
	
	return 1;
}



int elf_to_binary(char *data, int size, Elf32_Exec *ex)
{
	memset(ex, 0, sizeof(Elf32_Exec));
	ex->bin = data;
	ex->elfSize = size;
	ex->virtAdr = (unsigned long)-1;
	ex->libs_count = 0;

	/** хедер ельфа */
	memcpy(&ex->ehdr, data, sizeof(Elf32_Ehdr));

	/** проверка */
	if( !isElfValid(&ex->ehdr) )
	{
		printf("Bad elf\n");
		return -1;
	}

	dump1(&ex->ehdr);

	/** сегментация*/
	if( readSegments(ex) )
	  return -1;

	return 0;
}



int readSegments(Elf32_Exec *ex)
{
	Elf32_Phdr *EPH = ( Elf32_Phdr *)(ex->bin + ex->ehdr.e_phoff);
	Elf32_Shdr *ESH = ( Elf32_Shdr *)(ex->bin + ex->ehdr.e_shoff);

	printf("Elf32_Ehdr: %x\n", sizeof(Elf32_Ehdr));
	printf("Program header table is found at %X\n", EPH);
	printf("Sections header table is found at %X\n", ESH);

	printf("Number of sections: %i/%i\n",  ex->ehdr.e_phnum,  ex->ehdr.e_shnum);

	printf(" [+] Program header table struct dump\n");
	dump2(EPH);
	printf(" [+] Sections header table struct dump\n");

	ex->binarySize = calculateBinarySize(ex);
	printf("Raw binary output size: %d\n", ex->binarySize);

	if( !(ex->physAdr = (char*)malloc(ex->binarySize)) ){
		printf("Out of memory\n");
		return -1;
	}

	printf("physic addres: %08X\n", ex->physAdr);
	int scnt;
	// Перебор каких-то там секций =)
	/*scnt= ex->ehdr.e_shnum;
	printf("\n----- Load sections -----\n");
	while (scnt--) {

		switch (ESH->sh_type)
		{
		  
		  case SHT_STRTAB:
		    if(!ex->strtab) ex->strtab = ESH->sh_offset;
		    printf("FOUND SHT_STRTAB 0x%X\n", ESH->sh_offset);
		    
		    break;
		  case SHT_SYMTAB:
		    if(!ex->symtab) ex->symtab = ESH->sh_offset;
		    printf("FOUND SHT_SYMTAB 0x%X\n", ESH->sh_offset);
		    
		    break;
		    
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
		ESH = ( Elf32_Shdr *)((unsigned char *)ESH + ex->ehdr.e_shentsize);
	}
	*/
	// Перебор программных сегментов
	scnt = ex->ehdr.e_phnum;
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
	      if( relocDynamicSection(ex, EPH) )
		return -2;
	      break;
            default:
              if(EPH->p_filesz != 0) printf("Unknown segment (%i)\n", EPH->p_type); 
	  }
	  EPH = ( Elf32_Phdr *)((unsigned char *)EPH + ex->ehdr.e_phentsize);
	  
	}

	printf("----- Load sections end -----\n\n");

	//printf(" [+] EntryPoint: %X %X\n", VIRT2PHYS(ex->physAdr, ex->virtAdr, ex->ehdr.e_entry), ex->ehdr.e_entry-ex->virtAdr);
	//return (void*)VIRT2PHYS(ex->physAdr, ex->virtAdr, ex->ehdr.e_entry);
	return 0;
}


void *entryPoint(Elf32_Exec *ex)
{
  return (void*)VIRT2PHYS(ex->physAdr, ex->virtAdr, ex->ehdr.e_entry);
}


int relocDynamicSection(Elf32_Exec *ex, Elf32_Phdr *EPH)
{
 	Elf32_Dyn *dyn_sect = (Elf32_Dyn*)(ex->bin + EPH->p_offset);
	

 	unsigned int i = 0;

 	printf(" phdr.p_type:%i\n", EPH->p_type);
    printf(" phdr.p_offset:%X\n", EPH->p_offset);
    printf(" phdr.p_vaddr:%X\n", EPH->p_vaddr);
    printf(" phdr.p_paddr:%X\n", EPH->p_paddr);
    printf(" phdr.p_filesz:%X\n", EPH->p_filesz);
    printf(" phdr.p_memsz:%X\n", EPH->p_memsz);

 	printf("\n----- Parse Dynamic Section -----\n");

    while (dyn_sect[i].d_tag != DT_NULL)
    {
		if (dyn_sect[i].d_tag <= DT_FLAGS)
		{
			if(dyn_sect[i].d_tag == DT_NEEDED)
			{
				// Сохраняем смещения в .symtab на имена либ
				ex->needed[ex->libs_count++] = (char*)dyn_sect[i].d_un.d_val;
			}
			else
				ex->dyn[dyn_sect[i].d_tag]=dyn_sect[i].d_un.d_val;
		}
		++i;
    }

	// Подгружаем DT_NEEDED либы
	char *strtab = ex->bin + ex->dyn[DT_STRTAB] - ex->virtAdr;
	Elf32_Sym *symtab = (Elf32_Sym*)(ex->bin + ex->dyn[DT_SYMTAB] - ex->virtAdr);
	Elf32_Rel *jmprel = (Elf32_Rel*)(ex->bin + ex->dyn[DT_JMPREL] - ex->virtAdr);

	i = ex->libs_count;
	while(i--)
	{
		// Запишем в ex->needed полноценные указатели, для удобства
		ex->needed[i] += (int)strtab;
		printf(" --> I need the lib! It's name: '%s'\n", ex->needed[i]);
		
		sprintf(__env, "%s\0", ex->needed[i]);

		printf("Loading '%s'...\n", ex->needed[i]);
		fflush(stdout);
		
		ex->used_lib[ ex->used_lib_cnt++ ] = dlopen(__env); // открываем либу и записываем результат в массив
		if( ex->used_lib[ ex->used_lib_cnt-1 ] == -1 ) 	    // не открылась(
		{
		    printf("cant load %s\n", __env);
		    return -1;					    // выходим нафиг
		}
	}

	i = 0;
	while(i < ex->dyn[DT_PLTRELSZ]/sizeof(Elf32_Rel))	// биндим функции
	{
		Elf32_Word sym_idx = ELF32_R_SYM( jmprel[i].r_info );
		char *iname = strtab + symtab[sym_idx].st_name;
		void *adr = 0;

		printf(" --> Fuction needed! It's name: '%s', link offset: 0x%X\n", iname, jmprel[i].r_offset - ex->virtAdr);
		
		printf(" Search for function...\n");
		for(int a=0; a<ex->used_lib_cnt; ++a) 	// пробегаем по либам в поисках нужной функции
		{
		    if( (adr = dlsym(ex->used_lib[a], iname)) )	// нашли
		    {
		        break;
		    }
		}
		
		if(!adr){					// никуя не нашли
		    printf(" function not found!\n");
		    return -2;					// выходим нафиг
		}
		
		printf(" function found at addres 0x%X\n", adr);

		*(int*)(ex->physAdr + jmprel[i].r_offset) = (int)adr;	// пишем аддрес
		++i;
	}

    /*for (i = 0; i <= DT_BINDNOW; ++i)
    {
		printf("%i = %X\n", i, ex->dyn[i]);
    }*/
    // Do Relocation
	if (ex->dyn[DT_RELSZ]!=0)
	{
 	    i=0;
 	    long* addr;
 	    Elf32_Word r_type;
 	    Elf32_Rel *relTable = (Elf32_Rel*)((char*)dyn_sect + ex->dyn[DT_REL] - EPH->p_vaddr);
		printf("relTable offset=%X, realAddr=%X, sectionAddr=%X\n", ex->bin + ex->dyn[DT_REL] + ex->virtAdr, (char*)dyn_sect + ex->dyn[DT_REL] + EPH->p_vaddr, dyn_sect);
		
 	    while (i*sizeof(Elf32_Rel)<ex->dyn[DT_RELSZ])
		{
		 	printf("rel: of=%X, sym_idx=%X, rel_type=%i\n",
				relTable[i].r_offset,
				ELF32_R_SYM(relTable[i].r_info),
				(int)ELF32_R_TYPE(relTable[i].r_info)
			);

  		 	r_type = ELF32_R_TYPE(relTable[i].r_info);
  		 	switch(r_type)
  		 	{
 			  	case R_ARM_NONE: break;
			 	case R_ARM_RABS32:
					 printf("R_ARM_RABS32: ");
					 addr = (long*)(ex->physAdr + relTable[i].r_offset);
					 printf("from %X to %X\n", *addr, *addr + (long)(ex->physAdr + ex->virtAdr));
					 *addr+=(long)(ex->physAdr - ex->virtAdr);
					 break;
				case R_ARM_ABS32:
					 printf("R_ARM_ABS32: ");
					 addr = (long*)(ex->physAdr + relTable[i].r_offset - ex->virtAdr);
					 printf("from %X to %X\n", *addr, *addr + (long)ex->physAdr);
					 *addr+=(long)ex->physAdr;
					 break;
				case R_ARM_RELATIVE:
					 printf("R_ARM_RELATIVE: ");
					 addr = (long*)(ex->physAdr + relTable[i].r_offset - ex->virtAdr);
					 printf("from %X to %X\n", *addr, *addr + (long)(ex->physAdr - ex->virtAdr));
					 *addr+=(long)(ex->physAdr - ex->virtAdr);
					 break;
			 	default:
					 printf("Unknown REL type (%i)!\n", r_type);
			}
			++i;
 		}
	}
    printf("----- Parse Dynamic Section End -----\n\n");
 	return 0;
}


/** расчет размера бинарного файла */
int calculateBinarySize(Elf32_Exec *ex)
{
	unsigned int scnt = ex->ehdr.e_phnum, binsz = 0;
	unsigned long maxadr=0;
	unsigned int adr;
	Elf32_Phdr *ph = ( Elf32_Phdr *)(ex->bin + ex->ehdr.e_phoff);

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
		ph = ( Elf32_Phdr *)((unsigned char *)ph + ex->ehdr.e_phentsize);
	}

	return maxadr-ex->virtAdr;
}
