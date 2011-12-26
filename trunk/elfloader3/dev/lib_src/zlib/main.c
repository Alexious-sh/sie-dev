#include <swilib.h>


unsigned short maincsm_name_body[140];
unsigned int MAINCSM_ID = 0;
unsigned int MAINGUI_ID = 0;
const int minus11=-11;
int my_csm_id=0;

typedef struct
{
    CSM_RAM csm;
    int gui_id;
} MAIN_CSM;

typedef struct
{
    GUI gui;
    WSHDR *ws;
} MAIN_GUI;

/*------------------------------------------------------------------------*/
/*--------------------- Создание цсм процесса и гуя ----------------------*/
/*------------------------------------------------------------------------*/

char clrWhite[]= {0xFF,0xFF,0xFF,0x64};
char clrBlack[]= {0x00,0x00,0x00,0x64};

int scr_w,scr_h;

static void OnRedraw(MAIN_GUI *data)
{
    DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,clrBlack,clrWhite);

    wsprintf(data->ws, "%t\nHello World","Привет мир");

    DrawString(data->ws,0,28,scr_w-1,scr_h-1,FONT_SMALL,2+32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
}

static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
    data->ws = AllocWS(128);
    data->gui.state=1;
}

static void onClose(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    data->gui.state=0;
    FreeWS( data->ws );
}

static void onFocus(MAIN_GUI *data, void *(*malloc_adr)(int), void (*mfree_adr)(void *))
{
    data->gui.state=2;
    DisableIDLETMR();
#ifdef ELKA
    DisableIconBar(1);
#endif
}

static void onUnfocus(MAIN_GUI *data, void (*mfree_adr)(void *))
{
    if (data->gui.state!=2) return;
    data->gui.state=1;
}

static int OnKey(MAIN_GUI *data, GUI_MSG *msg)
{
    if ((msg->gbsmsg->msg==KEY_DOWN || msg->gbsmsg->msg==LONG_PRESS))
    {
        switch(msg->gbsmsg->submess)
        {
        case RIGHT_SOFT:
            return (1);
        }
    }
    return(0);
}

extern void kill_data(void *p, void (*func_p)(void *));

int method8(void)
{
    return(0);
}
int method9(void)
{
    return(0);
}

const void * const gui_methods[11]=
{
    (void *)OnRedraw,
    (void *)onCreate,
    (void *)onClose,
    (void *)onFocus,
    (void *)onUnfocus,
    (void *)OnKey,
    0,
    (void *)kill_data,
    (void *)method8,
    (void *)method9,
    0
};


const RECT Canvas= {0,0,0,0};
static void maincsm_oncreate(CSM_RAM *data)
{
    scr_w=ScreenW()-1;
    scr_h=ScreenH()-1;
    MAIN_CSM*csm=(MAIN_CSM*)data;
    MAIN_GUI *main_gui=malloc(sizeof(MAIN_GUI));
    zeromem(main_gui,sizeof(MAIN_GUI));
    main_gui->gui.canvas=(void *)(&Canvas);
    main_gui->gui.methods=(void *)gui_methods;
    main_gui->gui.item_ll.data_mfree=(void (*)(void *))mfree_adr();
    csm->csm.state=0;
    csm->csm.unk1=0;
    my_csm_id=csm->gui_id=CreateGUI(main_gui);
}

void ElfKiller(void)
{
    kill_elf();
}

static void maincsm_onclose(CSM_RAM *csm)
{
    SUBPROC((void *)ElfKiller);
}

static int maincsm_onmessage(CSM_RAM *data, GBS_MSG *msg)
{
    MAIN_CSM *csm=(MAIN_CSM*)data;
    if ((msg->msg==MSG_GUI_DESTROYED)&&((int)msg->data0==csm->gui_id))
    {
        csm->csm.state=-3;
    }
    return(1);
}


static const struct
{
    CSM_DESC maincsm;
    WSHDR maincsm_name;
} MAINCSM =
{
    {
        maincsm_onmessage,
        maincsm_oncreate,
#ifdef NEWSGOLD
        0,
        0,
        0,
        0,
#endif
        maincsm_onclose,
        sizeof(MAIN_CSM),
        1,
        &minus11
    },
    {
        maincsm_name_body,
        NAMECSM_MAGIC1,
        NAMECSM_MAGIC2,
        0x0,
        139
    }
};

void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NatAlarm");
}

#include <time.h>
#include <math.h>


int main(char *exename, char *fname)
{
    char buf[128];

    time_t _time;
    time(&_time);

    struct tm *t = localtime(&_time);

    sprintf(buf, "%d %d %g", t->tm_min, t->tm_mday, 3.7/2.2  );
    ShowMSG(1, (int)buf);

    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();
    return 0;
}
