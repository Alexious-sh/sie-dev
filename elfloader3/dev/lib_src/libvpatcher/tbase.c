//tbase.c - ������ � ����� ���������� ��� ���������� ����������� ������
//(C)Dimadze
//v1.2

#include <swilib.h>
#include "tbase.h"

int fiq_irq_value = 0;

void LockOSAction()
{
 LockSched();
 fiq_irq_value = DisableINT();
}

void UnLockOSAction()
{
 EnableINT(fiq_irq_value);
 UnlockSched();
}



void *virtual2physical(void *addr)
{
 //��������� ���� ������
 UnLockAccess();

 //�������� ����� ���� �������������
 int *trans_base = GetTBaseAddr();

 //���� ������� ������� ������� ������ ���� ������
 unsigned short element = ((int)addr>>0x14)&0xFFF;
 //���� ������� ������� ������� ������ ���� 0
 if (((trans_base[element])&0x03) == 0x00) return (void *)0xFFFFFFFF;

 //���� ������� ������� ������� ������ ���� ������ ������� ������� ������
 if (((trans_base[element])&0x03) == 0x01)
  {
   //��������� ������ ������� ������� ������ � ����� ������
   int *coarse_table = (int *)(((int)trans_base[element])&0xFFFFFE00);
   unsigned short subelement = ((int)addr>>0x0C)&0xFF;

   //���� ������� ������� ������ ������ ���� 0
   if (((coarse_table[subelement])&0x03) == 0x00) return (void *)0xFFFFFFFF;

   //���� ������� ������� ������ ������ ���� ������� �������� (64 ��)
   if (((coarse_table[subelement])&0x03) == 0x01)
    {
     int addr_page = (int)(((int)coarse_table[subelement])&0xFFFF0000);
     int offset = ((int)addr)&0xFFFF;
     return (void *)(addr_page + offset);
    }

   //���� ������� ������� ������ ������ ���� ��������� �������� (4 ��)
   if (((coarse_table[subelement])&0x03) == 0x02)
    {
     int addr_page = (int)(((int)coarse_table[subelement])&0xFFFFF000);
     int offset = ((int)addr)&0xFFF;
     return (void *)(addr_page + offset);
    }
  }

  //���� ������� ������� ������� ������ ���� ������
  if (((trans_base[element])&0x03) == 0x02)
  {
    int addr_section = (int)(((int)trans_base[element])&0xFFF00000);
    int offset = ((int)addr)&0xFFFFF;
    return (void *)(addr_section + offset);
  }

 return addr;
}



char GetByteFromAddress(void *addr)
{
 return *((char *)addr);
}


