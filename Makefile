CC=g++
CFLAGS=-std=c++11 -I./include -shared -fPIC -pthread
LDFLAGS=-lpthread

ws-player: src/*.cpp
	$(CC) $(CFLAGS) src/*cpp $(LDFLAGS) -o $@.so