
/*
 * Этот файл является частью программы ElfLoader
 * Copyright (C) 2011 by Z.Vova, Ganster
 * Licence: GPLv3
 */

#include "loader.h"
#include "env.h"

extern unsigned int realtime_libclean;
#ifndef _test_linux
extern int __e_div(int delitelb, int delimoe);
#endif
char tmp[258] = {0}, dlerr[128]={0};


Global_Queue* lib_top = 0;
Elf32_Lib** handles = 0;
int handles_cnt = 0;

__arch char __is_file_exist(const char *fl)
{
    int f;
    if( (f=fopen(fl, A_ReadOnly | A_BIN, P_READ,0)) != -1)
    {
        fclose(f,0);
        return 1;
    }
    return 0;
}


__arch unsigned long elfhash(const char* name)
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


__arch Elf32_Word findExport (Elf32_Exec* ex, const char* name)
{
    if(!ex || !ex->hashtab) return 0;

    long hash = elfhash(name);

    Elf32_Word nbucket = ex->hashtab[0];
    //Elf32_Word nchain = ex->hashtab[1];
    Elf32_Word* bucket = &ex->hashtab[2];
    Elf32_Word* chain = &ex->hashtab[2 + nbucket];
    Elf32_Word func = 0;
    int symidx;

    for (symidx = bucket[__e_div(nbucket, hash)]; symidx != STN_UNDEF; symidx = chain[symidx])
    {
        Elf32_Sym sym = ex->symtab[symidx];

        if (sym.st_value == 0 || sym.st_shndx == SHN_UNDEF)
            continue;

        switch (ELF_ST_TYPE(sym.st_info))
        {
        case STT_NOTYPE:
        case STT_FUNC:
        case STT_OBJECT:
            break;
        default:
            /* Not a code/data definition. */
            continue;
        }

        if (strcmp (ex->strtab + sym.st_name, name))
            /* Not the symbol we are looking for. */
            continue;

        switch (ELF_ST_BIND(sym.st_info))
        {
            case STB_GLOBAL:
                /* Global definition.  Just what we need. */
                return (Elf32_Word)ex->body + sym.st_value;
            case STB_WEAK:
                /* Weak definition.  Use this value if we don't find another. */
                func = (Elf32_Word)ex->body + sym.st_value;
                break;
        default:
            /* Local symbols are ignored.  */
            break;
        }
    }

    return func;
}


__arch Elf32_Word FindFunction(Elf32_Lib* lib, const char *name)
{
    if(!lib) return 0;
    return findExport(lib->ex, name);
}


__arch char * envparse(const char *str, char *buf, int num)
{
  if( !str || !buf || num < 0) return 0;
  const char *start = str;
  const char *s = strchr(str, ';');
  
  while(num-- && start)
  {
    start = s? s+1:0;
    if(start)
      s = strchr(start, ';');
    else s = 0;
  }
  
  if(!s){
    switch( start ? 1:0 )
    {
      case 0:
	return 0;
      case 1:
	s = str + strlen(str);
    }
  }

  memcpy(buf, start, s-start);
  buf[s-start] = 0;
  return buf;
}


__arch const char * findShared(const char *name)
{
    const char *env = getenv("LD_LIBRARY_PATH");
   
    for(int i=0;; ++i)
    {
        if( !envparse(env, tmp, i) ) return 0;
        strcat(tmp, name);
        if( __is_file_exist(tmp) )
        {
            return tmp;
        }
    }
    
    /* этого никогда не будет */
    //return 0;
}


