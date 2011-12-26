#include <swilib.h>
#include <stdlib.h>
#include <stdio.h>

#include "pthread.h"
#include "test.h"


unsigned short maincsm_name_body[140];
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


pgbs_proc thread, thread1;
pthread_t thread3;
MUTEX mutex;
int scr_w, scr_h;
int re = 0, kill_me = 0;
/*------------------------------------------------------------------------*/
/*--------------------- �������� ��� �������� � ��� ----------------------*/
/*------------------------------------------------------------------------*/

char clrWhite[]= {0xFF,0xFF,0xFF,0x64};
char clrBlack[]= {0x00,0x00,0x00,0x64};



void OnRedraw(MAIN_GUI *data)
{
    DrawRoundedFrame(0,0,scr_w,scr_h,0,0,0,clrBlack,clrWhite);
    wsprintf(data->ws, "%d %X %d", GetCPULoad(), getEmptyGbsProcess(0x43), re);
    DrawString(data->ws,0,28,scr_w-1,scr_h-1,FONT_SMALL,2+32,GetPaletteAdrByColorIndex(0),GetPaletteAdrByColorIndex(1));
}


void test_proc(int ac, void *av)
{
    for(int i=0; i<34; i++)
    {
        printf("Thread 1: %d\n", i);
        re ++;
    }

    ShowMSG(1, (int)"Oo");
}


static void onCreate(MAIN_GUI *data, void *(*malloc_adr)(int))
{
    data->ws = AllocWS(128);
    data->gui.state=1;
    setvbuf(stdout, 0, _IOFBF, 40*1024);

    printf("CepID: %X\n", pgbs_create(&thread1, "test_proc", test_proc, 0x43, 0x1, 0, 0));
    //pthread_create(&thread3, NULL, (void *(*) (void *))test_proc, NULL);
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

        case '5':
            break;
        }
    }
    REDRAW();
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
    /* ������������ ������ ��� ����� �������� ������������ ���� � _main � ����� ���� */
    MutexUnlock(&mutex);
    kill_me = 1;
}


static void maincsm_onclose(CSM_RAM *csm)
{
    ElfKiller();
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
        139,
        0
    }
};

void UpdateCSMname(void)
{
    wsprintf((WSHDR *)(&MAINCSM.maincsm_name),"NatAlarm");
}


void on_close()
{
    fflush(stdout);
    _close(fileno(stdout), 0);
}


int _main(char *exename, char *fname)
{
    int fd = _open("0:\\Misc\\class\\stdout.txt", A_Create | A_Truncate | A_WriteOnly | A_BIN, P_WRITE, 0);
    __setup_stdout_fd(fd);
    atexit(on_close);

    MAIN_CSM main_csm;
    LockSched();
    UpdateCSMname();
    CreateCSM(&MAINCSM.maincsm,&main_csm,0);
    UnlockSched();

    /* ���� ���� ������ �� �������������� */
    MutexLock(&mutex);

    /* ���� ������ �������������� - ������� ��� */
    MutexDestroy(&mutex);

    /* ������� �� ����� */
    kill_elf();
    ShowMSG(1, (int)"Exit");

    /* ����� ���������� ������� ������� ������� ������������� */
    return 0;
}


/* ������ ������ ����������� � �������� */
int main()
{
    /* ������ ������� � ������ "Test" � ������ 0x43 � ����������� 0x80 */
    pgbs_create(&thread, "Test", (void (*)(int, void*))_main, 0x43, 0x80, 0, 0);

    /* ������ ������ */
    MutexCreate(&mutex);

    /* ����� ��� */
    MutexLock(&mutex);
}



