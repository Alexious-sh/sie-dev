
/*
 * ���� ���� �������� ������ ��������� ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include "loader.h"

unsigned int ferr;

// �������� ���������� �����
__arch int CheckElf(Elf32_Ehdr *ehdr)
{
    if(memcmp(ehdr, elf_magic_header, sizeof(elf_magic_header))) return E_HEADER;
    if(ehdr->e_machine != EM_ARM) return E_MACHINE;

    return E_NO_ERROR;
}

// ��������� ������� ������� � ����
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
        zeromem_a(data, size);
        if(fread(ex->fp, data, size, &ferr) == size) return data;
        else mfree(data);
    }

    return 0;
}


/* ��������������� ������� */
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

// ���������
__arch int DoRelocation(Elf32_Exec* ex, Elf32_Dyn* dyn_sect, Elf32_Phdr* phdr)
{
    unsigned int i = 0;
    Elf32_Word libs_needed[64];
    unsigned int libs_cnt = 0;
    char dbg[128];

    //Elf32_Dyn* dyn_sect = (Elf32_Dyn*)LoadData(ex, phdr->p_offset, phdr->p_filesz);
    //if(!dyn_sect) return 0;
  
    // ����������� ����
    zeromem_a(ex->dyn, sizeof(ex->dyn));
    while (dyn_sect[i].d_tag != DT_NULL)
    {
        if (dyn_sect[i].d_tag <= DT_FLAGS)
        {
            switch(dyn_sect[i].d_tag)
            {
            case DT_SYMBOLIC:
                // ���� SYMBOLIC-���������. � d_val 0, ���� ��� ������� :(
                ex->dyn[dyn_sect[i].d_tag] = 1;
                break;
            case DT_NEEDED:
                // �������� �������� � .symtab �� ����� ���
                libs_needed[libs_cnt++] = dyn_sect[i].d_un.d_val;
                break;
            default:
                ex->dyn[dyn_sect[i].d_tag] = dyn_sect[i].d_un.d_val;
            }
        }
        i++;
    }

    // ��������. ����� ������ �����, � �� �������)
    ex->symtab = (Elf32_Sym*)(ex->body + ex->dyn[DT_SYMTAB] - ex->v_addr);
    ex->jmprel = (Elf32_Rel*)(ex->body + ex->dyn[DT_JMPREL] - ex->v_addr);
    ex->strtab = ex->body + ex->dyn[DT_STRTAB] - ex->v_addr;

    if(ex->type == EXEC_LIB)
    {
        Elf32_Word* hash_hdr = (Elf32_Word*)LoadData(ex, ex->dyn[DT_HASH], 8);
        if(hash_hdr)
        {
            int hash_size = hash_hdr[0] * sizeof(Elf32_Word) + hash_hdr[1] * sizeof(Elf32_Word) + 8;
            ex->hashtab = (Elf32_Word*)LoadData(ex, ex->dyn[DT_HASH], hash_size);
        }
    }

    // �������� ���������
    for(i=0; i < libs_cnt; i++)
    {
        char *lib_name = ex->strtab + libs_needed[i];
        Elf32_Lib* lib;
    if(lib = OpenLib(lib_name))
        {
            Libs_Queue* libq = malloc(sizeof(Libs_Queue));
            libq->lib = lib;

            if(ex->libs) libq->next = ex->libs;
            else libq->next = 0;

            ex->libs = libq;
        }
        else
        {
            sprintf(dbg, "�� ���� ��������� %s!", lib_name);
            l_msg(1, (int)dbg);
            return E_SHARED;
        }
    }

    // ���������
    if (ex->dyn[DT_RELSZ])
    {
        i=0;
        unsigned int* addr;
        Elf32_Word r_type;
    
        char* name;
        Elf32_Word func = 0;
        //Libs_Queue* lib;

        // ������� ���������
        Elf32_Rel* reltab = (Elf32_Rel*)LoadData(ex, phdr->p_offset + ex->dyn[DT_REL] - phdr->p_vaddr, ex->dyn[DT_RELSZ]);
    
        if(!reltab)
        {
            elfclose(ex);
            return E_RELOCATION;
        }

        while(i * sizeof(Elf32_Rel) < ex->dyn[DT_RELSZ])
        {
            r_type = ELF32_R_TYPE(reltab[i].r_info);
            Elf32_Sym *sym = &ex->symtab[ELF32_R_SYM(reltab[i].r_info)];

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
                name = ex->strtab + sym->st_name;
                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);

                //int sk = ELF32_R_SYM(reltab[i].r_info);

                printf("'%s' %X\n", name, *addr);
                // ���� ����� ��������� �� ����

                if(!strcmp("__ex", name))
                {
                    ex->__is_ex_import = 1;
                    printf("__ex: 0x%X\n", (int)ex);
                    *addr = (unsigned int)ex;
                    break;
                }

                /* !!! O_o !!! */
                /*if( ex->symtab[sk].st_info != 17  &&  18)
                {
                    printf("st_info != 17 0x%X %d '%s'\n", elfclose, ex->symtab[sk].st_info, name);
                    *addr = (unsigned int)ex->body;
                    break;
                }*/

                func = findExport(ex, name);
                printf("%x - %s\n", func, name);

                if(!func)
                    func = _look_sym(ex, name);

                if(!func && ELF_ST_BIND(sym->st_info) != STB_WEAK)
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
                addr = (unsigned int*)(ex->body + reltab[i].r_offset - ex->v_addr);
                name = ex->strtab + sym->st_name;
                printf(" strtab: '%s' \n", name);
                *addr = (unsigned int)(ex->body + sym->st_value);

                if( !sym->st_value )
                {
                    printf("Searching in libs...\n");
                    *addr = (unsigned int)_look_sym(ex, name);
                    if( !*addr && ELF_ST_BIND(sym->st_info) != STB_WEAK)
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

    // ������ �������
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


            // ���� ���������� �� SYMBOLIC - ������� ���� � ��� �����
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

// ������ ��������� �� �����
__arch int LoadSections(Elf32_Exec* ex)
{
    Elf32_Phdr* phdrs = malloc(sizeof(Elf32_Phdr) * ex->ehdr.e_phnum);
    if(!phdrs) return E_SECTION;

    unsigned int hdr_offset = ex->ehdr.e_phoff;
    int i = 0;

    // ������ ���������
    while(i < ex->ehdr.e_phnum)
    {
        if(lseek(ex->fp, hdr_offset, S_SET, &ferr, &ferr) == -1) break;
        if(read(ex->fp, &phdrs[i], sizeof(Elf32_Phdr), &ferr) < sizeof(Elf32_Phdr))
            break;

        // �� ���� ������������
        /*if( phdrs[i].p_offset != phdrs[i].p_vaddr )
        {
            printf("Bad align\n");
            mfree(phdrs);
            ShowMSG(1, (int)"Bad page size!");
            return E_ALIGN;
        }*/

        hdr_offset += ex->ehdr.e_phentsize;
        ++i;
    }

    if(i == ex->ehdr.e_phnum) // ���� ����������� ��� ���������
    {
        ex->bin_size = GetBinSize(ex, phdrs);

        if(ex->body = malloc(ex->bin_size+1)) // ���� ������� ����
        {
            zeromem(ex->body, ex->bin_size);

            for(i=0; i < ex->ehdr.e_phnum; i++)
            {
                Elf32_Phdr phdr = phdrs[i];
                Elf32_Dyn* dyn_sect;

                switch (phdr.p_type)
                {
                case PT_LOAD:
                    if(phdr.p_filesz == 0) break; // ���������� ������ ��������

                    if(lseek(ex->fp, phdr.p_offset, S_SET, &ferr, &ferr) != -1)
                    {
                        if(read(ex->fp, ex->body + phdr.p_vaddr - ex->v_addr, phdr.p_filesz, &ferr) == phdr.p_filesz)
                            break;
                    }

                    // �� ��������� ������� �����
                    mfree(ex->body);
                    ex->body = 0;
                    mfree(phdrs);
                    return E_SECTION;

                case PT_DYNAMIC:
                    if(phdr.p_filesz == 0) break; // ���������� ������ ��������

                    if(dyn_sect = (Elf32_Dyn*)LoadData(ex, phdr.p_offset, phdr.p_filesz))
                    {
                        if(!DoRelocation(ex, dyn_sect, &phdr))
                        {
                            mfree(dyn_sect);
                            break;
                        }
                    }

                    // ���� ���-�� ����� �� ���...
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
  size_t sz = ex->dyn[DT_INIT_ARRAYSZ];
  void ** arr = (void**)(ex->body + ex->dyn[DT_INIT_ARRAY] - ex->v_addr);

  printf("init_array sz: %d\n", sz);

   sz /= sizeof(void*);

  for(int i=0; i<sz; ++i)
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
