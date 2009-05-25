#include <unistd.h>
#include <sys/stat.h>
#include "byte.h"
#include "fmt.h"
#include "buffer.h"
#include "strerr.h"
#include "getln.h"
#include "cdb_make.h"
#include "stralloc.h"
#include "open.h"

#define FATAL "redir-data: fatal: "

void die_datatmp(void)
{
  strerr_die2sys(111,FATAL,"unable to create data.tmp: ");
}
void nomem(void)
{
  strerr_die1sys(111,FATAL);
}

int fdcdb;
struct cdb_make cdb;

buffer b;
char bspace[1024];

static stralloc line;
int match = 1;
unsigned long linenum = 0;

char strnum[FMT_ULONG];

void syntaxerror(const char *why)
{
  strnum[fmt_ulong(strnum,linenum)] = 0;
  strerr_die4x(111,FATAL,"unable to parse data line ",strnum,why);
}

int main()
{
  int fddata;
  int i;
  char ch;

  umask(022);

  fddata = open_read("data");
  if (fddata == -1)
    strerr_die2sys(111,FATAL,"unable to open data: ");

  buffer_init(&b,buffer_unixread,fddata,bspace,sizeof bspace);

  fdcdb = open_trunc("data.tmp");
  if (fdcdb == -1) die_datatmp();
  if (cdb_make_start(&cdb,fdcdb) == -1) die_datatmp();

  while (match) {
    ++linenum;
    if (getln(&b,&line,&match,'\n') == -1)
      strerr_die2sys(111,FATAL,"unable to read line: ");

    while (line.len) {
      ch = line.s[line.len - 1];
      if ((ch != ' ') && (ch != '\t') && (ch != '\n')) break;
      --line.len;
    }
    if (!line.len) continue;

    i = byte_chr(line.s,line.len,'#');
    if (i == line.len) syntaxerror(": missing separator");

    if ((line.s[i - 1] == '/') ^ (line.s[line.len - 1] == '/'))
      syntaxerror(": mixed partial and full paths");

    if (cdb_make_add(&cdb,line.s,i,line.s + i + 1,line.len - i - 1) == -1)
      die_datatmp();
  }

  if (cdb_make_finish(&cdb) == -1) die_datatmp();
  if (fsync(fdcdb) == -1) die_datatmp();
  if (close(fdcdb) == -1) die_datatmp(); /* NFS stupidity */
  if (rename("data.tmp","data.cdb") == -1)
    strerr_die2sys(111,FATAL,"unable to move data.tmp to data.cdb: ");

  _exit(0);
}
