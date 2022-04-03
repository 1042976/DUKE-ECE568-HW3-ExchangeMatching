#!/bin/bash
#Cratea a static library of XML parser
FILE=src/libpugixml.a
if [[ -f "$FILE" ]]; then
    echo "$FILE exists."
else
    cd pugixml
    echo "Generating libpugixml.a."
    make
    make clean
    cd ../
fi

#set the number of cores to run on

num="0"
threadpoolsize="5"
if [ -n "$2" ]
then
  num=$2
fi
if [ -n "$3" ] && [ "$3" -gt  0 ]; then
  threadpoolsize=$3
fi
last=$((${num}-1));
if [ ${last} != "-1" ] && [ ${last} != "0" ] && [ ${last} != "1" ] && [ ${last} != "2" ] && [ ${last} != "3" ]
then
  echo "Invalid input"
  exit 1
fi

cd src
echo "Generating exchangeMatchingEngine."
make clean
make

#FILE=exchangeMatchingEngine
#if [[ -f "$FILE" ]]; then
#    echo "$FILE exists."
#else
#    echo "Run make."
#    make clean
#    make
#fi
# shellcheck disable=SC2236
if [ ${last} == -1 ]; then
    ./exchangeMatchingEngine
elif [ ${last} == 0 ]; then
  echo "You are utilizing ${num} core"
  if [ -n "$1" ]
  then
    echo "$1"
    taskset -c 0 ./exchangeMatchingEngine "$1" ${threadpoolsize}
  else
    taskset -c 0 ./exchangeMatchingEngine localhost ${threadpoolsize}
  fi
else
  echo "You are utilizing ${num} cores"
  if [ -n "$1" ]; then
    taskset -c 0-${last} ./exchangeMatchingEngine "$1" ${threadpoolsize}
  else
    taskset -c 0-${last} ./exchangeMatchingEngine localhost ${threadpoolsize}
  fi
fi


