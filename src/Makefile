default: \
it

unix.a: \
makefile.build \
default.a.build \
makelib \
unix=l \
escapehtml.o \
mess822_token.o \
mess822_addr.o \
mess822_when.o \
mess822_ok.o \
mess822_line.o \
fd_move.o \
fd_copy.o \
uint32_unpack.o \
uint32_pack.o \
timeoutwrite.o \
timeoutread.o \
taia_uint.o \
taia_sub.o \
taia_now.o \
taia_less.o \
taia_frac.o \
taia_approx.o \
taia_add.o \
tai_unpack.o \
tai_sub.o \
tai_now.o \
strerr_sys.o \
strerr_die.o \
strerr.o \
stralloc_pend.o \
stralloc_opys.o \
stralloc_opyb.o \
stralloc_num.o \
stralloc_eady.o \
stralloc_copy.o \
stralloc_cats.o \
stralloc_catb.o \
stralloc_cat.o \
str_start.o \
str_rchr.o \
str_len.o \
str_diff.o \
str_cpy.o \
str_chr.o \
sig_catch.o \
sig.o \
seek_set.o \
scan_uint.o \
scan_ulong.o \
scan_long.o \
scan_sign.o \
prot.o \
percent.o \
pathexec_run.o \
pathexec_env.o \
pathdecode.o \
open_trunc.o \
open_read.o \
leapsecs_sub.o \
leapsecs_read.o \
leapsecs_init.o \
iopause.o \
httpdate.o \
getln2.o \
getln.o \
generic-conf.o \
fmt_xlong.o \
fmt_ulong.o \
filetype.o \
file.o \
error_temp.o \
error_str.o \
error.o \
env.o \
droproot.o \
droppriv.o \
setroot.o \
setpriv.o \
cdb_make.o \
cdb_hash.o \
cdb.o \
case_startb.o \
case_lowerb.o \
case_diffb.o \
case_diffs.o \
leapsecs_add.o \
caltime_tai.o \
caltime_utc.o \
caldate_fmjd.o \
caldate_mjd.o \
byte_rchr.o \
byte_diff.o \
byte_cr.o \
byte_copy.o \
byte_chr.o \
buffer_write.o \
buffer_read.o \
buffer_put.o \
buffer_get.o \
buffer_copy.o \
buffer_2.o \
buffer.o \
alloc_re.o \
alloc.o
	sh makefile.build default.a.build unix.a unix

uint64.h: \
makefile.build \
uint64.h.build \
uint64.h2 \
uint64.h1 \
tryulong64.c \
load \
compile \
choose
	sh makefile.build uint64.h.build uint64.h 

uint32_unpack.o: \
makefile.build \
default.o.build \
uint32_unpack.c \
compile \
uint32.h
	sh makefile.build default.o.build uint32_unpack.o uint32_unpack

uint32_pack.o: \
makefile.build \
default.o.build \
uint32_pack.c \
compile \
uint32.h
	sh makefile.build default.o.build uint32_pack.o uint32_pack

uint32.h: \
makefile.build \
uint32.h.build \
uint32.h2 \
uint32.h1 \
tryulong32.c \
load \
compile \
choose
	sh makefile.build uint32.h.build uint32.h 

timeoutwrite.o: \
makefile.build \
tai.h \
taia.h \
timeoutwrite.h \
error.h \
default.o.build \
timeoutwrite.c \
compile \
iopause.h \
uint64.h
	sh makefile.build default.o.build timeoutwrite.o timeoutwrite

timeoutread.o: \
makefile.build \
tai.h \
taia.h \
timeoutread.h \
error.h \
default.o.build \
timeoutread.c \
compile \
iopause.h \
uint64.h
	sh makefile.build default.o.build timeoutread.o timeoutread

taia_uint.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_uint.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_uint.o taia_uint

taia_sub.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_sub.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_sub.o taia_sub

taia_now.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_now.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_now.o taia_now

taia_less.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_less.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_less.o taia_less

taia_frac.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_frac.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_frac.o taia_frac

taia_approx.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_approx.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_approx.o taia_approx

taia_add.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
taia_add.c \
compile \
uint64.h
	sh makefile.build default.o.build taia_add.o taia_add

