# OS Simulator written by Xun Luo (xun.luo@ieee.org)
# Inspired by the implementation of Grant William Braught

INC_DIR := ./inc
SRC_DIR := ./src
OBJ_DIR := ./obj

CC := g++
AR := ar -v -q
CFLAGS := -I $(INC_DIR) 

all: ossim 

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ 

LIB_OBJS := \
        SimFramework.o \
	ProcessOperation.o \
	Process.o \
	Event.o \
	EventQueue.o \
	SystemTimer.o

SUBMISSION_OBJS := \
	ProcSched.o

libossim.a: $(LIB_OBJS)
	$(AR) $@ $(LIB_OBJS) 

ossim:  libossim.a $(SUBMISSION_OBJS)
	$(CC) -o OSSim libossim.a $(SUBMISSION_OBJS)

clean:
	rm -rf *.o *.a OSSim
