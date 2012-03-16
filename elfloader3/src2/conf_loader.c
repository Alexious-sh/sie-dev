#include "loader3/loader.h"
#include <inc/cfg_items.h>
#include "conf_loader.h"
#include "config_struct.h"


config_structure_t *config = 0;
__no_init const char *successed_config_filename;
extern void * memcpy_a (void *dest, const void *source, size_t cnt);


#ifdef NEWSGOLD
#define DEFAULT_DISK 4
#else
#define DEFAULT_DISK 0
#endif


__arm int LoadConfigData(const char *fname)
{
  int f;
  unsigned int ul;
  int result=0;
  void *cfg;
  void *cfg_init;
  unsigned int rlen, end;
  
  cfg=(void *)config;
  cfg_init = (void *)&config_structure;
  
  unsigned int len = sizeof(config_structure);
  if ((f=fopen(fname,A_ReadOnly+A_BIN,P_READ,&ul))!=-1)
  {
    rlen=fread(f, cfg, len, &ul);
    end=lseek(f,0,S_END,&ul,&ul);
    fclose(f,&ul);
    if (rlen!=end || rlen!=len)  goto L_SAVENEWCFG;
  }
  else
  {
  L_SAVENEWCFG:
    memcpy(cfg, cfg_init, len);
    if ((f=fopen(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
    {
      if (fwrite(f,cfg,len,&ul)!=len) result=-1;
      fclose(f,&ul);
    }
    else
      result=-1;
  }
  if (result>=0)
  {
    //DEFAULT_DISK_N=*fname-'0';
    successed_config_filename=fname;
  }
  return(result);
}



void InitConfig()
{
  /* теперь под конфиг выделяется явный хиповый адрес */
  /* его размер орграничевается теперь только лишь рамой */
  config = malloc(sizeof(config_structure_t));
  memcpy_a(config, &config_structure, sizeof(config_structure_t));
  
  if( LoadConfigData("4:\\ZBin\\etc\\ElfPack.bcfg")<0)
  {
    if (LoadConfigData("0:\\ZBin\\etc\\ElfPack.bcfg")<0)
    {}  //DEFAULT_DISK_N=DEFAULT_DISK;
  }
}
