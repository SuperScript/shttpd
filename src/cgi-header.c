#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "sig.h"
#include "buffer.h"
#include "strerr.h"
#include "byte.h"
#include "iopause.h"
#include "pathexec.h"
#include "fd.h"
#include "stralloc.h"
#include "mess822.h"
#include "env.h"
#include "scan.h"
#include "error.h"
#include "tai.h"
#include "caltime.h"
#include "httpdate.h"
#include "fmt.h"
#include "str.h"
#include "timeoutwrite.h"
#include "escapehtml.h"

#define FATAL "cgi-header: fatal: "

char prebuf[256];
struct tai now;
stralloc temp = {0};
stralloc nowstr = {0};
struct tai mtime;
mess822_time lastmodified = {0};
mess822_time date = {0};
stralloc mtimestr = {0};
stralloc value = {0};
stralloc theheader = {0};
stralloc status = {0};
stralloc location = {0};
stralloc content = {0};
stralloc contenttype = {0};
stralloc contentlength = {0};
stralloc transferencoding = {0};
stralloc setcookie = {0};
stralloc server = {0};
int flaglocation = 0;
int flagstatus = 0;
int flaginheader = 1;
int flagfixreply = 1;
int flagbody = 0;
int flaginbody = 0;
const char *servername = 0;
mess822_header h = MESS822_HEADER;
mess822_action a[] = {
  { "content-type", 0, 0, &contenttype, 0, 0 }
, { "status", &flagstatus, 0, &status, 0, 0 }
, { "location", &flaglocation, 0, &location, 0, 0 }
, { "content-length", 0, 0, &contentlength, 0, 0 }
, { "transfer-encoding", 0, 0, &transferencoding, 0, 0 }
, { "last-modified", 0, 0, 0, 0, &lastmodified }
, { "date", 0, 0, 0, 0, &date }
, { "set-cookie", 0, &setcookie, 0, 0, 0 }
, { "server", 0, 0, &server, 0, 0 }
, { 0, 0, &value, 0, 0, 0 }
, { 0, 0, 0, 0, 0, 0 }
};
unsigned int timeout = 60;
int leftstatus = 0;
char leftbuf[512];
int leftlen;
int leftpos;
int leftflagcr = 0;

int rightstatus = 0;
char rightbuf[512];
int rightlen;
int rightpos;
int rightflagcr = 0;

int protocolnum = 0;
char strnum[FMT_ULONG];

void die_nomem(void) {
  strerr_die2x(111,FATAL,"out of memory");
}

void trimleading(stralloc *sa) {
  unsigned int u;

  if (!sa->len) return;
  for (u = 0;u < sa->len;++u) {
    if (sa->s[u] != ' ') break;
  }
  byte_copy(sa->s,sa->len - u,sa->s + u);
  sa->len -= u;
}

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

void header(const char *code,const char *message) {
  out_puts("Status: "); out_puts(code); out_puts(message);
  out_puts("\r\nServer: cgi-header\r\nDate:");
  if (!httpdate(&nowstr,&now)) _exit(21);
  out_put(nowstr.s,nowstr.len);
}