int ApplyPatch(void *addr, char new_byte)
{
 //��������� ���� ������
 UnLockAccess();

 //�������� ����� ���� �������������
 int *trans_base = GetTBaseAddr();
 //�������� �� ����������� ������ ����� � �������� flash
 if (((int)addr >= 0xA0000000) && ((int)addr < 0xA4000000) && trans_base)
 {
  unsigned short element = ((int)addr>>0x14)&0xFFF;

  //���� ������� ������� ������� ������ �����������
  if (((trans_base[element])&0xFF) == 0x1E)
   {
    //��������� ������ � 1 �� �� ������ ������� �� 256 ������� �� 4 ��

    //������ ������ � ��������� ������ ������
    char *addr_section = (char *)(((int)addr)&0xFFF00000);
    int *coarse_table = (int *)malloc(0x100*4+0xFFF+4);
         coarse_table = (int *)virtual2physical((void *)coarse_table);

    //�������������� �������� ��� ���������� �������, ���� ����� �� ����������
    unsigned short element_d1 = ((int)coarse_table>>0x14)&0xFFF;
    if (((int)(trans_base[element_d1])&&0x03) == 0x00)
    trans_base[element_d1] = (((int)coarse_table)&0xFFF00000) + 0xFFE;
    if (((int)(trans_base[element_d1])&&0x03) == 0x01)
     {
      //��������� ������ ������� ������� ������ � ����� ������
      int *coarse_table_d = (int *)(((int)trans_base[element_d1])&0xFFFFFE00);
      unsigned short subelement_d = ((int)coarse_table>>0x0C)&0xFF;
      if (((int)(coarse_table_d[subelement_d])&&0x03) == 0x00)
      coarse_table_d[subelement_d] = (((int)coarse_table)&0xFFFFF000) + 0xFFE;
     }

    //����������� �����
    int *coarse_table_a = (int *)(((int)coarse_table + 0xFFF)&0xFFFFF000);

    //��������������� ���� ��������
    for (int i = 0; i < 0x100; i++) coarse_table_a[i] = (int)addr_section + 0x1000*i + 0x0E;

    //��������� ������������� ����� ������� ��������� ���������
    coarse_table_a[0x100] = (int)coarse_table;

    //������ ������ � ��������� ������ ��������
    unsigned short subelement = ((int)addr>>0x0C)&0xFF;
    char *addr_page = (char *)((coarse_table_a[subelement])&0xFFFFF000);
    char *rampage = (char *)malloc(0x1000 + 0xFFF + 4);
          rampage = (char *)virtual2physical((void *)rampage);

    //�������������� �������� ��� ���������� �������, ���� ����� �� ����������
    unsigned short element_d2 = ((int)rampage>>0x14)&0xFFF;
    if (((int)(trans_base[element_d2])&&0x03) == 0x00)
    trans_base[element_d2] = (((int)rampage)&0xFFF00000) + 0xFFE;
    if (((int)(trans_base[element_d2])&&0x03) == 0x01)
     {
      //��������� ������ ������� ������� ������ � ����� ������
      int *coarse_table_d = (int *)(((int)trans_base[element_d2])&0xFFFFFE00);
      unsigned short subelement_d = ((int)rampage>>0x0C)&0xFF;
      if (((int)(coarse_table_d[subelement_d])&&0x03) == 0x00)
      coarse_table_d[subelement_d] = (((int)rampage)&0xFFFFF000) + 0xFFE;
     }

    //����������� ����� � �������� ���� ���������� flash-��������
    char *rampage_a = (char *)(((int)rampage + 0xFFF)&0xFFFFF000);
    memcpy(rampage_a, addr_page, 0x1000);

    //��������� ������������� ����� ��������
    int *rampage_int = (int *)rampage_a;
    rampage_int[0x1000] = (int)rampage;

    //��������� ������ � �������� � ��� �������
    short patch_byte =  ((int)addr)&0xFFF;
    rampage_a[patch_byte] = new_byte;

    LockOSAction();

    //������������ ������ ������� � ������� �������
    coarse_table_a[subelement] = (int)rampage_a + 0x0E;
    trans_base[element] = (int)coarse_table_a + 0x11;

    //���������� � �����
    CleanCache(addr);
    CleanTLB();

    UnLockOSAction();
    return 1;
   }
    //���� ��� ������ �� ��������
    else
     {
      //��������� ������ ������� ������� ������ � ����� ������
      int *coarse_table = (int *)(((int)trans_base[element])&0xFFFFF000);
      unsigned short subelement = ((int)addr>>0x0C)&0xFF;
      char stat_page = (coarse_table[subelement]&0x0F000000)>>0x18;

      //���� flash �������� �� � RAM
      if ((stat_page > 0x00 && stat_page < 0x04) || stat_page == 0x00)
       {
        //������ ������ � ��������� ������ ��������
        char *addr_page = (char *)((coarse_table[subelement])&0xFFFFF000);
        char *rampage = (char *)malloc(0x1000 + 0xFFF + 4);
              rampage = (char *)virtual2physical((void *)rampage);

        //�������������� �������� ��� ���������� �������, ���� ����� �� ����������
        unsigned short element_d2 = ((int)rampage>>0x14)&0xFFF;
        if (((int)(trans_base[element_d2])&&0x03) == 0x00)
        trans_base[element_d2] = (((int)rampage)&0xFFF00000) + 0xFFE;
        if (((int)(trans_base[element_d2])&&0x03) == 0x01)
        {
          //��������� ������ ������� ������� ������ � ����� ������
          int *coarse_table_d = (int *)(((int)trans_base[element_d2])&0xFFFFFE00);
          unsigned short subelement_d = ((int)rampage>>0x0C)&0xFF;
          if (((int)(coarse_table_d[subelement_d])&&0x03) == 0x00)
          coarse_table_d[subelement_d] = (((int)rampage)&0xFFFFF000) + 0xFFE;
         }

        //����������� ����� � �������� ���� ���������� flash-��������
        char *rampage_a = (char *)(((int)rampage + 0xFFF)&0xFFFFF000);
        memcpy(rampage_a, addr_page, 0x1000);

        //��������� ������������� ����� ��������
        int *rampage_int = (int *)rampage_a;
        rampage_int[0x1000] = (int)rampage;

        //��������� ������ � �������� � ��� �������
        short patch_byte =  ((int)addr)&0xFFF;
        rampage_a[patch_byte] = new_byte;

        //���������� ��
        LockOSAction();


        //���������� � ������� ������� ������
        coarse_table[subelement] = (int)rampage_a + 0x0E;

        //���������� � �����
        CleanCache(addr);
        CleanTLB();

        //������������� ��
        UnLockOSAction();
        return 1;
       }

       //���� flash �������� � RAM
       else
         {
          //��������� ������ ��������
          char *rampage = (char *)((coarse_table[subelement])&0xFFFFF000);

          //���������� ��
          LockOSAction();

          //������������� ������� �� ������
          coarse_table[element] = coarse_table[element]|0xFF0;

          //��������� ������ � �������� � ��� �������
          short patch_byte =  ((int)addr)&0xFFF;
          rampage[patch_byte] = new_byte;

          //���������� ������� �� ������
          coarse_table[subelement] = (int)rampage + 0x0E;

          //���������� � �����
          CleanCache(addr);
          CleanTLB();

          //������������� ��
          UnLockOSAction();
          return 1;
         }

     }
 }
  else return 0;
}

