SOURCES=ExchangeMatchingEngine.cpp MyData.cpp ServerSocket.cpp XMLParser.cpp UniqueID.cpp Buffer.cpp
LIBS=libpugixml.a -lpthread -lpqxx -lpq
OBJS=$(patsubst %.cpp, %.o, $(SOURCES))
INC_PUGI=../pugixml
INC_CPTL=../cptl
CPPFLAGS=-I$(INC_PUGI) -I$(INC_CPTL) -Wall -Werror -pedantic -std=c++14

all: exchangeMatchingEngine
exchangeMatchingEngine: $(OBJS)
	g++ $(CPPFLAGS) -o exchangeMatchingEngine $(OBJS) $(LIBS)
%.o: %.cpp
	g++ $(CPPFLAGS) -c $<

clean:
	rm -f exchangeMatchingEngine *.o