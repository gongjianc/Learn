#!/bin/bash

case $1:$2 in
    (zorro:zorro|jerry:jerry)
    echo "ok!"
    ;;
    *)
    echo "auth failed!"
    ;;
esac

