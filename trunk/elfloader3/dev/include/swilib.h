


/**
*  Переделанная swilib.h для gcc.
*  (c) Z.Vova
*/


#ifndef __SWILIB__
#define __SWILIB__

#include <sys/types.h>
#include <stdarg.h>
#include <sha.h>
#include <rsa.h>
#include <md5.h>
#include <obs.h>
#include <swihelper.h>
#include <png.h>

#ifndef _NULL
#define _NULL 0
#endif

#ifndef NULL
#define NULL   _NULL
#endif /* NULL */

#define __int16 short
typedef int HObj;
#ifndef __JMPBUF_DECL
#define __JMPBUF_DECL
typedef int jmp_buf[11];
#endif

#define true 1
#define false 0

/* ======================================================= */

#define PC_FOREGROUND         100
#define PC_BACKGROUND         101
#define PC_HEADERFOREGROUND   102
#define PC_HEADERBACKGROUND   103

#define PC_SELECTFOREGROUND   115
#define PC_SELECTBACKGROUND   116
#define PC_SELECTBORDER       117
#define PC_SELECTSHADOW       118

#define PC_LIGHTTEXTFOREGROUND 122
#define PC_LIGHTTEXTBACKGROUND 123

#define PC_SCROLLBARSLIDER    127
#define PC_SCROLLBAR          128
#define PC_WINDOWFOREGROUND   129
#define PC_WINDOWBACKGROUND   130

#define PC_BORDER             133
#define PC_BORDERSHADOW       134

#define PC_POPUPSELECTFOREGROUD 135
#define PC_POPUPSELECTBACKGROUD 136

#define PC_PROGRESSBACKGROUND 138
#define PC_PROGRESSFOREGROUND 139
#define PC_PROGRESSBORDER     140

/* =========================================== */

#define LMAN_CONNECT_CNF 0x80
#define LMAN_CONNECT_REJ_IND 0x81
#define LMAN_DISCONNECT_IND 0x82
//#define 0x83
#define LMAN_DISCONNECT_CNF 0x84
//#define 0x85
//#define 0x86
//#define 0x87
//#define 0x88
#define LMAN_LINK_UP_IND 0x89
#define ENIP_BUFFER_FREE 0x8A
#define ENIP_BUFFER_FREE1 0x8B
#define ENIP_SOCK_DATA_READ 0x8C
#define ENIP_SOCK_CONNECTED 0x8D
#define ENIP_SOCK_CLOSED 0x8E
#define ENIP_SOCK_REMOTE_CLOSED 0x8F
#define ENIP_SOCK_ACCEPTED 0x90
#define ENIP_SOCK_ICMP_ERROR 0x91
#define ENIP_DNR_HOST_BY_NAME 0x94


#ifdef NEWSGOLD

#ifdef ELKA
#define DISPLACE_OF_EDGUI 0x90
#define DISPLACE_OF_USSDREQGUI_ID 0x44
#define DISPLACE_OF_EDGUI_ID 0x4C
#define DISPLACE_OF_IDLEGUI_ID 0x2C
#define DISPLACE_OF_IDLECANVAS 0x20

#else
#define DISPLACE_OF_EDGUI 0x88
#define DISPLACE_OF_USSDREQGUI_ID 0x44
#define DISPLACE_OF_EDGUI_ID 0x4C
#define DISPLACE_OF_IDLEGUI_ID 0x2C
#define DISPLACE_OF_IDLECANVAS 0x18

#endif

#define DISPLACE_OF_INCOMMINGGUI 0x34
#define DISPLACE_OF_OUTGOINGGUI 0x30
#define MSG_INCOMMING_CALL 0x6000
#define MSG_STATE_OF_CALL 0x6009
#define MSG_END_CALL 0x600B

#define MSG_HELPER_TRANSLATOR 0xDEAE
#define MSG_RECONFIGURE_REQ 0xDEAF
#define MSG_IPC 0xDEB0

#define MSG_SMS_RX 0x5803

#define MSG_USSD_RX 0x60E2
#define MSG_AUTOUSSD_RX 0x60E5
typedef struct{
  int pid_from;
  int msg; //MSG_USSD_RX
  void *pkt;
  char unk1;
  char encoding_type;
  char pkt_length;
  char unk2;
  void *data1;
} GBS_USSD_MSG;



#define MSG_PLAYFILE_REPORT 0x70BC
#define MSG_CSM_DESTROYED 0x6400
#define MSG_CSM_DESTROY_NOTIFY 0x6402
#define MSG_GUI_DESTROYED 0x640E
#define MSG_IDLETMR_EXPIRED 0x6401
#define MMI_CEPID 0x4209
#define LGP_NULL 0x7FFFFFFF
#define SET_SOFT_KEY_N 0
#define LGP_DOIT_PIC 0x7FFFC0FB
#else

#define DISPLACE_OF_EDGUI 0x50
#define DISPLACE_OF_USSDREQGUI_ID 0x40
#define DISPLACE_OF_EDGUI_ID 0x4C
#define DISPLACE_OF_IDLEGUI_ID 0x28
#define DISPLACE_OF_IDLECANVAS 0x14


#define DISPLACE_OF_INCOMMINGGUI 0x30
#define DISPLACE_OF_OUTGOINGGUI 0x2C
#define MSG_INCOMMING_CALL 0x08
#define MSG_STATE_OF_CALL 0x0B
#define MSG_END_CALL 0x0D

#define MSG_HELPER_TRANSLATOR 0x3F0
#define MSG_RECONFIGURE_REQ 0x3F1
#define MSG_IPC 0x3F2

#define MSG_SMS_RX 0xC2

#define MSG_USSD_RX 0x42
#define MSG_AUTOUSSD_RX 0x45

typedef struct{
  short pid_from;
  short msg; //MSG_USSD_RX
  void *pkt;
  char unk1;
  char encoding_type;
  char pkt_length;
  char unk2;
  void *data1;
} GBS_USSD_MSG;

#define MSG_CSM_DESTROYED 0x93
#define MSG_CSM_DESTROY_NOTIFY 0x92
#define MSG_GUI_DESTROYED 0x98
#define MSG_IDLETMR_EXPIRED 0x95
#define MMI_CEPID 0x4209
#define LGP_NULL 0x7FFF
#define SET_SOFT_KEY_N 1
#define LGP_DOIT_PIC 0x7FFFC0FB

#ifdef X75
#define MSG_PLAYFILE_REPORT 0x174
#else
#define MSG_PLAYFILE_REPORT 0x167
#endif

#endif

#define NAMECSM_MAGIC1 ((void *(*)(int))0x55AACCCC)
#define NAMECSM_MAGIC2 ((void (*)(void *))0x3333AA55)

typedef struct{
  unsigned long year;
  unsigned char month;
  unsigned char day;
} TDate;

typedef struct{
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
  unsigned int millisec;
} TTime;

typedef struct{
  unsigned int type;
  unsigned int param2;
  unsigned short param3;
  unsigned short wstr[25];
}StructUSSDStr;

typedef struct{
  void *p1;
  void *p2;
  StructUSSDStr *pussdstr;
}StructUSSD;

typedef struct
{
  void *first;
  void *last;
  void (*data_mfree)(void *);
}LLQ;

typedef struct{
  int param0;
  int param1;
  int param2;
  int param3;
  int param4;
  int param5;
  int param6;
} GBSTMR;

typedef struct{
#ifdef NEWSGOLD
  int pid_from;
  int msg;
#else
  short pid_from;
  short msg;
#endif
  int submess;
  void *data0;
  void *data1;
} GBS_MSG;

typedef struct{
  void *p;
  int i;
}MUTEX;

typedef struct{
  void *next;
  void *prev;
  void /*CSM_DESC*/ *constr;
  int id;
  int state;
  int unk1;
  int unk2;
  LLQ gui_ll;
} CSM_RAM;

typedef struct{
  int (*onMessage)(CSM_RAM*,GBS_MSG*);
  void (*onCreate)(CSM_RAM*);
#ifdef NEWSGOLD
  int zero1;
  int zero2;
  int zero3;
  int zero4;
#endif
  void (*onClose)(CSM_RAM*);
  int datasize;
  int statesize;
  const int *states;
} CSM_DESC;

#ifdef NEWSGOLD
typedef struct{
  int id;
  CSM_RAM *current_msg_processing_csm;
  LLQ csm;
  LLQ cmd;
  LLQ csm_background;
} CSMQ;
#else
typedef struct{
  int id;
  CSM_RAM *current_msg_processing_csm;
  LLQ csm;
  LLQ cmd;
  int unk;
  int unk1;
  void *methods;
  int unk2;
  void *tmr;
  short cepid;
} CSMQ;
#endif

typedef struct{
#ifdef NEWSGOLD
  void *next;
  void *prev;
  int flag1;
  int parent_id;
  int prio;
  int (*proc)(CSMQ *, void *);
  CSMQ *csm_q;
  int id;
  int flag2;
#else
  void *next;
  void *prev;
  char flag1;
  int parent_id;
  int (*proc)(CSMQ *, void *);
  CSMQ *csm_q;
  CSM_RAM *cmd_csm;
#endif
}MMICMD;



typedef struct{
  int under_idle_id;
  int idle_id;
  CSMQ *csm_q;
} CSMROOT;

typedef struct{
  short x;
  short y;
  short x2;
  short y2;
} RECT;

typedef struct{
  char dummy[0x24];
} DRWOBJ;

typedef struct{
  unsigned short *wsbody;
  void *(*ws_malloc)(int);
  void (*ws_mfree)(void *);
  int isbody_allocated;
  unsigned short maxlen;
  unsigned short unk1;
} WSHDR;

typedef struct
{
  void *next;
  void *prev;
  void *data;
}EDITQ;

typedef struct
{
  void *next;
  void *prev;
}LLIST;

typedef struct
{
#ifdef NEWSGOLD
  const char *ext;
  int unical_id;
  char enabled_options;
  char obex_path_id;
  short menu_flag;
  const int *icon1;
  const int *icon2;
  int Open_lgpid;	//0x109
  int Options_lgpid;	//0x10A
  int Enter_lgpid;	//0x7FFFC0FB
  void *proc;
  void *altproc;
#else
  const char *ext;
  int unical_id;
  signed char enabled_options;
  unsigned char obex_path_id;
  unsigned short menu_flag;
  const int *icon1;
  const int *icon2; //skip on reg
  void *proc;
  void *altproc;
#endif
}REGEXPLEXT;

typedef struct
{
  int zero;
  unsigned const int *icon1;
  int unical_id;
  char obex_path_id;
  char unk;
  unsigned short menu_flag;
  char enabled_options;
  char not_used[3];
  WSHDR* ext;
  void *proc;
  void *altproc;
  unsigned const int *icon2;
}REGEXPLEXT_ARM_NEW;

#ifdef NEWSGOLD
typedef struct
{
  RECT *canvas;
  void *methods;
  void *definition;
  char state;
  char unk2;
  char unk3;
  char unk4;
  int color1;
  int color2;
  LLQ item_ll;
  int unk5;
  char unk6;
  char unk7;
  char unk8;
  char unk9;
  int unk10;
  int flag30;
}GUI;
#else
typedef struct
{
  RECT *canvas;
  void *methods;
  void *definition;
  char state;
  char unk2;
  char unk3;
  char unk4;
  int color1;
//  int color2;
  LLQ item_ll;
  int unk5;
  char unk6;
  char unk7;
  char unk8;
  char unk9;
  int unk10;
  int flag30;
}GUI;
#endif


typedef struct
{
  char zero;
  char unk1;
  short keys;
  GBS_MSG *gbsmsg;
}GUI_MSG;



//Keyhook return codes
//Process key to next keyhook and to gui
#define KEYHOOK_NEXT (0)
//Process key only to next keyhook, not to gui
#define KEYHOOK_NEXT_NOT2GUI (1)
//Break process and exit without processing in gui
#define KEYHOOK_BREAK (2)
//Break process and exit with or not processing in gui (selected by prev values 0/1)
#define KEYHOOK_ENDQ (3)


//Constants
//Attributes
#define FA_READONLY 0x0001
#define FA_HIDDEN 0x0002
#define FA_SYSTEM 0x0004
#define FA_DIRECTORY 0x0010
#define FA_ARCHIVE 0x0020

//Seek
#define S_SET 0
#define S_CUR 1
#define S_END 2

//Permissions
#define P_WRITE 0x100
#define P_READ 0x80

//Access
#define A_ReadOnly 0
#define A_WriteOnly 1
#define A_ReadWrite 2
#define A_NoShare 4
#define A_Append 8
#define A_Exclusive 0x10
#define A_MMCStream 0x20
#define A_Create 0x100
#define A_Truncate 0x200
#define A_FailCreateOnExist 0x400
#define A_FailOnReopen 0x800

#define A_TXT 0x4000
#define A_BIN 0x8000

// kbd msg:
#define KEY_DOWN        0x193
#define KEY_UP          0x194
#define LONG_PRESS      0x195
// 196 - sent 1 second after last key release, but not always?
// 197 - ? unknown kbd msg
// 198 - ? unknown kbd msg

// For kbd msg:
// submsg - scan Code:
#ifdef NEWSGOLD
#define LEFT_SOFT       0x01
#define RIGHT_SOFT      0x04
#define GREEN_BUTTON    0x0B
#define RED_BUTTON      0x0C
#define VOL_UP_BUTTON   0x0D
#define VOL_DOWN_BUTTON 0x0E
#define UP_BUTTON       0x3B
#define DOWN_BUTTON     0x3C
#define LEFT_BUTTON     0x3D
#define RIGHT_BUTTON    0x3E
#define ENTER_BUTTON	0x1A
#ifdef ELKA
#define POC_BUTTON 0x15
#define MEDIA_BUTTON	0x11
#define EL_PLAY_BUTTON	0x1D
#else
#define INTERNET_BUTTON 0x11
#endif
#define PLAY_BUTTON	0x15
#define CAMERA_BUTTON	0x14
#else
#define LEFT_SOFT       0x01
#define RIGHT_SOFT      0x04
#define GREEN_BUTTON    0x0B
#define RED_BUTTON      0x0C
#ifdef X75
#define VOL_UP_BUTTON   0x14
#define VOL_DOWN_BUTTON 0x15
#else
#define VOL_UP_BUTTON   0x0D
#define VOL_DOWN_BUTTON 0x0E
#endif
#define UP_BUTTON       0x3B
#define DOWN_BUTTON     0x3C
#define LEFT_BUTTON     0x3D
#define RIGHT_BUTTON    0x3E
#define ENTER_BUTTON	0x1A
#define INTERNET_BUTTON     0x11
#endif
#define PTT_BUTTON	0x27
// '*', '#', '0'-'9'

