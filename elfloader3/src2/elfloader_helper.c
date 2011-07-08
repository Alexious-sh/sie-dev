

#include "conf_loader.h"
#include "loader3\loader.h"
#include "loader3\env.h"

#define MAX_PHNUM 10
/*
extern int vsprintf(char *, const char *, va_list);

int _f = -1;
char _tmp[128]={0};
__arm int _printf(const char *fmt)
{
  if(_f==-1) return 0;
  va_list va;
  va_start(va, fmt);
  int sz = vsprintf(_tmp, fmt, va);
  va_end(va);
  write(_f, _tmp, sz, 0);
  return 0;
}*/

typedef long TElfEntry(char *, void *);


#ifndef ARM
__arm void zeromem_a(void *d, int l){zeromem(d,l);}
__arm void l_msg(int a, int b) {ShowMSG(a, b);}
#endif


__arm int elf_load(char *filename, void *param1){

  //_f = open("0:\\Misc\\elfloader.log", A_WriteOnly | A_Append | A_Create | A_BIN, P_WRITE, 0);
  
  Elf32_Exec *ex = elfopen(filename);
  if(!ex){
    ShowMSG(1, (int)"Cant open elf");
    return -1;
  }
  
  int (*entry)(char *, void *) = (int (*)(char *, void *))elf_entry(ex);
  if(!entry){
   l_msg(1, (int)"Cant found entry");
   return -2;
  }
  
  
  extern __arm void ExecuteIMB(void);
  ExecuteIMB();
  
  run_INIT_Array(ex);
  entry(filename, param1);
  
  if(!ex->__is_ex_import && !ex->libs)
  {
    ex->body = 0;
    elfclose(ex);
  }
  
  //close(_f,0);
  return 0;
}


long elfload(char *filename, void *param1){

  return elf_load(filename, param1);
}


