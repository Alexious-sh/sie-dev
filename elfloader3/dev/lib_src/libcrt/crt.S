

 @@@@ ========= removed =========== @@@@
    .extern "C++" main
    .extern __crt_init_switab
    .extern _curent_dir
    .code 32
    .align 4
    .global _start
_start:

    stmfd   sp!, {r0-r4, lr}

    mov r3, r0                      @ �������� r0
    swi	0x80FF                      @ ������� ����� ����������
    mov r2, r0

    ldr r4, =__sys_switab_addres    @ �������� __sys_switab_addres
    str r2, [r4]                    @ ������� � __sys_switab_addres

    mov r0, r3                      @ ���������� r0
    mov r1, r0                      @ ��������� ��� ���� � ����� � ������ �������
    ldr r0, =_curent_dir            @ �������� ���������� ������� ����� ������� ���� � ����� �����
    mov r2, r0                      @ �������� �����, �� ��� ��� �����������
    swi 26                          @ strcpy

    mov r0, r2                      @ ������� ����� ������������ �����
    bl  _get_folder                 @ ������� ������� ������� �������� 0x0 ����� ���������� "\\"


    ldmfd   sp!, {r0-r4, lr}
    b       main                    @ ��� � ���� ��� ��������


