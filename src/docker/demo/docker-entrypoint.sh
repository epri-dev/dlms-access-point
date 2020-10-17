#!/bin/bash
set -e
if [ "$1" == "meter" ]; then
    program="Metersim"
else
    program="APsim"
fi
if [ -z "$2" ]; then
    echo "Error: must supply an IPv6 address to Metersim"
    exit 1
fi
/tmp/websocket/taskrunner "$3" &
exec "/tmp/${program}" "$2" 
