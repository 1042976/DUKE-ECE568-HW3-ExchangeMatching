WDIR=../sever/pugixml
all: test scalability_test

test: util.cpp TestClient.cpp test.cpp testcases.cpp
	g++ -Wall -g -pedantic -o test.out test.cpp TestClient.cpp util.cpp testcases.cpp -lpthread -lboost_system

scalability_test: scalability_test.cpp util.cpp TestClient.cpp
	g++ -Wall -o scalability_test.out scalability_test.cpp util.cpp TestClient.cpp -lpthread -lboost_system

.PHONY: clean
clean:
	rm *.out
