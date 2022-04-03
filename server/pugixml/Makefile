CC=g++
OUTPUT_FILE=libpugixml.a
OBJ_DIR=.
OUTPUT_DIR=../src
CPPFLAGS=-Wall -Werror -pedantic -std=c++14
$(OUTPUT_FILE): $(patsubst %.cpp,$(OBJ_DIR)/%.o,$(wildcard *.cpp))
	ar -r -o $(OUTPUT_DIR)/$@ $^
$(OBJ_DIR)/%.o: %.cpp
	$(CC) -c $(CPPFLAGS) -o $@  $<
clean:
	rm -f *.a *.o *.gch