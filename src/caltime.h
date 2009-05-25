/* Public domain. */

#ifndef CALTIME_H
#define CALTIME_H

#include "caldate.h"
#include "tai.h"

struct caltime {
  struct caldate date;
  int hour;
  int minute;
  int second;
  long offset;
} ;

extern void caltime_tai(struct caltime *,struct tai *);
extern void caltime_utc(struct caltime *,struct tai *,int *,int *);

extern unsigned int caltime_fmt(char *,struct caltime *);
extern unsigned int caltime_fmt_t(char *,struct caltime *);
extern unsigned int caltime_fmt_basic(char *,struct caltime *);
extern unsigned int caltime_scan(const char *,struct caltime *);
extern unsigned int caltime_scan_basic(const char *,struct caltime *);

#endif
