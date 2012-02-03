
#include "loader.h"


extern char ep_log_way[];
extern unsigned int max_log_size;


__arch void ep_log(Elf32_Exec *ex, const char *data, int size)
{
    if(!*ep_log_way || !max_log_size) return;
    
    unsigned int err;
    int log_size;
    TDate td;
    TTime tt;
    int fp = open(ep_log_way, A_Create | A_Append | A_BIN | A_WriteOnly, P_WRITE, &err);
    if(fp == -1) return;
    
    
    int mem = lseek(fp, 0, S_CUR, &err, &err);
    log_size = lseek(fp, 0, S_END, &err, &err);
    lseek(fp, mem, S_SET, &err, &err);
 
    char *_data = malloc( strlen(ex->fname) + size + 128 );
    
    GetDateTime(&td, &tt);
    int sz = sprintf(_data, " [%d.%02d.%02d %02d:%02d:%02d] %s: %s\n",
                     td.year, td.month, td.day,
                     tt.hour, tt.min, tt.sec,
                     ex->fname, data );
    
    if(log_size+sz > max_log_size) {
      setfilesize(fp, 0, &err);
    }
    
    write(fp, _data, sz, &err);
    close(fp, &err);
    mfree(_data);
}