void barf(const char *code,const char *message) {
  if (protocolnum > 0) {
    tai_now(&now);
    header(code,message);
    out_puts("\r\nContent-Length: ");
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

void append_header(const char *name,const char *alt,stralloc *line) {
  int i;

  if (!alt)
    if (!line || !line->len) return;
  if (name)
    if (!stralloc_cats(&theheader,name)) die_nomem();
  if (line && line->len) {
    if (name && line->s[0] != ' ')
      if (!stralloc_append(&theheader," ")) die_nomem();
    for (i = 0;i < line->len;++i) {
      if (line->s[i] == '\n')
	if (!stralloc_append(&theheader,"\r")) die_nomem();
      if (!stralloc_append(&theheader,line->s + i)) die_nomem();
    }
  }
  else {
    if (name && *alt != ' ')
      if (!stralloc_append(&theheader," ")) die_nomem();
    if (!stralloc_cats(&theheader,alt)) die_nomem();
    if (!stralloc_cats(&theheader,"\r\n")) die_nomem();
  }
}

void fixreply(void) {
  if (flaglocation) {
    trimleading(&location);
    if (location.len <= 1) {/* empty or trailing newline only */
      flaglocation = 0;
    }
    else {
      if (location.s[0] == '/') {/* relative URL */
	/* FIXME: handle internal redirection for Location */
	strerr_die1x(1,"cgi-header: internal redirects not supported");
	_exit(1);
      }
      else {/* absolute URL */
	flagstatus = 1;
	if (protocolnum > 1) {
	  if (!stralloc_copys(&status,"307 Temporary Redirect\n")) die_nomem();
	}
	else {
	  if (!stralloc_copys(&status,"302 Temporary Redirect\n")) die_nomem();
	}
	if (!contentlength.len) {
	  if (!stralloc_cats(&content,"\r\n"))
	    die_nomem();
	  if (!stralloc_cats(&content,"<html><body>Temporary Redirect to <a href=\""))
	    die_nomem();
	  if (!stralloc_catb(&content,location.s,location.len - 1)) die_nomem();
	  if (!stralloc_cats(&content,"\">")) die_nomem();
	  if (!escapehtml(&temp,&location,0)) die_nomem();
	  if (!stralloc_catb(&content,temp.s,temp.len - 1)) die_nomem();
	  if (!stralloc_cats(&content,"</a></body></html>\r\n")) die_nomem();
	  if (!stralloc_copyb(&contentlength,strnum,fmt_ulong(strnum,content.len - 2)))
	    die_nomem();
	  if (!stralloc_append(&contentlength,"\n"))
	    die_nomem();
	  append_header("Content-Length:",0,&contentlength);
	}
      }
    }
  }
  
  flagfixreply = 0;
  if (!stralloc_copys(&theheader,"")) die_nomem();
  if (protocolnum == 0) return;
  if (protocolnum == 2)
    append_header("HTTP/1.1","200 OK",&status);
  else
    append_header("HTTP/1.0","200 OK",&status);
  append_header("Status:",0,&status);
  append_header("Server:",servername,&server);
  if (flaglocation)
    append_header("Location:",0,&location);
  if (date.known)
    caltime_tai(&date.ct,&mtime);
  else
    tai_now(&mtime);
  if (!httpdate(&mtimestr,&mtime)) die_nomem();
  if (!stralloc_cats(&mtimestr,"\n")) die_nomem();
  append_header("Date:",0,&mtimestr);
  append_header("Content-Type:",0,&contenttype);
  append_header("Content-Length:",0,&contentlength);
  append_header("Transfer-Encoding:",0,&transferencoding);
  if (lastmodified.known) {
    caltime_tai(&lastmodified.ct,&mtime);
    tai_now(&now);
    if (tai_less(&mtime,&now)) {
      tai_sub(&now,&now,&mtime);
      if (tai_approx(&now) >= 60.0) {
	if (!httpdate(&mtimestr,&mtime)) die_nomem();
	if (!stralloc_cats(&mtimestr,"\n")) die_nomem();
	append_header("Last-Modified:",0,&mtimestr);
      }
    }
  }
  append_header(0,0,&setcookie);
  append_header(0,0,&value);
}

void doit(int fdleft,int fdright) {
  struct taia stamp;
  struct taia deadline;
  iopause_fd x[4];
  int xlen;
  iopause_fd *io0;
  iopause_fd *ioleft;
  iopause_fd *io1;
  iopause_fd *ioright;
  int r;
  int i;
  char ch;

  if (!mess822_begin(&h,a)) die_nomem();

  for (;;) {
    xlen = 0;

    io0 = 0;
    if (leftstatus == 0) {
      io0 = &x[xlen++];
      io0->fd = 0;
      io0->events = IOPAUSE_READ;
    }
    ioleft = 0;
    if (leftstatus == 1) {
      ioleft = &x[xlen++];
      ioleft->fd = fdleft;
      ioleft->events = IOPAUSE_WRITE;
    }

    ioright = 0;
    if (rightstatus == 0) {
      ioright = &x[xlen++];
      ioright->fd = fdright;
      ioright->events = IOPAUSE_READ;
    }
    io1 = 0;
    if (rightstatus == 1) {
      io1 = &x[xlen++];
      io1->fd = 1;
      io1->events = IOPAUSE_WRITE;
    }

    taia_now(&stamp);
    taia_uint(&deadline,timeout);
    taia_add(&deadline,&stamp,&deadline);
    iopause(x,xlen,&deadline,&stamp);

    if (io0 && io0->revents) {
      r = buffer_unixread(0,prebuf,sizeof prebuf);
      if (r < 0) {
	if (errno != error_intr) {
	  leftstatus = -1;
	  close(fdleft);
	}
      }
      else if (r == 0) {
        leftstatus = -1;
        close(fdleft);
      }
      else {
        leftstatus = 1;
	leftpos = 0;
	leftlen = 0;
	for (i = 0;i < r;++i) {
	  ch = prebuf[i];
	  if (ch == '\n')
	    if (!leftflagcr)
	      leftbuf[leftlen++] = '\r';
	  leftbuf[leftlen++] = ch;
	  leftflagcr = (ch == '\r');
	}
/* 	for (i = 0;i < r;++i) { */
/* 	  ch = prebuf[i]; */
/* 	  if (!leftflagcr) { */
/* 	    if (ch == '\r') { */
/* 	      leftflagcr = 1; */
/* 	      continue; */
/* 	    } */
/* 	    leftbuf[leftlen++] = ch; */
/* 	    continue; */
/* 	  } */
/* 	  if (ch != '\n') { */
/* 	    leftbuf[leftlen++] = '\r'; */
/* 	    if (ch == '\r') continue; */
/* 	  } */
/* 	  leftflagcr = 0; */
/* 	  leftbuf[leftlen++] = ch; */
/* 	} */
      }
    }

    if (ioleft && ioleft->revents) {
      r = buffer_unixwrite(fdleft,leftbuf + leftpos,leftlen - leftpos);
      if (r == -1) {
	if (errno != error_intr) break;
      }
      else {
	leftpos += r;
	if (leftpos == leftlen) leftstatus = 0;
      }
    }

    if (ioright && ioright->revents) {
      r = buffer_unixread(fdright,prebuf,sizeof prebuf);
      if (r < 0) {
	if (errno != error_intr) break;
      }
      else if (r == 0) {
	if (flaginheader)
	  barf("500 ","internal server error");
	rightstatus = -1;
	close(fdright);
	break;
      }
      else {
	i = 0;
	if (flaginheader)
	  for (;i < r;++i) {
	    ch = prebuf[i];
	    if (ch != '\n')
	      if (rightflagcr)
		if (!stralloc_append(&theheader,"\r")) die_nomem();
	    if (ch == '\r') {
	      rightflagcr = 1;
	      continue;
	    }
	    rightflagcr = 0;
	    if (!stralloc_append(&theheader,&ch)) die_nomem();
	    if (ch == '\n') {
	      if (mess822_ok(&theheader)) {
		if (!mess822_line(&h,&theheader)) die_nomem();
		theheader.len = 0;
		continue;
	      }
	      if (!mess822_end(&h)) die_nomem();
	      flaginheader = 0;
	      i -= theheader.len - 1;
	      break;
	    }
	  }
	if (!flaginheader) {
	  flaginbody = 1;
	  rightstatus = 1;
	  rightpos = 0;
	  rightlen = 0;
	  for (;i < r;++i) {
	    ch = prebuf[i];
	    if (ch == '\n')
	      if (!rightflagcr)
		rightbuf[rightlen++] = '\r';
	    rightbuf[rightlen++] = ch;
	    rightflagcr = (ch == '\r');
	  }
	}
      }
    }

    if (io1 && io1->revents) {
      if (!flaginheader) {
	if (flagfixreply) fixreply();
      	r = buffer_unixwrite(1,theheader.s + rightpos,theheader.len - rightpos);
	if (r == -1) break;
	rightpos += r;
	if (rightpos == theheader.len) {
	  rightpos = 0;
	  rightstatus = 0;
	  flaginbody = 1;
	}
      }
      if (flaginbody) {
	if (content.len) {
	  r = buffer_unixwrite(1,content.s + rightpos,content.len - rightpos);
	  if (r == -1) break;
	  rightpos += r;
	  if (rightpos == content.len) rightstatus = 0;
	}
	else {
	  r = buffer_unixwrite(1,rightbuf + rightpos,rightlen - rightpos);
	  if (r == -1) break;
	  rightpos += r;
	  if (rightpos == rightlen) rightstatus = 0;
	}
      }
    }
  }

  _exit(0);
}

void redirect(const char *path,char * const *argv) {
  int piin[2];
  int piout[2];

  if (pipe(piin) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");
  if (pipe(piout) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");

  switch(fork()) {
    case -1:
      strerr_die2sys(111,FATAL,"unable to fork: ");
    case 0:
      close(piin[1]);
      close(piout[0]);
      if (fd_move(0,piin[0]) == -1)
	strerr_die2sys(111,FATAL,"unable to move descriptors: ");
      if (fd_move(1,piout[1]) == -1)
	strerr_die2sys(111,FATAL,"unable to move descriptors: ");

      pathexec(argv + 1);
      strerr_warn5(FATAL,"unable to run ",argv[1],": ",error_str(errno),0);
      barf("404 ",error_str(errno));
    }
    sig_ignore(sig_pipe);
    close(piin[0]);
    close(piout[1]);
    doit(piin[1],piout[0]);
}

void handle_request(char * const *argv) {
  int piin[2];
  int piout[2];

  if (pipe(piin) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");
  if (pipe(piout) == -1)
    strerr_die2sys(111,FATAL,"unable to create pipe: ");

  switch(fork()) {
    case -1:
      strerr_die2sys(111,FATAL,"unable to fork: ");
    case 0:
      close(piin[1]);
      close(piout[0]);
      if (fd_move(0,piin[0]) == -1)
	strerr_die2sys(111,FATAL,"unable to move descriptors: ");
      if (fd_move(1,piout[1]) == -1)
	strerr_die2sys(111,FATAL,"unable to move descriptors: ");

      pathexec(argv + 1);
      strerr_warn5(FATAL,"unable to run ",argv[1],": ",error_str(errno),0);
      barf("404 ",error_str(errno));
    }
    sig_ignore(sig_pipe);
    close(piin[0]);
    close(piout[1]);
    doit(piin[1],piout[0]);
}

int main(int argc,char * const *argv) {
  const char *x;

  if (argc < 2)
    strerr_die1x(100,"cgi-header: usage: cgi-header child");

  x = env_get("TIMEOUT");
  if (x) scan_uint(x,&timeout);
  if (!timeout) timeout = 60;

  x = env_get("SERVER_PROTOCOL");
  protocolnum = 2;
  if (x) {
    if (str_equal(x,"HTTP/1.0"))
      protocolnum = 1;
    else if (str_equal(x,"HTTP/0.9"))
      protocolnum = 0;
  }

  x = env_get("REQUEST_METHOD");
  flagbody = 1;
  if (x) {
    if (str_equal(x,"HEAD"))
      flagbody = 0;
  }

  x = env_get("SERVER_NAME");
  if (x) {
    if (str_equal(x,""))
      x = 0;
  }
  servername = x ? x : "cgi-header";

  handle_request(argv);
}


