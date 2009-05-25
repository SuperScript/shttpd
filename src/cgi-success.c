#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "stralloc.h"
#include "sig.h"
#include "fmt.h"
#include "case.h"
#include "str.h"
#include "tai.h"
#include "httpdate.h"
#include "timeoutwrite.h"
#include "buffer.h"
#include "error.h"
#include "env.h"

int safewrite(int fd,char *buf,int len)
{
  int r;
  r = timeoutwrite(60,fd,buf,len);
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

void out_lines(const char *s) {
  out_puts(s);
  out_puts("\r\n");
}

void out_env(const char *s) {
  char *e;
  out_puts(s); out_puts("=");
  e = env_get(s);
  if (e)
    out_lines(e);
  else
    out_lines("unset");
}

char strnum[FMT_ULONG];

int protocolnum = 0;
int flagbody = 1;

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
  out_puts("\r\nServer: cgi-success\r\nDate:");
  if (!httpdate(&nowstr,&now)) _exit(21);
  out_put(nowstr.s,nowstr.len);
  out_puts("\r\n");
}

int main()
{
  char *s;

  sig_ignore(sig_pipe);

  s = env_get("SERVER_PROTOCOL");
  if (s) {
    if (case_equals(s,"http/1.0"))
      protocolnum = 1;
    else
      if (case_equals(s,"http/1.1"))
	protocolnum = 2;
  }

  tai_now(&now);
  header("200 ","OK");
  if (protocolnum == 2)
    out_lines("Connection: close");
  out_lines("Content-type: text/plain");
  out_lines("");
  out_lines("success");
  out_flush();
  if (protocolnum >= 2) {
    shutdown(1,1);
    sleep(1); /* XXX */
  }
}
