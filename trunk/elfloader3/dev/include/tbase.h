//tbase.h - Работа с базой трансляции для применения виртуальных патчей
//(C)Dimadze
//v1.2

#ifndef __VPATCHER_H__
#define __VPATCHER_H__

#ifdef __cplusplus
extern "C" {
#endif

void EnableINT(int fi);
int  DisableINT();
void CleanCache(void *vaddress);
void CleanTLB();
int *GetTBaseAddr();
void UnLockAccess();
void ExecuteIMB();

//Получение значения байта из адреса
char GetByteFromAddress(void *addr);
//Виртуальная запись байта по адресу (Применение патча)
int  ApplyPatch(void *addr, char new_byte);
//Виртуальная запись байта по адресу (Отмена патча)
int  UndoPatch(void *addr, char old_byte);

/* При отмене всех патчей было решено */
/* Разбивку секций оставить */
/* Хотя для очистки flash страниц есть все данные */
/* Участки RAM, которые отводится под flash страницы */
/* Не имеют доступа для записи, только чтение */
/* Чтобы в них писать надо разблокировывать доступ */


#ifdef __cplusplus
}
#endif


#endif
