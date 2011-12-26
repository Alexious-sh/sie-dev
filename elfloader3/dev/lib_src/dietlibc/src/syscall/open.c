
/**
    * 2011
    * (c) Z.Vova
*/


#include <fcntl.h>
#include <swilib.h>
#include <errno.h>


int open(const char *file, int flags, ...)
{
    int flag = 0, mode = 0, i = 0;
    unsigned int err;

    /* ������ ������ */
    if( flags & O_RDONLY ) flag |= A_ReadOnly;

    /* ������ ������ */
    if( flags & O_WRONLY ) flag |= A_WriteOnly;

    /* ������/������ */
    if( flags & O_RDWR ) flag |= A_ReadWrite;

    /* ������� ���� ���� �� ���������� */
    if( flags & O_CREAT ) flag |= A_Create;

    /* �� :-D */
    if( flags & O_EXCL ) flag |= A_Exclusive;

    /* �������� ���� */
    if( flags & O_TRUNC ) flag |= A_Truncate;

    /* ���������� � ����� */
    if( flags & O_APPEND ) flag |= A_Append;


    if( flag & A_WriteOnly || flag & A_ReadWrite )
        mode |= P_WRITE;

    if( flag & A_ReadWrite || flag & A_ReadOnly)
        mode |= P_READ;

    if( !(flags & A_TXT) )
        flag |= A_BIN;


    i = _open(file, flag, mode, &err);
    if(i == -1)
    __set_errno(ENOENT);
    //errno = 0;
    return i;
}




