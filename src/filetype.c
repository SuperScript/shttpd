#include "filetype.h"
#include "str.h"

void filetype(char *fn,stralloc *contenttype)
{
  char *x;
  const char *result;
  int i;
  char ch;

  if (!stralloc_copys(contenttype,"Content-Type: ")) _exit(21);

  x = fn + str_rchr(fn,'.');
  if (x[str_chr(x,'=')])
    for (i = 0;(i < 100) && (ch = x[i]);++i) {
      if ((ch != '=') && (ch != '-') && (ch != ':'))
	if ((ch < 'a') || (ch > 'z'))
	  if ((ch < '0') || (ch > '9'))
	    continue;
      if (ch == '=') ch = '/';
      if (ch == ':') ch = '.';
      if (!stralloc_append(contenttype,&ch)) _exit(21);
    }
  else {
    result = "text/plain";
    if (str_equal(x,".html")) result = "text/html";
    else if (str_equal(x,".gz")) result = "application/x-gzip";
    else if (str_equal(x,".dvi")) result = "application/x-dvi";
    else if (str_equal(x,".ps")) result = "application/postscript";
    else if (str_equal(x,".pdf")) result = "application/pdf";
    else if (str_equal(x,".gif")) result = "image/gif";
    else if (str_equal(x,".jpeg")) result = "image/jpeg";
    else if (str_equal(x,".png")) result = "image/png";
    else if (str_equal(x,".mpeg")) result = "video/mpeg";
    else if (str_equal(x,".css")) result = "text/css";

    if (!stralloc_cats(contenttype,result)) _exit(21);
  }

  if (!stralloc_cats(contenttype,"\r\n")) _exit(21);
}
