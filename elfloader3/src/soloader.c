 
#include "loader.h"

Elf32_Exec __ex_so_table[MAX_DLSO_BASE] = {0};


unsigned long elf_hash(const char* name)
{
  unsigned long int hash = 0;
  while (*name != '\0')
  {
    unsigned long int hi;
    hash = (hash << 4) + *name++;
    hi = hash & 0xf0000000;
    if (hi != 0)
    {
      hash ^= hi >> 24;
      hash ^= hi;
    }
  }
  return hash;
}


int lib_prepare(char *data, int data_size, Elf32_Exec *ex)
{
   printf("Starting load lib...\n");

	memset(ex, 0, sizeof(Elf32_Exec));

	ex->bin = data;
	ex->elfSize = data_size;
	ex->virtAdr = (unsigned long)-1;

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
	{
	  printf("Break with errors\n");
	  return -1;
	}
	
	printf("Shared lib loaded complete\n");
	return 0;
}


typedef uint32_t Elf_Symndx;
# define do_rem(result, n, base) ((result) = (n) % (base))


static inline Elf32_Sym *
check_match ( Elf32_Sym *sym, char *strtab, const char* undef_name)
{
	if (sym->st_value == 0)
	{
	  //printf("nv\n");
	  return NULL;
	}
	
	if (ELF_ST_TYPE(sym->st_info) != STT_FUNC 
		&& ELF_ST_TYPE(sym->st_info) != STB_GLOBAL)
	{
	  //printf("Type is not valid\n");
	  return NULL;
	}
	
	if ( strcmp(strtab + sym->st_name, undef_name) )
	{
	  //printf("%s != %s\n", strtab + sym->st_name, undef_name);
	  return NULL;
	}
	
	/* This is the matching symbol */
	return sym;
}


void *findFunctionAddres(Elf32_Exec *ex, const char *name)
{
  char *strtab = ex->bin + ex->dyn[DT_STRTAB] - ex->virtAdr;
  char *hashtab = ex->bin + ex->dyn[DT_HASH] - ex->virtAdr;
  Elf32_Sym *symtab = (Elf32_Sym*)(ex->bin + ex->dyn[DT_SYMTAB] - ex->virtAdr);
  Elf32_Sym *sym, *o_sym = 0;
  unsigned long hash = elf_hash(name);
  int symidx;
  char brk = 0;
  printf("search function: %s\n", name);

  Elf32_Word nbucket = *(ex->bin + ex->dyn[DT_HASH] - ex->virtAdr);
  Elf32_Word nchain = *(ex->bin + ex->dyn[DT_HASH] + 4 - ex->virtAdr);
  Elf32_Word* bucket = (Elf32_Word*)(ex->bin + ex->dyn[DT_HASH] + 8 - ex->virtAdr);
  Elf32_Word* chain = (Elf32_Word*)(ex->bin + ex->dyn[DT_HASH] + 8 + (nbucket*sizeof(Elf32_Word*)) - ex->virtAdr);

  for (symidx = bucket[hash % nbucket];
    symidx != STN_UNDEF;
    symidx = chain[symidx])
  {
    sym = &symtab[symidx];

    if (sym->st_value == 0 || /* No value. */
    sym->st_shndx == SHN_UNDEF)
    continue;

    switch (ELF_ST_TYPE(sym->st_info))
    {
      case STT_NOTYPE:
      case STT_FUNC:
      case STT_OBJECT:
	break;
      default:
      /* Not a code/data definition. */
      continue;
    }

    if (strcmp (strtab + sym->st_name, name))
    /* Not the symbol we are looking for. */
    continue;

    switch (ELF_ST_BIND(sym->st_info))
    {
      case STB_GLOBAL:
	/* Global definition. Just what we need. */
	printf("func STB_GLOBAL: %s\n", strtab + sym->st_name);
	brk = 1;
	o_sym = sym;
      break;
      case STB_WEAK:
	/* Weak definition. Use this value if we don't find
	another. */
	printf("func STB_WEAK: %s\n", strtab + sym->st_name);
	brk = 1;
	o_sym = sym;
      break;
      default:
	/* Local symbols are ignored. */
      break;
    }
    
    if(brk) break;
  }
  
  if(!o_sym) return 0;
  //printf("seek %x %s\n", o_sym->st_value, strtab+o_sym->st_name);
  return (void*)(ex->bin + o_sym->st_value - ex->virtAdr);
}



int dlopen(const char *name)
{
    char *buffer;
    int size;
    int i=-1;
    
    for(int ii=0; ii<MAX_DLSO_BASE; ++ii)
    {
      if( !__ex_so_table[ii].elfSize){
	if( i == -1 ) i = ii;
      } else
      {
	if( !strcmp(__ex_so_table[ii]._file, name) )	// нашли либу с таким же путём загруженную
	{
	  printf("Lib '%s' olready loaded\n", name);
	  return ii;
	}
      }
    }
    
    if(i == MAX_DLSO_BASE-1) return -2;
    
    
    FILE *fp = fopen(name, "rb");
    if(!fp){
        printf("Can`t open file '%s'\n", name);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    buffer = malloc(size+1);

    fread(buffer, size, 1, fp);
    fclose(fp);
	
    buffer[size] = 0;

    if( lib_prepare(buffer, size, &__ex_so_table[i]) )
      return -1;
    
    strcpy( __ex_so_table[i]._file, name);
    
    printf("SIZE: %d\n", i);
    return i;
}


void *dlsym(int dlopen_id, const char *name)
{
  if( !__ex_so_table[dlopen_id].elfSize ){
    printf("Empty dlsym id\n");
    return 0;
  }
  
  void * addr = findFunctionAddres(&__ex_so_table[dlopen_id], name);
  return addr;
}


int dlclose(int dlopen_id)
{
  if( !__ex_so_table[dlopen_id].elfSize ) return -1;
  
  free(__ex_so_table[dlopen_id].physAdr);
  free(__ex_so_table[dlopen_id].bin);
  
  memset(&__ex_so_table[dlopen_id], 0, sizeof(Elf32_Exec));
  return 0;
}


void clear_all_libs()
{
    for(int i=0; i<MAX_DLSO_BASE; ++i)
    {
        if( !dlclose(i) )
            printf("Cleaned %d\n", i);
    }
}
