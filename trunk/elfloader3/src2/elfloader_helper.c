

#include "conf_loader.h"
#include "loader3\loader.h"
#include "loader3\env.h"
#include "loader3\Mutex.h"


//#define __ELFTHREAD
#define ELF_PROC_RUNER_ID 0x4409
const int elf_run_prio = 0x2;
int lock_thread = 0;
extern unsigned int load_in_suproc;
extern unsigned int run_elf_in_thread;
extern char IMAGE_FOLDER[];
extern char DAEMONS_FOLDER[];
extern char SWIBLIB_WAY[];
//Mutex mutex;


__arm void __run_proc(void *entry, char *filename, void *param1, void *param2, void *param3);
#ifndef ARM
__arm void zeromem_a(void *d, int l){zeromem(d,l);}
__arm void l_msg(int a, int b) {ShowMSG(a, b);}
#endif


#ifdef __ELFTHREAD
struct __param
{
  void *p1, *p2, *p3, *p4;
};
#endif



/* Загрузка эльфа */

__arm int elf_load(char *filename, void *param1, void *param2, void *param3){

  //_f = open("0:\\Misc\\elfloader.log", A_WriteOnly | A_Append | A_Create | A_BIN, P_WRITE, 0);
  
  Elf32_Exec *ex = elfopen(filename);
  if(!ex){
    ShowMSG(1, (int)"Cant open elf");
    return -1;
  }
  
  int (*entry)(char *, void *, void*, void*) = (int (*)(char *, void *, void*, void*))elf_entry(ex);
  if(!entry){
   l_msg(1, (int)"Cant found entry");
   elfclose(ex);
   return -2;
  }
  
  
  extern __arm void ExecuteIMB(void);
  ExecuteIMB();
  
  //run_INIT_Array(ex);
#ifdef __ELFTHREAD
  __run_proc((void*)entry, filename, param1, param2, param3);
#else
  entry(filename, param1, param2, param3);
#endif
  
  if(!ex->__is_ex_import && !ex->libs)
  {
    ex->body = 0;
    elfclose(ex);
  }
  
  //close(_f,0);
  return 0;
}


__arm long elfload(char *filename, void *param1, void *p2, void *p3){

  return elf_load(filename, param1, p2, p3);
}

#ifdef __ELFTHREAD
__arm void elf_proc_func()
{
    GBS_MSG msg;
    if (GBS_RecActDstMessage(&msg))
    {
        if (msg.msg==1)
        {
            if (msg.data0)
            {
                ExecuteIMB();
                struct __param * p = (struct __param*)msg.data1;
                ((void (*)(void *, void*, void*, void*))(msg.data0))(p->p1, p->p2, p->p3, p->p4);
                mfree(p);
                //mutex_unlock(&mutex);
                //mutex_destroy(&mutex);
                lock_thread = 0;
            }
        }
    }
}


__arm void __run_proc(void *entry, char *filename, void *param1, void *param2, void *param3)
{
  lock_thread = 1;
  //mutex_init(&mutex);
  struct __param *p = malloc(sizeof(struct __param));
  p->p1 = filename;
  p->p2 = param1;
  p->p3 = param2;
  p->p4 = param3;
  GBS_SendMessage(ELF_PROC_RUNER_ID, 1, 0, entry, (void*)p);
  //mutex_lock(&mutex);
  while(lock_thread)
  {
    if(!lock_thread) break;
    NU_Sleep(5);
  }
}
#endif


__arm void InitLoaderSystem()
{
  setenv("LD_LIBRARY_PATH", "0:\\ZBin\\lib\\;4:\\ZBin\\lib\\;", 1);

#ifdef __ELFTHREAD
  static const char elf_p_name[]="ELF_PROC";
  extern const int elf_run_prio;
  CreateGBSproc(ELF_PROC_RUNER_ID, elf_p_name, elf_proc_func, elf_run_prio, 0);
#endif
  
}


int main()
{
  return 0;
}

#ifndef wintel
int elfloader_onload(WSHDR *filename, WSHDR *ext, void *param){
  char fn[128];
  ws_2str(filename,fn,126);
  if (elfload(fn, param, 0, 0)) return 0; else return 1;
}

