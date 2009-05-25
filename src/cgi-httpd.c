#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "pathdecode.h"
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
#include "strerr.h"
#include "error.h"
#include "getln.h"
#include "env.h"
#include "pathexec.h"
#include "setroot.h"
#include "setpriv.h"
#include "byte.h"
#include "scan.h"

#define FATAL "cgi-httpd: fatal: "

void usage(void) {
  strerr_die1x(21,"cgi-httpd: usage: cgi-httpd child");
}

unsigned int timeout = 60;
int safewrite(int fd,const char *buf,int len) {
  int r;
  r = timeoutwrite(timeout,fd,buf,len);
  if (r <= 0) _exit(0);
  return r;
}

char outbuf[1024];
buffer out = BUFFER_INIT(safewrite,1,outbuf,sizeof outbuf);

void out_put(const char *s,int len) {
  buffer_put(&out,s,len);
}

void out_puts(const char *s) {
  buffer_puts(&out,s);
}

void out_flush(void) {
  buffer_flush(&out);
}

char strnum[FMT_ULONG];

stralloc protocol = {0};
int protocolnum;
stralloc method = {0};
stralloc url = {0};
stralloc host = {0};
stralloc path = {0};
int flagbody;

stralloc query = {0};
stralloc cl = {0};
stralloc ct = {0};
stralloc cookie = {0};

struct tai now;
stralloc nowstr = {0};

void header(const char *code,const char *message) {
  if (protocolnum == 1)
    out_puts("HTTP/1.0 ");
  else
    out_puts("HTTP/1.1 ");
  out_puts(code);
  out_puts(message);
  out_puts("\r\nServer: cgi-httpd\r\nDate:");
  if (!httpdate(&nowstr,&now)) _exit(21);
  out_put(nowstr.s,nowstr.len);
  out_puts("\r\n");
}