//Draw flags
#define TEXT_ALIGNLEFT 1
#define TEXT_ALIGNMIDDLE 2
#define TEXT_ALIGNRIGHT 4
#define TEXT_UNDERLINE 8
#define TEXT_INVERT 16
#define TEXT_OUTLINE 32
#define TEXT_PASSWORD 64
#define TEXT_NOFORMAT 128
#define TEXT_INVERT2 256

#define RECT_DOT_OUTLINE 1
#define RECT_FILL_WITH_PEN 2
#define RECT_DRAW_INVERT 4

#define LINE_DOTTED 1
#define LINE_DOTTED2 2

//Fonts

#ifdef NEWSGOLD

#ifdef ELKA

#define FONT_LARGE 0
#define FONT_LARGE_BOLD 1
#define FONT_LARGE_ITALIC 2
#define FONT_LARGE_ITALIC_BOLD 3
#define FONT_MEDIUM 4
#define FONT_MEDIUM_BOLD 5
#define FONT_MEDIUM_ITALIC 6
#define FONT_MEDIUM_ITALIC_BOLD 7
#define FONT_NUMERIC_SMALL 8
#define FONT_NUMERIC_SMALL_BOLD 9
#define FONT_NUMERIC_XSMALL 8
#define FONT_SMALL 8
#define FONT_SMALL_BOLD 9
#define FONT_SMALL_ITALIC 10
#define FONT_SMALL_ITALIC_BOLD 11
#define FONT_NUMERIC_LARGE 0
#define FONT_NUMERIC_MEDIUM 4

//#define SMALL_FONT 8
//#define MIDDLE_FONT 4

#else //#ifdef ELKA

#define FONT_LARGE 0
#define FONT_LARGE_BOLD 1
#define FONT_LARGE_ITALIC 2
#define FONT_LARGE_ITALIC_BOLD 3
#define FONT_MEDIUM 4
#define FONT_MEDIUM_BOLD 5
#define FONT_MEDIUM_ITALIC 6
#define FONT_MEDIUM_ITALIC_BOLD 7
#define FONT_NUMERIC_SMALL 8
#define FONT_NUMERIC_SMALL_BOLD 9
#define FONT_NUMERIC_XSMALL 10
#define FONT_SMALL 11
#define FONT_SMALL_BOLD 12
#define FONT_SMALL_ITALIC 13
#define FONT_SMALL_ITALIC_BOLD 14
#define FONT_NUMERIC_LARGE 15
#define FONT_NUMERIC_MEDIUM 16

//#define SMALL_FONT 11
//#define MIDDLE_FONT 4

#endif //#ifdef ELKA

#else //#ifdef NEWSGOLD

#define FONT_LARGE 0
#define FONT_LARGE_BOLD 1
#define FONT_LARGE_ITALIC 0
#define FONT_LARGE_ITALIC_BOLD 1
#define FONT_MEDIUM 2
#define FONT_MEDIUM_BOLD 3
#define FONT_MEDIUM_ITALIC 2
#define FONT_MEDIUM_ITALIC_BOLD 3
#define FONT_NUMERIC_SMALL 4
#define FONT_NUMERIC_SMALL_BOLD 5
#define FONT_NUMERIC_XSMALL 6
#define FONT_SMALL 7
#define FONT_SMALL_BOLD 8
#define FONT_SMALL_ITALIC 7
#define FONT_SMALL_ITALIC_BOLD 8
#define FONT_NUMERIC_LARGE 9
#define FONT_NUMERIC_MEDIUM 10

//#define MIDDLE_FONT 3
//#define SMALL_FONT 7

#endif //#ifdef NEWSGOLD

// Phone info IDs
#define PI_BASE_INFO1   0 // mch_x75idbi14
#define PI_BASE_INFO2   1 // m75i14#main_1.0_V79.27.1
#define PI_BASE_INFO3   2 // M_x75plat_MP_main_1.0
#define PI_BASE_INFO4   3 // M_minosemea_TH
#define PI_BUILD        4 // OFFICIAL
#define PI_DATETIME1	5	// �����-�� ��� ����
#define PI_DATETIME2    6
#define PI_SW_NUMBER    7 // ������ ��������
#define PI_VENDOR       8 // = SIEMENS :)
#define PI_MODEL        9 // ������ ��������
#define PI_LG           10 // = LangPack
#define PI_T9           11 // = T9


#ifdef NEWSGOLD
#define FIRST_UCS2_BITMAP 0xE12A
#else
#define FIRST_UCS2_BITMAP 0xE137
#endif


#define HEADLINE_STANDART        0x0
#define HEADLINE_FULLSCREEN      0x1
#define BODY_STANDART            0x2
#define BODY_TAB                 0x3
#define BOTTOM_STANDART          0x4
#define BOTTOM_FULLSCREEN        0x5
#define POPUP_OPTIONS            0x6
#define POPUP_FEEDBACK           0x7
#define SELECTION_1_LINE         0x8
#define SELECTION_2_LINE         0x9
#define SELECTION_3_LINE         0xA
#define SELECTION_ICON_ONLY      0xB
#define POPUP_SEARCH_LINE        0xC
#define POPUP_QUICK_ACCESS_FIELD 0xD
#define PROGRESS_STATUSBAR       0xE
#define TAB_SELECTED             0xF
#define TAB_UNSELECTED           0x10
#define STATUSBAR_STANDART       0x11
#define STATUSBAR_FULLSCREEN     0x12

typedef struct
{
#ifdef ELKA
  unsigned short w;
  unsigned short h;
  int bpnum; //For BW=1, 8bit=5, 16bit=8, 0x80 - packed
#else
  unsigned char w;
  unsigned char h;
  unsigned short bpnum; //For BW=1, 8bit=5, 16bit=8, 0x80 - packed
#endif
  unsigned char *bitmap;
}IMGHDR;

typedef struct
{
  RECT rc;
  int *icon;
  int lgp_id;
  int lgp_null; //LGP_NULL
}HEADER_DESC;

typedef struct
{
  int *icon;
  int lgp_id_small;
  int lgp_id_large;
  int zero;
  const int *softkeys; //{6,0x22,0x1D}
  int flag1; //0
  int flag2; //0x59D
}MENUITEM_DESC;

typedef struct
{
  short key1;
  short key2;
  int lgp_id;
}SOFTKEY_DESC;

typedef struct
{
  const SOFTKEY_DESC *desc;
  int n;
}SOFTKEYSTAB;

typedef void ( *MENUPROCS_DESC)(GUI *);

typedef struct
{
  int flag; //0,8 etc
  int (*onkey)(void *, GUI_MSG *);
  void (*ghook)(void *, int ); //GUI * gui, int cmd
  void *proc3;
  const int *softkeys; //{6,0x22,0x1D}, mb default for all items, if item.softkeys==NULL
  const SOFTKEYSTAB *softkeystab;
  int flags2; //2
//  0x00000001 //Icons presens
//  0x00000002 //
//  0x00000008 //
//  0x00000010 //Enable scroll text
//  0x00000020 //
//  0x00000040 //Disable scrollbar
//  0x00000080 //Enable "big letters" check
//  0x00000100 //Right align
//  0x00000200 //
//  0x00000400 //Disable breakline
//  0x00000800
//  0x00001000
//  0x00004000
//  0x00008000 //
//  0x00010000 //Enable search???
//  0x40000000
//  0x80000000

  void (*itemproc)(void *, int, void *); //Called when draw item
  const MENUITEM_DESC *items; //Table of items desc;
  const MENUPROCS_DESC *procs;//  void ** procs; //Table of procs when item selected
  int n_items; //Number of items
}MENU_DESC;

typedef struct
{
  int flag; //0,8 etc
  int (*onkey)(void *, GUI_MSG *);
  void (*ghook)(void *, int ); //GUI * gui, int cmd
  void *proc3;
  const int *softkeys; //{6,0x22,0x1D}, mb default for all items, if item.softkeys==NULL
  const SOFTKEYSTAB *softkeystab;
  int flags2; //2
  void (*itemproc)(void *, int, void *); //Called when draw item
  const MENUITEM_DESC *items; //Table of items desc;
  const MENUPROCS_DESC *procs;//  void ** procs; //Table of procs when item selected
  int n_items; //Number of items
  int n_lines; //Number of lines in selected item
}ML_MENU_DESC;

#pragma pack(1)
typedef struct{
#ifdef NEWSGOLD
  char unk0[40];
  unsigned int file_size;
  short file_attr;
  unsigned int create_date_time;
  char unk1[2];
  char folder_name[128];
  char unk2;
  char file_name[128];
  char unk3[11];
  short wsfolder_name[128+1];
  short wsfile_name[128+1];
#else
  char unk1[14];
  short file_attr;
  unsigned int create_date_time;
  unsigned int file_size;
  char unk2[16];
  char folder_name[128];
  char unk3;
  char file_name[271];
#endif
}DIR_ENTRY;
#pragma pack()

typedef struct
{
  char unk1;
  char unk2;
  char unk3;
  char unk4;
  unsigned int size;
  char unk5[28];
  int file_attr;
}FSTATS;


typedef struct
{
  unsigned short ch_number;
  unsigned short ci;
  unsigned short lac;
  char power;
  char c1;
  char c2;
  char unk;
  unsigned short unk2;
}RAMNET;

//-----------------------------------------

typedef struct
{
  int one;
  int (*onKey)(GUI *gui, GUI_MSG *msg);
  void (*global_hook_proc)(GUI *gui, int cmd);
  void *locret;
  int zero1;
  const SOFTKEYSTAB *softkeystab;
  RECT rc;
  int font;
  int _100;
  int _101;
  int zero2;
//  0x00000001 - Выровнять по правому краю
//  0x00000002 - Выровнять по центру
//  0x00000004 - Инверсия знакомест
//  0x00000008 - UnderLine
//  0x00000020 - Не переносить слова
//  0x00000200 - bold
  int zero3;
//  0x00000002 - ReadOnly
//  0x00000004 - Не двигается курсор
//  0x40000000 - Поменять местами софт-кнопки
  int _0x40000000;
}INPUTDIA_DESC;

typedef struct
{
  char type;
  void *user_pointer;
  unsigned short cur_item;
  WSHDR *ws;
}USR_MENU_ITEM;

typedef struct
{
  unsigned short invert;  // |1
  unsigned short bitmask;
  char font;              // |2
  char align;             // |4
  char pen[4];            // |8
  char brush[4];          // |0x10
}EDITC_OPTIONS;

#pragma pack(2)
typedef struct
{
  char type;
  char unk2[3];
  int flag;
  char zero_cbox;  // if text input field: lang 0 - default, 2 - english
  char unk5;
  EDITC_OPTIONS ed_options;
  short maxlen;
  short num_of_items_cbox;
  short start_item_cbox;
  short unk9;
  short unk10;
  short unk11;
  int unk12;
  WSHDR *pWS;
}EDITCONTROL;
#pragma pack()

//EditControlTypes
#define ECT_READ_ONLY 0
#define ECT_HEADER 1
#define ECT_NUMBER_TYPING 2
#define ECT_NORMAL_TEXT 3
#define ECT_CURSOR_STAY 4
#define ECT_FIXED_STR_NUM 5
#define ECT_NORMAL_NUM 6
#define ECT_COMBO_BOX 7
#define ECT_READ_ONLY_SELECTED 8
#define ECT_LINK 9
#define ECT_CALENDAR 10
#define ECT_TIME 11

//EditControlFlags
#define ECF_NORMAL_STR 0
#define ECF_SKIP 4
#define ECF_INVISIBLE_CURSOR 8
#define ECF_PASSW 0x10
#define ECF_DELSTR 0x20
#define ECF_APPEND_EOL 0x40
#define ECF_SET_CURSOR_END 0x80

#define ECF_DISABLE_NUM 0x100
#define ECF_DISABLE_SMALL_LETTERS 0x200
#define ECF_DISABLE_BIG_LETTERS 0x400

#define ECF_DEFAULT_BIG_LETTER 0x1000  // Enable big letter after ". "
#define ECF_DISABLE_T9 0x2000
#define ECF_DEFAULT_ITAP 0x4000
#define ECF_DEFAULT_DIGIT 0x8000

#define ECF_DISABLE_EOLCHAR 0x10000  // In chartable
#define ECF_DISABLE_MINUS 0x20000  // Also disable #
#define ECF_DISABLE_POINT 0x40000  // Also disable *
#define ECF_LONGDIEZ_EQ_QUESTION 0x80000
#define ECF_ENABLE_TEXT_FORMAT 0x800000
#define ECF_MARKED 0x2000000
#define ECF_GRAY 0x4000000

//TextInput Dialog ghook commands
#define TI_CMD_CREATE          2
#define TI_CMD_DESTROY         3
#define TI_CMD_REDRAW          7
#define TI_CMD_UNFOCUS         9
#define TI_CMD_FOCUS           10
#define TI_CMD_SUBFOCUS_CHANGE 12
#define TI_CMD_COMBOBOX_FOCUS  13

