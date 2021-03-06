CC=g++
CFLAGS=-std=c++14 -I./include -fPIC -pthread
LDFLAGS=-lpthread
OS:=$(shell uname)

ifeq ($(OS), Darwin)
CFLAGS+=-dynamiclib
FILE_EXTENSION=dylib
else
CFLAGS+=-shared
FILE_EXTENSION=so
endif

ws-player: src/*.cpp
	$(CC) $(CFLAGS) src/*cpp $(LDFLAGS) -o $@.$(FILE_EXTENSION)