tai_unpack.o: \
makefile.build \
tai.h \
default.o.build \
tai_unpack.c \
compile \
uint64.h
	sh makefile.build default.o.build tai_unpack.o tai_unpack

tai_sub.o: \
makefile.build \
tai.h \
default.o.build \
tai_sub.c \
compile \
uint64.h
	sh makefile.build default.o.build tai_sub.o tai_sub

tai_now.o: \
makefile.build \
tai.h \
default.o.build \
tai_now.c \
compile \
uint64.h
	sh makefile.build default.o.build tai_now.o tai_now

systype: \
makefile.build \
x86cpuid.c \
trycpp.c \
find-systype.sh \
default.build
	sh makefile.build default.build systype systype

sysdeps: \
makefile.build \
default.build \
uint64.h \
select.h \
iopause.h \
hasshsgr.h \
hassgact.h \
load \
compile \
systype
	sh makefile.build default.build sysdeps sysdeps

strerr_sys.o: \
makefile.build \
error.h \
strerr.h \
default.o.build \
strerr_sys.c \
compile
	sh makefile.build default.o.build strerr_sys.o strerr_sys

strerr_die.o: \
makefile.build \
buffer.h \
strerr.h \
default.o.build \
strerr_die.c \
compile
	sh makefile.build default.o.build strerr_die.o strerr_die

strerr.o: \
makefile.build \
gen_alloc.h \
strerr.h \
stralloc.h \
default.o.build \
strerr.c \
compile
	sh makefile.build default.o.build strerr.o strerr

stralloc_pend.o: \
makefile.build \
gen_alloc.h \
gen_allocdefs.h \
stralloc.h \
alloc.h \
default.o.build \
stralloc_pend.c \
compile
	sh makefile.build default.o.build stralloc_pend.o stralloc_pend

stralloc_opys.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
str.h \
byte.h \
default.o.build \
stralloc_opys.c \
compile
	sh makefile.build default.o.build stralloc_opys.o stralloc_opys

stralloc_opyb.o: \
makefile.build \
gen_alloc.h \
byte.h \
stralloc.h \
default.o.build \
stralloc_opyb.c \
compile
	sh makefile.build default.o.build stralloc_opyb.o stralloc_opyb

stralloc_num.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
default.o.build \
stralloc_num.c \
compile
	sh makefile.build default.o.build stralloc_num.o stralloc_num

stralloc_eady.o: \
makefile.build \
gen_alloc.h \
gen_allocdefs.h \
stralloc.h \
alloc.h \
default.o.build \
stralloc_eady.c \
compile
	sh makefile.build default.o.build stralloc_eady.o stralloc_eady

stralloc_copy.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
byte.h \
default.o.build \
stralloc_copy.c \
compile
	sh makefile.build default.o.build stralloc_copy.o stralloc_copy

stralloc_cats.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
str.h \
byte.h \
default.o.build \
stralloc_cats.c \
compile
	sh makefile.build default.o.build stralloc_cats.o stralloc_cats

stralloc_catb.o: \
makefile.build \
gen_alloc.h \
byte.h \
stralloc.h \
default.o.build \
stralloc_catb.c \
compile
	sh makefile.build default.o.build stralloc_catb.o stralloc_catb

stralloc_cat.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
byte.h \
default.o.build \
stralloc_cat.c \
compile
	sh makefile.build default.o.build stralloc_cat.o stralloc_cat

str_start.o: \
makefile.build \
str.h \
default.o.build \
str_start.c \
compile
	sh makefile.build default.o.build str_start.o str_start

str_rchr.o: \
makefile.build \
str.h \
default.o.build \
str_rchr.c \
compile
	sh makefile.build default.o.build str_rchr.o str_rchr

str_len.o: \
makefile.build \
str.h \
default.o.build \
str_len.c \
compile
	sh makefile.build default.o.build str_len.o str_len

str_diff.o: \
makefile.build \
str.h \
default.o.build \
str_diff.c \
compile
	sh makefile.build default.o.build str_diff.o str_diff