//=======================================================================
//
//=======================================================================
extern void(*OldOnClose)(void *);
extern void(*OldOnCreate)(void *);
#ifdef NEWSGOLD
extern void(*OldShowMsg)(int, int);
#else
extern void(*OldTxtOpen)(WSHDR*, WSHDR*);
#endif

extern unsigned int DEFAULT_DISK_N;

//-----------------------------------------------------------------------

#ifdef NEWSGOLD
//#define HELPER_CEPID 0x4339
#define HELPER_CEPID 0x440A
#else
//#define HELPER_CEPID 0x4331
#define HELPER_CEPID 0x4407

#endif
#define MSG_HELPER_RUN 0x0001

__arm void proc_HELPER(void)
{
  GBS_MSG msg;
  if (GBS_RecActDstMessage(&msg))
  {
    if (msg.msg==MSG_HELPER_RUN)
    {
      if (msg.data0)
      {
	((void (*)(int, void *))(msg.data0))(msg.submess,msg.data1);
      }
    }
    else
    {
      GBS_SendMessage(MMI_CEPID,MSG_HELPER_TRANSLATOR,msg.pid_from,msg.msg,msg.submess);
    }
  }
}

__arm void CreateHELPER_PROC(void)
{
  static const char name[]="HELPER";
  CreateGBSproc(HELPER_CEPID, name, proc_HELPER, 0x80, 0);
}

__arm void REDRAW_impl(void)
{
  LockSched();
#ifdef NEWSGOLD
  PendedRedrawGUI();
#else
  PendedRedrawGUI();
  GBS_SendMessage(MMI_CEPID,0x90);
#endif
  UnlockSched();
}

__arm void SUBPROC_impl(void *f, int p2, void *p1)
{
  GBS_SendMessage(HELPER_CEPID,MSG_HELPER_RUN,p2,f,p1);
}

__thumb void SEQKILLER_impl(void *data, void(*next_in_seq)(void *), void *data_to_kill)
{
  next_in_seq(data);
  mfree(data_to_kill);
}

__arm void MyIDLECSMonClose(void *data)
{
  extern BXR1(void *, void (*)(void *));
  KillGBSproc(HELPER_CEPID);
#ifdef __ELFTHREAD
  KillGBSproc(ELF_PROC_RUNER_ID);
#endif
  clearenv();
  BXR1(data,OldOnClose);
  //  OldOnClose(data);
  //  asm("NOP\n");
}

__arm void LoadDaemons(void)
{

  DIR_ENTRY de;
  unsigned int err;
  unsigned int pathlen;
  char name[256];
  strcpy(name, DAEMONS_FOLDER);
  //name[0]=DEFAULT_DISK_N+'0';
  pathlen=strlen(name);
  strcat(name,"*.elf");
  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      //strcpy(name,path);
      name[pathlen]=0;
      strcat(name,de.file_name);
      elfload(name,0,0,0);
    }
    while(FindNextFile(&de,&err));
  }
  FindClose(&de,&err);
}

__no_init void *(*pLIB_TOP)[];
extern void *Library[];

int get_file_size(const char * fname)
{
  FSTATS fs;
  unsigned int err;
  if (GetFileStats(fname,&fs,&err)==-1) return (-1);
  else return (fs.size);
}

__arm void LoadLibrary(void)
{
  void *(*lt)[]=pLIB_TOP;
#define LIB_EMPTY ((void *)-1L)
  unsigned int ul;
  int sz;
  int f;
  char fn[64];
  strcpy(fn, SWIBLIB_WAY);
  //fn[0]=DEFAULT_DISK_N+'0';
  if (lt)
  {
    pLIB_TOP=NULL;
    mfree(lt);
    lt=NULL;
  }
  if ((sz=get_file_size(fn))==-1) return;
  if (sz!=16384)
  {
    ShowMSG(1,(int)"Illegal library size!");
    return;
  }
  f=fopen(fn,A_ReadOnly+A_BIN, P_READ, &ul);
  if (f==-1) return;
  lt=malloc(16384);
  if (fread(f,lt,sz,&ul)!=sz)
  {
    fclose(f,&ul);
    ShowMSG(1,(int)"Can't read library!");
  LERR:
    mfree(lt);
    return;
  }
  fclose(f,&ul);
  f=0;
  do
  {
    if (((*lt)[f]!=LIB_EMPTY)&&(Library[f]!=LIB_EMPTY))
    {
      if ((*lt)[f]!=Library[f])
      {
	char s[50];
	sprintf(s,"Function %d conflict!",f);
	ShowMSG(1,(int)s);
	goto LERR;
      }
    }
    if ((*lt)[f]==LIB_EMPTY)
    {
      (*lt)[f]=Library[f];
    }
    f++;
  }
  while(f<4096);
  pLIB_TOP=lt;
#undef LIB_EMPTY
}
extern void InitPngBitMap(void);

