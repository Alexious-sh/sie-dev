

 @@@@ ========= removed =========== @@@@
    .extern "C++" main
    .extern __crt_init_switab
    .extern _curent_dir
    .code 32
    .align 4
    .global _start
_start:

    stmfd   sp!, {r0-r4, lr}

    mov r3, r0                      @ забекаим r0
    swi	0x80FF                      @ возьмем адрес свитаблицы
    mov r2, r0

    ldr r4, =__sys_switab_addres    @ загрузим __sys_switab_addres
    str r2, [r4]                    @ запишем в __sys_switab_addres

    mov r0, r3                      @ востановим r0
    mov r1, r0                      @ пересунем наш путь к проге в первый регистр
    ldr r0, =_curent_dir            @ загрузим переменную которая будет хранить путь к папке эльфа
    mov r2, r0                      @ забеками адрес, он нам ещё понадобится
    swi 26                          @ strcpy

    mov r0, r2                      @ возьмем опять забекапленый адрес
    bl  _get_folder                 @ вызовем функцию которая поставит 0x0 после последнего "\\"


    ldmfd   sp!, {r0-r4, lr}
    b       main                    @ идём в майн без возврата