str_cpy.o: \
makefile.build \
str.h \
default.o.build \
str_cpy.c \
compile
	sh makefile.build default.o.build str_cpy.o str_cpy

str_chr.o: \
makefile.build \
str.h \
default.o.build \
str_chr.c \
compile
	sh makefile.build default.o.build str_chr.o str_chr

sig_catch.o: \
makefile.build \
sig.h \
default.o.build \
sig_catch.c \
compile \
hassgact.h
	sh makefile.build default.o.build sig_catch.o sig_catch

sig.o: \
makefile.build \
sig.h \
default.o.build \
sig.c \
compile
	sh makefile.build default.o.build sig.o sig

setroot.o: \
makefile.build \
env.h \
default.o.build \
setroot.c \
compile
	sh makefile.build default.o.build setroot.o setroot

setpriv.o: \
makefile.build \
prot.h \
scan.h \
env.h \
default.o.build \
setpriv.c \
compile
	sh makefile.build default.o.build setpriv.o setpriv

select.h: \
makefile.build \
select.h.build \
select.h2 \
select.h1 \
trysysel.c \
compile \
choose
	sh makefile.build select.h.build select.h 

seek_set.o: \
makefile.build \
seek.h \
default.o.build \
seek_set.c \
compile
	sh makefile.build default.o.build seek_set.o seek_set

scan_ulong.o: \
makefile.build \
scan.h \
default.o.build \
scan_ulong.c \
compile
	sh makefile.build default.o.build scan_ulong.o scan_ulong

scan_uint.o: \
makefile.build \
scan.h \
default.o.build \
scan_uint.c \
compile
	sh makefile.build default.o.build scan_uint.o scan_uint

scan_sign.o: \
makefile.build \
scan.h \
default.o.build \
scan_sign.c \
compile
	sh makefile.build default.o.build scan_sign.o scan_sign

scan_long.o: \
makefile.build \
scan.h \
default.o.build \
scan_long.c \
compile
	sh makefile.build default.o.build scan_long.o scan_long

redir-httpd.o: \
makefile.build \
gen_alloc.h \
scan.h \
strerr.h \
env.h \
byte.h \
setroot.h \
setpriv.h \
cdb.h \
getln.h \
error.h \
buffer.h \
timeoutwrite.h \
timeoutread.h \
httpdate.h \
tai.h \
str.h \
case.h \
fmt.h \
sig.h \
stralloc.h \
percent.h \
file.h \
pathdecode.h \
default.o.build \
redir-httpd.c \
compile \
uint32.h \
uint64.h
	sh makefile.build default.o.build redir-httpd.o redir-httpd

redir-httpd: \
makefile.build \
redir-httpd=x \
default.build \
unix.a \
redir-httpd.o \
load
	sh makefile.build default.build redir-httpd redir-httpd

redir-data.o: \
makefile.build \
gen_alloc.h \
open.h \
stralloc.h \
cdb_make.h \
getln.h \
strerr.h \
buffer.h \
fmt.h \
byte.h \
default.o.build \
redir-data.c \
compile \
uint32.h
	sh makefile.build default.o.build redir-data.o redir-data

redir-data: \
makefile.build \
redir-data=x \
default.build \
unix.a \
redir-data.o \
load
	sh makefile.build default.build redir-data redir-data

redir-config: \
makefile.build \
default.build \
redir-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
shout.sh \
barf.sh \
safe.sh \
warn-auto.sh \
redir-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
safe.sh \
warn-auto.sh \
makescrip \
makeinclude \
redir-config=s
	sh makefile.build default.build redir-config redir-config

prot.o: \
makefile.build \
prot.h \
default.o.build \
prot.c \
compile \
hasshsgr.h
	sh makefile.build default.o.build prot.o prot

percent.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
percent.h \
default.o.build \
percent.c \
compile
	sh makefile.build default.o.build percent.o percent

pathexec_run.o: \
makefile.build \
gen_alloc.h \
pathexec.h \
env.h \
str.h \
stralloc.h \
error.h \
default.o.build \
pathexec_run.c \
compile
	sh makefile.build default.o.build pathexec_run.o pathexec_run