/*
!!!! DELETED !!!!
!!!! Use EDIT_GetCursorPos & EDIT_SetCursorPos
#ifdef NEWSGOLD
#ifdef ELKA
#define EDIT_CURSOR_POS(DATA) (((int ****)DATA)[0x90/4][0x8C/4][0x28/4][0x38/4])
#else
#define EDIT_CURSOR_POS(DATA) (((short ****)DATA)[0x88/4][0x84/4][0x28/4][0x34/2])
#endif
#else
#define EDIT_CURSOR_POS(DATA) (((short ****)DATA)[0x50/4][0x4C/4][0x28/4][0x30/2])
#endif
*/
//Menu flags and screen displace
#ifdef NEWSGOLD
#ifdef ELKA
#define MENU_FLAG3 3
#define MENU_FLAG2 0x5A2
#define YDISP (24)
#else
#define MENU_FLAG3 3
#define MENU_FLAG2 0x59D
#define YDISP (0)
#endif
#else
#define MENU_FLAG3 3
#define MENU_FLAG2 0x578
#define YDISP (0)
#endif


typedef struct
{
#ifdef NEWSGOLD
  short field_0;
  char no_data;
  char item_type;
  int field_4;
  void *data;
#else
  char field_0;
  char no_data;
  char field_2;
  char item_type;
  int field_4;
  void *data;
#endif
}AB_UNPRES_ITEM;

typedef struct{
#ifdef NEWSGOLD
  short NrOfFlds;
  short number_of_records;
  short NrAllocFlds;
  short field_6;
  short NrJ;
  char field_A;
  char field_B;
  AB_UNPRES_ITEM *record_list;
  char EntTp;
  char field_11;
  short EntId;
  int field_14;
  int field_18;
#else
  short field_0;
  short field_2;
  short field_4;
  short number_of_records;
  short field_8;
  short field_A;
  AB_UNPRES_ITEM *record_list;
  char field_10;
  char field_11;
  short field_12;
  int field_14;
  int field_18;
#endif
}AB_UNPRES;

#pragma pack(1)
typedef struct
{
  char _1D;
  char format;
  char data_size;
  char _0_1;
  char _0_2;
//  char _0_3;
  char data[128];
}PKT_NUM;
#pragma pack()

//================================
// Sockets
//================================
typedef struct
{
  unsigned short family; //1
  unsigned short port; //network order
  unsigned int ip; //network order
  unsigned int unk1;
  unsigned int unk2;
}SOCK_ADDR;

#pragma pack(2)
typedef struct
{
  unsigned short _0x0080;
  unsigned short _0xFFFF;
  unsigned short _0x0000;
}REGSOCKCEPID_DATA;
#pragma pack()

typedef struct
{
  char body[0x204];
}CTX_NAPDEF;

typedef struct
{
  unsigned int len;
  CTX_NAPDEF *ctx_napdef;
  unsigned int zero;
  CTX_NAPDEF napdef;
}NAP_PARAM_CONT;

typedef struct
{
#ifdef NEWSGOLD
  int zero;
  void *native_profile;
  char _1;
  char _4;
  unsigned short _0;
  NAP_PARAM_CONT *NapParameterContainer;
#else
  char unk1;
  char *unk;
  char one;
  char isactivated;
#endif
}LMAN_DATA;

#define IP_ADDR(A,B,C,D) (((unsigned long)A<<24)|((unsigned long)B<<16)|((unsigned long)C<<8)|((unsigned long)D<<0))

typedef struct{
#ifdef NEWSGOLD
  unsigned short repeat_num;  //0 - non limited
  unsigned short unk;
  int time_between_play;
  int play_first; // 0 - play all
  int volume;
  int unk5;
  int unk6;
  int unk7;
  int unk8;
  int unk9;
#else
#ifdef X75
  unsigned short repeat_num;  //0 - non limited
  unsigned short unk;
  int time_between_play;
  int play_first; // 0 - play all
  int volume;
  int unk4;     //0x80000000
  int unk5;     //1
  int unk6;     //0
  int unk7;
#else
  unsigned short repeat_num;  //0 - non limited
  unsigned short unk;
  int time_between_play;
  int play_first; // 0 - play all
  int volume;
  int unk5;
  int unk6;
#endif
#endif
}PLAYFILE_OPT;


typedef struct
{
 char data[0xB2];
}PDU;

typedef struct
{
  const char *name_to;
  const char *name_from;
  void *data;
}IPC_REQ;

typedef struct
{
  unsigned short type; //00
  WSHDR *wfilename; //04
  int unk_08 ; //08
  int unk_0C; //0C
  int unk_10 ; //10
  int unk_14 ; //14
  long length; //18 <-- return
  int unk_1C; //1C
  int unk_20 ; //20
} TWavLen;


typedef struct
{
  char yearNormBudd; //1 - norm, 2 - buddhist
  char dateFormat;
  char timeFormat; //0-24h, 1-12h
  char timeZone; //internal representation
  char isAutoTime1;
  char isAutoTime2;
}TDateTimeSettings;

typedef struct
{
#ifdef NEWSGOLD
  unsigned int type;  //from debugger 0x400 general(size) | 0x4000 picture, | 0xA800(3gp) video, | 0x1000 audio (wav/midi), 0x1800 (mp3/aac)
  WSHDR *filename;
  WSHDR *resolution_ws;
  int uid;
  unsigned short width;
  unsigned short height;
  WSHDR *duration_mp3_ws;
  unsigned int duration_mp3;
  WSHDR *duration_wav_ws;
  unsigned int duration_wav;
  WSHDR *tag_title_ws;
  WSHDR *tag_artist_ws;
  WSHDR *tag_album_ws;
  WSHDR *tag_track_ws;
  WSHDR *tag_year_ws;
  WSHDR *unk_tag_2_ws;
  WSHDR *bitrate_ws;
  WSHDR *audio_frequency_ws;
  WSHDR *size_ws;
#else
  unsigned short type;  // | 0x8000 picture, | 0x4000 video, | 0x2000 audio
  WSHDR *filename;
  WSHDR *text_resol;
  int uid;
  unsigned short width;
  unsigned short height;
  WSHDR *duration_wav_ws;
  unsigned int duration_wav;
  WSHDR *video_ws;
  int video_len;
#endif
} FILE_PROP;

typedef struct
{
  int mode; //0-normal mode, 1-select mode
  int dir_enum; //0x26 by default
  WSHDR* path_to_file; //path to file
  WSHDR* file_name; //put cursor to this file
  int unk5;
  int is_exact_dir;
  int unk7;
  int unk8;
  int unk9;
  int unk10;
  int unk11;
  WSHDR* full_filename;
  int unk13;
  int (*user_handler)(void*); //called in select mode
  void* this_struct_addr;
  int unk16;
  int unk17_26[10];
}NativeExplorerData;

typedef int  ( MenuSearchCallBack)(void *gui,WSHDR * searchstr);  // return -1


typedef struct POPUP_DESC
{
  int flag; //8, 0
  int (*onkey)(void *, GUI_MSG *);
  void (*ghook)(void *, int );
  void *proc3; //locret
  const int *softkeys;
  const SOFTKEYSTAB *softkeystab;
  int flags2; //1
  int lgp_null; //?
  const int *icons;
  int zero0; //0
  int font; //2
  int _100;
  int _101;
  int time; //0
} POPUP_DESC;

typedef struct SMS_POS_INDEX_DATA
{
  char unk0;
  char unk1;
  short pos_index;
} SMS_POS_INDEX_DATA;

typedef struct INDEX_ID_DATA
{
  void *unk_proc0; //0xA09D901D S7Cv47
  void *unk_proc1; //0xA09D9025 S7Cv47
  short index; //1,2,3 ...
  char unk[6];
  short unk1; //another index ?
  char cnt_all;
  char cnt_received; // =cnt0;
  unsigned short *data_id; //depend on cnt, for GetSmsPosIndex to get pos_index
  short unk2; //0, 0x700
  short unk3; //0
  char unk4; // 3
  char unk5; //0,3
  char type; // 0, in read, 1, in new, 3,sent
  char new_cnt_data; // ?
} INDEX_ID_DATA;

typedef struct SMS_DATA_LIST
{
  void *next;
  void *prev;
  INDEX_ID_DATA *index_id_data;
  //short unk0; //0xB2 pdu_size ?
  //short unk1; //0x4077 SMS_SIMIF_SM_CEPID ?
  //int unk2; //0x190 ?
  //void *unk3; //a list ?
} SMS_DATA_LIST;

typedef struct SMS_DATA_LLIST
{
  SMS_DATA_LIST *first;
  SMS_DATA_LIST *last;
  void *unk_proc; //0xA09DA18D //S7Cv47
} SMS_DATA_LLIST;

typedef struct SMS_DATA_ROOT
{
  SMS_DATA_LLIST in_msg;
  SMS_DATA_LLIST out_msg;
  short unk_0xFFFF;
  short unk_0_1;
  short cnt_msg_in_read;
  short cnt_in_new_sms_dat;
  short cnt_in_new_opwv; //? ems ?
  short cnt_sms_sent;
  short cnt_sms_draft;
  short unk_0_2;
  short cnt_freespace_data; // -0x28 == ?
  short cnt_in_data;
  short cnt_new_in_data;
  short cnt_sent_data;
  short cnt_draft_data;
  short unk_0_3; //maybe sending box
} SMS_DATA_ROOT;

typedef struct AbNumList
{
  void *next; //0
  void *prev; //0
  int flag_data; //type?
  void *data; //pkt_num
} AbNumList;

typedef struct NativeAbData
{
  AbNumList *first;
  AbNumList *last;
  void *mfree_adr;
  void *unk;
} NativeAbData;

typedef struct SS_RAM
{
  CSM_RAM ss_csm;
  void *unk;
  int ss_gui_id; //draw screensaver
  int keylock_gui_id; //keylock screen
} SS_RAM;

typedef struct TABGUI_DESC
{
  int flag; //8,a,b
  int (*onkey)(void *, GUI_MSG *); //,null
  void (*ghook)(void *, int ); //,null
  void *locret; //,null
  const int *softkeys;//,null
  const SOFTKEYSTAB *softkeystab; //,null
} TABGUI_DESC;


typedef struct EAM_DATA
{
  short dat_index; //
  char unk_FF[2];
  int opmsg_id; //browser killer, -1
  char unk[8];
} EAM_DATA;

typedef struct RAM_EMS_ADMIN
{
//  char unk_FF[0x10];
  int unk;
  EAM_DATA data[101];
} RAM_EMS_ADMIN;


typedef struct TVIEW_DESC
{
  int flag;
  int (*onkey)(void *, GUI_MSG *);
  void (*ghook)(void *, int);
  void (*locret)(void);
  const int *softkeys;
  const SOFTKEYSTAB *softkeystab;
  RECT rc;
  int font;
  int unk1; //0x64, 0x81
  int unk2; //0x65, 0x82
  int unk3; //0
  int unk4; //0,1,2
} TVIEW_DESC;

typedef struct LIGHT_PARAM
{
	char timeout; // Таймаут гашения
	char unk0; // Нечто загадочтное, бывает 0, или 5
	char perm_illum; // Не совсем понятно, похоже уровень, который будет удерживаться при вызове IllumTimeRequest,
                            //переписывается вызовом SaveMaxIllumination(), и зачем-то ограничено 80%
	char max_illum; // Ну здесь как известно яркость подсветки
} LIGHT_PARAM;

typedef struct I2C_MSG
{
  char chip_addr; // 7-ми битный адрес чипа(см.datasheet)
#ifdef NEWSGOLD
  char unk1;
  char unk2;
  char unk3;
  short nRegister; // номер регистра
#else
  char  unk1;
  short nRegister;// номер регистра
  char  unk2;
  char  tf; // Суда дописываеться потом самой ф-ей байтик как бы ID ф-ии
            // (1 - i2c_transfer, 2 - i2c_receive, 4 - i2c_unknown)
#endif
  short cb_data;
  int (*callback)(void *i2c_msg);
  void *data;
  int size;   //размер данных
} I2C_MSG;



//type
#define VOLUME 0x12
#define VOLUME_ALARMCLOCK 0xD
#define VOLUME_INCALL 2



