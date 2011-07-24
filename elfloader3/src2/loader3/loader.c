/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include "loader.h"

unsigned int ferr;

// Проверка валидности эльфа
__arch int CheckElf(Elf32_Ehdr *ehdr)
{
    if(memcmp(ehdr, elf_magic_header, sizeof(elf_magic_header))) return E_HEADER;
    if(ehdr->e_machine != EM_ARM) return E_MACHINE;

    return E_NO_ERROR;
}

// Получение нужного размера в раме
__arch unsigned int GetBinSize(Elf32_Exec *ex, Elf32_Phdr* phdrs)
{
    unsigned int i = 0;
    unsigned long maxadr=0;
    unsigned int end_adr;

    while (i < ex->ehdr.e_phnum)
    {
        Elf32_Phdr phdr = phdrs[i];

        if (phdr.p_type == PT_LOAD)
        {
            if (ex->v_addr > phdr.p_vaddr) ex->v_addr = phdr.p_vaddr;
            end_adr = phdr.p_vaddr + phdr.p_memsz;
            if (maxadr < end_adr) maxadr = end_adr;
        }
        i++;
    }
    return maxadr - ex->v_addr;
}

__arch char* LoadData(Elf32_Exec* ex, int offset, int size)
{
    if(size && lseek(ex->fp, offset - ex->v_addr, S_SET, &ferr, &ferr))
    {
        char* data = malloc(size+1);
        //zeromem_a(data, size+1);
        if(fread(ex->fp, data, size, &ferr) == size) {
          data[size] = 0;
          return data;
        }
        else mfree(data);
    }

    return 0;
}


/* Вспомогательная функция */
__arch static inline unsigned int _look_sym(Elf32_Exec *ex, const char *name)
{
    Libs_Queue *lib = ex->libs;
    unsigned int func = 0;
    while(lib && !func)
    {
        func = (unsigned int)FindFunction(lib->lib, name);
        lib = lib->next;
    }
    return func;
}