pathexec_env.o: \
makefile.build \
gen_alloc.h \
pathexec.h \
env.h \
byte.h \
str.h \
alloc.h \
stralloc.h \
default.o.build \
pathexec_env.c \
compile
	sh makefile.build default.o.build pathexec_env.o pathexec_env

pathdecode.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
pathdecode.h \
default.o.build \
pathdecode.c \
compile
	sh makefile.build default.o.build pathdecode.o pathdecode

open_trunc.o: \
makefile.build \
open.h \
default.o.build \
open_trunc.c \
compile
	sh makefile.build default.o.build open_trunc.o open_trunc

open_read.o: \
makefile.build \
open.h \
default.o.build \
open_read.c \
compile
	sh makefile.build default.o.build open_read.o open_read

mess822_when.o: \
makefile.build \
caldate.h \
gen_alloc.h \
tai.h \
caltime.h \
stralloc.h \
case.h \
str.h \
httpdate.h \
scan.h \
mess822.h \
default.o.build \
mess822_when.c \
compile \
uint64.h
	sh makefile.build default.o.build mess822_when.o mess822_when

mess822_token.o: \
makefile.build \
tai.h \
caldate.h \
gen_alloc.h \
caltime.h \
stralloc.h \
mess822.h \
default.o.build \
mess822_token.c \
compile \
uint64.h
	sh makefile.build default.o.build mess822_token.o mess822_token

mess822_ok.o: \
makefile.build \
tai.h \
caldate.h \
gen_alloc.h \
caltime.h \
stralloc.h \
byte.h \
mess822.h \
default.o.build \
mess822_ok.c \
compile \
uint64.h
	sh makefile.build default.o.build mess822_ok.o mess822_ok

mess822_line.o: \
makefile.build \
tai.h \
caldate.h \
gen_alloc.h \
caltime.h \
stralloc.h \
case.h \
str.h \
mess822.h \
default.o.build \
mess822_line.c \
compile \
uint64.h
	sh makefile.build default.o.build mess822_line.o mess822_line

mess822_addr.o: \
makefile.build \
tai.h \
caldate.h \
gen_alloc.h \
caltime.h \
stralloc.h \
str.h \
mess822.h \
default.o.build \
mess822_addr.c \
compile \
uint64.h
	sh makefile.build default.o.build mess822_addr.o mess822_addr

makescrip: \
makefile.build \
home \
conf-scrip \
print-makescrip.sh \
warn-auto.sh \
default.build
	sh makefile.build default.build makescrip makescrip

makelib: \
makefile.build \
default.build \
warn-auto.sh \
systype \
print-ar.sh
	sh makefile.build default.build makelib makelib

makeinclude: \
makefile.build \
warn-auto.sh \
makeinclude.sh \
default.build
	sh makefile.build default.build makeinclude makeinclude

load: \
makefile.build \
warn-auto.sh \
print-load.sh \
conf-ld \
default.build
	sh makefile.build default.build load load

leapsecs_sub.o: \
makefile.build \
tai.h \
leapsecs.h \
default.o.build \
leapsecs_sub.c \
compile \
uint64.h
	sh makefile.build default.o.build leapsecs_sub.o leapsecs_sub

leapsecs_read.o: \
makefile.build \
leapsecs.h \
alloc.h \
tai.h \
default.o.build \
leapsecs_read.c \
compile \
uint64.h
	sh makefile.build default.o.build leapsecs_read.o leapsecs_read

leapsecs_init.o: \
makefile.build \
leapsecs.h \
default.o.build \
leapsecs_init.c \
compile
	sh makefile.build default.o.build leapsecs_init.o leapsecs_init

leapsecs_add.o: \
makefile.build \
tai.h \
leapsecs.h \
default.o.build \
leapsecs_add.c \
compile \
uint64.h
	sh makefile.build default.o.build leapsecs_add.o leapsecs_add

it: \
makefile.build \
it=d \
default.build \
sysdeps \
cgi-header \
cgi-env \
cgi-success \
cgi-dispatch \
redir-httpd \
redir-data \
redir-config \
echo-httpd \
echo-config \
constant-httpd \
constant-config \
cgiuser-httpd \
cgiuser-config \
cgi-httpd \
cgi-config
	sh makefile.build default.build it it

