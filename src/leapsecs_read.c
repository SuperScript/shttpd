#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "tai.h"
#include "alloc.h"
#include "leapsecs.h"

struct tai *leapsecs = 0;
int leapsecs_num = 0;

int leapsecs_read()
{
  int fd;
  struct stat st;
  struct tai *t;
  int n;
  int i;
  struct tai u;

  fd = open("/etc/leapsecs.dat",O_RDONLY | O_NDELAY);
  if (fd == -1) {
    if (errno != ENOENT) return -1;
    if (leapsecs) alloc_free((char *)leapsecs);
    leapsecs = 0;
    leapsecs_num = 0;
    return 0;
  }

  if (fstat(fd,&st) == -1) { close(fd); return -1; }

  t = (struct tai *) alloc(st.st_size);
  if (!t) { close(fd); return -1; }

  n = read(fd,(char *) t,st.st_size);
  close(fd);
  if (n != st.st_size) { alloc_free((char *)t); return -1; }

  n /= sizeof(struct tai);

  for (i = 0;i < n;++i) {
    tai_unpack((char *) &t[i],&u);
    t[i] = u;
  }

  if (leapsecs) alloc_free((char *)leapsecs);

  leapsecs = t;
  leapsecs_num = n;
}
