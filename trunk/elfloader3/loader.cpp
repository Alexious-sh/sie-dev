
#include "loader.h"


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
	Elf32_Ehdr ehdr;
	Elf32_Exec ex;

	ex.ehdr = &ehdr;
	ex.bin = data;
	ex.elfSize = size;

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
	readSegments(&ex);




	return 0;
	bad_rez:
	return 1;
}



void * readSegments(Elf32_Exec *ex)
{
	Elf32_Phdr *EPH = ( Elf32_Phdr *)(ex->bin + ex->ehdr->e_phoff);
    Elf32_Phdr *ESH = ( Elf32_Phdr *)(ex->bin + ex->ehdr->e_shoff);

	printf("Program header table is found at %X\n", EPH);
	printf("Sections header table is found at %X\n", ESH);

	printf("Number of sections: %i/%i\n",  ex->ehdr->e_phnum,  ex->ehdr->e_shnum);

	printf(" [+] Program header table struct dump\n");
	dump(EPH);
	printf(" [+] Sections header table struct dump\n");
	dump(ESH);

	ex->binarySize = calculateBinarySize(ex);
	printf("Raw binary output size: %d\n", ex->binarySize);

	if( !(ex->physAdr = (char*)malloc(ex->binarySize)) ){
		printf("Out of memory\n");
		return 0;
	}

	printf("physic addres: %08X\n", ex->physAdr);


	// секции
	int scnt = ex->ehdr->e_phnum;
	while (scnt--) {

		if (EPH->p_type == PT_LOAD) {
			memcpy ((char*)ex->physAdr+EPH->p_vaddr, ex->bin + EPH->p_offset, EPH->p_filesz);
			printf("Loading section (%X, size %i)\n", ex->bin + EPH->p_offset, EPH->p_filesz);
		}
		EPH = ( Elf32_Phdr *)((unsigned char *)EPH + ex->ehdr->e_phentsize);
	}

	return 0;
}


/** расчет размера бинарного файла */
int calculateBinarySize(Elf32_Exec *ex)
{
	unsigned int scnt = ex->ehdr->e_phnum, binsz = 0;
	Elf32_Phdr *ph = ( Elf32_Phdr *)(ex->bin + ex->ehdr->e_phoff);

	while (scnt--)
	{
		if (ph->p_type == PT_LOAD) {
			binsz += ph->p_filesz;
		}
		ph = ( Elf32_Phdr *)((unsigned char *)ph + ex->ehdr->e_phentsize);
	}

	return binsz;
}