__no_init char smallicons_str[32];
__no_init char bigicons_str[32];


__arm void MyIDLECSMonCreate(void *data)
{
  static const int smallicons[2]={(int)smallicons_str,0};
  static const int bigicons[2]={(int)bigicons_str,0};
  
#ifdef NEWSGOLD
  static const REGEXPLEXT elf_reg=
  {
    "elf",
    0x55,
    0xFF,
    8, //Каталог Misc
    MENU_FLAG2,
    smallicons,
    bigicons,
    (int)"Open",    //LGP "Открыть"
    (int)"AltOpen", //LGP "Опции"
    LGP_DOIT_PIC,
    (void *)elfloader_onload,
    0
  };
#else
  static const REGEXPLEXT elf_reg=
  {
    "elf",
    0x55,
    0xFF,
    7,
    MENU_FLAG2,
    smallicons,
    bigicons,
    (void *)elfloader_onload,
    0
  };
#endif
  CreateHELPER_PROC();
  InitConfig();
  LoadLibrary();
  InitPngBitMap();
  InitLoaderSystem();
  //strcpy(smallicons_str+1,":\\ZBin\\img\\elf_small.png");
  //strcpy(bigicons_str+1,":\\ZBin\\img\\elf_big.png");
  //smallicons_str[0]=bigicons_str[0]=DEFAULT_DISK_N+'0';
  sprintf(smallicons_str, "%self_small.png", IMAGE_FOLDER);
  sprintf(bigicons_str, "%self_big.png", IMAGE_FOLDER);
  RegExplorerExt(&elf_reg);

  /* ну а хуле, плюшки для блондинок */
  if( *RamPressedKey() != '#')
#ifndef __ELFTHREAD
  if(load_in_suproc)
    SUBPROC((void*)LoadDaemons);
  else
#endif
    LoadDaemons();
  
  extern BXR1(void *, void (*)(void *));
  BXR1(data,OldOnCreate);

  //  OldOnCreate(data);
  //  asm("NOP\n");
}

unsigned int char8to16(int c)
{
  if (c==0xA8) c=0x401;
  if (c==0xAA) c=0x404;
  if (c==0xAF) c=0x407;
  if (c==0xB8) c=0x451;
  if (c==0xBA) c=0x454;
  if (c==0xBF) c=0x457;
  if (c==0xB2) c=0x406;
  if (c==0xB3) c=0x456;
  if ((c>=0xC0)&&(c<0x100)) c+=0x350;
  return(c);
}

void ascii2ws(char *s, WSHDR *ws)
{
  int c;
  while((c=*s++))
  {
    wsAppendChar(ws,char8to16(c));
  }
}
#ifdef NEWSGOLD
__arm void ESI(WSHDR *ws, int dummy, char *s)
#else
__arm void ESI(char *s, WSHDR *ws)
#endif
{
  if (((unsigned int)s>>28)==0xA) //Только строки из области RAM/FLASH
  {
    CutWSTR(ws,0); 
    ascii2ws(s,ws);
  }
  else
  {
    wsprintf(ws,"|%d|",s);
  }
}

/*int toupper(int c)
{
if ((c>='a')&&(c<='z')) c+='A'-'a';
return(c);
}*/

//static const char extfile[]=DEFAULT_DISK ":\\ZBin\\etc\\extension.cfg";

__arm void DoUnknownFileType(WSHDR *filename)
{
  WSHDR *wsmime=AllocWS(15);
  wsprintf(wsmime,"txt");
  ExecuteFile(filename,wsmime,0);
  FreeWS(wsmime);
}

#ifdef NEWSGOLD 
__no_init int *EXT2_AREA;
#ifdef ELKA
__no_init int EXT2_CNT @ "REGEXPL_CNT";
#endif

