mod_http:
	cd userland/examples/plus_mod/http_webmail; make; ./cp.sh  

mod_nids:
	cd userland/examples/session_capv0.0/src; make; ./cp.sh 

mod_smtp:
	cd userland/examples/plus_mod/codesmtp_log; make; ./cp.sh

mod_WL:
	cd userland/examples/plus_mod/WLflter/Debug; make; ./cp.sh

all:
	cd userland/examples/plus_mod/http_webmail; make; ./cp.sh 
#	cd userland/examples/plus_mod/http_webmail; ./cp.sh;
	cd userland/examples/session_capv0.0/src; make; ./cp.sh 
	cd userland/examples/plus_mod/codesmtp_log; make; ./cp.sh
#	cd userland/examples/plus_mod/codesmtp_log; ./cp.sh; 
	cd userland/examples/plus_mod/WLflter/Debug; make; ./cp.sh
#	cd userland/examples/plus_mod/WLflter/Debug; ./cp.sh; 
	cd userland/examples/; make;./cp.sh

install:
	cp userland/examples/session_cap ./

clean:
	cd userland/examples/plus_mod/http_webmail; make clean
	cd userland/examples/plus_mod/codesmtp_log; make clean
	cd userland/examples/session_capv0.0/src; make clean
	cd userland/examples/plus_mod/WLflter/Debug; make clean
	cd userland/examples/; make clean