#ifdef __cplusplus
extern "C" {
#endif

/*
#ifdef __NO_LIBC
#define __INITSWITAB int *__sys_switab_addres;\
void __attribute__((constructor)) __init_switab()\
{\
  register int ret=0;\
  asm volatile("swi	0x80FF\n\t"\
	       "mov	%0, r0"\
    :"=r"(ret)\
    :\
    :"lr");\
  __sys_switab_addres = ( int*)ret;\
}\

#endif
*/

__inl
void kill_data(void *p, void (*func_p)(void *))
{
    register void * p0 asm("r0")= p;
    register void * p1 asm("r1")= (void*)func_p;
    asm volatile("bx	%1\n\t"::"r"(p0),"r"(p1));
}

__inl
void loopback0()
__defn(0)

__inl
void loopback1()
__defn( 1)

__inl
void loopback2()
__defn( 2)

__inl
void loopback3()
__defn( 3)

__inl
void loopback4()
__defn( 4)

__inl
int GetAkku(int param1,int param2)
__def( 6, int, param1, param2)

__inl
long SetIllumination(unsigned char dev,unsigned long param1,unsigned short bright,unsigned long delay)
__def( 8, long, dev, param1, bright, delay)

__inl
void SwitchPhoneOff()
__defn( 0x9)

__inl
int _open(const char * cFileName, unsigned int iFileFlags, unsigned int iFileMode, unsigned int *ErrorNumber)
__def( 10, int, cFileName, iFileFlags, iFileMode, ErrorNumber)

__inl
int _read(int FileHandler, void *cBuffer, int iByteCount, unsigned int *ErrorNumber)
__def( 11, int, FileHandler, cBuffer, iByteCount, ErrorNumber)

__inl
int _write(int FileHandler, void const * cBuffer, int iByteCount, unsigned int *ErrorNumber)
__def( 12, int, FileHandler, cBuffer, iByteCount, ErrorNumber)

__inl
int _close(int FileHandler, unsigned int *ErrorNumber)
__def( 13, int, FileHandler, ErrorNumber)

__inl
int _flush(int stream, unsigned int *ErrorNumber)
__def( 14, int, stream, ErrorNumber)

__inl
long _lseek(int FileHandler, unsigned int offset, unsigned int origin, unsigned int *ErrorNumber, unsigned int *ErrorNumber2)
__def( 15, long, FileHandler, offset, origin, ErrorNumber, ErrorNumber2)


/*
__inl
int fopen(const char * cFileName, unsigned int iFileFlags, unsigned int iFileMode, unsigned int *ErrorNumber)
__def( 10, int, cFileName, iFileFlags, iFileMode, ErrorNumber)

__inl
int fread(int FileHandler, void *cBuffer, int iByteCount, unsigned int *ErrorNumber)
__def( 11, int, FileHandler, cBuffer, iByteCount, ErrorNumber)

__inl
unsigned int fwrite(int FileHandler, void const * cBuffer, int iByteCount, unsigned int *ErrorNumber)
__def( 12, unsigned int, FileHandler, cBuffer, iByteCount, ErrorNumber)

__inl
void fclose(int FileHandler, unsigned int *ErrorNumber)
__defn( 13, FileHandler, ErrorNumber)
*/


#ifdef _S_FUNC

#define fopen _open
#define fread _read
#define fwrite _write
#define fclose _close

#endif



__inl
int _mkdir(const char * cFileName, unsigned int *ErrorNumber)
__def( 16, int, cFileName, ErrorNumber)

__inl
int GetFileAttrib(const char *cFileName, unsigned char *cAttribute, unsigned int *ErrorNumber)
__def( 18, int, cFileName, cAttribute, ErrorNumber)

__inl
int SetFileAttrib(const char *cFileName, unsigned char cAttribute, unsigned int *ErrorNumber)
__def( 19, int, cFileName, cAttribute, ErrorNumber)


#ifdef __NO_LIBC

#ifdef VALGRIND
void *val_malloc(size_t sz, const char *file, int line);
void val_free(void *data, const char *file, int line);
#define malloc(sz) val_malloc(sz, __FILE__, __LINE__)
#define free(_data) val_free(_data, __FILE__, __LINE__)
#define mfree free
#else /*VALGRIND*/

__inl
void *malloc(size_t size)
__def( 20, void *, size)

__inl
void free(void * param1)
__defn( 21, param1)

__inl
void mfree(void * param1)
__defn( 21, param1)

#endif /*VALGRIND*/
#else /*__NO_LIBC*/
__inl
void mfree(void * param1)
__defn( 21, param1)

#endif

__inl
void *malloc_adr(void)
__def( 0x8014, void *)

__inl
void *mfree_adr(void)
__def( 0x8015, void *)

/*
__inl
int sprintf(char *buf, const char *str, ...)
__def( 22, int, buf, str, __VA_ARGS__)
*/
#ifdef __NO_LIBC
#define sprintf(dest, fmt, ...) __cdef(22, int, (char*)dest, fmt, ##__VA_ARGS__)
#endif

#ifdef __NO_LIBC
__inl
char * strcat (char * param1,const char * param2)
__def( 23, char *, param1, param2)

__inl
char * strchr (const char * param1,int param2)
__def( 24, char *, param1, param2)

__inl
int  strcmp (const char * param1,const char * param2)
__def( 25, int , param1, param2)

__inl
char * strcpy (char *dest,const char *source)
__def( 26, char *, dest, source)

__inl
unsigned int strlen(const char * param1)
__def( 27, unsigned int, param1)
#endif

__inl
void  wsAppendChar(WSHDR *ws,int wchar)
__defn( 28, ws, wchar)

__inl
int wsInsertChar(WSHDR *ws,int wchar,int pos)
__def( 29, int, ws, wchar, pos)

__inl
IMGHDR *GetPITaddr(int num)
__def( 30, IMGHDR *, num)

__inl
int GetImgHeight(int picture)
__def( 33, int, picture)

__inl
int GetImgWidth(int picture)
__def( 34, int, picture)

__inl
void DrawImg(unsigned int x, unsigned int y, unsigned int picture)
__defn( 35, x, y, picture)

__inl
void DrawCanvas(void *data, int x1, int y1, int x2, int y2, int flag_one)
__defn( 37, data, x1, y1, x2, y2, flag_one)

__inl
void DrawImgBW(unsigned int x, unsigned int y, unsigned int picture, char *pen, char *brush)
__defn( 38, x, y, picture, pen, brush)

__inl
int EEFullGetBlockInfo(unsigned int block,int *size,char *version)
__def( 0x2C, int, block, size, version)

__inl
void PlaySound(long param1, long param2, long param3, long tone, long param5)
__defn( 50, param1, param2, param3, tone, param5)

__inl
void PlaySoundLoop(int _0x2F, int param2, int duration)
__defn( 0x0034, _0x2F, param2, duration)

__inl
void AlarmClockRing()
__defn( 0x0035)

__inl
int *GetEELiteBlockAddress(short blocknum)
__def( 42, int *, blocknum)

__inl
int setfilesize(int FileHandler, unsigned int iNewFileSize, unsigned int *ErrorNumber)
__def( 60, int, FileHandler, iNewFileSize, ErrorNumber)

__inl
int IsScreenSaver(void)
__def( 0x0042, int)

__inl
int GetVibraStatus (void)
__def( 0x40 , int)

__inl
char GetProvider(WSHDR* name, int unk_1)
__def( 0x41 , char,name,unk_1)

__inl
int IsUnlocked(void)
__nul_def( 67, int)

__inl
#ifdef NEWSGOLD
int GetWavLen(char *pr)
#else
int GetWavLen(TWavLen *pr)
#endif
__def( 0x45, int, pr)

__inl
int HasNewSMS(void)
__nul_def( 0x0046, int)

__inl
int HasFlashSMS(void)
__nul_def( 0x0047, int)

__inl
#ifdef X75
short PlayFile(int flags, WSHDR *foldername, WSHDR *filename, int zero, int CepId, int Msg, const PLAYFILE_OPT *sfo)
__def( 0x4A, short, flags, foldername, filename, zero, CepId, Msg, sfo)
#else
short PlayFile(int flags, WSHDR *foldername, WSHDR *filename, int CepId, int Msg, const PLAYFILE_OPT *sfo)
__def( 0x4A, short, flags, foldername, filename, CepId, Msg, sfo)
#endif

__inl
int GetMissedCallsCount(void)
__nul_def( 0x004C, int)

__inl
void GBS_StartTimerProc(void *htimer, long ms, void ptr(GBSTMR*))
__defn( 77, htimer, ms, ptr)

__inl
int MsgBoxError(int flag, int LgpId)
__def( 0x4E, int, flag, LgpId)

__inl
int MsgBoxYesNo(int flag, int LgpID, void CallBackProc(int))
__def( 0x4F, int, flag, LgpID, CallBackProc)

__inl
int MsgBoxOkCancel(int flag, int LgpID, void CallBackProc(int))
__def( 0x50, int, flag, LgpID, CallBackProc)

__inl
char *GetIlluminationDataTable()
#ifdef NEWSGOLD
__def( 0x8052, char *)
#else
__def( 0x0052, char *)
#endif

__inl
int StrToInt(char *s, char **endp)
__def( 0x0054, int, s, endp)

__inl
void SetCpuClockLow(int const_2)
__defn( 0x221, const_2)

__inl
void SetCpuClockTempHi(int const_2)
__defn( 0x222, const_2)

__inl
unsigned char GetCPUClock()
__def( 92, unsigned char)

__inl
unsigned char GetCPULoad()
__def( 93, unsigned char)

__inl
int GetFreeRamAvail()
__def( 95, int)

__inl
int  FindFirstFile (DIR_ENTRY *DIRENTRY, const char *mask,unsigned int *ErrorNumber)
__def( 107, int , DIRENTRY, mask, ErrorNumber)

__inl
int  FindNextFile (DIR_ENTRY *DIRENTRY,unsigned int *ErrorNumber)
__def( 108, int , DIRENTRY, ErrorNumber)

__inl
int  FindClose (DIR_ENTRY *DIRENTRY,unsigned int *ErrorNumber)
__def( 109, int , DIRENTRY, ErrorNumber)

__inl
int IsCalling(void)
__nul_def( 0x06E, int)

__inl
void RefreshGPRSTraffic()
__defn( 111)

__inl
int *GetGPRSTrafficPointer()
__def( 112, int *)

__inl
int GetTypeOfCall()
__def( 0x0073, int)

__inl
void* GetFunctionPointer(char * _shortcut)
__def( 116, void*, _shortcut)

__inl
unsigned int SetVibration(unsigned int power)
__def( 124, unsigned int, power)

__inl
int ShowCallList(int list, int zero)
__def( 128, int, list, zero)

__inl
int GetFileStats(const char *cFileName, FSTATS * StatBuffer, unsigned int *errornumber)
__def( 132, int, cFileName, StatBuffer, errornumber)

__inl
int strcmpi(const char *s1, const char *s2)
__def( 0x0085, int, s1, s2)

__inl
void EEFullReadBlock(int block, void *buf, int offset, int size, int cepid, int msg)
__defn( 0x86, block, buf, offset, size, cepid, msg)

__inl
void EEFullWriteBlock(int block, void *buf, int offset, int size, int cepid, int msg)
__defn( 0x87, block, buf, offset, size, cepid, msg)

__inl
int GetFreeFlexSpace(int DriveNum, unsigned int *ErrorNumber)
__def( 138, int, DriveNum, ErrorNumber)

__inl
int GetTotalFlexSpace(int DriveNum, unsigned int *ErrorNumber)
__def( 139, int, DriveNum, ErrorNumber)

__inl
void RegExplorerExt(REGEXPLEXT const * param1)
__defn( 140, param1)

__inl
int fmove(const char * SourceFileName, const char * DestFileName, unsigned int *ErrorNumber)
__def( 142, int, SourceFileName, DestFileName, ErrorNumber)

__inl
int _rmdir(const char * cDirectory, unsigned int *ErrorNumber)
__def( 143, int, cDirectory, ErrorNumber)

__inl
int _truncate(int FileHandler, int length, int *errornumber)
__def( 144, int, FileHandler, length, errornumber)

__inl
int isdir(const char * cDirectory, unsigned int *ErrorNumber)
__def( 145, int, cDirectory, ErrorNumber)

#ifdef __NO_LIBC

#ifdef VALGRIND
void *val_calloc(size_t nmemb, size_t size, const char *file, int line);
#define calloc(n, sz) val_calloc(n, sz, __FILE__, __LINE__)
#else
__inl
void *calloc(size_t nelem, size_t elsize)
__def( 146, void *, nelem, elsize)
#endif

#endif

__inl
int ExecuteFile (WSHDR *filepath, WSHDR *mimetype, void *param)
__def( 148, int, filepath, mimetype, param)

__inl
int UnRegExplorerExt(REGEXPLEXT const * param1)
__def( 149, int, param1)

__inl
int PlayMelodyInMem(char Unk_0x11, void * MelAddr, int MelSize, int CepId, int Msg, int Unk_0)
__def( 0x99, int, Unk_0x11, MelAddr, MelSize, CepId, Msg, Unk_0)

__inl
int ChangeVolume(unsigned char Volume)
__def( 154, int, Volume)

__inl
void ws_2str(WSHDR *ws, char *str, unsigned int size)
__defn( 163, ws, str, size)

__inl
void ResetGPRSTraffic()
__defn( 166)

__inl
char *GetCurrentLocale()
__def( 170, char *)

__inl
void loopback171()
__defn( 171)

__inl
void GetDateTime(TDate * param1, TTime * param2)
__defn( 180, param1, param2)

__inl
char GetWeek(TDate* param1)
__def( 181, char, param1)

__inl
char GetProfile()
__def( 182, char)

__inl
char SetProfile(char param1)
__def( 183, char, param1)

__inl
int sdiv(int denom,int number)
__def( 184, int, denom, number)

__inl
char IsGPRSEnabled()
__def( 167, char)

__inl
char IsGPRSConnected()
__def( 168, char)

__inl
void KbdLock()
__defn( 169)

__inl
void KbdUnlock()
__defn( 185)

#ifdef __NO_LIBC

#ifdef VALGRIND
void *val_realloc(void *ptr, size_t size, const char *file, int line);
#define realloc(adr, sz) val_realloc(adr, sz, __FILE__, __LINE__)
#else
__inl
void *realloc(void *ptr, size_t size)
__def( 186, void *, ptr, size)

#endif

__inl
void *memset(void *s, int c, size_t n)
__def( 187, void *, s, c, n)
#endif

__inl
int udiv(unsigned int denom,unsigned int number)
__def( 188, int, denom, number)

__inl
int DivBy10(int divident)
__def( 189, int, divident)

__inl
void SpellTime()
__defn( 0x00BF)

__inl
void *GetBuffer(int patch_id)
__def( 0x00C4, void *, patch_id)

__inl
char GetAkkuCapacity()
__def( 198, char)

__inl
char GetAccessoryType(void)
__nul_def( 0x088, char)

__inl
char* RamAccPoint(void)
__def( 0x80D0 , char*)

__inl
void RebootPhone()
__defn( 0x93)

__inl
unsigned short *RamCap()
__def( 0x80C8, unsigned short *)

__inl
unsigned short *RamLS()
__def( 0x80C9, unsigned short *)

__inl
RAMNET * RamNet()
__def( 0x80CA, RAMNET *)

__inl
char * RamMissedCalls()
__def( 0x80CC, char *)

__inl
char * RamMissedSMS()
__def( 0x80CD, char *)

__inl
char * RamMissedMessage()
__def( 0x80D4, char *)

__inl
char * RamMenuAnywhere()
__def( 0x80D8, char *)

__inl
char *RamPressedKey()
__def( 0x80DF, char *)

__inl
void * RamScreenBuffer()
__def( 0x80E0, void *)

__inl
unsigned int *RamMPlayer_CtrlCode()
__def( 0x80E1, unsigned int *)

__inl
void *FreeRAM()
__def( 0x80E4, void *)

__inl
void *BuildCanvas(void)
__def( 0x80F1, void *)

__inl
unsigned int AddrLibrary()
__def( 0x80FF, unsigned int)

/*
__inl
void GBS_SendMessage(int cepid_to, int msg, ...)
__defn( 0x0100, cepid_to, msg, __VA_ARGS__)
*/

#define GBS_SendMessage(cepid_to, msg, ...) __cdefn(0x0100, (int)cepid_to, msg, ##__VA_ARGS__)

__inl
int GBS_ReciveMessage(GBS_MSG * param1)
__def( 0x0101, int, param1)

__inl
void GBS_AcceptMessage(void)
__nul_defn( 0x0102)

__inl
void GBS_ClearMessage(void)
__nul_defn( 0x0103)

__inl
void GBS_PendMessage(GBS_MSG * param1)
__defn( 0x0104, param1)

__inl
void CreateGBSproc(int cepid, const char *name, void (*onMsg)(void), int prio, int unk_zero)
__defn( 0x0105, cepid, name, onMsg, prio, unk_zero)

__inl
CSMROOT *CSM_root()
__def( 0x8106, CSMROOT *)

__inl
int CreateCSM(const CSM_DESC* param1,void * param2,int param3)
__def( 0x0107, int, param1, param2, param3)

__inl
CSM_RAM * FindCSMbyID (int id)
__def( 0x0108, CSM_RAM *, id)

__inl
void DoIDLE(void)
__defn( 0x0109)

__inl
void *GetConfig(unsigned int id)
__def( 0x010A, void *, id)

__inl
void *GetMinAdrScan()
__def( 0x810B, void *)

__inl
void *GetMaxAdrScan()
__def( 0x810C, void *)

__inl
unsigned short *GetCI()
__def( 0x810D, unsigned short *)

__inl
void Vibration(unsigned int power, unsigned int duration)
__defn( 0x010E, power, duration)

__inl
void freeUSSD()
__defn( 0x010F)

__inl
char *wstrcopy(unsigned short *dst, const unsigned short *src)
__def( 0x0110, char *, dst, src)

__inl
char GetTypeUSSD()
__def( 0x0111, char)

__inl
unsigned int SetUSSD(StructUSSDStr * param1)
__def( 0x0112, unsigned int, param1)

#ifdef __NO_LIBC
__inl
char * strpbrk (const char *s1,const char *s2)
__def( 0x0113, char *, s1, s2)

__inl
size_t strcspn(const char *s1, const char *s2)
__def( 0x0113, int, s1, s2)

__inl
char * strncat (char *dest, const char *substr, size_t maxSubLen)
__def( 0x0114, char *, dest, substr, maxSubLen)

__inl
int  strncmp (const char *s1,const char *s2,size_t n)
__def( 0x0115, int , s1, s2, n)

__inl
char * strncpy (char *dest,const char *source, size_t maxlen)
__def( 0x0116, char*, dest, source, maxlen)

__inl
char * strrchr (const char *s,int c)
__def( 0x0117, char *, s, c)

__inl
char * strstr (const char *s1,const char *s2)
__def( 0x0118, char *, s1, s2)

__inl
long  strtol (const char *nptr,char **endptr,int base)
__def( 0x0119, int , nptr, endptr, base)

__inl
unsigned long  strtoul (const char *nptr,char **endptr,int base)
__def( 0x011A, int , nptr, endptr, base)
#endif
/*
__inl
int snprintf (char * param1,int n,const char *format,...)
__def( 0x011B, int, param1, n, format, __VA_ARGS__)
*/
#ifdef __NO_LIBC
#define snprintf(param1, n, fmt, ...) __cdef(0x011B, int, (char*)param1, n, fmt, ##__VA_ARGS__)
#endif

#ifdef __NO_LIBC
__inl
int  memcmp (const void *m1,const void *m2, size_t n)
__def( 0x011C, int , m1, m2, n)
#endif

__inl
void  zeromem (void *dest,int n)
__defn( 0x011D, dest, n)

#ifdef __NO_LIBC
__inl
void * memcpy (void *dest,const void *source,size_t cnt)
__def( 0x011E, void *, dest, source, cnt)
#endif

__inl
WSHDR * wstrcpy (WSHDR *wshdr_d,WSHDR *wshdr_s)
__def( 0x011F, WSHDR *, wshdr_d, wshdr_s)

__inl
WSHDR * wstrncpy (WSHDR * param1,WSHDR * param2,int param3)
__def( 0x0120, WSHDR *, param1, param2, param3)

__inl
WSHDR * wstrcat (WSHDR *wshdr_d,WSHDR *wshdr_s)
__def( 0x0121, WSHDR *, wshdr_d, wshdr_s)

__inl
WSHDR * wstrncat (WSHDR *wshdr_d,WSHDR *wshdr_s,int n)
__def( 0x0122, WSHDR *, wshdr_d, wshdr_s, n)

__inl
int  wstrlen (WSHDR *wshdr)
__def( 0x0123, int , wshdr)

/*
__inl
int wsprintf (WSHDR * param1,const char *format,...)
__def( 0x0124, int, param1, format, __VA_ARGS__)
*/
#define wsprintf(ws, fmt, ...) __cdef(0x0124, int, ws, fmt, ##__VA_ARGS__)

__inl
WSHDR * AllocWS (int len)
__def( 0x0125, WSHDR *, len)

__inl
void  CutWSTR (void *WSHDR,int len)
__defn( 0x0126, WSHDR, len)

__inl
WSHDR * CreateLocalWS (WSHDR *wshdr,unsigned __int16 *wsbody,int len)
__def( 0x0127, WSHDR *, wshdr, wsbody, len)

__inl
int  CreateWS (void *_malloc,void *mfree,int len)
__def( 0x0128, int , _malloc, mfree, len)

__inl
void  FreeWS (WSHDR *wshdr)
__defn( 0x0129, wshdr)

__inl
void  DrawObject (DRWOBJ * param1)
__defn( 0x012A, param1)

__inl
void  AddKeybMsgHook (void *proc)
__defn( 0x012B, proc)

__inl
int  AddKeybMsgHook_end (void *proc)
__def( 0x012C, int , proc)

__inl
void  RemoveKeybMsgHook (void *proc)
__defn( 0x012D, proc)

__inl
char *GetPaletteAdrByColorIndex (int ColorIndex)
__def( 0x012E, char *, ColorIndex)

__inl
void  getRGBcolor (int ColorIndex,void *dest)
__defn( 0x012F, ColorIndex, dest)

__inl
void  getRGBbyPaletteAdr (int *paletteAdr,void *dest)
__defn( 0x0130, paletteAdr, dest)

__inl
void  setColor (int a,int r,int g,int b,void *dest)
__defn( 0x0131, a, r, g, b, dest)

#ifdef __NO_LIBC
__inl
void *memmove(void *dest,const void *source, size_t cnt)
__def( 0x0132, void *, dest, source, cnt)
#endif

__inl
void  StoreXYWHtoRECT (void *RECT,int x,int y,int w,int h)
__defn( 0x0133, RECT, x, y, w, h)

__inl
void  StoreXYXYtoRECT (void *RECT,int x,int y,int x2,int y2)
__defn( 0x0134, RECT, x, y, x2, y2)

__inl
int  IsGuiOnTop (int id)
__def( 0x0135, int , id)

__inl
int  CreateGUI_ID (void *gui,int id)
__def( 0x0136, int , gui, id)

__inl
int  CreateGUI (void *gui)
__def( 0x0137, int , gui)

__inl
int  CreateGUI_30or2 (void *gui)
__def( 0x0138, int , gui)

__inl
int  CreateGUIWithDummyCSM (void *gui,int flag)
__def( 0x0139, int , gui, flag)

__inl
int  CreateGUIWithDummyCSM_30or2 (void *gui,int flag)
__def( 0x013A, int , gui, flag)

__inl
void  GeneralFuncF1 (int cmd)
__defn( 0x013B, cmd)

__inl
void  GeneralFuncF0 (int cmd)
__defn( 0x013C, cmd)

__inl
void  GeneralFunc_flag1 (int id, int cmd)
__defn( 0x013D, id, cmd)

__inl
void  GeneralFunc_flag0 (int id, int cmd)
__defn( 0x013E, id, cmd)

__inl
void DirectRedrawGUI (void)
__defn( 0x013F)

__inl
void  DirectRedrawGUI_ID (int id)
__defn( 0x0140, id)

__inl
void PendedRedrawGUI (void)
__defn( 0x0141)

__inl
CSM_RAM * FindCSM (void *csm_q,int id)
__def( 0x0142, CSM_RAM *, csm_q, id)

__inl
void  GBS_StartTimer (GBSTMR *tmr,int time,int msg,int unk,int cepid)
__defn( 0x0143, tmr, time, msg, unk, cepid)

__inl
void  GBS_StopTimer (GBSTMR *tmr)
__defn( 0x0144, tmr)

__inl
int GBS_GetCurCepid (void)
__def( 0x0145, int)

__inl
void LockSched(void)
__defn( 0x0146)

__inl
void UnlockSched(void)
__defn( 0x0147)

__inl
int ShowMSG(int flag, int lgp_id)
__def( 0x0148, int, flag, lgp_id)

__inl
void SetPropTo_Obj1(DRWOBJ *drwobj,void *rect,int rect_flag,WSHDR *wshdr,int font,int text_flag)
__defn( 0x0149, drwobj, rect, rect_flag, wshdr, font, text_flag)

__inl
void FreeDrawObject_subobj(DRWOBJ *drwobj)
__defn( 0x014A, drwobj)

__inl
void SetColor(DRWOBJ *drwobj, const char *color1, const char *color2)
__defn( 0x014B, drwobj, color1, color2)

__inl
void DrawString(WSHDR *WSHDR,int x1,int y1,int x2,int y2,int font,int text_attribute,const char *Pen,const char *Brush)
__defn( 0x014C, WSHDR, x1, y1, x2, y2, font, text_attribute, Pen, Brush)

__inl
GUI *GetTopGUI(void)
__def( 0x014D, GUI *)

__inl
void *FindItemByID(GUI *gui,int id)
__def( 0x014E, void *, gui, id)

__inl
void *GetDataOfItemByID(GUI *gui,int id)
__def( 0x014F, void *, gui, id)

__inl
void DrawRoundedFrame(int x1,int y1,int x2,int y2,int x_round,int y_round,int flags,const char *pen,const char *brush)
__defn( 0x0150, x1, y1, x2, y2, x_round, y_round, flags, pen, brush)

__inl
void SetPropTo_Obj5(DRWOBJ * param1, RECT * param2, int zero, IMGHDR *Image)
__defn( 0x0151, param1, param2, zero, Image)

__inl
int CreateMenu(int is_small,int zero1,const MENU_DESC *menu, const HEADER_DESC *hdr,int start_item,int n_items,void *user_pointer,int *to_remove)
__def( 0x0152, int, is_small, zero1, menu, hdr, start_item, n_items, user_pointer, to_remove)

__inl
int GBS_RecActDstMessage(GBS_MSG * param1)
__def( 0x0153, int, param1)

__inl
void KillGBSproc(int cepid)
__defn( 0x0154, cepid)

#ifdef __SWILIB_PNG__
__inl
png_structp png_create_read_struct(png_const_charp user_png_ver, png_voidp error_ptr, png_error_ptr error_fn, png_error_ptr warn_fn)
__def( 0x0155, png_structp, user_png_ver, error_ptr, error_fn, warn_fn)

__inl
png_infop png_create_info_struct(png_structp png_ptr)
__def( 0x0156, png_infop, png_ptr)

__inl
void png_destroy_read_struct(png_structpp png_ptr_ptr, png_infopp info_ptr_ptr, png_infopp end_info_ptr_ptr)
__defn( 0x0157, png_ptr_ptr, info_ptr_ptr, end_info_ptr_ptr)

__inl
void png_set_sig_bytes(png_structp png_ptr,int num_bytes)
__defn( 0x0158, png_ptr, num_bytes)

__inl
void png_read_info(png_structp png_ptr, png_infop info_ptr)
__defn( 0x0159, png_ptr, info_ptr)

__inl
png_uint_32 png_get_IHDR(png_structp png_ptr,png_infop info_ptr, png_uint_32 *width, png_uint_32 *height, int *bit_depth, int *color_type, int *interlace_method, int *compression_method, int *filter_method)
__def( 0x015A, png_uint_32, png_ptr, info_ptr, width, height, bit_depth, color_type, interlace_method, compression_method, filter_method)

__inl
png_voidp png_malloc(png_structp png_ptr, png_uint_32 size)
__def( 0x015B, png_voidp, png_ptr, size)

__inl
void png_read_image(png_structp png_ptr, png_bytepp image)
__defn( 0x015C, png_ptr, image)

__inl
void png_read_end(png_structp png_ptr, png_infop info_ptr)
__defn( 0x015D, png_ptr, info_ptr)

__inl
png_voidp png_get_io_ptr(png_structp png_ptr)
__def( 0x015E, png_voidp, png_ptr)

__inl
png_uint_32 png_get_rowbytes(png_structp png_ptr,png_infop info_ptr)
__def( 0x015F, png_uint_32, png_ptr, info_ptr)

__inl
void png_read_row(png_structp png_ptr,png_bytep row,png_bytep dsp_row)
__defn( 0x0160, png_ptr, row, dsp_row)

__inl
void png_init_io(png_structp png_ptr, int fp)
__defn( 0x0161, png_ptr, fp)
#endif

#ifdef __NO_LIBC
__inl
int setjmp(jmp_buf jmpbuf)
__def( 0x0162, int, jmpbuf)
#endif

__inl
EDITQ *AllocEQueue(void *malloc_a, void *mfree_a)
__def( 0x0163, EDITQ *, malloc_a, mfree_a)

__inl
EDITCONTROL *PrepareEditControl(EDITCONTROL * param1)
__def( 0x0164, EDITCONTROL *, param1)

__inl
void ConstructEditControl(EDITCONTROL *EditControl,int type,int flag,WSHDR *ws,int maxlen)
__defn( 0x0165, EditControl, type, flag, ws, maxlen)

__inl
int AddEditControlToEditQend(void *EditQ,EDITCONTROL *EditControl,void *malloc_a)
__def( 0x0166, int, EditQ, EditControl, malloc_a)

__inl
int CreateInputTextDialog(const INPUTDIA_DESC *input_desc, const HEADER_DESC *header_desc,void *editq,int do_mfree,void *user_pointer)
__def( 0x0167, int, input_desc, header_desc, editq, do_mfree, user_pointer)

__inl
void SetSoftKey(void *gui,const SOFTKEY_DESC * param2,int n)
__defn( 0x0168, gui, param2, n)

__inl
int ExtractEditControl(void *guidata,int n,EDITCONTROL * param3)
__def( 0x0169, int, guidata, n, param3)

__inl
int StoreEditControl(void *guidata,int n,EDITCONTROL * param3)
__def( 0x016A, int, guidata, n, param3)

__inl
int _unlink(const char *cFileName,unsigned int *errornumber)
__def( 0x016B, int, cFileName, errornumber)

__inl
int str_2ws(WSHDR *ws,const char *str,unsigned int size)
__def( 0x016C, int, ws, str, size)

__inl
int UnpackABentry(AB_UNPRES * param1,void *abentry,int filesz,int flag)
__def( 0x016D, int, param1, abentry, filesz, flag)

__inl
int FreeUnpackABentry(AB_UNPRES * param1, void *mfree_adr)
__def( 0x016E, int, param1, mfree_adr)

__inl
int GetTypeOfAB_UNPRES_ITEM(int param1)
__def( 0x016F, int, param1)

__inl
int MakeVoiceCall(const char *number,int _0x10, int _0x20C0)
__def( 0x0170, int, number, _0x10, _0x20C0)

/*
__inl
void SUBPROC(void * param1,...)
__defn( 0x0171, param1, __VA_ARGS__)
*/

#define SUBPROC(proc, ...) __cdefn( 0x0171, proc, ##__VA_ARGS__)

__inl
void REDRAW(void)
__defn( 0x0172)

__inl
int socket(int af,int type,int protocol)
__def( 0x0173, int, af, type, protocol)

__inl
int connect(int sock,SOCK_ADDR * param2,int name_length)
__def( 0x0174, int, sock, param2, name_length)

__inl
int bind(int sock,SOCK_ADDR * param2,int name_length)
__def( 0x0175, int, sock, param2, name_length)

__inl
int closesocket(int socket)
__def( 0x0176, int, socket)

__inl
int shutdown(int socket,int how)
__def( 0x0177, int, socket, how)

__inl
int recv(int socket,void *buf,int len,int flag)
__def( 0x0178, int, socket, buf, len, flag)

__inl
int send(int socket,const void *buf,int len,int flag)
__def( 0x0179, int, socket, buf, len, flag)

__inl
unsigned short htons(unsigned short p)
__def( 0x017A, unsigned short, p)

__inl
unsigned int htonl(unsigned int p)
__def( 0x017B, unsigned int, p)

__inl
int *socklasterr(void)
__def( 0x817C, int *)

__inl
void SetIDLETMR(int time_ms,int msg)
__defn( 0x017D, time_ms, msg)

__inl
void RestartIDLETMR(void)
__defn( 0x017E)

__inl
void DisableIDLETMR(void)
__defn( 0x017F)

__inl
int EDIT_GetItemNumInFocusedComboBox(void *gui)
__def( 0x0180, int, gui)

__inl
void EDIT_SetTextToFocused(void *gui,WSHDR *ws)
__defn( 0x0181, gui, ws)

__inl
int EDIT_GetFocus(void *gui)
__def( 0x0182, int, gui)

__inl
int EDIT_GetUnFocus(void *gui)
__def( 0x0183, int, gui)

__inl
void ConstructComboBox(EDITCONTROL *EditControl,int type,int attr,WSHDR* param4,int maxlen,int unk_zero,int num_of_items,int start_item)
__defn( 0x0184, EditControl, type, attr, param4, maxlen, unk_zero, num_of_items, start_item)

__inl
int GBS_WaitForMsg(const int *msg_list, int msgs_num, GBS_MSG* param3, int timeout)
__def( 0x0185, int, msg_list, msgs_num, param3, timeout)

__inl
int RegisterCepidForSocketWork(REGSOCKCEPID_DATA * param1)
__def( 0x0186, int, param1)

__inl
int RequestLMANConnect(LMAN_DATA * param1)
__def( 0x0187, int, param1)

__inl
int ScreenW(void)
__def( 0x8188, int)

__inl
int ScreenH(void)
__def( 0x8189, int)

__inl
int HeaderH(void)
__def( 0x818A, int)

__inl
int SoftkeyH(void)
__def( 0x818B, int)

__inl
void GBS_DelTimer(GBSTMR * param1)
__defn( 0x018C, param1)

__inl
int available0(int sock)
__def( 0x018D, int, sock)

__inl
void GPRS_OnOff(int on, int _1)
__defn( 0x018E, on, _1)

__inl
int GetCurMenuItem(void *gui)
__def( 0x018F, int, gui)

__inl
void MutexCreate(MUTEX *mtx)
__defn( 0x0190, mtx)

__inl
void MutexDestroy(MUTEX *mtx)
__defn( 0x0191, mtx)

__inl
void MutexLock(MUTEX *mtx)
__defn( 0x0192, mtx)

__inl
void MutexLockEx(MUTEX *mtx, int flag)
__defn( 0x0193, mtx, flag)

__inl
void MutexUnlock(MUTEX *mtx)
__defn( 0x0194, mtx)

__inl
void SetMenuItemIcon(void *gui,int item_n,int icon_n)
__defn( 0x0195, gui, item_n, icon_n)

__inl
void RefreshGUI(void)
__defn( 0x0196)

__inl
void *AllocMenuItem(void *gui)
__def( 0x0197, void *, gui)

__inl
WSHDR *AllocMenuWS(void *gui, int len)
__def( 0x0198, WSHDR *, gui, len)

__inl
void SetMenuItemIconArray(void *gui,void *item,const int *icons)
__defn( 0x0199, gui, item, icons)

__inl
void SetMenuItemText(void *gui,void *item,WSHDR *text,int item_n)
__defn( 0x019A, gui, item, text, item_n)

__inl
void EDIT_SetFocus(void *gui, int item)
__defn( 0x019B, gui, item)

__inl
void SEQKILLER(void *data, void(*next_in_seq)(void *), void *data_to_kill)
__defn( 0x019C, data, next_in_seq, data_to_kill)

__inl
void *SEQKILLER_ADR()
__def( 0x819C, void *)

__inl
unsigned long long getCpuUsedTime_if_ena(void)
__def( 0x019D, unsigned long long)

__inl
void StartCpuUsageCount(void)
__defn( 0x019E)

#ifdef __SWI_LIBZ
__inl
int inflateInit2_ (z_streamp strm, int windowBits,const char *version, int stream_size)
__def( 0x019F, int, strm, windowBits, version, stream_size)

__inl
int inflate (z_streamp strm, int flush)
__def( 0x01A0, int, strm, flush)

__inl
int inflateEnd (z_streamp strm)
__def( 0x01A1, int, strm)
#endif

__inl
int ActivateDialUpProfile(unsigned int cep_id,int profile_num)
__def( 0x01A2, int, cep_id, profile_num)

__inl
int GetCurrentGPRSProfile(void)
__def( 0x01A3, int)

__inl
int GetDisconnectTime(void)
__def( 0x01A4, int)

__inl
void RegisterCepIdForCurProfile(unsigned int cep_id,int profile_num,unsigned int csd_or_gprs)
__defn( 0x01A5, cep_id, profile_num, csd_or_gprs)

__inl
int IsTimerProc(GBSTMR * param1)
__def( 0x01A6, int, param1)

__inl
char* Get_CC_NC()
__def( 0x1A7, char*)

#ifdef __SWILIB_PNG__
__inl
png_structp png_create_read_struct_2(png_const_charp user_png_ver, png_voidp error_ptr,
                                                 png_error_ptr error_fn, png_error_ptr warn_fn, png_voidp mem_ptr,
                                                 png_malloc_ptr malloc_fn, png_free_ptr free_fn)
__def( 0x1A8, png_structp, user_png_ver, error_ptr, error_fn, warn_fn, mem_ptr, malloc_fn, free_fn)

__inl
void png_set_read_fn(png_structp png_ptr, png_voidp io_ptr, png_rw_ptr read_data_fn)
__defn( 0x1A9, png_ptr, io_ptr, read_data_fn)

__inl
void png_read_update_info(png_structp png_ptr,void *info_ptr)
__defn( 0x1AA, png_ptr, info_ptr)

__inl
void png_set_palette_to_rgb(png_structp png_ptr)
__defn( 0x1AB, png_ptr)

__inl
png_uint_32 png_get_valid(png_structp png_ptr,png_infop info_ptr, png_uint_32 flag)
__def( 0x1AC, png_uint_32, png_ptr, info_ptr, flag)

__inl
void png_set_tRNS_to_alpha(png_structp png_ptr)
__defn( 0x1AD, png_ptr)

__inl
void png_set_filler(png_structp png_ptr,png_uint_32 filler, int flags)
__defn( 0x1AE, png_ptr, filler, flags)

__inl
void png_set_strip_16(png_structp png_ptr)
__defn( 0x1AF, png_ptr)

__inl
void png_set_packing(png_structp png_ptr)
__defn( 0x1B0, png_ptr)

__inl
void png_set_gray_to_rgb(png_structp png_ptr)
__defn( 0x1B1, png_ptr)

__inl
int png_check_sig(png_bytep sig, int num)
__def( 0x1B2, int, sig, num)
#endif

__inl
void DrawLine(int x,int y,int x2,int y2,int type,const char *pen)
__defn( 0x1B3, x, y, x2, y2, type, pen)

__inl
void DrawRectangle(int x,int y,int x2,int y2,int flags,const char *pen,const char *brush)
__defn( 0x1B4, x, y, x2, y2, flags, pen, brush)

__inl
int isnewSGold(void)
__def( 0x81B5, int)

__inl
int async_gethostbyname(const char *host_name,int ****DNR_RES,int *DNR_ID)
__def( 0x1B6, int, host_name, DNR_RES, DNR_ID)

__inl
unsigned int str2ip(const char * param1)
__def( 0x1B7, unsigned int, param1)

__inl
void *EXT_TOP(void)
__def( 0x81B8, void *)

__inl
void *PNG_TOP(void)
__def( 0x81B9, void *)

__inl
void *LIB_TOP(void)
__def( 0x81BA, void *)

/*
__inl
SFN_DATA *DATA_N_SFB(void)
__def( 0x81BB, SFN_DATA *)
*/

__inl
void SHA1_Init(SHA_CTX *c)
__defn( 0x1BC, c)

__inl
void SHA1_Update(SHA_CTX *c, const void *data, unsigned long len)
__defn( 0x1BD, c, data, len)

__inl
void SHA1_Final(char *md, SHA_CTX *c)
__defn( 0x1BE, md, c)

__inl
RSA * RSA_new_method(RSA_METHOD *method)
__def( 0x1BF, RSA *, method)

__inl
void RSA_free (RSA *r)
__defn( 0x1C0, r)

__inl
int RSA_sign(int type, char *m, unsigned int m_len, char *sigret, unsigned int *siglen, RSA *rsa)
__def( 0x1C1, int, type, m, m_len, sigret, siglen, rsa)

__inl
BIGNUM *BN_bin2bn(const char *s,int len,BIGNUM *ret)
__def( 0x1C2, BIGNUM *, s, len, ret)

__inl
void SetMenuItemCount(void *gui,int n)
__defn( 0x1C3, gui, n)

__inl
void SetCursorToMenuItem(void *gui,int n)
__defn( 0x1C4, gui, n)

__inl
int GetFontYSIZE(int font)
__def( 0x1C5, int, font)

__inl
void EDIT_ExtractFocusedControl(void *gui,EDITCONTROL *ec)
__defn( 0x1C6, gui, ec)

__inl
int EDIT_StoreControlToFocused(void *gui,EDITCONTROL *ec)
__def( 0x1C7, int, gui, ec)

__inl
EDITC_OPTIONS *PrepareEditCOptions(EDITC_OPTIONS *ec_options)
__def( 0x1C8, EDITC_OPTIONS *, ec_options)

__inl
void SetInvertToEditCOptions(EDITC_OPTIONS *ec_options, int invert)
__defn( 0x1C9, ec_options, invert)

__inl
void SetFontToEditCOptions(EDITC_OPTIONS *ec_options, int font)
__defn( 0x1CA, ec_options, font)

__inl
void SetAlignToEditCOptions(EDITC_OPTIONS *ec_options, int align)
__defn( 0x1CB, ec_options, align)

__inl
void SetPenColorToEditCOptions(EDITC_OPTIONS *ec_options, int pen)
__defn( 0x1CC, ec_options, pen)

__inl
void SetBrushColorToEditCOptions(EDITC_OPTIONS *ec_options, int brush)
__defn( 0x1CD, ec_options, brush)

__inl
void CopyOptionsToEditControl(EDITCONTROL *ec,EDITC_OPTIONS *ec_options)
__defn( 0x1CE, ec, ec_options)


#ifdef NEWSGOLD
#define TREGEXPLEXT REGEXPLEXT
#else
#define TREGEXPLEXT REGEXPLEXT_ARM_NEW
#endif

__inl
TREGEXPLEXT *get_regextpnt_by_uid(int uid)
__def( 0x1CF, TREGEXPLEXT *, uid)

__inl
int GetExtUid_ws(WSHDR * ext)
__def( 0x1D0, int, ext)

__inl
void EDIT_RemoveEditControl(void *gui,int n)
__defn( 0x1D1, gui, n)

__inl
void EDIT_InsertEditControl(void *gui,int n,EDITCONTROL * param3)
__defn( 0x1D2, gui, n, param3)

__inl
int EDIT_GetCursorPos(void *gui)
__def( 0x1D3, int, gui)

__inl
void EDIT_SetCursorPos(void *gui, int pos)
__defn( 0x1D4, gui, pos)

__inl
char *Get_Phone_Info(int InfoIndex)
__def( 0x1D5, char *, InfoIndex)

#ifdef __SWILIB_PNG__
__inl
void png_set_gray_1_2_4_to_8(png_structp png_ptr)
__defn( 0x1D6, png_ptr)

__inl
void png_set_invert_mono(png_structp png_ptr)
__defn( 0x1D7, png_ptr)
#endif

__inl
int EDIT_IsMarkModeActive(void *gui)
__def( 0x1D8, int, gui)

__inl
int EDIT_GetMarkedText(void *gui,WSHDR *dest)
__def( 0x1D9, int, gui, dest)

__inl
void SendSMS(WSHDR *ws,const char *num,int cepid,int msg,int flag)
__defn( 0x1DA, ws, num, cepid, msg, flag)

__inl
PDU *IncommingPDU(void)
__def( 0x81DB, PDU *)

__inl
short wstrchr(WSHDR *ws,unsigned int start_pos,unsigned int wchar)
__def( 0x1DC, short, ws, start_pos, wchar)

__inl
short wstrrchr(WSHDR *ws,unsigned int max_pos,unsigned int wchar)
__def( 0x1DD, short, ws, max_pos, wchar)

__inl
int EDIT_SetTextToEditControl(void *gui, int n, WSHDR *ws)
__def( 0x1DE, int, gui, n, ws)

__inl
void *EDIT_GetUserPointer(void *gui)
__def( 0x1DF, void *, gui)

__inl
void *MenuGetUserPointer(void *gui)
__def( 0x1E0, void *, gui)

__inl
int GetSymbolWidth(int wchar,int font)
__def( 0x1E1, int, wchar, font)

__inl
int ws_2utf8( WSHDR *from, char *to , int *result_length, unsigned int max_len)
__def( 0x1E2, int, from, to, result_length, max_len)

__inl
int utf8_2ws(WSHDR *ws,const char *utf8_str, unsigned int maxLen)
__def( 0x1E3, int, ws, utf8_str, maxLen)

__inl
int GetPicNByUnicodeSymbol(int wchar)
__def( 0x1E4, int, wchar)

__inl
int EDIT_OpenOptionMenuWithUserItems(void *gui, void (*itemhandler)(USR_MENU_ITEM *item), void *user_pointer, int to_add)
__def( 0x1E5, int, gui, itemhandler, user_pointer, to_add)

__inl
void Menu_SetItemCountDyn(void *gui, int n)
__defn( 0x1E6, gui, n)

__inl
char *GetLastJavaApplication()
__def( 0x1E7, char *)

__inl
int IsNoJava()
__def( 0x1E8, int)

__inl
IMGHDR *CreateIMGHDRFromPngFile(const char *fname, int type)
__def( 0x1E9, IMGHDR *, fname, type)

__inl
int GetSecondsFromDateTime(int *result, TDate *t_date, TTime *time, TDate *s_date)
__def( 0x2C6, int, result, t_date, time, s_date)

__inl
int GetSecondsFromDateTimeSince1997(int *result, TDate *date, TTime *time)
__def( 0x2C7, int, result, date, time)

__inl
int GetSecondsFromTime(TTime *time)
__def( 0x2C8, int, time)

__inl
int GetDateTimeFromSeconds(int *seconds, TDate *t_date, TTime *time, TDate *s_date)
__def( 0x2C9, int, seconds, t_date, time, s_date)

__inl
int GetDateTimeFromSecondsSince1997(int *seconds, TDate *date, TTime *time)
__def( 0x2CA, int, seconds, date, time)

__inl
void GetTimeFromSeconds(TTime *time, int seconds)
__defn( 0x2CB, time, seconds)

__inl
void InitDate(TDate *date, unsigned long year, unsigned char month, unsigned char day)
__defn( 0x2CC, date, year, month, day)

__inl
void InitTime(TTime *time, unsigned char hour, unsigned char min, unsigned char sec, unsigned long millisec)
__defn( 0x2CD, time, hour, min, sec, millisec)

__inl
int CmpDates(TDate *date1, TDate *date2)
__def( 0x2CE, int, date1, date2)

__inl
int CmpTimes(TTime *time1, TTime *time2)
__def( 0x2CF, int, time1, time2)

__inl
int GetDaysFromMonth(TDate *date)
__def( 0x2D0, int, date)

__inl
int IsLeapYear(TDate *date)
__def( 0x2D1, int, date)

__inl
void StartNativeBrowserCore()
__defn( 0x2D2)

__inl
int GetFilledNAbDataCount(NativeAbData *nab)
__def( 0x2D3, int, nab)

__inl
void *GetDataFromNAbData(NativeAbData *nab, int n)
__def( 0x2D4, void *, nab, n)

__inl
void UnpackNumPkt(PKT_NUM *pkt, char *buf, int maxlen)
__defn( 0x2D5, pkt, buf, maxlen)

__inl
void *TViewGetGUI(void *malloc_adr, void *mfree_adr)
__def( 0x2D6, void *, malloc_adr, mfree_adr)

__inl
void TViewSetDefinition(void *gui, const TVIEW_DESC * param2)
__defn( 0x2D7, gui, param2)

__inl
void TViewSetText(void *gui, WSHDR *text, void *malloc_adr, void *mfree_adr)
__defn( 0x2D8, gui, text, malloc_adr, mfree_adr)

__inl
void TViewSetUserPointer(void *gui,void *user_pointer)
__defn( 0x2D9, gui, user_pointer)

__inl
void *TViewGetUserPointer(void *gui)
__def( 0x2DA, void *, gui)

__inl
int SetGUIToTabGUI(void *tabgui, int n, const int *tabicons, void *gui)
__def( 0x2BD, int, tabgui, n, tabicons, gui)

__inl
int SetCursorTab(void *tab_gui, int n)
__def( 0x2BE, int, tab_gui, n)

__inl
int UpdateTab1stItem(void *tab_gui, int unk_0)
__def( 0x2BF, int, tab_gui, unk_0)

__inl
int GetCursorTab(void *tab_gui)
__def( 0x2C0, int, tab_gui)

__inl
void *GetGuiByTab(void *tab_gui, int n)
__def( 0x2C1, void *, tab_gui, n)

__inl
int GetSubprovider(WSHDR* ws)
__def( 0x2C2, int, ws)

__inl
int Registry_GetHMIKeyID(char *Keyword)
__def( 0x038, int, Keyword)

__inl
void Registry_SetResourcePath(int HMIKeyID, int Prio, WSHDR *Path)
__defn( 0x039, HMIKeyID, Prio, Path)

__inl
void Registry_DeleteResourcePath(int HMIKeyID, int Prio)
__defn( 0x03A, HMIKeyID, Prio)

__inl
void Registry_GetResourcePathFromLevel(int HMIKeyID1, int Prio, int *HMIKeyID2, WSHDR *Path)
__defn( 0x03E, HMIKeyID1, Prio, HMIKeyID2, Path)

__inl
void StartMidlet(char* param1)
__defn( 0x004B, param1)

__inl
void SaveMaxIllumination(int level)
__defn( 0x2DC, level)

__inl
void SetIlluminationoffTimeout(int time_sec)
__defn( 0x2DD, time_sec)

__inl
void IllumTimeRequest(int TimeMode, int Counter)
__defn( 0x2DE, TimeMode, Counter)

__inl
void IllumTimeRelease(int TimeMode, int Counter)
__defn( 0x2DF, TimeMode, Counter)

__inl
void MediaSendCSM_Open(WSHDR *file, WSHDR *path)
__defn( 0x2DB, file, path)

__inl
char *RamRingtoneStatus()
__def( 0x80CB, char *)

__inl
int RefreshAlarmClock()
__def( 0x291, int)

__inl
IMGHDR * GetIMGHDRFromThemeCache(int index)
__def( 0x2E0, IMGHDR *, index)


__inl
int GetMaxDynIcon(void)
__def( 0x1EA, int)

__inl
char *RAM_IMSI(void)
__def( 0x81EB, char *)

__inl
void FreeDynIcon(int picture)
__defn( 0x1EC, picture)

__inl
int IsDynIconBusy(int picture)
__def( 0x1ED, int, picture)

__inl
void SetDynIcon(int picture,IMGHDR *img,char *bitmap)
__defn( 0x1EF, picture, img, bitmap)

__inl
unsigned int GetSessionAge(void)
__def( 0x1F3, unsigned int)

/*
__inl
int sscanf(const char * param1, const char * param2, ...)
__def( 0x1F5, int, param1, param2, __VA_ARGS__)
*/
#ifdef __NO_LIBC
#define sscanf(dest, fmt, ...) __cdef(0x1F5, int, (char*)dest, fmt, ##__VA_ARGS__)
#endif

__inl
IMGHDR* GetPitAdrBy0xE200Symbol(int uni_symbol)
__def( 0x1F6, IMGHDR*, uni_symbol)

__inl
int GetPlayStatus(void)
__def( 0x1F8, int)

__inl
char *RamMediaIsPlaying()
__def( 0x81FB, char *)

__inl
void CloseCSM(int id)
__defn( 0x1FC, id)

__inl
void SetProp2ImageOrCanvas(DRWOBJ * param1, RECT * param2, int zero, IMGHDR *Image, int bleed_x, int bleed_y)
__defn( 0x201, param1, param2, zero, Image, bleed_x, bleed_y)

__inl
void fillTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int flag, char *pen, char *brush)
__defn( 0x202, x1, y1, x2, y2, x3, y3, flag, pen, brush)