#else
__no_init TREGEXPLEXT *EXT2_AREA;
__no_init int EXT2_CNT @ "REGEXPL_CNT";
#endif

#ifdef NEWSGOLD 
#ifdef ELKA
#else
__arm int *GET_EXT2_TABLE(void)
{
  int *p=EXT2_AREA;
  if (p)
  {
    return(p+1);
  }
  p=malloc(4);
  *p=0;
  return ((EXT2_AREA=p)+1);
}
#endif
#endif  

#ifdef NEWSGOLD 
#ifdef ELKA
__arm int *EXT2_REALLOC(void)
{
  int size;
  size=sizeof(REGEXPLEXT);
  int *p;
  int *p2;
  int n;
  LockSched();
  n=EXT2_CNT;
  p=EXT2_AREA;
  p2=malloc((n+1)*size);
  if (p) 
  {    
    memcpy(p2,p,n*size);
    mfree(p);
  }
  EXT2_CNT=n+1;
  EXT2_AREA=p2;
  p2+=(n*(size/sizeof(int)));
  UnlockSched();
  return (p2);
}
#else
__arm int *EXT2_REALLOC(void)
{
  int size;
  size=sizeof(REGEXPLEXT);
  int *p;
  int *p2;
  int n;
  LockSched();
  n=*(p=EXT2_AREA);
  p2=malloc((n+1)*size+4);
  memcpy(p2,p,n*size+4);
  *p2=n+1;
  mfree(p);
  EXT2_AREA=p2;
  p2+=(n*(size/sizeof(int)))+1;
  UnlockSched();
  return (p2);
}
#endif
#else
__arm TREGEXPLEXT *EXT2_REALLOC(void)
{
  TREGEXPLEXT *p,*p2;
  int n;
  LockSched();
  n=EXT2_CNT;
  p=EXT2_AREA;
  p2=malloc((n+1)*sizeof(TREGEXPLEXT));
  zeromem(p2,(n+1)*sizeof(TREGEXPLEXT));
  if (p) 
  {    
    memcpy(p2,p,n*sizeof(TREGEXPLEXT));
    mfree(p);
  }
  EXT2_CNT=n+1;
  EXT2_AREA=p2;
  UnlockSched();
  return (p2+n);
}


#endif



#ifdef NEWSGOLD
__thumb MyShowMSG(int p1, int p2)
{
#ifdef ELKA
  if (p2!=0x1DD1)
#else
    if (p2!=(0x1DCC+5))
#endif    
    {
      OldShowMsg(p1,p2);
      return;
    }
  asm("MOVS R0,R6\n");
  DoUnknownFileType((WSHDR *)p1);
}
#else

__arm void PropertyPatch(WSHDR *unk_foldername, WSHDR *unk_filename)
{
  WSHDR *ws;
  ws=AllocWS(255);
  wstrcpy(ws,unk_foldername);
  wsAppendChar(ws,'\\');
  wstrcat (ws,unk_filename);
  DoUnknownFileType(ws);
  FreeWS(ws);
}

#endif

__arm void FUNC_ABORT(int f)
{
  char s[32];
  extern void StoreErrInfoAndAbort(int code,const char *module_name,int type,int unk3);
  extern void StoreErrString(const char *);
  sprintf(s,"%d(%03X)",f,f);
  StoreErrString(s);
  loopback2();
  StoreErrInfoAndAbort(0xFFFF,"\1\1No function in lib\xA1",2,2);
}

//Патчи
#pragma diag_suppress=Pe177
__root static const int NEW_ONCREATE @ "PATCH_ONCREATE" = (int)MyIDLECSMonCreate;

__root static const int NEW_ONCLOSE @ "PATCH_ONCLOSE" = (int)MyIDLECSMonClose;

#ifdef NEWSGOLD
__root static const int NEW_SHOWMSG @ "PATCH_SHOWMSG_BLF" = (int)MyShowMSG;
#endif

__root static const int SWILIB_FUNC171 @ "SWILIB_FUNC171" = (int)SUBPROC_impl;

__root static const int SWILIB_FUNC172 @ "SWILIB_FUNC172" = (int)REDRAW_impl;

__root static const int SWILIB_FUNC19C @ "SWILIB_FUNC19C" = (int)SEQKILLER_impl;
#pragma diag_default=Pe177
#endif

