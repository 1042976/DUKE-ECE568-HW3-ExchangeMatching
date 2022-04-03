#!bin/bash
chdir ../server/src
make clean
make
# ./exchangeMatchingEngine

chdir ../../test
make test
for i in 1
do
    ../server/src/exchangeMatchingEngine
    processId=$(ps -ef | grep 'exchangeMatchingEngine' | grep -v 'grep' | awk '{ printf $2 }')
    ./test.out localhost 12345 $i

    if [ $? -eq 0 ]
    then
        echo "passed functionality test $i"
        kill -9 $processId
    else
        echo "failed funcitonality test $i"
        kill -9 $processId
        break
    fi

done

echo "passed all functionality tests"
