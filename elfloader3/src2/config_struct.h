
#ifndef __CONFIG_STRUCT_H__
#define __CONFIG_STRUCT_H__

#include <inc/cfg_items.h>

#pragma pack(1)
typedef struct
{
  const CFG_HDR cfghdr1;
  unsigned int ALPHA_THRESHOLD;

  const CFG_HDR cfghdr2;
  unsigned int CACHE_PNG;

  const CFG_HDR cfghdr3;
#if NEWSGOLD || X75
  unsigned int DEFAULT_COLOR;
#else
  unsigned int DEFAULT_COLOR;
#endif
  CFG_CBOX_ITEM cfgcbox3_1[3];

  const CFG_HDR cfghdr4;
  unsigned int load_in_suproc;

  const CFG_HDR cfghdr5;
#if NEWSGOLD || X75
  char IMAGE_FOLDER[128];
#else
  char IMAGE_FOLDER[128];
#endif

  const CFG_HDR cfghdr6;
#if NEWSGOLD || X75
  char DAEMONS_FOLDER[128];
#else
  char DAEMONS_FOLDER[128];
#endif

  CFG_HDR cfghdr7;
#if NEWSGOLD || X75
  char SWIBLIB_WAY[128];
#else
  char SWIBLIB_WAY[128];
#endif

  const CFG_HDR cfghdr8;
  unsigned int realtime_libclean;

  const CFG_HDR cfghdr10;
  char LD_LIBRARY_PATH_env[256];

  const CFG_HDR cfghdr11;
  char ep_log_way[128];

  const CFG_HDR cfghdr12;
  unsigned int max_log_size;

  const CFG_HDR cfghdr13;
  unsigned int loader_warnings;
}config_structure_t;



extern const config_structure_t config_structure;
extern config_structure_t *config;



#endif
