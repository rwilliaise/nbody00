NAME=nbody00
CC=gcc
CFLAGS=$(shell pkg-config --cflags raylib)
LDFLAGS=$(shell pkg-config --libs raylib) -lm
OBJECTS=nbody00.o

$(NAME): $(OBJECTS)