// Релокация
__arch int DoRelocation(Elf32_Exec* ex, Elf32_Dyn* dyn_sect, Elf32_Phdr* phdr)
{
    unsigned int i = 0;
    Elf32_Word libs_needed[64];
    unsigned int libs_cnt = 0;
    char dbg[128];

    // Вытаскиваем теги
    while (dyn_sect[i].d_tag != DT_NULL)
    {
        if (dyn_sect[i].d_tag <= DT_FLAGS)
        {
            switch(dyn_sect[i].d_tag)
            {
            case DT_SYMBOLIC:
                // Флаг SYMBOLIC-библиотек. В d_val 0, даже при наличии :(
                ex->dyn[dyn_sect[i].d_tag] = 1;
                break;
            case DT_NEEDED:
                // Получаем смещения в .symtab на имена либ
                libs_needed[libs_cnt++] = dyn_sect[i].d_un.d_val;
                break;
            default:
                ex->dyn[dyn_sect[i].d_tag] = dyn_sect[i].d_un.d_val;
            }
        }
        i++;
    }

    // Таблички. Нужны только либам, и их юзающим)
    ex->symtab = ex->dyn[DT_SYMTAB]? (Elf32_Sym*)(ex->body + ex->dyn[DT_SYMTAB] - ex->v_addr) : 0;
    ex->jmprel = (Elf32_Rel*)(ex->body + ex->dyn[DT_JMPREL] - ex->v_addr);
    ex->strtab = ex->dyn[DT_STRTAB]? ex->body + ex->dyn[DT_STRTAB] - ex->v_addr : 0;
    
    printf("STRTAB: %X\n", ex->dyn[DT_STRTAB]);
    printf("SYMTAB: %X\n", ex->dyn[DT_SYMTAB]);

    if(ex->type == EXEC_LIB)
    {
        Elf32_Word* hash_hdr = (Elf32_Word*)LoadData(ex, ex->dyn[DT_HASH], 8);
        if(hash_hdr)
        {
            int hash_size = hash_hdr[0] * sizeof(Elf32_Word) + hash_hdr[1] * sizeof(Elf32_Word) + 8;
            ex->hashtab = (Elf32_Word*)LoadData(ex, ex->dyn[DT_HASH], hash_size);
	    if(!ex->hashtab) goto __hash_err;
        }
        else
	{
	  __hash_err:
	  ShowMSG(1, (int)"Error loading hashtab");
	  return E_HASTAB;
	}
    }

    // Загрузка библиотек
    for(i=0; i < libs_cnt; i++)
    {
        char *lib_name = ex->strtab + libs_needed[i];
        Elf32_Lib* lib;
        if( (lib = OpenLib(lib_name)) )
        {
            Libs_Queue* libq = malloc(sizeof(Libs_Queue));
            libq->lib = lib;

            if(ex->libs) libq->next = ex->libs;
            else libq->next = 0;

            ex->libs = libq;
        }
        else
        {
            sprintf(dbg, "Не могу загрузить %s!", lib_name);
            l_msg(1, (int)dbg);
            return E_SHARED;
        }
    }

    // Релокация
    if (ex->dyn[DT_RELSZ])
    {
        i=0;
        unsigned int* addr;
        Elf32_Word r_type;
    
        char* name;
        Elf32_Word func = 0;
        //Libs_Queue* lib;

        // Таблица релокаций
        Elf32_Rel* reltab = (Elf32_Rel*)LoadData(ex, phdr->p_offset + ex->dyn[DT_REL] - phdr->p_vaddr, ex->dyn[DT_RELSZ]);
    
        if(!reltab)
        {
            elfclose(ex);
            return E_RELOCATION;
        }

        while(i * sizeof(Elf32_Rel) < ex->dyn[DT_RELSZ])
        {
            r_type = ELF32_R_TYPE(reltab[i].r_info);
            Elf32_Sym *sym = ex->symtab? &ex->symtab[ELF32_R_SYM(reltab[i].r_info)] : 0;
            int bind_type = sym ? ELF_ST_BIND(sym->st_info) : 0;

            switch(r_type)
            {
            case R_ARM_NONE:
                break;
            case R_ARM_RABS32:
                printf("R_ARM_RABS32\n");
                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr += (unsigned int)(ex->body - ex->v_addr);
                break;
            case R_ARM_ABS32:
                printf("R_ARM_ABS32\n");
		
		if( !ex->symtab )
		{
		   sprintf(dbg, "Relocation R_ARM_ABS32 cannot run without symtab\n");
                   printf(1, (int)dbg);
		   printf("warning: symtab not found, but relocation R_ARM_ABS32 is exist\n");
		   *addr = (unsigned int)ex->body;
		   break;
                   //mfree(reltab);
                   //return E_SYMTAB;
		}
		
		if( !ex->strtab )
		{
		   sprintf(dbg, "Relocation R_ARM_ABS32 cannot run without strtab\n");
                   printf(1, (int)dbg);
		   printf("warning: strtab not found, but relocation R_ARM_ABS32 is exist\n");
		   *addr = (unsigned int)ex->body;
		   break;
                   //mfree(reltab);
                   //return E_STRTAB;
		}
		
                name = ex->strtab + sym->st_name;
                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);

                //int sk = ELF32_R_SYM(reltab[i].r_info);

                printf("'%s' %X\n", name, *addr);
                // Если нужен указатель на эльф

                //if( *(int*)name == *(int*)"__ex" ) // че оно пикает?!
                //if( !strcmp(name, "__ex") )
                if( name[4] == 0   && 
                    name[0] == '_' &&
                    name[1] == '_' &&
                    name[2] == 'e' &&
                    name[3] == 'x'
                    )
                {
                    ex->__is_ex_import = 1;
                    printf("__ex: 0x%X\n", (int)ex);
                    *addr = (unsigned int)ex;
                    break;
                }

                func = findExport(ex, name);
                

                if(!func)
                    func = _look_sym(ex, name);
		
		printf("%x - %s\n", func, name);

                if(!func && bind_type != STB_WEAK)
                {
                    sprintf(dbg, "Undefined reference to `%s'\n", name);
                    l_msg(1, (int)dbg);
                    mfree(reltab);
                    return E_UNDEF;
                }

                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr = func;
                printf("addres: %X\n", name, *addr);
                break;
            case R_ARM_RELATIVE:
                printf("R_ARM_RELATIVE\n");
                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr += (unsigned int)(ex->body - ex->v_addr);
                break;
            case R_ARM_GLOB_DAT:
                printf("R_ARM_GLOB_DAT\n");
		
		if( !ex->symtab )
		{
		   sprintf(dbg, "Relocation R_ARM_GLOB_DAT cannot run without symtab\n");
                   l_msg(1, (int)dbg);
                   mfree(reltab);
                   return E_SYMTAB;
		}
		
		if( !ex->strtab )
		{
		   sprintf(dbg, "Relocation R_ARM_GLOB_DAT cannot run without strtab\n");
                   l_msg(1, (int)dbg);
                   mfree(reltab);
                   return E_STRTAB;
		}
		
                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);
                name = ex->strtab + sym->st_name;
                printf(" strtab: '%s' \n", name);
                *addr = (unsigned int)(ex->body + sym->st_value);

                if( !sym->st_value )
                {
                    printf("Searching in libs...\n");
                    *addr = (unsigned int)_look_sym(ex, name);
                    if( !*addr && bind_type != STB_WEAK)
                    {
                        sprintf(dbg, "Undefined reference to `%s'\n", name);
                        l_msg(1, (int)dbg);
                        mfree(reltab);
                        return E_UNDEF;
                    }

                    if(*addr){
                        printf("found at 0x%X\n", *addr);
                    }
                }

                break;
            default:
                printf("unknow relocation type '%d'\n", r_type);
                sprintf(dbg, "Fatal error! Unknown type relocation '%d'!\n", r_type);
                l_msg(1, (int)dbg);
                mfree(reltab);
                return E_RELOCATION;
                //break;
            }
            i++;
        }

        mfree(reltab);
    }

    // Биндим функции
    if(ex->dyn[DT_PLTRELSZ])
    {
        i=0;
        while(i * sizeof(Elf32_Rel) < ex->dyn[DT_PLTRELSZ])
        {
            int sym_idx = ELF32_R_SYM(ex->jmprel[i].r_info);
            char* name = ex->strtab + ex->symtab[sym_idx].st_name;
            Elf32_Sym *sym = &ex->symtab[sym_idx];
            Elf32_Word func = 0;
            //Libs_Queue* lib = ex->libs;


            // Если библиотека не SYMBOLIC - сначала ищем в ней самой
            if(ex->type == EXEC_LIB && !ex->dyn[DT_SYMBOLIC])
                func = findExport(ex, name);

            if(!func)
                func = _look_sym(ex, name);

            printf("function addres: %x name: '%s'\n", func, name);

            if(!func && ELF_ST_BIND(sym->st_info) != STB_WEAK)
            {
                sprintf(dbg, "Undefined reference to `%s'\n", name);
                l_msg(1, (int)dbg);
                return E_UNDEF;
            }

            *((Elf32_Word*)(ex->body + ex->jmprel[i].r_offset)) = func;
            i++;
        }
    }

    return E_NO_ERROR;
}