/*
long elfload(char *filename, void *param1)
{
  Elf32_Ehdr ehdr;				                        //çàãîëîâîê åëüôà
  Elf32_Phdr phdrs[MAX_PHNUM];	                                        //çàãîëîâêè ïðîãðàì
  Elf32_Word dyn[DT_BINDNOW+1];	                                        //òýãè äèíàìè÷åñêîé ñåêöèè
  char *reloc, *base;
  unsigned long minadr=(unsigned long)-1, maxadr=0;//, maxadrsize;
  int n,m;
  
  
  zeromem_a(dyn, sizeof(dyn));
  
  /////////////////////////////////////////
  //WINTEL
#ifdef wintel
  FILE *fin=NULL;
  if ((fin=open(filename,"rb"))==NULL) return -1;			//íå îòêðûâàåòñÿ åëüô
  if (read(&ehdr,sizeof(Elf32_Ehdr),1,fin)!=1) return -2;	        //íå ÷èòàåòñÿ åëüô
#endif
  
  //ARM
#ifndef wintel
  int fin;
  unsigned int iError, iError2;
  if ((fin=open(filename, A_ReadOnly+A_BIN, P_READ, &iError))<0) return -1;	//íå îòêðûâàåòñÿ åëüô
  if (read(fin, &ehdr, sizeof(Elf32_Ehdr), &iError)!=sizeof(Elf32_Ehdr))	//íå ÷èòàåòñÿ åëüô
  {close(fin, &iError); return -2;}
#endif
  /////////////////////////////////////////
  
  if (*((long *)ehdr.e_ident)!=0x464C457F){                               //äà è íå åëüô ýòî âîâñå
#ifndef wintel
    close(fin, &iError);
#endif
    return -3;
  }
  
#ifdef wintel
  cout << "Elf header"<<endl;
  cout << "ehdr.e_entry:"<<ehdr.e_entry<<endl;
  cout << "ehdr.e_phoff:"<<ehdr.e_phoff<<endl;
#endif
  
  //ïðî÷èòàåì âñå ïðîãðàìíûå ñåãìåíòû è âû÷èñëèì íåîáõîäèìóþ îáëàñòü â ðàìå
  if (ehdr.e_phnum>MAX_PHNUM) return -9;					//ñëèøêîì ìíîãî ïðîãðàìíûõ ñåãìåíòîâ
  for(n=0;n<ehdr.e_phnum;n++){
    ////////////////////////////////////////////////////
    //WINTEL
#ifdef wintel
    if (fseek(fin,ehdr.e_phoff+n*ehdr.e_phentsize,SEEK_SET)!=0) return -4;	//íå ñèêàåòñÿ ïðîãðàìíûé çàãîëîâîê
    if (read(&(phdrs[n]),sizeof(Elf32_Phdr),1,fin)!=1) return -5;		//íå ÷èòàåòñÿ ïðîãðàìíûé çàãîëîâîê
#endif
    
    //ARM
#ifndef wintel
    if (lseek(fin, ehdr.e_phoff+n*ehdr.e_phentsize, S_SET, &iError, &iError2)!=ehdr.e_phoff+n*ehdr.e_phentsize)
    {close(fin, &iError); return -4;}				//íå ñèêàåòñÿ ïðîãðàìíûé çàãîëîâîê
    if (read(fin, &phdrs[n], sizeof(Elf32_Phdr), &iError)!=sizeof(Elf32_Phdr))
    {close(fin, &iError); return -5;}				//íå ÷èòàåòñÿ ïðîãðàìíûé çàãîëîâîê
#endif
    /////////////////////////////////////////////////////
    if (phdrs[n].p_type==PT_LOAD) {
      if (minadr>phdrs[n].p_vaddr) minadr=phdrs[n].p_vaddr;
      if (maxadr<(phdrs[n].p_vaddr+phdrs[n].p_memsz))
      {
	maxadr=phdrs[n].p_vaddr+phdrs[n].p_memsz;
      }
    }
#ifdef wintel
    cout << "minadr:"<<hex<<minadr<<endl;
    cout << "maxadr:"<<hex<<maxadr<<endl;
    cout << "Program header"<<endl;
    cout << "phdr.p_type:"<<phdrs[n].p_type<<endl;
    cout << "phdr.p_offset:"<<phdrs[n].p_offset<<endl;
    cout << "phdr.p_vaddr:"<<phdrs[n].p_vaddr<<endl;
    cout << "phdr.p_paddr:"<<phdrs[n].p_paddr<<endl;
    cout << "phdr.p_filesz:"<<phdrs[n].p_filesz<<endl;
    cout << "phdr.p_memsz:"<<phdrs[n].p_memsz<<endl;
#endif
  }
  
  //âûäåëèì ýòó îáëàñòü è î÷èñòèì åå
  if ((base=(char *)malloc(maxadr-minadr))==0){		//íå âûäåëÿåòüñÿ ïàìÿòü ïîä åëüô
#ifndef wintel
    close(fin, &iError);
#endif
    return -14;
  }
  //  t_zeromem(base,maxadr-minadr);
  zeromem_a(base,maxadr-minadr);
  for(n=0;n<ehdr.e_phnum;n++){ //  îáõîä âñåõ ñåãìåíòîâ
    ////////////////////////////////////////////////////////////////////
    //WINTEL
#ifdef wintel
    if (fseek(fin,phdrs[n].p_offset,SEEK_SET)!=0) return -6;	//íå ñèêàåòñÿ äèíàìè÷åñêèé ñåãìåíò
#endif
    
    //ARM
#ifndef wintel
    if (lseek(fin, phdrs[n].p_offset, S_SET, &iError, &iError)!=phdrs[n].p_offset)
    {close(fin, &iError); mfree(base); return -6;}		//íå ñèêàåòñÿ äèíàìè÷åñêèé ñåãìåíò
#endif
    /////////////////////////////////////////////////////////////////////
    switch (phdrs[n].p_type){
    case PT_LOAD:
      //çàãðóçèì ïðîãðàìíûå ñåãìåíòû ñ ðàçìåðîì áîëüøå 0
      if (phdrs[n].p_filesz!=0) {
	/////////////////////////////////////////////////////////////////////
	//WINTEL
#ifdef wintel
	if (read((void *)&base[phdrs[n].p_vaddr-minadr],phdrs[n].p_filesz,1,fin)!=1) return -11;	//íå ÷èòàåòñÿ ïðîãðàìíûé ñåãìåíò
#endif
	
	//ARM
#ifndef wintel
	if (read(fin, &base[phdrs[n].p_vaddr-minadr], phdrs[n].p_filesz, &iError)!= phdrs[n].p_filesz)
	{close(fin, &iError); mfree(base); return -11;}//íå ÷èòàåòñÿ ïðîãðàìíûé ñåãìåíò
#endif
	///////////////////////////////////////////////////////////////////////
      }
      break;
    case PT_DYNAMIC:
      //ïðî÷òåì äèíàìè÷åñêóþ ñåêöèþ
      if ((reloc=(char *)malloc(phdrs[n].p_filesz))==0) {//íå âûäåëÿåòñÿ ðàìà ïîä äèíàìè÷åñêèé ñåãìåíò
#ifndef wintel
	close(fin, &iError);
#endif
	mfree(base);
	return -7;
      }
      ///////////////////////////////////////////////////////////////////////
      //WINTEL
#ifdef wintel
      cout << "dyn seg: off="<<hex<<phdrs[n].p_offset<<", sz="<<phdrs[n].p_filesz<<endl;
      if (read(reloc,phdrs[n].p_filesz,1,fin)!=1) {mfree(reloc); return -8;} //íå ÷èòàåòñÿ äèíàìè÷åñêèé ñåãìåíò
#endif
      
      //ARM
#ifndef wintel
      if (read(fin, reloc, phdrs[n].p_filesz, &iError)!=phdrs[n].p_filesz)
      {close(fin, &iError); mfree(reloc); mfree (base); return -8;}	//íå ÷èòàåòñÿ äèíàìè÷åñêèé ñåãìåíò
#endif
      ////////////////////////////////////////////////////////////////////////
      //				memset(dyn,0, sizeof(dyn));
      //âûòàùèì âñå òýãè èç äèíàìè÷åñêîé ñåêöèè
      m=0;
      while (((Elf32_Dyn *)reloc)[m].d_tag!=DT_NULL){
	if (((Elf32_Dyn *)reloc)[m].d_tag<=DT_BINDNOW) {
#ifdef wintel
          cout<<"d_tag="<<((Elf32_Dyn *)reloc)[m].d_tag;
          cout<<" d_val="<<((Elf32_Dyn *)reloc)[m].d_val<<endl;
#endif
	  dyn[((Elf32_Dyn *)reloc)[m].d_tag]=((Elf32_Dyn *)reloc)[m].d_un.d_val;
	}
	m++;
      }
#ifdef wintel
      cout << "Dynamic section" << endl;
      for (m = 0; m <= DT_BIND_NOW; m++) {
	cout << dec << m <<" = "<< hex << dyn[m]<<endl;
      }
      cout<<"dyn[DT_REL]="<<dyn[DT_REL]<<", dyn[DT_RELA]="<<dyn[DT_RELA]<<endl;
      cout<<"dyn[DT_RELSZ]="<<dyn[DT_RELSZ]<<", dyn[DT_RELASZ]="<<dyn[DT_RELASZ]<<endl;
#endif
      
      m=0;
      //âûïîëíèì ðåëîêàöèþ REL
      if (dyn[DT_RELSZ]!=0) {
	while (m*sizeof(Elf32_Rel)<dyn[DT_RELSZ]){
#ifdef wintel
	  cout<<"rel: of="<<hex<<((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_offset
	    <<" , sym_idx="<<ELF32_R_SYM(((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_info)
              <<" , rel_type="<<dec<<(int) ELF32_R_TYPE(((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_info)<<endl;
#endif
          Elf32_Word ri=ELF32_R_TYPE(((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_info);
          if (ri!=R_ARM_RBASE)
          {
            if (ri==R_ARM_RABS32)
            {
              *((long*)(base+((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_offset))+=(long)base-minadr;
            }
            else
              switch(ri){
                
              case R_ARM_NONE: break; // ïóñòîé ðåëîêåéøåí
              
              case R_ARM_ABS32:
#ifdef wintel
                cout << "base="<<hex<<(long)base<< endl;
                cout << "of="<<hex<<((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_offset-minadr<<endl;
#endif
                *((long*)(base+((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_offset-minadr))+=(long)base;
                break;
                
              case R_ARM_RELATIVE: // âîîáùå ãîâîðÿ íå minadr à íà÷àëî ñåãìåíòà ñîäåðæàùåãî ñèìâîë
                *((long*)(base+((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_offset-minadr))+=(long)base-minadr;
                break; // ignore
                
                //	  case R_ARM_RABS32:
                //	    *((long*)(base+((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_offset))+=(long)base-minadr;
                //            break;
                
                //              case R_ARM_RBASE: break;
              default: 	//íåèçâåñòíûé òèï ðåëîêàöèè
#ifdef wintel
                cout << "Invalid reloc type: " <<dec<<(unsigned)ELF32_R_TYPE(((Elf32_Rel *)(reloc+dyn[DT_REL]-phdrs[n].p_vaddr))[m].r_info) << endl;
#else
                close(fin, &iError);
#endif
                mfree(base);
                mfree(reloc);
                return -13;
              }
          }
	  m++;
	}
      }
#ifdef wintel
      else
      {
        cout << "No relocation information dyn[DT_RELSZ]=0" << endl;
      }
#endif
      mfree(reloc);
      break;
    default:	//íåèçâåñòíûé òèï ïðîãðàìíîãî ñåãìåíòà
#ifndef wintel
      close(fin, &iError);
#endif
      mfree(base);
      return -12;
    }
  }

  close(fin, &iError);
  {
    extern __arm void ExecuteIMB(void);
    ExecuteIMB();	
  }
  ((TElfEntry *)(base+ehdr.e_entry-minadr))(filename,param1);
  //	mfree(base);
  return 0;
}
*/

