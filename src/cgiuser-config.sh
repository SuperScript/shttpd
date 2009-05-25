#include "safe.sh"
[ $# -ge 5 ] || usage "cgiuser-config user loguser dir port prog"
cgiuser_config1="$1"
cgiuser_config2="$2"
cgiuser_config3="$3"
cgiuser_config4="$4"
shift; shift; shift; shift
safe service_run "$cgiuser_config3" "$cgiuser_config1" \
softlimit -o\$OPENLIMIT -d\$DATALIMIT \
tcpserver -vDRHl0 -b\$BACKLOG -c\$CONCURRENCY 0 "$cgiuser_config4" \
"#HOME#/command/cgiuser-httpd" \
"$@"
safe echo "$cgiuser_config3/root" > "$cgiuser_config3/env/ROOT"
safe idu "$cgiuser_config1" > "$cgiuser_config3/env/UID"
safe idg "$cgiuser_config1" > "$cgiuser_config3/env/GID"
safe echo 20 > "$cgiuser_config3/env/OPENLIMIT"
safe echo 100000 > "$cgiuser_config3/env/DATALIMIT"
safe echo 100 > "$cgiuser_config3/env/CONCURRENCY"
safe echo 50 > "$cgiuser_config3/env/BACKLOG"
safe mkdir -p \
  "$cgiuser_config3/root" \
  "$cgiuser_config3/root/host"
safe chmod -R 02755 "$cgiuser_config3/root"
safe service_log "$cgiuser_config3" "$cgiuser_config2" "cgi-httpd/$1"
[ -d "$cgiuser_config3/log/main" ] || [ -h "$cgiuser_config3/log/main" ] \
  || safe mkdir -p "$cgiuser_config3/log/main"
safe chown "$cgiuser_config2" "$cgiuser_config3/log/main"
