
#include <swilib.h>
#include "test.h"

int pgbs_destroy(pgbs_proc *th);
unsigned short getEmptyGbsProcess(unsigned char);
void* (*GBS_ProcData)(unsigned short cepid) = (void* (*)(unsigned short))0xA0098F70;


void __tmp_proc_func()
{
    GBS_MSG msg;
    if (GBS_RecActDstMessage(&msg))
    {
        if (msg.msg==1 && msg.data0)
        {
            pgbs_proc *th = (pgbs_proc*)msg.data0;
            if(!th->proc) return;
            th->proc(th->argc, th->argv);
            pgbs_destroy(th);
        }
    }
}


int pgbs_create(pgbs_proc *th, const char *name, void (*proc)(int, void*), unsigned char group, unsigned int prio, int argc, void *argv)
{
    if(!th || !name || !proc) return -1;

    unsigned short cepid = getEmptyGbsProcess(group);
    if(cepid == 0xFFFF) return -2;

    th->argc = argc;
    th->argv = argv;
    th->cepid = cepid;
    th->proc = proc;

    CreateGBSproc(cepid, name, __tmp_proc_func, prio, 1);
    GBS_SendMessage(cepid, 1, 0, th, 0);

    return cepid;
}



int pgbs_destroy(pgbs_proc *th)
{
    if(!th) return -1;
    KillGBSproc(th->cepid);
    return 0;
}



int isPosibleToCreateProcessInGroup(unsigned short cepid)
{
    int ret = 0;
    asm(
        "LDR     R1, =0xA8D37F38 \n"
        "MOV     R0, %1, LSR#8 \n"
        "ADD     R0, R1, R0, LSL#2 \n"
        "LDR     R1, [R0,#8] \n"
        "LDR     R0, [R1,#4] \n"
        "LDR     R1, =0x70726770 \n"
        "CMP     R0, R1 \n"
        "MOVEQ   %0, #1 \n"
        "MOVNE   %0, #0 \n"
        :"=r"(ret)
        :"r"(cepid)
        :
        );

    return ret;
}


unsigned short getEmptyGbsProcess(unsigned char group)
{
    unsigned short id = group;
    for(int i = id << 8; i<65536; i++)
    {
        if(!GBS_ProcData(i))
        {
            if(isPosibleToCreateProcessInGroup(i))
                return i;
        }
    }
    return 0xFFFF;
}