__inl
void SetPixel(int x,int y,char *color)
__defn( 0x203, x, y, color)

__inl
void drawArc(int x1,int y1,int x2,int y2,int a1,int a2,int flag,char *pen,char *brush)
__defn( 0x204, x1, y1, x2, y2, a1, a2, flag, pen, brush)

__inl
void SetProp2Square_v1(void)
__defn( 0x205)

#ifdef __NO_LIBC
__inl
void srand(unsigned int seed)
__defn( 0x206, seed)
#endif

__inl
void DrawScrollString(WSHDR *WSHDR,int x1,int y1,int x2,int y2,int xdisp,int font,int text_attribute,const char *Pen,const char *Brush)
__defn( 0x207, WSHDR, x1, y1, x2, y2, xdisp, font, text_attribute, Pen, Brush)

__inl
int Get_WS_width(WSHDR *ws, int font)
__def( 0x208, int, ws, font)

__inl
void *FindGUIbyId(int id, CSM_RAM **csm)
__def( 0x209, void *, id, csm)

__inl
void EDIT_SetDate(void *gui,int n,TDate *dd)
__defn( 0x20A, gui, n, dd)

__inl
void EDIT_SetTime(void *gui,int n,TTime *tt)
__defn( 0x20B, gui, n, tt)

__inl
void EDIT_GetDate(void *gui,int n,TDate *dd)
__defn( 0x20C, gui, n, dd)

