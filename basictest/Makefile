SOURCES=test.cpp ClientSocket.cpp Common.cpp Buffer.cpp
LIBS=-lpthread
OBJS=$(patsubst %.cpp, %.o, $(SOURCES))
CPPFLAGS=-Wall -Werror -pedantic -std=c++14

all: test
test: $(OBJS)
	g++ $(CPPFLAGS) -o test $(OBJS) $(LIBS)
%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

clean:
	rm -f test *.o