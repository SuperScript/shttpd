#include "escapehtml.h"
#include "fmt.h"
#include "byte.h"
#include "str.h"

static char strnum[FMT_ULONG];

int escapehtml(stralloc *to,stralloc *from,int all) {
  unsigned int i;
  unsigned int j;
  unsigned char ch;
  char *x;
  char *y;
  
  if (!from->len)
    return stralloc_copys(to,"");

  i = 0;
  j = 0;
  if (all) {
    x = from->s;
    i = from->len;
    while (i--) {
      j += 4;
      if (*x > 9) ++j;
      if (*x > 99) ++j;
      ++x;
    }
  }
  else {
    x = from->s;
    i = from->len;
    while (i--) {
      ch = *x++;
      switch (ch) {
	case '&': j += 5; break;
	case '<': j += 4; break;
	case '>': j += 4; break;
	case '"': j += 6; break;
	case 139: j += 6; break;
	case 155: j += 6; break;
	default: j += 1; break;
      }
    }
  }

  if (!stralloc_ready(to,j)) return 0;

  i = from->len;
  x = to->s + j - i;
  y = to->s;
  byte_copyr(x,i,from->s);

  if (all) {
    while (i--) {
      ch = *x++;
      str_copy(y,"&#");
      y += 2;
      j = fmt_ulong(strnum,ch);
      byte_copy(y,strnum,j);
      y += j;
      stralloc_cats(to,";");
      ++y;
    }
  }
  else {
    to->len = j;
    while (i--) {
      ch = *x;
      switch (ch) {
	case '&': str_copy(y,"&amp;"); y += 4; break;
	case '<': str_copy(y,"&lt;"); y += 3; break;
	case '>': str_copy(y,"&gt;"); y += 3; break;
	case '"': str_copy(y,"&quot;"); y += 6; break;
	case 139: str_copy(y,"&#139;"); y += 6; break;
	case 155: str_copy(y,"&#155;"); y += 6; break;
	default: str_copy(y,x); ++y; break;
      }
      ++x;
    }
  }

  return 1;
}

int escapehtml_cat(stralloc *to,stralloc *from,int all) {
  unsigned int i;
  unsigned int j;
  unsigned char ch;
  
  if (!from->len)
    if (!stralloc_cats(to,"")) return 0;
    else return 1;

  i = 0;
  j = 0;
  if (all) {
    for (i = 0;i < from->len;++i) {
      ++j;
      if (from->s[i] > 9) ++j;
      if (from->s[i] > 99) ++j;
    }
  }
  else {
    for (i = 0;i < from->len;++i) {
      ch = from->s[i];
      switch (ch) {
	case '&': j += 5; break;
	case '<': j += 4; break;
	case '>': j += 4; break;
	case '"': j += 6; break;
	case 139: j += 6; break;
	case 155: j += 6; break;
	default: j += 1; break;
      }
    }
  }

  if (!stralloc_readyplus(to,j)) return 0;

  if (all) {
    for (i = 0;i < from->len; ++i) {
      stralloc_cats(to,"&#");
      j = fmt_ulong(strnum,from->s[i]);
      stralloc_catb(to,strnum,j);
      stralloc_cats(to,";");
      to->len += j + 3;
    }
  }
  else {
    for (i = 0;i < from->len;++i) {
      ch = from->s[i];
      switch (ch) {
	case '&': stralloc_cats(to,"&amp;"); break;
	case '<': stralloc_cats(to,"&lt;"); break;
	case '>': stralloc_cats(to,"&gt;"); break;
	case '"': stralloc_cats(to,"&quot;"); break;
	case 139: stralloc_cats(to,"&#139;"); break;
	case 155: stralloc_cats(to,"&#155;"); break;
	default: stralloc_append(to,from->s + i); break;
      }
    }
  }

  return 1;
}
