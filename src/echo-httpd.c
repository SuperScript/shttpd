#include <unistd.h>
#include "stralloc.h"
#include "sig.h"
#include "str.h"
#include "fmt.h"
#include "httpdate.h"
#include "timeoutread.h"
#include "timeoutwrite.h"
#include "buffer.h"
#include "getln.h"
#include "setroot.h"
#include "setpriv.h"
#include "strerr.h"
#include "scan.h"
#include "env.h"

#define FATAL "echo-httpd: fatal: "

unsigned int timeout = 60;
int safewrite(int fd,char *buf,int len)
{
  int r;
  r = timeoutwrite(timeout,fd,buf,len);
  if (r <= 0) _exit(0);
  return r;
}

char outbuf[1024];
buffer out = BUFFER_INIT(safewrite,1,outbuf,sizeof outbuf);

void out_put(const char *s,int len)
{
  buffer_put(&out,s,len);
}

void out_puts(const char *s)
{
  buffer_puts(&out,s);
}

void out_flush(void)
{
  buffer_flush(&out);
}

char strnum[FMT_ULONG];

struct tai now;
stralloc nowstr = {0};

void header(const char *code,const char *message)
{
  out_puts("HTTP/1.0 ");
  out_puts(code);
  out_puts(message);
  out_puts("\r\nServer: echo-httpd\r\nDate:");
  tai_now(&now);
  if (!httpdate(&nowstr,&now)) _exit(21);
  out_put(nowstr.s,nowstr.len);
  out_puts("\r\n");
}

stralloc line = {0};

int saferead(int fd,char *buf,int len)
{
  int r;
  r = timeoutread(timeout,fd,buf,len);
  if (r <= 0) _exit(0);
  return r;
}

char inbuf[512];
buffer in = BUFFER_INIT(saferead,0,inbuf,sizeof inbuf);

int main () {
  int match;
  const char *x;

  if (setroot() == -1)
    strerr_die2sys(111,FATAL,"unable to set root directory: ");
  if (setpriv() == -1)
    strerr_die2sys(111,FATAL,"unable to set privilege: ");

  sig_ignore(sig_pipe);

  x = env_get("TIMEOUT");
  if (x) scan_uint(x,&timeout);
  if (!timeout) timeout = 60;

  header("200 ","OK");
  out_puts("\r\n");
  out_flush();

  for (;;) {
    if (getln(&in,&line,&match,'\n') == -1) _exit(21);
    if (!line.len) _exit(0);
    out_put(line.s,line.len);
    out_flush();
    if (!match) _exit(0);
  }
}