iopause.o: \
makefile.build \
tai.h \
taia.h \
default.o.build \
iopause.c \
compile \
iopause.h \
select.h \
uint64.h
	sh makefile.build default.o.build iopause.o iopause

iopause.h: \
makefile.build \
iopause.h.build \
iopause.h2 \
iopause.h1 \
trypoll.c \
load \
compile \
choose
	sh makefile.build iopause.h.build iopause.h 

idu.sh: \
makefile.build \
systype \
print-idu.sh \
conf-idu \
idu.sh.build
	sh makefile.build idu.sh.build idu.sh 

httpdate.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
tai.h \
caldate.h \
httpdate.h \
caltime.h \
default.o.build \
httpdate.c \
compile \
uint64.h
	sh makefile.build default.o.build httpdate.o httpdate

hasshsgr.h: \
makefile.build \
hasshsgr.h.build \
warn-shsgr \
chkshsgr \
hasshsgr.h2 \
hasshsgr.h1 \
tryshsgr.c \
compile \
choose
	sh makefile.build hasshsgr.h.build hasshsgr.h 

hassgact.h: \
makefile.build \
hassgact.h.build \
hassgact.h2 \
hassgact.h1 \
trysgact.c \
compile \
choose
	sh makefile.build hassgact.h.build hassgact.h 

getln2.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
buffer.h \
getln.h \
byte.h \
default.o.build \
getln2.c \
compile
	sh makefile.build default.o.build getln2.o getln2

getln.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
buffer.h \
getln.h \
byte.h \
default.o.build \
getln.c \
compile
	sh makefile.build default.o.build getln.o getln

generic-conf.o: \
makefile.build \
generic-conf.h \
open.h \
buffer.h \
strerr.h \
default.o.build \
generic-conf.c \
compile
	sh makefile.build default.o.build generic-conf.o generic-conf

fmt_xlong.o: \
makefile.build \
fmt.h \
default.o.build \
fmt_xlong.c \
compile
	sh makefile.build default.o.build fmt_xlong.o fmt_xlong

fmt_ulong.o: \
makefile.build \
fmt.h \
default.o.build \
fmt_ulong.c \
compile
	sh makefile.build default.o.build fmt_ulong.o fmt_ulong

filetype.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
str.h \
filetype.h \
default.o.build \
filetype.c \
compile
	sh makefile.build default.o.build filetype.o filetype

file.o: \
makefile.build \
open.h \
env.h \
tai.h \
str.h \
byte.h \
file.h \
error.h \
buffer.h \
default.o.build \
file.c \
compile \
uint64.h
	sh makefile.build default.o.build file.o file

fd_move.o: \
makefile.build \
fd.h \
default.o.build \
fd_move.c \
compile
	sh makefile.build default.o.build fd_move.o fd_move

fd_copy.o: \
makefile.build \
fd.h \
default.o.build \
fd_copy.c \
compile
	sh makefile.build default.o.build fd_copy.o fd_copy

escapehtml.o: \
makefile.build \
gen_alloc.h \
stralloc.h \
str.h \
byte.h \
fmt.h \
escapehtml.h \
default.o.build \
escapehtml.c \
compile
	sh makefile.build default.o.build escapehtml.o escapehtml

error_temp.o: \
makefile.build \
error.h \
default.o.build \
error_temp.c \
compile
	sh makefile.build default.o.build error_temp.o error_temp

error_str.o: \
makefile.build \
error.h \
default.o.build \
error_str.c \
compile
	sh makefile.build default.o.build error_str.o error_str

error.o: \
makefile.build \
error.h \
default.o.build \
error.c \
compile
	sh makefile.build default.o.build error.o error

env.o: \
makefile.build \
env.h \
str.h \
default.o.build \
env.c \
compile
	sh makefile.build default.o.build env.o env

echo-httpd.o: \
makefile.build \
tai.h \
gen_alloc.h \
env.h \
scan.h \
strerr.h \
setpriv.h \
setroot.h \
getln.h \
buffer.h \
timeoutwrite.h \
timeoutread.h \
httpdate.h \
fmt.h \
str.h \
sig.h \
stralloc.h \
default.o.build \
echo-httpd.c \
compile \
uint64.h
	sh makefile.build default.o.build echo-httpd.o echo-httpd