// Открыть библиотеку
__arch Elf32_Lib* OpenLib(const char *name, Elf32_Exec *_ex)
{
    if(!name || !*name) return 0;
    printf("Starting loading shared library '%s'...\n", name);
    int fp, _size = 0;
    Elf32_Ehdr ehdr;
    Elf32_Exec* ex;

    // Поищем среди уже загруженых
    Global_Queue* ready_libs = lib_top;
    
    const char *cmp_share_name = strrchr(name, '\\');
    if(!cmp_share_name) cmp_share_name = name;
    else cmp_share_name++;
    while(ready_libs)
    {
        Elf32_Lib* lib = ready_libs->lib;

        if(!strcmp (lib->soname, cmp_share_name))
        {
            printf(" '%s' is olready loaded\n", cmp_share_name);
            lib->users_cnt++;
	    memset(dlerr, 0, 2);
            return lib;
        }
        ready_libs = ready_libs->prev;
    }

    
    const char  *ld_path;
    
    /* путь у нас реальный */
    if(name[1] == ':')
      ld_path = name;
    else
      ld_path = findShared(name);
    
    if(!ld_path) return 0;
    
try_again:

    /* Открываем */
    if((fp = fopen(ld_path, A_ReadOnly+A_BIN,P_READ, &ferr)) == -1) {
      strcpy(dlerr, NO_FILEORDIR);
      return 0;
    }
    /* Читаем хедер */
    if( (_size = fread(fp, &ehdr, sizeof(Elf32_Ehdr), &ferr)) <= 0) {
      strcpy(dlerr, BADFILE);
      return 0;
    }
    
    /* Проверяем шо это вообще такое */
    if( _size < sizeof(Elf32_Ehdr) || CheckElf(&ehdr) ) // не эльф? о_О мб симлинк?!
    {
      int ns = lseek(fp, 0, S_END, &ferr, &ferr); // если длина файл больше 256 байт то нахрен такой путь...
      if(ns < 256 && ns > 0)
      {
        lseek(fp, 0, S_SET, &ferr, &ferr);
        if(fread(fp, tmp, ns, &ferr) != ns){
          fclose(fp, &ferr);
          return 0;
        }
        tmp[ns] = 0;
        ld_path = tmp;
        fclose(fp, &ferr);
        goto try_again;
      }
      strcpy(dlerr, BADFILE);
      fclose(fp, &ferr);
      return 0;
    }
    
    /* Выделим память под структуру эльфа */
    if( !(ex = malloc(sizeof(Elf32_Exec))) ) {
      strcpy(dlerr, OUTOFMEM);
      return 0;
    }
    
    memcpy(&ex->ehdr, &ehdr, sizeof(Elf32_Ehdr));
    ex->v_addr = (unsigned int)-1;
    ex->fp = fp;
    ex->type = EXEC_LIB;
    ex->libs = 0;
    ex->complete = 0;
    ex->meloaded = (void*)_ex;
    ex->switab = (int*)AddrLibrary();

    /* Начинаем копать структуру либы */
    if( LoadSections(ex) ){
        strcpy(dlerr, BADFILE);
        fclose(fp, &ferr);
        elfclose(ex);
        return 0;
    }

    /* Он уже не нужен */
    fclose(fp, &ferr);

    /* Глобальная база либ */
    Elf32_Lib* lib;
    if( !(lib = malloc(sizeof(Elf32_Lib))) ){
        elfclose(ex);
        strcpy(dlerr, OUTOFMEM);
        return 0;
    }

    lib->ex = ex;
    lib->users_cnt = 1;
    
    const char *soname;
    
    
    if(!ex->dyn[DT_SONAME]) // пустой блок с именем либы о_О
    {
      if(name[1]==':') // путь относительный
      {
        soname = strrchr(name, '\\'); // отчекрыжим путь, берём имя
        if(!soname) // шо за бляин путь такой?! 
        {
          soname = name; // лан, пох ...
        }else
          ++soname;
      }else // путь не относительный
      {
        soname = name;
      }
    }else // все норм, имя либы есть
    {
      soname = ex->strtab + ex->dyn[DT_SONAME];
    }
    
    strcpy(lib->soname, soname?soname : "Error Lib!");

    /*  Ведь капуста^W память всем нужна)) */
    Global_Queue* global_ptr = malloc(sizeof(Global_Queue));
    if(!global_ptr)    // ?????...?? ??? :'(
    {
        strcpy(dlerr, OUTOFMEM);
        CloseLib(lib, 0);
        return 0;
    }

    /* Ну тут заполняем */
    global_ptr->lib = lib;
    global_ptr->next = 0;
    lib->glob_queue = global_ptr;

    if(lib_top)
    {
        lib_top->next = global_ptr;
        global_ptr->prev = lib_top;
    }
    else global_ptr->prev = 0;
    
    lib_top = global_ptr;

    /* запустим контсрукторы */
    //run_INIT_Array(ex);
    ex->complete = 1;

    /* запустим функциюю инициализации либы, если таковая имеется */
    if(ex->dyn[DT_INIT])
    {
        printf("init function found\n");
#ifndef _test_linux
        (( void (*)(const char*) )(ex->body + ex->dyn[DT_INIT] - ex->v_addr))(name);
#endif
    }

    printf(" '%s' Loade complete\n", name);
    dlerr[0] = 0;
    dlerr[1] = 0;
    return lib;
}


