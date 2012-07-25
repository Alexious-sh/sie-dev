#include <swilib.h>
#include <cfg_items.h>
#include <conf_loader.h>

/*
    Переделанный загрузчик конфига под gcc.
    (c) Z.Vova
*/


char *successed_config_filename="";


//__attribute__((section("CONFIG_C")))
int LoadConfigData(const char *fname, CFG_HDR *cfghdr0, int * __config_begin, int * __config_end)
{
  int f;
  unsigned int ul;
  char *buf;
  int result=0;
  void *cfg;

  //extern const int __config_end, __config_begin;
  cfg=(void*)cfghdr0;
  volatile unsigned int _segb = (volatile unsigned int)__config_begin;
  volatile unsigned int _sege = (volatile unsigned int)__config_end;


  unsigned int len = (_sege - _segb)-4;
  if (!(buf=malloc(len))) return -1;
  if ((f=_open(fname,A_ReadOnly+A_BIN,0,&ul))!=-1)
  {
    if (_read(f,buf,len,&ul) == (int)len)
    {
      memcpy(cfg,buf,len);
      _close(f,&ul);
    }
    else
    {
      _close(f,&ul);
      goto L_SAVENEWCFG;
    }
  }
  else
  {
  L_SAVENEWCFG:
    if ((f=_open(fname,A_ReadWrite+A_Create+A_Truncate,P_READ+P_WRITE,&ul))!=-1)
    {
      if (_write(f,cfg,len,&ul)!= (int)len) result=-1;
      _close(f,&ul);
    }
    else
      result=-1;
  }
  mfree(buf);
  if (result>=0) successed_config_filename = (char*)fname;
  return(result);
}


void InitConfig()
{
    __CONFIG_EXTERN(0, cfghdr0);    // нулевой конфиг и первый его элемент cfghdr0
    LoadConfigData("0:\\Misc\\elf3\\test.bcfg", __CONFIG(0, cfghdr0));


    __CONFIG_EXTERN(1, cfghdr2);
    LoadConfigData("0:\\Misc\\elf3\\test2.bcfg", __CONFIG(1, cfghdr2));
}