int UndoPatch(void *addr, char old_byte)
{
 //��������� ���� ������
 UnLockAccess();

 //�������� ����� ���� �������������
 int *trans_base = GetTBaseAddr();
 //�������� �� ����������� ������ ����� � �������� flash
 if (((int)addr >= 0xA0000000) && ((int)addr < 0xA4000000) && trans_base)
 {
  unsigned short element = ((int)addr>>0x14)&0xFFF;

  //���� ������� ������� ������� ������ �������������
  if (((trans_base[element])&0xFF) == 0x11)
   {
      //��������� ������ ������� ������� ������ � ����� ������
      int *coarse_table = (int *)(((int)trans_base[element])&0xFFFFF000);
      unsigned short subelement = ((int)addr>>0x0C)&0xFF;
      char stat_page = (coarse_table[subelement]&0x0F000000)>>0x18;

      //���� flash �������� ��� � RAM
      if (stat_page > 0x03)
       {
          //��������� ������ ��������
          char *rampage = (char *)((coarse_table[subelement])&0xFFFFF000);

          //���������� ��
          LockOSAction();

          //������������� ������� �� ������
          coarse_table[element] = coarse_table[element]|0xFF0;

          //��������� ������ � �������� � ��� �������
          short patch_byte =  ((int)addr)&0xFFF;
          rampage[patch_byte] = old_byte;

          //���������� ������� �� ������
          coarse_table[subelement] = (int)rampage + 0x0E;

          //���������� � �����
          CleanCache(addr);
          CleanTLB();

          //������������� ��
          UnLockOSAction();
          return 1;
       }
   }

  return -1;
 }
 else
   return 0;
}

