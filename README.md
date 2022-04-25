# DUKE-ECE-568-Scalability: Exchange Matching

## Start server:

```shell
cd server
sudo docker-compose up
```

You can modify the number of cores in docker-compose.yml. For example, 

`command: bash -c "./run.sh db 4"`

would make the program run on four cores.

To check if the server works,

```shell
cd basictest
make
./test <hostname> <xmlfile_1> <xmlfile_2> ... <xmldile_n>
```



## How to run tests:

1. Go to the server directory ```server``` and ```./run.sh``` to start the server. Note that you will need to install libpqxx-dev
2. ```cd testing``` to go to the testing directory. Note that you will need to install libboost-all-dev
3. To do functionality testing, run ```make``` then run ```./test.out localhost 12345 20```. This program will load xmls from the ```xmls``` and ```expected``` directory and compare them for testing.
4. To do scalability testing, run ```make``` then run ```./scalability_test.out <#threads> <#epoch> localhost 12345``` while the server is running. The total response one thread will send to the server equals epochs x 20. When you start the server, you can indicate the cores and size of thread pool: ```./run.sh localhost <#cores> <thread-pool-size>```



## Our Grade: 101/100

The grading consisted of running a set of test cases: (1) simple account creation, (2) simple transactions, (3) highly concurrent account creation, (4) highly concurrent transactions, (5) some corner case error tests.

There were 6 points added to each raw score. With that curve, the average score was 93 and median 92.9.