__arch void sub_clients(Elf32_Lib* lib)
{
  lib->users_cnt--;
}

__arch int CloseLib(Elf32_Lib* lib, int immediate)
{
    if(!lib) return E_EMPTY;

    if(lib->users_cnt < 1) // нету больше юзеров либы :(
    {
        if(!realtime_libclean && !immediate) goto end;
        
        Elf32_Exec* ex = lib->ex;
        if(lib->glob_queue)
        {
	    // Функция финализации
            if(ex->dyn[DT_FINI]) ((LIB_FUNC*)(ex->body + ex->dyn[DT_FINI] - ex->v_addr))();

            Global_Queue* glob_queue = lib->glob_queue;

            Global_Queue* tmp = glob_queue->next;
            
            if( glob_queue == lib_top && !lib_top->prev) lib_top = 0;
            else
            if( glob_queue == lib_top ) lib_top = glob_queue->prev;
              
            if(tmp) tmp->prev = glob_queue->prev;
            if(tmp = glob_queue->prev) tmp->next = glob_queue->next;
            mfree(glob_queue);
        }

        elfclose(ex);
        mfree(lib);
    }
end:
    return E_NO_ERROR;
}


int dlopen(const char *name)
{
  int handle = -1;
  
  if(!name) return -1;
  
  // Первый клиент! :)
  if(!handles_cnt)
  {
    handles_cnt = 256;
    handles = malloc(sizeof(Elf32_Lib*) * handles_cnt);
    
    if(!handles) return -1;
    
    zeromem_a(handles, sizeof(Elf32_Lib*) * handles_cnt);
  }
  
  // Ищем свободный слот
  for(int i=0; i<handles_cnt; ++i)
  {
    if(handles[i] == 0)
    {
      handle = i;
      break;
    }
  }
  
  // Не нашли O_o
  if(handle == -1)
  {
    Elf32_Lib** new_handles = realloc(handles, sizeof(Elf32_Lib*) * (handles_cnt + 64));
    
    // Места нет, и рама кончилась :'(
    if(!new_handles) return -1;
    
    handle = handles_cnt;
    zeromem_a(&new_handles[handles_cnt], sizeof(Elf32_Lib*) * 64);
    handles_cnt += 64;
    handles = new_handles;
  }
  
  Elf32_Lib* lib = OpenLib(name, 0);
  if(!lib) return -1;
  
  handles[handle] = lib;
  return handle;
}


int dlclose(int handle)
{
  if(0 > handle > handles_cnt - 1) return 0;
  
  if(handles[handle])
  {
    Elf32_Lib* lib = handles[handle];
    handles[handle] = 0;
    sub_clients(lib);
    // То что здесь стоит возвращать это? handle все равно же потерли...
    return CloseLib(lib, 0);
  }
  
  return 0;
}


Elf32_Word dlsym(int handle, const char *name)
{
  if(0 > handle > handles_cnt - 1) return 0;
  
  if(handles[handle]) return FindFunction(handles[handle], name);
  
  return 0;
}


__arch const char *dlerror()
{
  return dlerr;
}


__arch void *SHARED_TOP()
{
  return lib_top;
}



__arch int dlclean_cache()
{
  if(!lib_top) return -1;
  
  Elf32_Lib *bigger = 0;
  Global_Queue *tmp = lib_top, *mem = lib_top;
  int cleaned = 0;
  while(tmp)
  {
    // найдем либу которая юзает само больше либ
    bigger = tmp->lib;
    
    if( bigger->users_cnt < 1 )
    {
      // закроем её, и она закроет весб хлам который сама юзает
      CloseLib(bigger, 1); // срочняком кроим их!
      ++cleaned;
    }
    
    // либ у нас поменьшало, мб топ изменился, чекаем
    if(mem != lib_top )
    {
      tmp = lib_top;
      mem = lib_top;
    }
    else // не неизменился, идем дальше тогда
      tmp = tmp->prev;
  }
  
  return cleaned;
}






