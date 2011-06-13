
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
    if(!memcmp(ehdr, elf_magic_header, sizeof(elf_magic_header))) return E_NO_ERROR;
    return E_HEADER;
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
        char* data = malloc(size);
        zeromem(data, size);
        if(read(ex->fp, data, size, &ferr) == size) return data;
        else mfree(data);
    }

    return 0;
}


void test()
{
    ShowMSG(1, (int)"Hello");
}

// Релокация
__arch int DoRelocation(Elf32_Exec* ex, Elf32_Dyn* dyn_sect, Elf32_Phdr* phdr)
{
    zeromem(ex->dyn, sizeof(ex->dyn));
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
    //if(ex->type == EXEC_LIB || libs_cnt)
    {
        ex->symtab = (Elf32_Sym*)(ex->body + ex->dyn[DT_SYMTAB] - ex->v_addr);//LoadData(ex, ex->dyn[DT_SYMTAB], ex->dyn[DT_SYMENT]);
        ex->jmprel = (Elf32_Rel*)(ex->body + ex->dyn[DT_JMPREL] - ex->v_addr);//LoadData(ex, ex->dyn[DT_JMPREL], ex->dyn[DT_PLTRELSZ]);
        ex->strtab = ex->body + ex->dyn[DT_STRTAB] - ex->v_addr;//LoadData(ex, ex->dyn[DT_STRTAB], ex->dyn[DT_STRSZ]);
    }

    if(ex->type == EXEC_LIB)
    {
        Elf32_Word* hash_hdr = (Elf32_Word*)LoadData(ex, ex->dyn[DT_HASH], 8);
        if(hash_hdr)
        {
            int hash_size = hash_hdr[0] * sizeof(Elf32_Word) + hash_hdr[1] * sizeof(Elf32_Word) + 8;
            ex->hashtab = (Elf32_Word*)LoadData(ex, ex->dyn[DT_HASH], hash_size);
        }
    }

    // Загрузка библиотек
    for(i=0; i < libs_cnt; i++)
    {
        char *lib_name = ex->strtab + libs_needed[i];
        Elf32_Lib* lib;
        if(lib = dlopen(lib_name))
        {
            Libs_Queue* libq = malloc(sizeof(Libs_Queue));
            libq->lib = lib;

            if(ex->libs) libq->next = ex->libs;
            else libq->next = 0;

            ex->libs = libq;
        }
        else
        {
            sprintf(dbg, "Библиотека %s не найдена!", lib_name);
            ShowMSG(1, (int)dbg);
            return E_SHARED;
        }
    }

    // Релокация
    if (ex->dyn[DT_RELSZ])
    {
        i=0;
        unsigned long* addr;
        Elf32_Word r_type;
        Elf32_Rel* reltab;
        char* name;
        Elf32_Word func = 0;
        Libs_Queue* lib;

        // Таблица релокаций
        reltab = (Elf32_Rel*)LoadData(ex, phdr->p_offset + ex->dyn[DT_REL] - phdr->p_vaddr, ex->dyn[DT_RELSZ]);
        //reltab = (Elf32_Rel*)((char*)dyn_sect + ex->dyn[DT_REL] -  phdr->p_vaddr);
        if(!reltab)
        {
            elfclose(ex);
            return E_RELOCATION;
        }

        while(i * sizeof(Elf32_Rel) < ex->dyn[DT_RELSZ])
        {
            r_type = ELF32_R_TYPE(reltab[i].r_info);

            switch(r_type)
            {
            case R_ARM_NONE:
                break;
            case R_ARM_RABS32:
                printf("R_ARM_RABS32\n");
                addr = (unsigned long*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr += (unsigned long)(ex->body - ex->v_addr);
                break;
            case R_ARM_ABS32:
                printf("R_ARM_ABS32\n");
                name = ex->strtab + ex->symtab[ELF32_R_SYM(reltab[i].r_info)].st_name;
                addr = (unsigned long*)(ex->body + reltab[i].r_offset - ex->v_addr);

                int sk = ELF32_R_SYM(reltab[i].r_info);

                printf("'%s' %X\n", name, *addr);
                // Если нужен указатель на эльф


                if(!strcmp("__sys_switab_addres", name))
                {
                    *addr = (volatile unsigned int) *(int*)AddrLibrary();
                }
                else if(!strcmp("__ex", name))
                {
                    printf("__ex: 0x%X\n", (int)ex);
                    *addr = (unsigned int)ex;
                    break;
                }
                else if(!strcmp("elfclose", name))
                {
                    *addr = (unsigned int)elfclose;
                    printf(" [+] elfclose: 0x%X\n", *addr);
                    break;
                }

                /* !!! O_o !!! */
                if( ex->symtab[sk].st_info != 17  && ex->symtab[sk].st_info != 18)
                {
                    printf("st_info != 17 0x%X %d '%s'\n", elfclose, ex->symtab[sk].st_info, name);
                    *addr = (unsigned int)ex->body;
                    break;
                }


                func = 0;
                lib = ex->libs;

                while(lib && !func)
                {
                    func = dlsym(lib->lib, name);
                    lib = lib->next;
                }

                if(!func)
                {
                    sprintf(dbg, "Undefined reference to `%s'\n", name);
                    ShowMSG(1, (int)dbg);
                    mfree(reltab);
                    return E_SHARED;
                }

                addr = (unsigned long*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr = func;
                printf("addres: %X\n", name, *addr);
                break;
            case R_ARM_RELATIVE:
                printf("R_ARM_RELATIVE\n");
                addr = (unsigned long*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr += (unsigned long)(ex->body - ex->v_addr);
                break;
            case R_ARM_GLOB_DAT:
                printf("R_ARM_GLOB_DAT\n");
                addr = (unsigned long*)(ex->body + reltab[i].r_offset - ex->v_addr);
                *addr = (unsigned long)(ex->body + ex->symtab[ELF32_R_SYM(reltab[i].r_info)].st_value);
                break;
            default:
                printf("unknow relocation type '%d'\n", r_type);
                break;
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
            Elf32_Word func = 0;
            Libs_Queue* lib = ex->libs;

            if(!strcmp("__sys_switab_addres", name))
            {
                    func = *(volatile unsigned int*)AddrLibrary();
            }
            else if(!strcmp("elfclose", name))
            {
                printf("elfclose rel\n");
                func = (long)elfclose;
            }
            else
            {
                // Если библиотека не SYMBOLIC - сначала ищем в ней самой
                if(ex->type == EXEC_LIB && !ex->dyn[DT_SYMBOLIC])
                {
                    func = findExport(ex, name);
                }

                while(lib && !func)
                {
                    func = dlsym(lib->lib, name);
                    lib = lib->next;
                }

                if(!func)
                {
                    sprintf(dbg, "Undefined reference to `%s'\n", name);
                    ShowMSG(1, (int)dbg);
                    return E_SHARED;
                }
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

    // Читаем заголовки
    while(i < ex->ehdr.e_phnum)
    {
        if(lseek(ex->fp, hdr_offset, S_SET, &ferr, &ferr) == -1) break;
        if(read(ex->fp, &phdrs[i], sizeof(Elf32_Phdr), &ferr) < sizeof(Elf32_Phdr))
            break;

        hdr_offset += ex->ehdr.e_phentsize;
        ++i;
    }

    if(i == ex->ehdr.e_phnum) // Если прочитались все заголовки
    {
        ex->bin_size = GetBinSize(ex, phdrs);

        if(ex->body = malloc(ex->bin_size+1)) // Если хватило рамы
        {
            zeromem(ex->body, ex->bin_size);

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
                        if(read(ex->fp, ex->body + phdr.p_vaddr - ex->v_addr, phdr.p_filesz, &ferr) == phdr.p_filesz)
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
  size_t sz = ex->dyn[DT_INIT_ARRAYSZ];
  void ** arr = (void**)(ex->body + ex->dyn[DT_INIT_ARRAY] - ex->v_addr);

  printf("init_array sz: %d\n", sz);

  size_t cnt = sz/sizeof(void*);

  for(int i=0; i<cnt; ++i)
  {
     printf("init %d: 0x%X\n", i, arr[i]);
     ( (void (*)())arr[i])();
  }
}


/* destructors */
__arch void run_FINI_Array(Elf32_Exec *ex)
{
  size_t sz = ex->dyn[DT_FINI_ARRAYSZ];
  void ** arr = (void**)(ex->body + ex->dyn[DT_FINI_ARRAY] - ex->v_addr);

  printf("fini_array sz: %d\n", sz);

  size_t cnt = sz/sizeof(void*);

  for(int i=0; i<cnt; ++i)
  {
     printf("fini %d: 0x%X\n", i, arr[i]);
     ( (void (*)())arr[i])();
  }
}
