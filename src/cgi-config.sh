#include "safe.sh"
[ $# -ge 5 ] || usage "cgi-config user loguser dir port prog"
cgi_config1="$1"
cgi_config2="$2"
cgi_config3="$3"
cgi_config4="$4"
shift; shift; shift; shift
safe service_run "$cgi_config3" "$cgi_config1" \
softlimit -o\$OPENLIMIT -d\$DATALIMIT \
tcpserver -vDRHl0 -b\$BACKLOG -c\$CONCURRENCY 0 "$cgi_config4" \
"#HOME#/command/cgi-httpd" \
"$@"
safe echo "$cgi_config3/root" > "$cgi_config3/env/ROOT"
safe idu "$cgi_config1" > "$cgi_config3/env/UID"
safe idg "$cgi_config1" > "$cgi_config3/env/GID"
safe echo 20 > "$cgi_config3/env/OPENLIMIT"
safe echo 50000 > "$cgi_config3/env/DATALIMIT"
safe echo 100 > "$cgi_config3/env/CONCURRENCY"
safe echo 50 > "$cgi_config3/env/BACKLOG"
safe mkdir -p \
  "$cgi_config3/root" \
  "$cgi_config3/root/bin" \
  "$cgi_config3/root/host"
safe chmod -R 02755 "$cgi_config3/root"
safe service_log "$cgi_config3" "$cgi_config2" "cgi-httpd/$1"
[ -d "$cgi_config3/log/main" ] || [ -h "$cgi_config3/log/main" ] \
  || safe mkdir -p "$cgi_config3/log/main"
safe chown "$cgi_config2" "$cgi_config3/log/main"
