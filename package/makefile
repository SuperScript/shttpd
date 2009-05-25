#!/bin/sh
shout() { echo "package/makefile: $@" >&2; }
barf() { shout "fatal: $@"; exit 111; }
safe() { "$@" || barf "cannot $@"; }
usage() {
  shout "usage: package/makefile [ --help ]"
  exit 100
}
####
umask 022
[ -d compile ] || barf "no compile directory"
here=`env - PATH=$PATH pwd`
PATH="$here/compile:/command:$PATH"
export PATH
#
[ "$1" = "--help" ] && usage
#
safe cd compile
safe "$here/package/build-uptodate" makefile > Makefile
exit 0