__inl
void EDIT_GetTime(void *gui,int n,TTime *tt)
__defn( 0x20D, gui, n, tt)

__inl
int ConstructEditTime(EDITCONTROL *EditControl,TTime *time)
__def( 0x20E, int, EditControl, time)

__inl
int ConstructEditDate(EDITCONTROL *EditControl,TDate *date)
__def( 0x20F, int, EditControl, date)

__inl
int CardExplGetCurItem(void *csm)
__def( 0x212, int, csm)

__inl
void CardExplGetFName(void *csm, int cur_item, WSHDR *dest)
__defn( 0x213, csm, cur_item, dest)

__inl
void wstrcpybypos(WSHDR *dest,WSHDR *src,int from,int len)
__defn( 0x214, dest, src, from, len)

__inl
void wsRemoveChars(WSHDR *ws,int from,int to)
__defn( 0x215, ws, from, to)

/*
__inl
void wstrcatprintf(WSHDR * param1,const char *format,...)
__defn( 0x216, param1, format, __VA_ARGS__)
*/
#define wstrcatprintf(param1, fmt, ...) __cdefn(0x216, param1, fmt, ##__VA_ARGS__)

__inl
int CreateMultiLinesMenu(int dummy,int dummy2,const ML_MENU_DESC * param3,const HEADER_DESC * param4,int to_item,int n)
__def( 0x217, int, dummy, dummy2, param3, param4, to_item, n)

