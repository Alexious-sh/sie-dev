//tbase.c - Работа с базой трансляции для применения виртуальных патчей
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
 //Разрешаем себе доступ
 UnLockAccess();

 //Получаем адрес базы переадресации
 int *trans_base = GetTBaseAddr();

 //Если элемент таблицы первого уровня есть секция
 unsigned short element = ((int)addr>>0x14)&0xFFF;
 //Если элемент таблицы первого уровня есть 0
 if (((trans_base[element])&0x03) == 0x00) return (void *)0xFFFFFFFF;

 //Если элемент таблицы первого уровня есть грубая таблица второго уровня
 if (((trans_base[element])&0x03) == 0x01)
  {
   //Получение адреса таблицы второго уровня и адрес ячейки
   int *coarse_table = (int *)(((int)trans_base[element])&0xFFFFFE00);
   unsigned short subelement = ((int)addr>>0x0C)&0xFF;

   //Если элемент таблицы втрого уровня есть 0
   if (((coarse_table[subelement])&0x03) == 0x00) return (void *)0xFFFFFFFF;

   //Если элемент таблицы втрого уровня есть большая страница (64 Кб)
   if (((coarse_table[subelement])&0x03) == 0x01)
    {
     int addr_page = (int)(((int)coarse_table[subelement])&0xFFFF0000);
     int offset = ((int)addr)&0xFFFF;
     return (void *)(addr_page + offset);
    }

   //Если элемент таблицы втрого уровня есть маленькая страница (4 Кб)
   if (((coarse_table[subelement])&0x03) == 0x02)
    {
     int addr_page = (int)(((int)coarse_table[subelement])&0xFFFFF000);
     int offset = ((int)addr)&0xFFF;
     return (void *)(addr_page + offset);
    }
  }

  //Если элемент таблицы первого уровня есть секция
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
 //Разрешаем себе доступ
 UnLockAccess();

 //Получаем адрес базы переадресации
 int *trans_base = GetTBaseAddr();
 //Проверка на соответсвие адреса патча с адресами flash
 if (((int)addr >= 0xA0000000) && ((int)addr < 0xA4000000) && trans_base)
 {
  unsigned short element = ((int)addr>>0x14)&0xFFF;

  //Если элемент таблицы первого уровня стандартный
  if (((trans_base[element])&0xFF) == 0x1E)
   {
    //Разбиваем секцию в 1 МБ на грубую таблицу из 256 страниц по 4 КБ

    //Захват памяти и получение адреса секции
    char *addr_section = (char *)(((int)addr)&0xFFF00000);
    int *coarse_table = (int *)malloc(0x100*4+0xFFF+4);
         coarse_table = (int *)virtual2physical((void *)coarse_table);

    //Дополнительная разбивка для физических адресов, если такая не обнаружена
    unsigned short element_d1 = ((int)coarse_table>>0x14)&0xFFF;
    if (((int)(trans_base[element_d1])&&0x03) == 0x00)
    trans_base[element_d1] = (((int)coarse_table)&0xFFF00000) + 0xFFE;
    if (((int)(trans_base[element_d1])&&0x03) == 0x01)
     {
      //Получение адреса таблицы второго уровня и адрес ячейки
      int *coarse_table_d = (int *)(((int)trans_base[element_d1])&0xFFFFFE00);
      unsigned short subelement_d = ((int)coarse_table>>0x0C)&0xFF;
      if (((int)(coarse_table_d[subelement_d])&&0x03) == 0x00)
      coarse_table_d[subelement_d] = (((int)coarse_table)&0xFFFFF000) + 0xFFE;
     }

    //Выравниваем адрес
    int *coarse_table_a = (int *)(((int)coarse_table + 0xFFF)&0xFFFFF000);

    //Непосредственно сама разбивка
    for (int i = 0; i < 0x100; i++) coarse_table_a[i] = (int)addr_section + 0x1000*i + 0x0E;

    //Сохраняем невыравненный адрес таблицы последним элементом
    coarse_table_a[0x100] = (int)coarse_table;

    //Захват памяти и получение адреса страницы
    unsigned short subelement = ((int)addr>>0x0C)&0xFF;
    char *addr_page = (char *)((coarse_table_a[subelement])&0xFFFFF000);
    char *rampage = (char *)malloc(0x1000 + 0xFFF + 4);
          rampage = (char *)virtual2physical((void *)rampage);

    //Дополнительная разбивка для физических адресов, если такая не обнаружена
    unsigned short element_d2 = ((int)rampage>>0x14)&0xFFF;
    if (((int)(trans_base[element_d2])&&0x03) == 0x00)
    trans_base[element_d2] = (((int)rampage)&0xFFF00000) + 0xFFE;
    if (((int)(trans_base[element_d2])&&0x03) == 0x01)
     {
      //Получение адреса таблицы второго уровня и адрес ячейки
      int *coarse_table_d = (int *)(((int)trans_base[element_d2])&0xFFFFFE00);
      unsigned short subelement_d = ((int)rampage>>0x0C)&0xFF;
      if (((int)(coarse_table_d[subelement_d])&&0x03) == 0x00)
      coarse_table_d[subelement_d] = (((int)rampage)&0xFFFFF000) + 0xFFE;
     }

    //Выравниваем адрес и копируем туда содержимое flash-страницы
    char *rampage_a = (char *)(((int)rampage + 0xFFF)&0xFFFFF000);
    memcpy(rampage_a, addr_page, 0x1000);

    //Сохраняем невыравненный адрес страницы
    int *rampage_int = (int *)rampage_a;
    rampage_int[0x1000] = (int)rampage;

    //Получение адреса в странице и сам патчинг
    short patch_byte =  ((int)addr)&0xFFF;
    rampage_a[patch_byte] = new_byte;

    LockOSAction();

    //Конфигурация таблиц первого и второго уровней
    coarse_table_a[subelement] = (int)rampage_a + 0x0E;
    trans_base[element] = (int)coarse_table_a + 0x11;

    //Требования с кешем
    CleanCache(addr);
    CleanTLB();

    UnLockOSAction();
    return 1;
   }
    //Если уже разбит на страницы
    else
     {
      //Получение адреса таблицы второго уровня и адрес ячейки
      int *coarse_table = (int *)(((int)trans_base[element])&0xFFFFF000);
      unsigned short subelement = ((int)addr>>0x0C)&0xFF;
      char stat_page = (coarse_table[subelement]&0x0F000000)>>0x18;

      //Если flash страница не в RAM
      if ((stat_page > 0x00 && stat_page < 0x04) || stat_page == 0x00)
       {
        //Захват памяти и получение адреса страницы
        char *addr_page = (char *)((coarse_table[subelement])&0xFFFFF000);
        char *rampage = (char *)malloc(0x1000 + 0xFFF + 4);
              rampage = (char *)virtual2physical((void *)rampage);

        //Дополнительная разбивка для физических адресов, если такая не обнаружена
        unsigned short element_d2 = ((int)rampage>>0x14)&0xFFF;
        if (((int)(trans_base[element_d2])&&0x03) == 0x00)
        trans_base[element_d2] = (((int)rampage)&0xFFF00000) + 0xFFE;
        if (((int)(trans_base[element_d2])&&0x03) == 0x01)
        {
          //Получение адреса таблицы второго уровня и адрес ячейки
          int *coarse_table_d = (int *)(((int)trans_base[element_d2])&0xFFFFFE00);
          unsigned short subelement_d = ((int)rampage>>0x0C)&0xFF;
          if (((int)(coarse_table_d[subelement_d])&&0x03) == 0x00)
          coarse_table_d[subelement_d] = (((int)rampage)&0xFFFFF000) + 0xFFE;
         }

        //Выравниваем адрес и копируем туда содержимое flash-страницы
        char *rampage_a = (char *)(((int)rampage + 0xFFF)&0xFFFFF000);
        memcpy(rampage_a, addr_page, 0x1000);

        //Сохраняем невыравненный адрес страницы
        int *rampage_int = (int *)rampage_a;
        rampage_int[0x1000] = (int)rampage;

        //Получение адреса в странице и сам патчинг
        short patch_byte =  ((int)addr)&0xFFF;
        rampage_a[patch_byte] = new_byte;

        //Блокировка ОС
        LockOSAction();


        //Добавление к таблице второго уровня
        coarse_table[subelement] = (int)rampage_a + 0x0E;

        //Требования с кешем
        CleanCache(addr);
        CleanTLB();

        //Разблокировка ОС
        UnLockOSAction();
        return 1;
       }

       //Если flash страница в RAM
       else
         {
          //Получение адреса страницы
          char *rampage = (char *)((coarse_table[subelement])&0xFFFFF000);

          //Блокировка ОС
          LockOSAction();

          //Разблокировка доступа на запись
          coarse_table[element] = coarse_table[element]|0xFF0;

          //Получение адреса в странице и сам патчинг
          short patch_byte =  ((int)addr)&0xFFF;
          rampage[patch_byte] = new_byte;

          //Блокировка доступа на запись
          coarse_table[subelement] = (int)rampage + 0x0E;

          //Требования с кешем
          CleanCache(addr);
          CleanTLB();

          //Разблокировка ОС
          UnLockOSAction();
          return 1;
         }

     }
 }
  else return 0;
}

