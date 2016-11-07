#!/bin/bash

IPADDR='www.baidu.com'
INTERVAL=1

while true
do
    while ping -c 1 $IPADDR &> /dev/null
    do
        sleep $INTERVAL
    done

    echo "$IPADDR ping error!" 1>&2

    until ping -c 1 $IPADDR &> /dev/null
    do 
        sleep $INTERVAL
    done

    echo "$IPADDR ping ok!"
done
