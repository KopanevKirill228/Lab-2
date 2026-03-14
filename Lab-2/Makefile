CC = g++
CFLAGS = -Wall -std=c++17
GTEST_DIR = googletest/googletest
GTEST_FLAGS = -I$(GTEST_DIR)/include -I$(GTEST_DIR)

program: main.cpp
	$(CC) $(CFLAGS) main.cpp -o program

tests: tests.cpp $(GTEST_DIR)/src/gtest-all.cc $(GTEST_DIR)/src/gtest_main.cc
	$(CC) $(CFLAGS) $(GTEST_FLAGS) tests.cpp \
	$(GTEST_DIR)/src/gtest-all.cc \
	$(GTEST_DIR)/src/gtest_main.cc \
	-o tests

clean:
	rm -f program tests