int UndoPatch(void *addr, char old_byte)
{
 //Разрешаем себе доступ
 UnLockAccess();

 //Получаем адрес базы переадресации
 int *trans_base = GetTBaseAddr();
 //Проверка на соответсвие адреса патча с адресами flash
 if (((int)addr >= 0xA0000000) && ((int)addr < 0xA4000000) && trans_base)
 {
  unsigned short element = ((int)addr>>0x14)&0xFFF;

  //Если элемент таблицы первого уровня нестандартный
  if (((trans_base[element])&0xFF) == 0x11)
   {
      //Получение адреса таблицы второго уровня и адрес ячейки
      int *coarse_table = (int *)(((int)trans_base[element])&0xFFFFF000);
      unsigned short subelement = ((int)addr>>0x0C)&0xFF;
      char stat_page = (coarse_table[subelement]&0x0F000000)>>0x18;

      //Если flash страница уже в RAM
      if (stat_page > 0x03)
       {
          //Получение адреса страницы
          char *rampage = (char *)((coarse_table[subelement])&0xFFFFF000);

          //Блокировка ОС
          LockOSAction();

          //Разблокировка доступа на запись
          coarse_table[element] = coarse_table[element]|0xFF0;

          //Получение адреса в странице и сам патчинг
          short patch_byte =  ((int)addr)&0xFFF;
          rampage[patch_byte] = old_byte;

          //Блокировка доступа на запись
          coarse_table[subelement] = (int)rampage + 0x0E;

          //Требования с кешем
          CleanCache(addr);
          CleanTLB();

          //Разблокировка ОС
          UnLockOSAction();
          return 1;
       }
   }

  return -1;
 }
 else
   return 0;
}

