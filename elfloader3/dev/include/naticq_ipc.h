#ifndef _NATICQ_IPC_H
#define _NATICQ_IPC_H

#define IPC_SENDMSG 5

#define IPC_RECVMSG 6

typedef struct{
  unsigned long uin;
  char *msg;
}IPCMsg;

typedef struct
{
  unsigned long uin;
  char *msg;
  int drop_msg; // не 0, если сообщение необходимо выбросить
  IPC_REQ *ipc; // указатель на родительский IPC_REQ, не трогать!!!
}IPCMsg_RECVMSG;

#endif