__inl
void SetMLMenuItemText(void *data,void *item,WSHDR *ws1,WSHDR *ws2,int n)
__defn( 0x218, data, item, ws1, ws2, n)

__inl
void *AllocMLMenuItem(void *data)
__def( 0x219, void *, data)

__inl
int EDIT_SetTextInvert(void *gui,int cursor_pos,int len)
__def( 0x21A, int, gui, cursor_pos, len)

__inl
void EDIT_RemoveTextInvert(void *gui)
__defn( 0x21B, gui)

__inl
int EDIT_IsBusy(void *gui)
__def( 0x21C, int, gui)

__inl
int GSMTXT_Decode(WSHDR *dest,void *pkt,int len,int encoding_type, void *(*malloc_adr)(int), void (*mfree_adr)(void))
__def( 0x21D, int, dest, pkt, len, encoding_type, malloc_adr, mfree_adr)

__inl
void FocusGUI(int id)
__defn( 0x21E, id)

__inl
void UnfocusGUI(void)
__defn( 0x21F)

__inl
int PlayMelody_ChangeVolume(int handle,int volume)
__def( 0x225, int, handle, volume)

__inl
int PlayMelody_StopPlayback(int handle)
__def( 0x226, int, handle)

__inl
int PlayMelody_PausePlayback(int handle)
__def( 0x227, int, handle)

__inl
int PlayMelody_ResumePlayBack(int handle)
__def( 0x228, int, handle)

#ifdef __NO_LIBC
__inl
void longjmp(jmp_buf param1, int param2)
__defn( 0x229, param1, param2)
#endif

