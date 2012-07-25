#include <swilib.h>
#include <conf_loader.h>
#include <cfg_items.h>

/** Адаптация конфига под гсс
 * (с) Z.Vova
 */

// на случай если несколько конфигов
//__CFG_BEGIN(1)
//__CFG_BEGIN(2)
// и так далее

__CFG_BEGIN(0)

__root const CFG_HDR cfghdr0={CFG_CBOX,"Использовать stderr",0,2};
__root const int _use_stderr = 1;
__root const CFG_CBOX_ITEM cfgcbox0_1[2]={"Нет","Да"};


__root const CFG_HDR cfghdr1={CFG_CBOX,"Использовать stdout",0,2};
__root const int _use_stdout = 1;
__root const CFG_CBOX_ITEM cfgcbox1_1[2]={"Нет","Да"};

__CFG_END(0)


/* второй конфиг ;) */

__CFG_BEGIN(1)

__root const CFG_HDR cfghdr2={CFG_CBOX,"Использовать stderr",0,2};
__root const int _use_stderr2 = 1;
__root const CFG_CBOX_ITEM cfgcbox2_1[2]={"Нет","Да"};


__root const CFG_HDR cfghdr3={CFG_CBOX,"Использовать stdout",0,2};
__root const int _use_stdout2 = 1;
__root const CFG_CBOX_ITEM cfgcbox3_1[2]={"Нет","Да"};

__CFG_END(1)


/** Правая кнопка по config_data.c
  * Properties -> Advanced -> Use custom command to build this file
  * Ставим галочку
  * Вставляем туда $compiler $options -xc $includes -c $file -o $object -O0
*/
