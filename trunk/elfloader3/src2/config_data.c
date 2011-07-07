#include <cfg_items.h>
#include <swilib.h>
//Конфигурация

__root CFG_HDR cfghdr1={CFG_UINT,"Alpha-chan threshold",0,255};
__root unsigned int ALPHA_THRESHOLD=128;

__root CFG_HDR cfghdr2={CFG_UINT,"PNG cache size",20,200};
__root unsigned int CACHE_PNG=50;

#if NEWSGOLD || X75
__root CFG_HDR cfghdr3={CFG_CBOX,"Default bit-depth",0,3};
__root unsigned int DEFAULT_COLOR = 2;
__root CFG_CBOX_ITEM cfgcbox3_1[3]={"8 bits","16 bits","24 bits+alpha"};
#else
__root CFG_HDR cfghdr3={CFG_CBOX,"Default bit-depth",0,2};
__root unsigned int DEFAULT_COLOR = 1;
__root CFG_CBOX_ITEM cfgcbox3_1[2]={"8 bits","16 bits"};
#endif

