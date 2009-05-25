#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "pathdecode.h"
#include "stralloc.h"
#include "fmt.h"
#include "tai.h"
#include "httpdate.h"
#include "timeoutwrite.h"
#include "buffer.h"
#include "strerr.h"
#include "error.h"
#include "env.h"
#include "pathexec.h"
#include "str.h"
#include "scan.h"

unsigned int timeout = 60;
int safewrite(int fd,const char *buf,int len)
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

int protocolnum;
int flagbody;

struct tai now;
stralloc nowstr = {0};

void header(const char *code,const char *message)
{
  if (protocolnum == 1)
    out_puts("HTTP/1.0 ");
  else
    out_puts("HTTP/1.1 ");
  out_puts(code);
  out_puts(message);
  out_puts("\r\nServer: cgi-dispatch\r\nDate:");
  if (!httpdate(&nowstr,&now)) _exit(21);
  out_put(nowstr.s,nowstr.len);
  out_puts("\r\n");
}

void barf(const char *code,const char *message)
{
  if (protocolnum > 0) {
    tai_now(&now);
    header(code,message);
    out_puts("Content-Length: ");
    out_put(strnum,fmt_ulong(strnum,str_len(message) + 28));
    out_puts("\r\n");
    if (protocolnum == 2)
      out_puts("Connection: close\r\n");
    out_puts("Content-Type: text/html\r\n\r\n");
  }
  
  if (str_diff(env_get("REQUEST_METHOD"),"HEAD")) {
    out_puts("<html><body>");
    out_puts(message);
    out_puts("</body></html>\r\n");
  }
  out_flush();
  if (protocolnum >= 2) {
    shutdown(1,1);
    sleep(1); /* XXX */
  }
  _exit(0);
}

stralloc fn = {0};

void get()
{
  char *cmd[2];
  char *path;

  path = env_get("PATH_INFO");
  if (!path) _exit(21);

  if (!stralloc_copys(&fn,"./bin/")) _exit(21);
  if (!stralloc_cats(&fn,path)) _exit(21);
  pathdecode(&fn);
  if (!stralloc_0(&fn)) _exit(21);

  cmd[0] = fn.s;
  cmd[1] = 0;
  pathexec(cmd);
  barf("404 ",error_str(errno));
}

int main()
{
  char * protocol;
  const char *x;

  protocol = env_get("SERVER_PROTOCOL");
  protocolnum = 2;
  if (str_equal(protocol,"HTTP/1.0")) protocolnum = 1;
  if (str_equal(protocol,"HTTP/0.9")) protocolnum = 0;

  x = env_get("TIMEOUT");
  if (x) scan_uint(x,&timeout);
  if (!timeout) timeout = 60;

  get();
}