echo-httpd: \
makefile.build \
echo-httpd=x \
default.build \
unix.a \
echo-httpd.o \
load
	sh makefile.build default.build echo-httpd echo-httpd

echo-config: \
makefile.build \
default.build \
echo-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
shout.sh \
barf.sh \
safe.sh \
warn-auto.sh \
echo-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
safe.sh \
warn-auto.sh \
makescrip \
makeinclude \
echo-config=s
	sh makefile.build default.build echo-config echo-config

droproot.o: \
makefile.build \
strerr.h \
prot.h \
scan.h \
env.h \
default.o.build \
droproot.c \
compile
	sh makefile.build default.o.build droproot.o droproot

droppriv.o: \
makefile.build \
strerr.h \
prot.h \
scan.h \
env.h \
default.o.build \
droppriv.c \
compile
	sh makefile.build default.o.build droppriv.o droppriv

constant-httpd.o: \
makefile.build \
gen_alloc.h \
scan.h \
strerr.h \
setpriv.h \
setroot.h \
env.h \
byte.h \
getln.h \
error.h \
buffer.h \
timeoutwrite.h \
timeoutread.h \
httpdate.h \
tai.h \
str.h \
case.h \
fmt.h \
sig.h \
stralloc.h \
percent.h \
filetype.h \
file.h \
pathdecode.h \
default.o.build \
constant-httpd.c \
compile \
uint64.h
	sh makefile.build default.o.build constant-httpd.o constant-httpd

constant-httpd: \
makefile.build \
constant-httpd=x \
default.build \
unix.a \
constant-httpd.o \
load
	sh makefile.build default.build constant-httpd constant-httpd

constant-config: \
makefile.build \
default.build \
constant-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
shout.sh \
barf.sh \
safe.sh \
warn-auto.sh \
constant-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
safe.sh \
warn-auto.sh \
makescrip \
makeinclude \
constant-config=s
	sh makefile.build default.build constant-config constant-config

compile: \
makefile.build \
default.build \
warn-auto.sh \
systype \
print-compile.sh \
conf-cc
	sh makefile.build default.build compile compile

choose: \
makefile.build \
choose.sh \
warn-auto.sh \
choose.sh \
warn-auto.sh \
default.build \
makescrip \
makeinclude \
choose=s
	sh makefile.build default.build choose choose

chkshsgr.o: \
makefile.build \
default.o.build \
chkshsgr.c \
compile
	sh makefile.build default.o.build chkshsgr.o chkshsgr

chkshsgr: \
makefile.build \
chkshsgr=x \
default.build \
chkshsgr.o \
load
	sh makefile.build default.build chkshsgr chkshsgr

cgiuser-httpd.o: \
makefile.build \
gen_alloc.h \
scan.h \
byte.h \
droppriv.h \
pathexec.h \
env.h \
getln.h \
error.h \
strerr.h \
buffer.h \
timeoutwrite.h \
timeoutread.h \
httpdate.h \
tai.h \
str.h \
case.h \
fmt.h \
sig.h \
stralloc.h \
percent.h \
pathdecode.h \
default.o.build \
cgiuser-httpd.c \
compile \
uint64.h
	sh makefile.build default.o.build cgiuser-httpd.o cgiuser-httpd

cgiuser-httpd: \
makefile.build \
cgiuser-httpd=x \
default.build \
unix.a \
cgiuser-httpd.o \
load
	sh makefile.build default.build cgiuser-httpd cgiuser-httpd

cgiuser-config: \
makefile.build \
default.build \
cgiuser-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
shout.sh \
barf.sh \
safe.sh \
warn-auto.sh \
cgiuser-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
safe.sh \
warn-auto.sh \
makescrip \
makeinclude \
cgiuser-config=s
	sh makefile.build default.build cgiuser-config cgiuser-config

