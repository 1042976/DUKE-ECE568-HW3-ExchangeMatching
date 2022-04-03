#!/bin/bash
pid=`pidof exchangeMatchingEngine`
echo ${pid}
taskset -cp ${pid}