__arm void InitLoaderSystem()
{
}


int main()
{
  return 0;
}

#ifndef wintel
int elfloader_onload(WSHDR *filename, WSHDR *ext, void *param){
  char fn[128];
  ws_2str(filename,fn,126);
  if (elfload(fn,param)) return 0; else return 1;
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
  strcpy(name+1,":\\ZBin\\Daemons\\");
  name[0]=DEFAULT_DISK_N+'0';
  pathlen=strlen(name);
  strcat(name,"*.elf");
  if (FindFirstFile(&de,name,&err))
  {
    do
    {
      //strcpy(name,path);
      name[pathlen]=0;
      strcat(name,de.file_name);
      elfload(name,0);
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
  strcpy(fn+1,":\\Zbin\\swi.blib");
  fn[0]=DEFAULT_DISK_N+'0';
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
    8, //������� Misc
    MENU_FLAG2,
    smallicons,
    bigicons,
    (int)"Open",    //LGP "�������"
    (int)"AltOpen", //LGP "�����"
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
  strcpy(smallicons_str+1,":\\ZBin\\img\\elf_small.png");
  strcpy(bigicons_str+1,":\\ZBin\\img\\elf_big.png");
  smallicons_str[0]=bigicons_str[0]=DEFAULT_DISK_N+'0';
  RegExplorerExt(&elf_reg);
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
  if (((unsigned int)s>>28)==0xA) //������ ������ �� ������� RAM/FLASH
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

//�����
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

