#!/bin/sh

mod 1777 /dev/shm/tmp

mount --bind /dev/shm/tmp /tmp

mkdir /tmp/test

chmod 777 /tmp/test

cp session_cap /tmp/test

chmod 777 /tmp/test/session_cap

cd /tmp/test

./session_cap -e 0 -f tcp -l 65534
