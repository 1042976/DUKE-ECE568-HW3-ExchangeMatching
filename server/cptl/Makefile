CC=g++
OUTPUT_FILE=libcptl.a
OBJ_DIR=.
OUTPUT_DIR=../
CPPFLAGS=-Wall -Werror -pedantic -std=c++14
$(OUTPUT_FILE): $(patsubst %.cpp,%.o,$(wildcard *.cpp))
	ar -r -o $(OUTPUT_DIR)/$@ $^
$(OBJ_DIR)/%.o: %.cpp %.h
	$(CC) -c $(CPPFLAGS) -o $@  $<
clean:
	rm -f *.a *.o *.gch