__inl
int GetLunarDate(TDate* pdate, TDate* pLdate)
__def( 0x234, int, pdate, pLdate)

__inl
int GetLunarYearID(int year)
__def( 0x235, int, year)

__inl
int GetLunarAnimal(int year)
__def( 0x236, int, year)

__inl
void FreeGSMTXTpkt(void *pkt)
__defn( 0x22A, pkt)

__inl
char *RamIconBar()
__def( 0x8237, char *)

__inl
TDateTimeSettings *RamDateTimeSettings(void)
__def( 0x822F, TDateTimeSettings *)

__inl
int GetTimeZoneShift(TDate * param1, TTime * param2, int timeZone)
__def( 0x230, int, param1, param2, timeZone)

#ifdef __NO_LIBC
__inl
int rand(void)
__def( 0x7E, int)
#endif

__inl
void SendAutoUSSDack(void)
__defn( 0x238)

__inl
int EEFullDeleteBlock(unsigned int block,int cepid,int msg)
__def( 0x239, int, block, cepid, msg)

__inl
int EEFullCreateBlock(unsigned int block,int size,int version,int cepid,int msg)
__def( 0x23A, int, block, size, version, cepid, msg)

__inl
void DisableIconBar(int disable)
__defn( 0x23B, disable)

__inl
void AddIconToIconBar(int pic, short* num)
__defn(0x27, pic, num)

__inl
int SetCurrentGPRSProfile(int profile)
__def( 0x23C, int, profile)

__inl
void MD5_Init(MD5_CTX *c)
__defn( 0x23D, c)

__inl
void MD5_Update(MD5_CTX *c, const void *data, unsigned long len)
__defn( 0x23E, c, data, len)

__inl
void MD5_Final(unsigned char *md, MD5_CTX *c)
__defn( 0x23F, md, c)

__inl
char *MD5_Hash(const unsigned char *data, unsigned long n, unsigned char *md)
__def( 0x240, char *, data, n, md)

__inl
void SetDateTime(TDate * param1, TTime * param2)
__defn( 0x241, param1, param2)

__inl
void gb2ws(WSHDR *ws, const char *s, int len)
__defn( 0x242, ws, s, len)

__inl
void ws2gb(WSHDR *ws, char *s, int len)
__defn( 0x243, ws, s, len)

__inl
int Base64Encode(void *inbuf, int insize, void *outbuf, int outsize)
__def( 0x244, int, inbuf, insize, outbuf, outsize)

__inl
int Base64Decode(char *inbuf, int insize, void *outbuf, int outsize, char *_null, int *unk5)
__def( 0x245, int, inbuf, insize, outbuf, outsize, _null, unk5)

__inl
int GetFileProp(FILE_PROP *wl,WSHDR *file,WSHDR *folder)
__def( 0x246, int, wl, file, folder)

__inl
void SetMenuSearchCallBack(void *gui, MenuSearchCallBack proc)
__defn( 0x24B, gui, proc)

__inl
int  GetPeripheryState (int device, int check_state)
__def( 0x24C, int , device, check_state)

__inl
int PlayMelody_SetPosition(int handle, int pos_ms)
__def( 0x24D, int, handle, pos_ms)

__inl
void* GetPlayObjById(int handle)
__def( 0x24E, void*, handle)

__inl
int GetPlayObjPosition(void *playobj_3D0, int *pos_ms)
__def( 0x24F, int, playobj_3D0, pos_ms)

__inl
int GetPlayObjDuration(void *playobj_3D0, int *pos_ms)
__def( 0x250, int, playobj_3D0, pos_ms)

__inl
int StartNativeExplorer(NativeExplorerData* data)
__def( 0x254, int, data)

__inl
void MenuSetUserPointer(void *gui,void *user_pointer)
__defn( 0x255, gui, user_pointer)

__inl
int CalcBitmapSize(short w,short h, char typy)
__def( 0x258, int, w, h, typy)

__inl
HObj Obs_CreateObject(int uid_in, int uid_out, int prio, int msg_callback, int emb4, int sync,unsigned int *ErrorNumber)
__def( 0x259, HObj, uid_in, uid_out, prio, msg_callback, emb4, sync, ErrorNumber)

__inl
int Obs_DestroyObject(HObj hObj)
__def( 0x25A, int, hObj)

__inl
int Obs_SetInput_File (HObj hObj, int unk_zero, WSHDR *path)
__def( 0x25B, int, hObj, unk_zero, path)

__inl
int Obs_GetInputImageSize (HObj hObj, short *w, short *h)
__def( 0x25C, int, hObj, w, h)

__inl
int Obs_SetOutputImageSize (HObj hObj, short w, short h)
__def( 0x25D, int, hObj, w, h)

__inl
int Obs_Start(HObj hObj)
__def( 0x25E, int, hObj)

__inl
int Obs_Output_GetPictstruct(HObj hObj, IMGHDR **img)
__def( 0x25F, int, hObj, img)

__inl
int Obs_Graphics_SetClipping(HObj hObj, short x, short y, short w, short h)
__def( 0x260, int, hObj, x, y, w, h)

__inl
int Obs_SetRotation(HObj hObj, int angle)
__def( 0x261, int, hObj, angle)

__inl
int Obs_GetInfo (HObj hObj, int unk_0or1)
__def( 0x262, int, hObj, unk_0or1)

__inl
int Obs_SetScaling (HObj hObj, int unk5)
__def( 0x263, int, hObj, unk5)

__inl
int Obs_TranslateMessageGBS (GBS_MSG *msg, OBSevent *event_handler)
__def( 0x264, int, msg, event_handler)

__inl
int Obs_Pause (HObj hObj)
__def( 0x265, int, hObj)

__inl
int Obs_Resume (HObj hObj)
__def( 0x266, int, hObj)

__inl
int Obs_Stop (HObj hObj)
__def( 0x267, int, hObj)

__inl
int Obs_Prepare (HObj hObj)
__def( 0x268, int, hObj)

__inl
int Obs_SetRenderOffset (HObj hObj,short x,short y)
__def( 0x269, int, hObj, x, y)

__inl
int Obs_SetPosition (HObj hObj,int ms)
__def( 0x26A, int, hObj, ms)

__inl
int Obs_Mam_SetPurpose (HObj hObj,char purpose)
__def( 0x26B, int, hObj, purpose)

__inl
int Obs_Sound_SetVolumeEx (HObj hObj, char vol, char delta)
__def( 0x26C, int, hObj, vol, delta)

__inl
int Obs_Sound_GetVolume (HObj hObj, char *vol)
__def( 0x26D, int, hObj, vol)

__inl
int Obs_Sound_SetPurpose (HObj hObj,int purpose)
__def( 0x26E, int, hObj, purpose)

__inl
int runMidletLinkablely(const char *midlet_name, WSHDR *filename)
__def( 0x283, int, midlet_name, filename)

__inl
void getProfileNameByN(WSHDR *ws, int n, int _0)
__defn( 0x296, ws, n, _0)

__inl
void UpdateMenuCursorItem(void *gui, int n)
__defn( 0x2A3, gui, n)

__inl
void *GetMenuGUI(void *malloc_adr, void *mfree_adr)
__def( 0x2A4, void *, malloc_adr, mfree_adr)

__inl
void *GetMultiLinesMenuGUI(void *malloc_adr, void *mfree_adr)
__def( 0x2A5, void *, malloc_adr, mfree_adr)

__inl
void SetMenuToGUI(void *gui, const void *menu_desc)
__defn( 0x2A7, gui, menu_desc)

__inl
int GetMenuItemCount(void *gui)
__def( 0x2A8, int, gui)

__inl
int CreatePopupGUI(int flag, void *user_pointer, const POPUP_DESC * param3, int lgp_msg)
__def( 0x2A9, int, flag, user_pointer, param3, lgp_msg)

__inl
int CreatePopupGUI_ws(int flag, void *user_pointer, const POPUP_DESC * param3, WSHDR *msg)
__def( 0x2AA, int, flag, user_pointer, param3, msg)

__inl
void *GetPopupUserPointer(void *gui)
__def( 0x2AB, void *, gui)

__inl
int DeleteSMS(int index, short *del_cnt)
__def( 0x2AC, int, index, del_cnt)

__inl
int SetSmsStatus(int index, int type)
__def( 0x2AD, int, index, type)

__inl
void *GetHeaderPointer(void *gui)
__def( 0x2AE, void *, gui)

__inl
void SetHeaderText(void *hdr_pointer, WSHDR *txt, void *malloc_adr, void *mfree_adr)
__defn( 0x2AF, hdr_pointer, txt, malloc_adr, mfree_adr)

__inl
void SetMenuSoftKey(void *gui, const SOFTKEY_DESC * param2,int n)
__defn( 0x2B0, gui, param2, n)

__inl
SMS_DATA_ROOT *SmsDataRoot()
__def( 0x82B1, SMS_DATA_ROOT *)

__inl
int GetSmsPosIndex(SMS_POS_INDEX_DATA *buf, int data_id)
__def( 0x2B2, int, buf, data_id)

__inl
NativeAbData *AllocNativeAbData(void)
__def( 0x2B3, NativeAbData *)

__inl
int FreeNativeAbData(NativeAbData *ab_data)
__def( 0x2B4, int, ab_data)

__inl
int GetNativeAbDataType(NativeAbData *nab, int n)
__def( 0x2B5, int, nab, n)

__inl
WSHDR *GetNumFromNativeAbData(NativeAbData *nab, int type, int unk0)
__def( 0x2B6, WSHDR *, nab, type, unk0)

__inl
int GetNativeAbDataStatus(NativeAbData *nab, int n)
__def( 0x2B7, int, nab, n)

__inl
int SetNumToNativeAbData(NativeAbData *abd, int unk, WSHDR *num)
__def( 0x2B8, int, abd, unk, num)

__inl
unsigned int OpenNativeAddressbook(int type, int unk1, int unk2, void *data)
__def( 0x2B9, unsigned int, type, unk1, unk2, data)

__inl
void SetHeaderIcon(void *hdr_pointer, const int *icon, void *malloc_adr, void *mfree_adr)
__defn( 0x2BA, hdr_pointer, icon, malloc_adr, mfree_adr)

__inl
void *GetTabGUI(void *malloc_adr, void *mfree_adr)
__def( 0x2BB, void *, malloc_adr, mfree_adr)

__inl
void SetGUIDefinition(void *gui, const void *definition)
__defn( 0x2BC, gui, definition)


__inl
int GetRoamingState()
__def( 0x2C3, int)

__inl
int GetProfileVolumeSetting(int profile, int type)
__def( 0x2C4, int, profile, type)

__inl
RAM_EMS_ADMIN *RAM_EMS_Admin()
__def( 0x82C5, RAM_EMS_ADMIN *)

__inl
char *RamDisplayLanguage()
__def( 0x829D, char *)

__inl
int GetExtUidByFileName_ws(WSHDR * fn)
__def( 0x2E1, int, fn)

__inl
int wstrcmp(WSHDR*ws1,WSHDR*ws2)
__def( 0x2E2, int, ws1, ws2)

__inl
int fexists(WSHDR* FileName)
__def( 0x2E3, int, FileName)

__inl
int SettingsAE_Update_ws(WSHDR * param1,int set,char * entry,char *keyword)
__def( 0x2E4, int, param1, set, entry, keyword)

__inl
int SettingsAE_Read_ws(WSHDR * param1,int set,char * entry,char *keyword)
__def( 0x2E5, int, param1, set, entry, keyword)

__inl
int SettingsAE_SetFlag(int val,int set,char * entry,char *keyword)
__def( 0x2E6, int, val, set, entry, keyword)

__inl
int SettingsAE_GetFlag(int *res,int set,char * entry,char *keyword)
__def( 0x2E7, int, res, set, entry, keyword)

__inl
int SettingsAE_Update(int val,int set,char * entry,char *keyword)
__def( 0x2E8, int, val, set, entry, keyword)

__inl
int SettingsAE_Read(int *res,int set,char * entry,char *keyword)
__def( 0x2E9, int, res, set, entry, keyword)

__inl
void* SettingsAE_GetEntryList(int set)
__def( 0x2EA, void*, set)

__inl
int SettingsAE_RemoveEntry(int set,char *entry,int flag)
__def( 0x2EB, int, set, entry, flag)

__inl
int i2c_transfer(I2C_MSG *msg)
__def( 0x2EC, int, msg)

__inl
int i2c_receive(I2C_MSG *msg)
__def( 0x2ED, int, msg)

__inl
char *RamExtendedCameraState()
__def( 0x8299, char *)

__inl
char *RamInputLanguage()
__def( 0x829C, char *)

__inl
void SetHeaderToMenu(void *gui, const HEADER_DESC *hdr, void *malloc_adr)
__defn( 0x2A6, gui, hdr, malloc_adr)

__inl
int elfclose(void* ex)
__def( 0x2EE, int, ex)

__inl
int _dlopen(const char *name)
__def( 0x2EF, int, name)

__inl
void *_dlsym(int handle, const char *name)
__def( 0x2F0, void *, handle, name)

__inl
int _dlclose(int handle)
__def( 0x2F1, int, handle)

#ifdef __NO_LIBC
__inl
int setenv(const char *name, const char *value, int replace)
__def( 0x2F2, int, name, value, replace)

__inl
int unsetenv(const char *name)
__def( 0x2F3, int, name)

__inl
char * getenv(const char *var)
__def( 0x2F4, char *, var)

__inl
int clearenv(void)
__def( 0x2F5, int)

__inl
char ** getBaseEnviron()
__def( 0x2F6, char **)

__inl
const char * dlerror()
__def( 0x2F7, const char *)
#endif

__inl
int dlclean_cache()
__def( 0x2F8, int)

__inl
void * SHARED_TOP()
__def( 0x2F9, void *)

__inl
 int CopyTextToClipboard(WSHDR *src, int pos, int len)
 __def( 0x301, int, src, pos, len)

void kill_elf();

#ifdef __cplusplus
}
#endif

#endif


