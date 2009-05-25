#include <unistd.h>
#include "env.h"

int setroot(void) {
  const char *x;

  x = env_get("ROOT");
  if (!x) return 0;
  if (chdir(x) == -1) return -1;
  if (chroot(".") == -1) return -1;
  return 1;
}
