CC = g++
CFLAGS = -g -Wall
SRCS = main.cpp
PROG = asciiVideo

OPENCV = `pkg-config opencv4 --cflags --libs`
LIBS = $(OPENCV)

$(PROG):$(SRCS)
	$(CC) $(CFLAGS) -o $(PROG) $(SRCS) $(LIBS)
