#ifndef ESCAPEHTML_H
#define ESCAPEHTML_H

#include "stralloc.h"

extern int escapehtml(stralloc *,stralloc *,int);
extern int escapehtml_cat(stralloc *,stralloc *,int);

#endif
