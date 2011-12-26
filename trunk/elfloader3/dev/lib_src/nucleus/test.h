

#ifndef __PGBS_THREAD_H__
#define __PGBS_THREAD_H__


typedef struct
{
    unsigned short cepid;
    void (*proc)(int, void*);
    int argc;
    void *argv;
}pgbs_proc;


unsigned short getEmptyGbsProcess(unsigned char group);

int pgbs_create(pgbs_proc *th, const char *name, void (*proc)(int, void*), unsigned char group, unsigned int prio, int argc, void *argv);



#endif

