#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "buffer.h"
#include "error.h"
#include "file.h"
#include "byte.h"
#include "str.h"
#include "tai.h"
#include "env.h"
#include "open.h"

static void log(const char *fn,const char *result1,const char *result2,int flagread)
{
  int i;
  char ch;
  const char *x;

  x = env_get("TCPREMOTEIP");
  if (!x) x = "0";
  buffer_puts(buffer_2,x);
  buffer_puts(buffer_2,flagread ? " read ": " dir ");

  for (i = 0;i < 100;++i) {
    ch = fn[i];
    if (!ch) break;
    if (ch < 32) ch = '?';
    if (ch > 126) ch = '?';
    if (ch == ' ') ch = '_';
    buffer_put(buffer_2,&ch,1);
  }
  if (i == 100)
    buffer_puts(buffer_2,"...");

  buffer_puts(buffer_2,result1);
  buffer_puts(buffer_2,result2);
  buffer_puts(buffer_2,"\n");
  buffer_flush(buffer_2);
}

int file_open(const char *fn,struct tai *mtime,unsigned long *length,int flagread)
{
  struct stat st;
  int fd;

  fd = open_read(fn);
  if (fd == -1) {
    log(fn,": open failed: ",error_str(errno),flagread);
    if (error_temp(errno)) _exit(23);
    return -1;
  }
  if (fstat(fd,&st) == -1) {
    log(fn,": fstat failed: ",error_str(errno),flagread);
    close(fd);
    if (error_temp(errno)) _exit(23);
    return -1;
  }
  if ((st.st_mode & 0444) != 0444) {
    log(fn,": ","not ugo+r",flagread);
    close(fd);
    errno = error_acces;
    return -1;
  }
  if ((st.st_mode & 0101) == 0001) {
    log(fn,": ","o+x but u-x",flagread);
    close(fd);
    errno = error_acces;
    return -1;
  }
  if (flagread)
    if ((st.st_mode & S_IFMT) != S_IFREG) {
      log(fn,": ","not a regular file",flagread);
      close(fd);
      errno = error_acces;
      return -1;
    }

  log(fn,": ","success",flagread);

  *length = st.st_size;
  tai_unix(mtime,st.st_mtime);

  return fd;
}
