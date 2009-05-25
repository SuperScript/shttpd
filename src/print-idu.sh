cc="`head -1 conf-idu`"
systype="`cat systype`"
case "$cc:$systype" in
  auto:solaris*)
    id=/usr/xpg4/bin/id
    ;;
  auto:*)
    id=/usr/bin/id
    ;;
esac
echo "idu() { $id -u" '${1+"$@"}' "; }"
echo "idg() { $id -g" '${1+"$@"}' "; }"

