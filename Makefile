CC=g++
CFLAGS=-std=c++11 -I./include -fPIC -pthread -shared
LDFLAGS=-lpthread

ifeq ($(OS), Darwin)
CFLAGS+=-dynamiclib
FILE_XTENSION=so
else
CFLAGS+=-shared
FILE_EXTENSION=dylib
endif

ws-player: src/*.cpp
	$(CC) $(CFLAGS) src/*cpp $(LDFLAGS) -o $@.$(FILE_EXTENSION)