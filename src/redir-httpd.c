#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "pathdecode.h"
#include "file.h"
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
#include "uint32.h"
#include "cdb.h"
#include "setpriv.h"
#include "setroot.h"
#include "byte.h"
#include "env.h"
#include "strerr.h"
#include "scan.h"

#define FATAL "redir-httpd: fatal: "

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
stralloc key = {0};
stralloc data = {0};
stralloc ref = {0};
uint32 pos;
uint32 len;
int flagbody = 1;

char filebuf[1024];

struct tai now;
stralloc nowstr = {0};
struct tai mtime;
struct tai mtimeage;
stralloc mtimestr = {0};
struct cdb c;

void header(const char *code,const char *message)
{
  if (protocolnum == 1)
    out_puts("HTTP/1.0 ");
  else
    out_puts("HTTP/1.1 ");
  out_puts(code);
  out_puts(message);
  out_puts("\r\nServer: redir-httpd\r\nDate:");
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

static void putlog(const char *result,int flagfound)
{
  int i;
  char ch;
  const char *x;

  x = env_get("TCPREMOTEIP");
  if (!x) x = "0";
  buffer_puts(buffer_2,x);

  if (host.len) {
    buffer_puts(buffer_2," ");
    for (i = 0;(i < 100) && (i < host.len);++i) {
      ch = host.s[i];
      if (ch < 32) ch = '?';
      if (ch > 126) ch = '?';
      if (ch == ' ') ch = '_';
      buffer_put(buffer_2,&ch,1);
    }
    if (i == 100)
      buffer_puts(buffer_2,"...");
  }
  else {
    buffer_puts(buffer_2," 0");
  }

 if (ref.len) {
    buffer_puts(buffer_2," ");
    for (i = 0;(i < 100) && (i < ref.len);++i) {
      ch = ref.s[i];
      if (ch < 32) ch = '?';
      if (ch > 126) ch = '?';
      if (ch == ' ') ch = '_';
      buffer_put(buffer_2,&ch,1);
    }
    if (i == 100)
      buffer_puts(buffer_2,"...");
  }
  else {
    buffer_puts(buffer_2," 0");
  }

  buffer_puts(buffer_2," ");
  buffer_puts(buffer_2,result);

  buffer_puts(buffer_2," ");
  if (key.len) {
    for (i = 0;(i < 100) && (i < key.len);++i) {
      ch = key.s[i];
      if (ch < 32) ch = '?';
      if (ch > 126) ch = '?';
      if (ch == ' ') ch = '_';
      buffer_put(buffer_2,&ch,1);
    }
    if (i == 100)
      buffer_puts(buffer_2,"...");
  }

  if (flagfound) {
    buffer_puts(buffer_2," ");
    if (data.len) {
      for (i = 0;(i < 100) && (i < data.len);++i) {
	ch = data.s[i];
	if (ch < 32) ch = '?';
	if (ch > 126) ch = '?';
	if (ch == ' ') ch = '_';
	buffer_put(buffer_2,&ch,1);
      }
      if (i == 100)
	buffer_puts(buffer_2,"...");
    }
  }

  buffer_puts(buffer_2,"\n");
  buffer_flush(buffer_2);
}

int redirect()
{
  int r;
  int q;
  int s;
  int at;

  q = byte_chr(path.s,path.len,'?');
  if (!q) _exit(21);
  s = q;
  if (path.s[s - 1] != '/') {
    if (!stralloc_copyb(&key,path.s,s)) _exit(21);
    percent(&key);
    r = cdb_find(&c,key.s,key.len);
    if (r == -1) _exit(21);
    if (r) {
      pos = cdb_datapos(&c);
      len = cdb_datalen(&c);
      if (!stralloc_ready(&data,len)) _exit(21);
      data.len = len;
      if (cdb_read(&c,data.s,len,pos) == -1) _exit(21);
      putlog("exact",1);
      if (!stralloc_catb(&data,path.s + s,path.len - s)) _exit(21);
      return 1;
    }
  }
  at = s;
  for (;;) {
    r = byte_rchr(path.s,at,'/');
    if (r == at) {
      if (!stralloc_copyb(&key,path.s,q)) _exit(21);
      percent(&key);
      putlog("unknown",0);
      return 0;
    }
    at = r;
    if (!stralloc_copyb(&key,path.s,at + 1)) _exit(21);
    percent(&key);
    r = cdb_find(&c,key.s,key.len);
    if (r == -1) _exit(21);
    if (r) break;
  }
  pos = cdb_datapos(&c);
  len = cdb_datalen(&c);
  if (!stralloc_ready(&data,len)) _exit(21);
  data.len = len;
  if (cdb_read(&c,data.s,len,pos) == -1) _exit(21);
  putlog("partial",1);
  if (!stralloc_catb(&data,path.s + at + 1,path.len - at - 1)) _exit(21);
  return 1;
}

stralloc fn = {0};

void get()
{
  unsigned long length;
  int fd;

  host.len = byte_chr(host.s,host.len,':');
  if (!host.len) {
    if (protocolnum > 1)
      barf("400 ","HTTP/1.1 requests must include a host name");
    if (!stralloc_copys(&host,"0")) _exit(21);
  }
  case_lowerb(host.s,host.len);

  if (!stralloc_copys(&fn,"./")) _exit(21);
  if (!stralloc_cat(&fn,&host)) _exit(21);
  if (!stralloc_cats(&fn,"/data.cdb")) _exit(21);
  pathdecode(&fn);
  pathdecode(&host);
  if (!stralloc_0(&fn)) _exit(21);

  fd = file_open(fn.s,&mtime,&length,1);
  if (fd == -1) barf("404 ",error_str(errno));
  cdb_init(&c,fd);
  if (!redirect()) {
    barf("404 ","not found");
  }
  cdb_free(&c);
  close(fd);
  if (protocolnum > 0) {
    tai_now(&now);
    if (!httpdate(&mtimestr,&mtime)) _exit(21);
    if (protocolnum > 1)
      header("307 ","Temporary Redirect");
    else
      header("302 ","Temporary Redirect");
    out_puts("Location: ");
    out_put(data.s,data.len);
    out_puts("\r\n");
    if (tai_less(&mtime,&now)) {
      tai_sub(&mtimeage,&now,&mtime);
      if (tai_approx(&mtimeage) >= 60.0) {
        out_puts("Last-Modified:");
	out_put(mtimestr.s,mtimestr.len);
	out_puts("\r\n");
      }
    }
    out_puts("Content-Length: ");
    out_put(strnum,fmt_ulong(strnum,2 * data.len + 65));
    out_puts("\r\n");
    out_puts("Content-Type: text/html\r\n\r\n");
  }
  if (flagbody) {
    out_puts("<html><body>Temporary Redirect to <a href=\"");
    out_put(data.s,data.len);
    out_puts("\">");
    out_put(data.s,data.len);
    out_puts("</a></body></html>\r\n");
  }
  out_flush();
  if (protocolnum < 2) {
    out_flush();
    _exit(0);
  }
}

stralloc field = {0};
stralloc line = {0};

int saferead(int fd,char *buf,int l)
{
  int r;
  out_flush();
  r = timeoutread(timeout,fd,buf,l);
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

int main()
{
  int i;
  int spaces;
  const char *x;

  if (setroot() == -1)
    strerr_die2sys(111,FATAL,"unable to set root directory: ");
  if (setpriv() == -1)
    strerr_die2sys(111,FATAL,"unable to set privilege: ");

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
    if (!stralloc_copys(&ref,"")) _exit(21);
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

    if (!path.len)
      if (!stralloc_cats(&path,"/")) _exit(21);

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
          if (case_startb(field.s,field.len,"referer:"))
            if (!ref.len)
              for (i = 8;i < field.len;++i)
                if (field.s[i] != ' ')
                  if (field.s[i] != '\t')
                    if (!stralloc_append(&ref,&field.s[i])) _exit(21);
          if (case_startb(field.s,field.len,"if-modified-since:"))
	    if (!stralloc_copyb(&ims,field.s + 18,field.len - 18)) _exit(21);
          field.len = 0;
        }
        if (!line.len) break;
        if (!stralloc_cat(&field,&line)) _exit(21);
      }
    }

    get();
  }
}
