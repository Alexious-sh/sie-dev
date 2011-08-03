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
  int drop_msg; // �� 0, ���� ��������� ���������� ���������
  IPC_REQ *ipc; // ��������� �� ������������ IPC_REQ, �� �������!!!
}IPCMsg_RECVMSG;

#endif