// Чтение сегментов из файла
__arch int LoadSections(Elf32_Exec* ex)
{
    Elf32_Phdr* phdrs = malloc(sizeof(Elf32_Phdr) * ex->ehdr.e_phnum);
    if(!phdrs) return E_SECTION;

    unsigned int hdr_offset = ex->ehdr.e_phoff;
    int i = 0;

    unsigned long maxadr=0;
    unsigned int end_adr;

    // Читаем заголовки
    while(i < ex->ehdr.e_phnum)
    {
        if(lseek(ex->fp, hdr_offset, S_SET, &ferr, &ferr) == -1) break;
<<<<<<< .mine
        if(read(ex->fp, &phdrs[i], sizeof(Elf32_Phdr), &ferr) != sizeof(Elf32_Phdr))
	{
//#warning This is good?
           /* кривой заголовок, шлём нафиг этот эльф */
	   mfree(ex->body);
           ex->body = 0;
           mfree(phdrs);
           return E_PHDR;
	}
        
        /* тут же и размер бинарника посчитаем */
        if (phdrs[i].p_type == PT_LOAD)
=======
        if(fread(ex->fp, &phdrs[i], sizeof(Elf32_Phdr), &ferr) < sizeof(Elf32_Phdr))
            break;

        // не наше выравнивание
        /*if( phdrs[i].p_offset != phdrs[i].p_vaddr )
>>>>>>> .r42
        {
            if (ex->v_addr > phdrs[i].p_vaddr) ex->v_addr = phdrs[i].p_vaddr;
            end_adr = phdrs[i].p_vaddr + phdrs[i].p_memsz;
            if (maxadr < end_adr) maxadr = end_adr;
        }
	
        hdr_offset += ex->ehdr.e_phentsize;
        ++i;
    }
    
    ex->bin_size = maxadr - ex->v_addr;

    if(i == ex->ehdr.e_phnum) // Если прочитались все заголовки
    {
        //ex->bin_size = GetBinSize(ex, phdrs);

        if(ex->body = malloc(ex->bin_size+1)) // Если хватило рамы
        {
            zeromem_a(ex->body, ex->bin_size+1);
            zeromem_a(ex->dyn, sizeof(ex->dyn));

            for(i=0; i < ex->ehdr.e_phnum; i++)
            {
                Elf32_Phdr phdr = phdrs[i];
                Elf32_Dyn* dyn_sect;

                switch (phdr.p_type)
                {
                case PT_LOAD:
                    if(phdr.p_filesz == 0) break; // Пропускаем пустые сегменты

                    if(lseek(ex->fp, phdr.p_offset, S_SET, &ferr, &ferr) != -1)
                    {
                        if(fread(ex->fp, ex->body + phdr.p_vaddr - ex->v_addr, phdr.p_filesz, &ferr) == phdr.p_filesz)
                            break;
                    }

                    // Не прочитали сколько нужно
                    mfree(ex->body);
                    ex->body = 0;
                    mfree(phdrs);
                    return E_SECTION;

                case PT_DYNAMIC:
                    if(phdr.p_filesz == 0) break; // Пропускаем пустые сегменты

                    if(dyn_sect = (Elf32_Dyn*)LoadData(ex, phdr.p_offset, phdr.p_filesz))
                    {
                        if(!DoRelocation(ex, dyn_sect, &phdr))
                        {
                            mfree(dyn_sect);
                            break;
                        }
                    }

                    // Если что-то пошло не так...
                    mfree(dyn_sect);
                    mfree(ex->body);
                    ex->body = 0;
                    mfree(phdrs);
                    return E_SECTION;
                }
            }

            mfree(phdrs);
            return E_NO_ERROR;
        }
    }

    mfree(ex->body);
    ex->body = 0;
    mfree(phdrs);
    return E_RAM;
}


/* constructors */
__arch void run_INIT_Array(Elf32_Exec *ex)
{
  if(!ex->dyn[DT_FINI_ARRAY]) return;
  size_t sz = ex->dyn[DT_INIT_ARRAYSZ];
  void ** arr = (void**)(ex->body + ex->dyn[DT_INIT_ARRAY] - ex->v_addr);

  printf("init_array sz: %d\n", sz);

  for(int i=0; i*sizeof(void*) < sz; ++i)
  {
     printf("init %d: 0x%X\n", i, arr[i]);
#ifndef _test_linux
     ( (void (*)())arr[i])();
#endif
  }
}


/* destructors */
__arch void run_FINI_Array(Elf32_Exec *ex)
{
  if(!ex->dyn[DT_FINI_ARRAY]) return;
  size_t sz = ex->dyn[DT_FINI_ARRAYSZ];
  void ** arr = (void**)(ex->body + ex->dyn[DT_FINI_ARRAY] - ex->v_addr);

  printf("fini_array sz: %d\n", sz);

  for(int i=0; i*sizeof(void*) < sz; ++i)
  {
     printf("fini %d: 0x%X\n", i, arr[i]);
#ifndef _test_linux
     ( (void (*)())arr[i])();
#endif
  }
}
