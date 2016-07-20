#!/bin/bash

process_number=`pgrep MonitorService |wc -l`

if [ $process_number  == 0 ];then
   echo “process MonitorService is not running”
   exit 1
fi
