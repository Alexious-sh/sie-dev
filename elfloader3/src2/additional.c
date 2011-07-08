#include "loader3\loader.h"

#ifndef NEWSGOLD
extern TREGEXPLEXT *EXT2_REALLOC(void);
extern TREGEXPLEXT *EXT2_AREA;
extern int EXT2_CNT;

// В этот файл включены некоторые функции которых нет в прошивке для SGOLD

// ========================================== Reg Files =============================================
__arm void RegFile(WSHDR*ext,int unical_id,int menu_flag,unsigned int* icon1,int obex_path_id,int enabled_options,void *proc1,void *proc2,unsigned int *icon2)
{
  TREGEXPLEXT *reg;
  unsigned int *icon2new;
  reg=EXT2_REALLOC();
  LockSched();
  reg->zero=0;
  reg->icon1=icon1;
  reg->unical_id=unical_id;
  reg->obex_path_id=obex_path_id;
  reg->menu_flag=menu_flag;
  reg->enabled_options=enabled_options;
  reg->ext=ext;
  reg->proc=proc1;
  reg->altproc=proc2;
   
  icon2new=malloc(sizeof(int)*2);
  icon2new[0]=*icon1>>28==0xA?*icon2:*icon1+1;
  icon2new[1]=0;
  reg->icon2=icon2new;
  UnlockSched();
}

__arm int GetBigIcon(const unsigned int icon, int uid)
{
  asm("mov r1, r8");
  TREGEXPLEXT *reg;
  if ((icon>>28)!=0xA) return (icon+1);
  reg=get_regextpnt_by_uid(uid);
  if (!reg) return (0);
  return (*(reg->icon2));
}

__arm void UnregExplExt_impl(REGEXPLEXT const * reg_orig)
{
  char ext[16];
  TREGEXPLEXT *reg=(TREGEXPLEXT*)EXT2_AREA;
  for (int i=0;i!=EXT2_CNT;i++)
  {
    if (reg_orig->unical_id==reg[i].unical_id)
    {
      ws_2str(reg[i].ext,ext,15);
      if (!strcmp(ext,reg_orig->ext))
      {
        FreeWS(reg[i].ext);
        mfree((void*)reg[i].icon2);
        EXT2_CNT--;
        memcpy(&reg[i],&reg[i+1],sizeof(TREGEXPLEXT)*(EXT2_CNT-i));
        return;
      }
    }
  }
}

// ========================================= Keyb Hooks ===========================================
typedef struct
{
  void *next;
  void *prev;
  int (*proc)(int submsg,int msg);
  char is_first;
}PLIST;



__no_init LLQ _plistq;

#pragma optimize=no_inline
static void LLaddToEnd(LLQ *ll, void *data)
{
  LLIST *d=data;
  d->next=NULL;
  d->prev=ll->last;
  if (ll->last)
  {
    ((LLIST *)ll->last)->next=d;
  }
  else
  {
    ll->first=d;
  }
  ll->last=d;
}

#pragma optimize=no_inline
static void LLaddToBegin(LLQ *ll, void *data)
{
  LLIST *d=data;
  d->next=ll->first;
  d->prev=0;
  if (!ll->last)
  {
    ll->last=d;
  }
  else
  {
    ((LLIST *)ll->first)->prev=d;
  }
  ll->first=d;
}

#pragma optimize=no_inline
static void LLremoveFromQ(LLQ *ll, void *data, int is_free)
{
  LLIST *d=data;
  if (d->prev)
  {
    ((LLIST *)d->prev)->next=d->next;
  }
  else
  {
    ll->first=d->next;
  }
  if (d->next)
  {
    ((LLIST *)d->next)->prev=d->prev;
  }
  else
  {
    ll->last=d->prev;
  }
  if (is_free)
  {
    ll->data_mfree(d);
  }
}