cgi-success.o: \
makefile.build \
gen_alloc.h \
env.h \
error.h \
buffer.h \
timeoutwrite.h \
httpdate.h \
tai.h \
str.h \
case.h \
fmt.h \
sig.h \
stralloc.h \
default.o.build \
cgi-success.c \
compile \
uint64.h
	sh makefile.build default.o.build cgi-success.o cgi-success

cgi-success: \
makefile.build \
cgi-success=x \
default.build \
static.lib \
unix.a \
cgi-success.o \
load
	sh makefile.build default.build cgi-success cgi-success

cgi-httpd.o: \
makefile.build \
gen_alloc.h \
scan.h \
byte.h \
setpriv.h \
setroot.h \
pathexec.h \
env.h \
getln.h \
error.h \
strerr.h \
buffer.h \
timeoutwrite.h \
timeoutread.h \
httpdate.h \
tai.h \
str.h \
case.h \
fmt.h \
sig.h \
stralloc.h \
percent.h \
pathdecode.h \
default.o.build \
cgi-httpd.c \
compile \
uint64.h
	sh makefile.build default.o.build cgi-httpd.o cgi-httpd

cgi-httpd: \
makefile.build \
cgi-httpd=x \
default.build \
unix.a \
cgi-httpd.o \
load
	sh makefile.build default.build cgi-httpd cgi-httpd

cgi-header.o: \
makefile.build \
caldate.h \
gen_alloc.h \
taia.h \
escapehtml.h \
timeoutwrite.h \
str.h \
fmt.h \
httpdate.h \
caltime.h \
tai.h \
error.h \
scan.h \
env.h \
mess822.h \
stralloc.h \
fd.h \
pathexec.h \
byte.h \
strerr.h \
buffer.h \
sig.h \
default.o.build \
cgi-header.c \
compile \
iopause.h \
uint64.h
	sh makefile.build default.o.build cgi-header.o cgi-header

cgi-header: \
makefile.build \
cgi-header=x \
default.build \
static.lib \
unix.a \
cgi-header.o \
load
	sh makefile.build default.build cgi-header cgi-header

cgi-env.o: \
makefile.build \
gen_alloc.h \
env.h \
error.h \
buffer.h \
timeoutwrite.h \
httpdate.h \
tai.h \
str.h \
case.h \
fmt.h \
sig.h \
stralloc.h \
default.o.build \
cgi-env.c \
compile \
uint64.h
	sh makefile.build default.o.build cgi-env.o cgi-env

cgi-env: \
makefile.build \
cgi-env=x \
default.build \
static.lib \
unix.a \
cgi-env.o \
load
	sh makefile.build default.build cgi-env cgi-env

cgi-dispatch.o: \
makefile.build \
gen_alloc.h \
scan.h \
str.h \
pathexec.h \
env.h \
error.h \
strerr.h \
buffer.h \
timeoutwrite.h \
httpdate.h \
tai.h \
fmt.h \
stralloc.h \
pathdecode.h \
default.o.build \
cgi-dispatch.c \
compile \
uint64.h
	sh makefile.build default.o.build cgi-dispatch.o cgi-dispatch

cgi-dispatch: \
makefile.build \
cgi-dispatch=x \
default.build \
static.lib \
unix.a \
cgi-dispatch.o \
load
	sh makefile.build default.build cgi-dispatch cgi-dispatch

cgi-config: \
makefile.build \
cgi-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
warn-auto.sh \
cgi-config.sh \
idu.sh \
service_log.sh \
service_run.sh \
warn-auto.sh \
makescrip \
makeinclude \
cgi-config=s \
default.build
	sh makefile.build default.build cgi-config cgi-config

cdb_make.o: \
makefile.build \
buffer.h \
cdb_make.h \
cdb.h \
alloc.h \
error.h \
seek.h \
default.o.build \
cdb_make.c \
compile \
uint32.h
	sh makefile.build default.o.build cdb_make.o cdb_make

cdb_hash.o: \
makefile.build \
cdb.h \
default.o.build \
cdb_hash.c \
compile \
uint32.h
	sh makefile.build default.o.build cdb_hash.o cdb_hash

cdb.o: \
makefile.build \
cdb.h \
byte.h \
seek.h \
error.h \
default.o.build \
cdb.c \
compile \
uint32.h
	sh makefile.build default.o.build cdb.o cdb