void barf(const char *code,const char *message) {
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

void get(char * const *cmd) {
  char *s;

  host.len = byte_chr(host.s,host.len,':');
  if (!host.len) {
    if (protocolnum > 1)
      barf("400 ","HTTP/1.1 requests must include a host name");
    if (!stralloc_copys(&host,"0")) _exit(21);
  }
  case_lowerb(host.s,host.len);
  percent(&path);

  if (!stralloc_copys(&fn,"./host/")) _exit(21);
  if (!stralloc_cat(&fn,&host)) _exit(21);
  pathdecode(&fn);

  if (!stralloc_0(&fn)) _exit(21);
  if (!stralloc_0(&host)) _exit(21);
  if (!stralloc_0(&path)) _exit(21);

  if (!pathexec_env("AUTH_TYPE",0)) _exit(21);
  if (!pathexec_env("CONTENT_LENGTH",0)) _exit(21);
  if (cl.len) {
    if (!stralloc_0(&cl)) _exit(21);
    if (!pathexec_env("CONTENT_LENGTH",cl.s)) _exit(21);
  }
  if (!pathexec_env("CONTENT_TYPE",0)) _exit(21);
  if (ct.len) {
    if (!stralloc_0(&ct)) _exit(21);
    if (!pathexec_env("CONTENT_TYPE",ct.s)) _exit(21);
  }
  if (!pathexec_env("HTTP_COOKIE",0)) _exit(21);
  if (cookie.len) {
    if (!stralloc_0(&cookie)) _exit(21);
    if (!pathexec_env("HTTP_COOKIE",cookie.s)) _exit(21);
  }
  if (!pathexec_env("GATEWAY_INTERFACE","CGI/1.1")) _exit(21);
  if (!pathexec_env("PATH_INFO",path.s)) _exit(21);
  if (!pathexec_env("PATH_TRANSLATED",0)) _exit(21);
  if (!pathexec_env("QUERY_STRING",query.s)) _exit(21);  
  s = env_get("TCPREMOTEIP");
  if (!s) _exit(21);
  if (!pathexec_env("REMOTE_ADDR",s)) _exit(21);
  if (!pathexec_env("REMOTE_HOST",env_get("TCPREMOTEHOST"))) _exit(21);
  if (!pathexec_env("REMOTE_USER",env_get("TCPREMOTEINFO"))) _exit(21);
  if (!pathexec_env("REQUEST_METHOD",method.s)) _exit(21);
  if (!pathexec_env("SCRIPT_NAME","")) _exit(21);
  if (!pathexec_env("SERVER_NAME",host.s)) _exit(21);
  s = env_get("TCPLOCALPORT");
  if (!s) _exit(21);
  if (!pathexec_env("SERVER_PORT",s)) _exit(21);
  switch (protocolnum) {
    case 0: if (!pathexec_env("SERVER_PROTOCOL","HTTP/0.9")) _exit(21); break;
    case 1: if (!pathexec_env("SERVER_PROTOCOL","HTTP/1.0")) _exit(21); break;
    case 2: if (!pathexec_env("SERVER_PROTOCOL","HTTP/1.1")) _exit(21); break;
  }
  if (!pathexec_env("SERVER_SOFTWARE","cgi-httpd")) _exit(21);
  tai_now(&now);
  if (!httpdate(&nowstr,&now)) _exit(21);
  if (!stralloc_0(&nowstr)) _exit(21);
  if (!pathexec_env("SERVER_DATE", nowstr.s)) _exit(21);

  if (chdir(fn.s) == -1) barf("404 ",error_str(errno));
  pathexec(cmd);
  strerr_warn5(FATAL,"unable to run ",*cmd,": ",error_str(errno),0);
  barf("404 ",error_str(errno));
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

char inbuf[1];
buffer in = BUFFER_INIT(saferead,0,inbuf,sizeof inbuf);

void readline(void)
{
  int match;

  if (getln(&in,&line,&match,'\n') == -1) _exit(21);
  if (!match) _exit(0);
  if (line.len && (line.s[line.len - 1] == '\n')) --line.len;
  if (line.len && (line.s[line.len - 1] == '\r')) --line.len;
}

int main(int argc,char * const *argv) {
  int i;
  int spaces;
  const char *x;

  if (setroot() == -1)
    strerr_die2sys(111,FATAL,"unable to set root directory: ");
  if (setpriv() == -1)
    strerr_die2sys(111,FATAL,"unable to set privilege: ");

  if (!argv[1]) usage();

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
    if (!stralloc_copys(&query,"")) _exit(21);
    if (!stralloc_copys(&protocol,"")) _exit(21);
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
    else
      if (!str_equal(method.s,"GET"))
	if (!str_equal(method.s,"POST") || (protocolnum < 1))
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

    i = byte_chr(path.s,path.len,'?');
    if (i < path.len) {
      if (!stralloc_copyb(&query,path.s + i + 1,path.len - i - 1)) _exit(21);
      path.len = i;
    }
    if (!stralloc_0(&query)) _exit(21);

    if (!stralloc_copys(&cl,"")) _exit(21);
    if (!stralloc_copys(&ct,"")) _exit(21);
    if (!stralloc_copys(&cookie,"")) _exit(21);
    if (protocolnum > 0) {
      if (!stralloc_copys(&field,"")) _exit(21);
      for (;;) {
	readline();
	if (!line.len || ((line.s[0] != ' ') && (line.s[0] != '\t'))) {
	  if (case_startb(field.s,field.len,"content-length:"))
	    for (i = 15;i < field.len;++i)
	      if (field.s[i] != ' ')
		if (field.s[i] != '\t')
		  if (!stralloc_append(&cl,&field.s[i])) _exit(21);
	  if (case_startb(field.s,field.len,"content-type:"))
	    for (i = 13;i < field.len;++i)
	      if (field.s[i] != ' ')
		if (field.s[i] != '\t')
		  if (!stralloc_append(&ct,&field.s[i])) _exit(21);
	  if (case_startb(field.s,field.len,"cookie:")) {
	    for (i = 7;i < field.len;++i)
	      if (field.s[i] != ' ')
		if (field.s[i] != '\t') break;
	    for (;i < field.len;++i)
	      if (!stralloc_append(&cookie,&field.s[i])) _exit(21);
	  }
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
	  field.len = 0;
	}
	if (!line.len) break;
	if (!stralloc_cat(&field,&line)) _exit(21);
      }
    }

    get(++argv);
  }
}
