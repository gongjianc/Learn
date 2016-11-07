#!/bin/bash

exec 3< /etc/passwd
exec 0<&3
cat

exec 3>&-

exec 3> /tmp/stdout
exec 4> /tmp/stderr

exec 1>&3-
exec 2>&4-

find /etc/ -name "passed"

exec 3>&-
exec 4>&-
