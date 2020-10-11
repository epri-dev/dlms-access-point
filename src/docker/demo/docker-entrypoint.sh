#!/bin/bash
set -e
if [ -z "$1" ]; then
    echo "Error: must supply an IPv6 address to Metersim"
    exit 1
fi
/tmp/websocket/taskrunner "$2" &
exec "/tmp/Metersim" "$1" 
