# service_run dir user prog
service_run() {
  service_run_tmp1="$1"
  shift
  safe mkdir -p "$service_run_tmp1" "$service_run_tmp1/env"
  safe cat > "$service_run_tmp1/run.tmp" <<EOF
#!/bin/sh
exec 2>&1
exec envdir ./env sh -c '
  exec envuidgid $@
'
EOF
  safe chmod 0755 "$service_run_tmp1/run.tmp"
  safe mv "$service_run_tmp1/run.tmp" "$service_run_tmp1/run"
}
