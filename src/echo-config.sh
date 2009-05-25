#include "safe.sh"
[ $# -eq 4 ] || usage "echo-config user loguser dir port"
safe service_run "$3" "$1" \
softlimit -o\$OPENLIMIT -d\$DATALIMIT \
tcpserver -vDRHl0 -b\$BACKLOG -c\$CONCURRENCY 0 "$4" \
"#HOME#/command/echo-httpd"
safe echo "$3/root" > "$3/env/ROOT"
safe idu "$1" > "$3/env/UID"
safe idg "$1" > "$3/env/GID"
safe echo 20 > "$3/env/OPENLIMIT"
safe echo 50000 > "$3/env/DATALIMIT"
safe echo 100 > "$3/env/CONCURRENCY"
safe echo 50 > "$3/env/BACKLOG"
safe mkdir -p "$3/root"
safe chmod 02755 "$3/root"
safe service_log "$3" "`echo $2: | cut -d: -f1`" "echo-httpd"
[ -d "$3/log/main" ] || [ -h "$3/log/main" ] \
  || safe mkdir -p "$3/log/main"
safe chown "$2" "$3/log/main"
