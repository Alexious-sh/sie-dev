//tbase.h - ������ � ����� ���������� ��� ���������� ����������� ������
//(C)Dimadze
//v1.2

#ifndef __VPATCHER_H__
#define __VPATCHER_H__

void EnableINT(int fi);
int  DisableINT();
void CleanCache(void *vaddress);
void CleanTLB();
int *GetTBaseAddr();
void UnLockAccess();
void ExecuteIMB();

//��������� �������� ����� �� ������
char GetByteFromAddress(void *addr);
//����������� ������ ����� �� ������ (���������� �����)
int  ApplyPatch(void *addr, char new_byte);
//����������� ������ ����� �� ������ (������ �����)
int  UndoPatch(void *addr, char old_byte);

/* ��� ������ ���� ������ ���� ������ */
/* �������� ������ �������� */
/* ���� ��� ������� flash ������� ���� ��� ������ */
/* ������� RAM, ������� ��������� ��� flash �������� */
/* �� ����� ������� ��� ������, ������ ������ */
/* ����� � ��� ������ ���� ���������������� ������ */


#endif
