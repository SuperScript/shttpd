#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "pathdecode.h"
#include "file.h"
#include "filetype.h"
#include "percent.h"
#include "stralloc.h"
#include "sig.h"
#include "fmt.h"
#include "case.h"
#include "str.h"
#include "tai.h"
#include "httpdate.h"
#include "timeoutread.h"
#include "timeoutwrite.h"
#include "buffer.h"
#include "error.h"
#include "getln.h"
#include "byte.h"
#include "env.h"
#include "setroot.h"
#include "setpriv.h"
#include "strerr.h"
#include "scan.h"

#define FATAL "constant-httpd: fatal: "

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

stralloc protocol = {0};
int protocolnum = 0;
stralloc method = {0};
stralloc url = {0};
stralloc host = {0};
stralloc path = {0};
stralloc ims = {0};
int flagbody = 1;

char filebuf[1024];

struct tai now;
stralloc nowstr = {0};
struct tai mtime;
struct tai mtimeage;
stralloc mtimestr = {0};

void header(const char *code,const char *message)
{
  if (protocolnum == 1)
    out_puts("HTTP/1.0 ");
  else
    out_puts("HTTP/1.1 ");
  out_puts(code);
  out_puts(message);
  out_puts("\r\nServer: constant-httpd\r\nDate:");
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
  if (flagbody) {
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
stralloc contenttype = {0};

void get(const char *f)
{
  unsigned long length;
  int fd;
  int r;
  char *s;

  host.len = byte_chr(host.s,host.len,':');
  if (!host.len) {
    if (protocolnum > 1)
      barf("400 ","HTTP/1.1 requests must include a host name");
    if (!stralloc_copys(&host,"0")) _exit(21);
  }
  s = env_get("REQUESTHOST");
  if (s)
    if (!stralloc_copys(&host,s)) _exit(21);
  case_lowerb(host.s,host.len);

  if (!stralloc_copys(&fn,"./")) _exit(21);
  if (!stralloc_cat(&fn,&host)) _exit(21);
  if (!stralloc_cats(&fn,"/")) _exit(21);
  if (!stralloc_cats(&fn,f)) _exit(21);
  pathdecode(&fn);
  if (!stralloc_0(&fn)) _exit(21);

  fd = file_open(fn.s,&mtime,&length,1);
  if (fd == -1)
    barf("404 ",error_str(errno));

  if (protocolnum > 0) {
    tai_now(&now);
    if (!httpdate(&mtimestr,&mtime)) _exit(21);
    if ((ims.len < mtimestr.len) || byte_diff(mtimestr.s,mtimestr.len,ims.s))
      header("200 ","OK");
    else {
      header("304 ","OK");
      flagbody = 0;
    }
    if (tai_less(&mtime,&now)) {
      tai_sub(&mtimeage,&now,&mtime);
      if (tai_approx(&mtimeage) >= 60.0) {
        out_puts("Last-Modified:");
	out_put(mtimestr.s,mtimestr.len);
	out_puts("\r\n");
      }
    }
    filetype(fn.s,&contenttype);
    out_put(contenttype.s,contenttype.len);
    if (protocolnum >= 2)
      out_puts("Transfer-Encoding: chunked\r\n");
    else {
      out_puts("Content-Length: ");
      out_put(strnum,fmt_ulong(strnum,length)); /* XXX: could change */
      out_puts("\r\n");
    }
    out_puts("\r\n");
  }

  if (protocolnum < 2) {
    if (flagbody)
      for (;;) {
        r = read(fd,filebuf,sizeof filebuf);
        if (r == -1) _exit(23);
        if (r == 0) break;
        out_put(filebuf,r);
      }
    out_flush();
    _exit(0);
  }

  if (flagbody)
    for (;;) {
      r = read(fd,filebuf,sizeof filebuf);
      if (r == -1) _exit(23);
      if (r == 0) { out_puts("0\r\n\r\n"); out_flush(); break; }
      out_put(strnum,fmt_xlong(strnum,(unsigned long) r));
      out_puts("\r\n");
      out_put(filebuf,r);
      out_puts("\r\n");
    }

  close(fd);
}

stralloc field = {0};
stralloc line = {0};

int saferead(int fd,char *buf,int len)
{
  int r;
  out_flush();
  r = timeoutread(timeout,fd,buf,len);
  if (r <= 0) _exit(0);
  return r;
}

char inbuf[512];
buffer in = BUFFER_INIT(saferead,0,inbuf,sizeof inbuf);

void readline(void)
{
  int match;

  if (getln(&in,&line,&match,'\n') == -1) _exit(21);
  if (!match) _exit(0);
  if (line.len && (line.s[line.len - 1] == '\n')) --line.len;
  if (line.len && (line.s[line.len - 1] == '\r')) --line.len;
}

void doit(const char *f)
{
  int i;
  int spaces;
  const char *x;

  sig_ignore(sig_pipe);

  x = env_get("TIMEOUT");
  if (x) scan_uint(x,&timeout);
  if (!timeout) timeout = 60;

for (;;) {
    readline();

    if (!line.len) continue;

    if (!stralloc_copys(&method,"")) _exit(21);
    if (!stralloc_copys(&url,"")) _exit(21);
    if (!stralloc_copys(&host,"")) _exit(21);
    if (!stralloc_copys(&path,"")) _exit(21);
    if (!stralloc_copys(&protocol,"")) _exit(21);
    if (!stralloc_copys(&ims,"")) _exit(21);
    protocolnum = 2;

    spaces = 0;
    for (i = 0;i < line.len;++i)
      if (line.s[i] == ' ') {
        if (!i || (line.s[i - 1] != ' ')) {
          ++spaces;
          if (spaces >= 3) break;
        }
      }
      else
        switch(spaces) {
          case 0:
            if (!stralloc_append(&method,line.s + i)) _exit(21);
            break;
          case 1:
            if (!stralloc_append(&url,line.s + i)) _exit(21);
            break;
          case 2:
            if (!stralloc_append(&protocol,line.s + i)) _exit(21);
            break;
        }

    if (!protocol.len)
      protocolnum = 0;
    else {
      if (!stralloc_0(&protocol)) _exit(21);
      if (case_equals(protocol.s,"http/1.0"))
        protocolnum = 1; /* if client uses http/001.00, tough luck */
    }

    if (!stralloc_0(&method)) _exit(21);
    flagbody = 1;
    if (str_equal(method.s,"HEAD"))
      flagbody = 0;
    else if (!str_equal(method.s,"GET"))
      barf("501 ","method not implemented");

    if (case_startb(url.s,url.len,"http://")) {
      if (!stralloc_copyb(&host,url.s + 7,url.len - 7)) _exit(21);
      i = byte_chr(host.s,host.len,'/');
      if (!stralloc_copyb(&path,host.s + i,host.len - i)) _exit(21);
      host.len = i;
    }
    else if (case_startb(url.s,url.len,"https://")) {
      if (!stralloc_copyb(&host,url.s + 8,url.len - 8)) _exit(21);
      i = byte_chr(host.s,host.len,'/');
      if (!stralloc_copyb(&path,host.s + i,host.len - i)) _exit(21);
      host.len = i;
    }
    else
      if (!stralloc_copy(&path,&url)) _exit(21);

    if (!path.len || (path.s[path.len - 1] == '/'))
      if (!stralloc_cats(&path,"index.html")) _exit(21);

    if (protocolnum > 0) {
      if (!stralloc_copys(&field,"")) _exit(21);
      for (;;) {
        readline();
        if (!line.len || ((line.s[0] != ' ') && (line.s[0] != '\t'))) {
          if (case_startb(field.s,field.len,"content-length:"))
            barf("501 ","I do not accept messages");
          if (case_startb(field.s,field.len,"transfer-encoding:"))
            barf("501 ","I do not accept messages");
          if (case_startb(field.s,field.len,"expect:"))
            barf("417 ","I do not accept Expect");
          if (case_startb(field.s,field.len,"if-match:"))
            barf("412 ","I do not accept If-Match");
          if (case_startb(field.s,field.len,"if-none-match:"))
            barf("412 ","I do not accept If-None-Match");
          if (case_startb(field.s,field.len,"if-unmodified-since:"))
            barf("412 ","I do not accept If-Unmodified-Since");
          if (case_startb(field.s,field.len,"host:"))
            if (!host.len)
              for (i = 5;i < field.len;++i)
                if (field.s[i] != ' ')
                  if (field.s[i] != '\t')
                    if (!stralloc_append(&host,&field.s[i])) _exit(21);
          if (case_startb(field.s,field.len,"if-modified-since:"))
	    if (!stralloc_copyb(&ims,field.s + 18,field.len - 18)) _exit(21);
          field.len = 0;
        }
        if (!line.len) break;
        if (!stralloc_cat(&field,&line)) _exit(21);
      }
    }

    get(f);
  }
}

int main(int argc,char **argv)
{
  if (setroot() == -1)
    strerr_die2sys(111,FATAL,"unable to set root directory: ");
  if (setpriv() == -1)
    strerr_die2sys(111,FATAL,"unable to set privilege: ");

  if (!argv[1]) _exit(100);
  doit(argv[1]);
}
