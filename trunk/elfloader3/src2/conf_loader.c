#include <inc\swilib.h>
#include <inc\cfg_items.h>
#include "conf_loader.h"


#ifdef NEWSGOLD
#define DEFAULT_DISK 4
#else
#define DEFAULT_DISK 0
#endif

__no_init const char *successed_config_filename;
__no_init unsigned int DEFAULT_DISK_N;

#pragma segment="CONFIG_DATA_ID"
#pragma segment="CONFIG_DATA_I"
__arm int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  int result=0;
  void *cfg;
  void *cfg_init;
  unsigned int rlen, end;

  cfg=(void *)__segment_begin("CONFIG_DATA_I");
  cfg_init=(void *)__segment_begin("CONFIG_DATA_ID");
  
  unsigned int len=(char *)__segment_end("CONFIG_DATA_ID")-(char *)__segment_begin("CONFIG_DATA_ID");
  if ((f=open(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    rlen=read(f,cfg,len,&ul);
    end=lseek(f,0,S_END,&ul,&ul);
    close(f,&ul);
    if (rlen!=end || rlen!=len)  goto L_SAVENEWCFG;
  }
  else
  {
  L_SAVENEWCFG:
    memcpy(cfg,cfg_init,len);
    if ((f=open(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
    {
      if (write(f,cfg,len,&ul)!=len) result=-1;
      close(f,&ul);
    }
    else
      result=-1;
  }
  if (result>=0)
  {
    DEFAULT_DISK_N=*fname-'0';
    successed_config_filename=fname;
  }
  return(result);
}

void InitConfig()
{
  if (LoadConfigData("4:\\ZBin\\etc\\ElfPack.bcfg")<0)
  {
    if (LoadConfigData("0:\\ZBin\\etc\\ElfPack.bcfg")<0)
      DEFAULT_DISK_N=DEFAULT_DISK;
  }
}