__arm void AddKeybMsgHook_impl(int (*proc)(int submsg,int msg))
{
  LLQ *plistq=&_plistq;
  PLIST *newp;
  if (!plistq->data_mfree)
  {
    plistq->first=0;
    plistq->last=0;
    plistq->data_mfree=(void(*)(void *))mfree_adr();
  }
  newp=malloc(sizeof(PLIST));
  if (newp)
  {
    newp->proc=proc;
    newp->is_first=0;
    LLaddToEnd(plistq,newp);
  }
}


__arm int AddKeybMsgHook_end_impl(int (*proc)(int submsg,int msg))
{
  LLQ *plistq=&_plistq;
  PLIST *newp, *pfirst;
  if (!plistq->data_mfree)
  {
    plistq->first=0;
    plistq->last=0;
    plistq->data_mfree=(void(*)(void *))mfree_adr();
  }
  if ((pfirst=plistq->first))
  {
    if (pfirst->is_first==1) return 0;
  }
  newp=malloc(sizeof(PLIST));
  if (!newp) return 0;
  newp->proc=proc;
  newp->is_first=1;
  LLaddToBegin(plistq,newp);
  return 1;
}  

extern BXR1(void *, void (*)(void *));


__arm void RemoveKeybMsgHook_impl(int (*proc)(int submsg,int msg))
{
  LLQ *plistq=&_plistq;
  PLIST *plist=plistq->first;
  while(plist)
  {
    if (plist->proc==proc)
    {
      LLremoveFromQ(plistq,plist,1);
      return;      
    }  
    plist=plist->next;    
  }
}


int PatchKeybMsg(int submsg, int msg)
{
  LLQ *plistq=&_plistq;
  PLIST *plist=plistq->first;
  int proc_ret;
  int is_no_gui=0;
  while(plist)
  {
    if (plist->proc)
    {
      proc_ret=plist->proc(submsg,msg);
      switch(proc_ret)
      {
      case KEYHOOK_NEXT:
        break;
      case KEYHOOK_NEXT_NOT2GUI:
        is_no_gui=1;
        break;
      case KEYHOOK_BREAK:
        return (2);
      case KEYHOOK_ENDQ:
        return is_no_gui==1?2:0;
      default:
        break;
      }      
    }
    plist=plist->next;
  }
  return is_no_gui;
}

// ========================================= fread/fwrite ===========================================
extern int (*FReadOld)(int FileHandler, void *cBuffer, int iByteCount, unsigned int *ErrorNumber);
extern unsigned int(*FWriteOld)(int FileHandler, char * cBuffer, int iByteCount, unsigned int *ErrorNumber);

int fread32(int fh, void *buf, int len, unsigned int *err)
{
  int clen;
  int rlen;
  int total=0;
  while(len)
  {
    clen=len>=0x4000?0x4000:len;
    total+=(rlen=FReadOld(fh, buf, clen, err));
    if (rlen!=clen) break;
    buf=(char *)buf+rlen;
    len-=clen;
  }
  return(total);
}


unsigned int fwrite32(int fh, void *buf, int len, unsigned int *err)
{
  int clen;
  unsigned int wlen;
  int total=0;
  while(len)
  {
    clen=len>=0x4000?0x4000:len;
    total+=(wlen=FWriteOld(fh, buf, clen, err));
    if (wlen!=clen) break;
    buf=(char *)buf+wlen;
    len-=clen;
  }
  return(total);
}



#pragma diag_suppress=Pe177
__root static const int SWILIB_FUNC00B @ "SWILIB_FUNC00B" = (int)fread32;
__root static const int SWILIB_FUNC00C @ "SWILIB_FUNC00C" = (int)fwrite32;
__root static const int SWILIB_FUNC095 @ "SWILIB_FUNC095" = (int)UnregExplExt_impl;
__root static const int SWILIB_FUNC12B @ "SWILIB_FUNC12B_12D" = (int)AddKeybMsgHook_impl;
__root static const int SWILIB_FUNC12C @ "SWILIB_FUNC12B_12D" = (int)AddKeybMsgHook_end_impl;
__root static const int SWILIB_FUNC12D @ "SWILIB_FUNC12B_12D" = (int)RemoveKeybMsgHook_impl;
#pragma diag_default=Pe177



#endif

