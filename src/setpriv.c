#include <unistd.h>
#include "env.h"
#include "scan.h"
#include "prot.h"

int setpriv(void) {
  const char *x;
  unsigned long id;
  int r = 0;

  x = env_get("GID");
  if (x) {
    scan_ulong(x,&id);
    if (prot_gid((int) id) == -1) return -1;
    ++r;
  }

  x = env_get("UID");
  if (!x) return r;
  scan_ulong(x,&id);
  if (prot_uid((int) id) == -1) return -1;
  return r + 2;
}

