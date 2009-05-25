# service_log dir user name prog
service_log() {
  service_log_tmp1="$1/log"
  service_log_tmp2="$2"
  service_log_tmp3="$3"
  shift; shift; shift
  safe mkdir -p "$service_log_tmp1"
  safe cat > "$service_log_tmp1/run.tmp" <<EOF
#!/bin/sh
exec setuidgid '$service_log_tmp2' argv0 multilog '$service_log_tmp3' \\
${1:+"$@"}${1:-"t ./main"}
EOF
  safe chmod 0755 "$service_log_tmp1/run.tmp"
  safe mv "$service_log_tmp1/run.tmp" "$service_log_tmp1/run"
}
