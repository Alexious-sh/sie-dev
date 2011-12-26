#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/types.h>


/* Change the owner and group of the file referred to by FD.  */
int
fchown (fd, owner, group)
     int fd;
     uid_t owner;
     gid_t group;
{
  if (fd < 0)
    {
      __set_errno (EINVAL);
      return -1;
    }

  __set_errno (ENOSYS);
  return -1;
}

int
chown (const char *f, uid_t owner, gid_t group)
{
  __set_errno (ENOSYS);
  return -1;
}