case_startb.o: \
makefile.build \
case.h \
default.o.build \
case_startb.c \
compile
	sh makefile.build default.o.build case_startb.o case_startb

case_lowerb.o: \
makefile.build \
case.h \
default.o.build \
case_lowerb.c \
compile
	sh makefile.build default.o.build case_lowerb.o case_lowerb

case_diffs.o: \
makefile.build \
case.h \
default.o.build \
case_diffs.c \
compile
	sh makefile.build default.o.build case_diffs.o case_diffs

case_diffb.o: \
makefile.build \
case.h \
default.o.build \
case_diffb.c \
compile
	sh makefile.build default.o.build case_diffb.o case_diffb

caltime_utc.o: \
makefile.build \
caltime.h \
caldate.h \
leapsecs.h \
tai.h \
default.o.build \
caltime_utc.c \
compile \
uint64.h
	sh makefile.build default.o.build caltime_utc.o caltime_utc

caltime_tai.o: \
makefile.build \
caltime.h \
caldate.h \
leapsecs.h \
tai.h \
default.o.build \
caltime_tai.c \
compile \
uint64.h
	sh makefile.build default.o.build caltime_tai.o caltime_tai

caldate_mjd.o: \
makefile.build \
caldate.h \
default.o.build \
caldate_mjd.c \
compile
	sh makefile.build default.o.build caldate_mjd.o caldate_mjd

caldate_fmjd.o: \
makefile.build \
caldate.h \
default.o.build \
caldate_fmjd.c \
compile
	sh makefile.build default.o.build caldate_fmjd.o caldate_fmjd

byte_rchr.o: \
makefile.build \
byte.h \
default.o.build \
byte_rchr.c \
compile
	sh makefile.build default.o.build byte_rchr.o byte_rchr

byte_diff.o: \
makefile.build \
byte.h \
default.o.build \
byte_diff.c \
compile
	sh makefile.build default.o.build byte_diff.o byte_diff

byte_cr.o: \
makefile.build \
byte.h \
default.o.build \
byte_cr.c \
compile
	sh makefile.build default.o.build byte_cr.o byte_cr

byte_copy.o: \
makefile.build \
byte.h \
default.o.build \
byte_copy.c \
compile
	sh makefile.build default.o.build byte_copy.o byte_copy

byte_chr.o: \
makefile.build \
byte.h \
default.o.build \
byte_chr.c \
compile
	sh makefile.build default.o.build byte_chr.o byte_chr

buffer_write.o: \
makefile.build \
buffer.h \
default.o.build \
buffer_write.c \
compile
	sh makefile.build default.o.build buffer_write.o buffer_write

buffer_read.o: \
makefile.build \
buffer.h \
default.o.build \
buffer_read.c \
compile
	sh makefile.build default.o.build buffer_read.o buffer_read

buffer_put.o: \
makefile.build \
error.h \
byte.h \
str.h \
buffer.h \
default.o.build \
buffer_put.c \
compile
	sh makefile.build default.o.build buffer_put.o buffer_put

buffer_get.o: \
makefile.build \
error.h \
byte.h \
buffer.h \
default.o.build \
buffer_get.c \
compile
	sh makefile.build default.o.build buffer_get.o buffer_get

buffer_copy.o: \
makefile.build \
buffer.h \
default.o.build \
buffer_copy.c \
compile
	sh makefile.build default.o.build buffer_copy.o buffer_copy

buffer_2.o: \
makefile.build \
buffer.h \
default.o.build \
buffer_2.c \
compile
	sh makefile.build default.o.build buffer_2.o buffer_2

buffer.o: \
makefile.build \
buffer.h \
default.o.build \
buffer.c \
compile
	sh makefile.build default.o.build buffer.o buffer

alloc_re.o: \
makefile.build \
byte.h \
alloc.h \
default.o.build \
alloc_re.c \
compile
	sh makefile.build default.o.build alloc_re.o alloc_re

alloc.o: \
makefile.build \
error.h \
alloc.h \
default.o.build \
alloc.c \
compile
	sh makefile.build default.o.build alloc.o alloc

