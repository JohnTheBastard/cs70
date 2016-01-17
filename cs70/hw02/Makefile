#
# This is the Makefile for CS70, Spring 2004, Homework Assignment 2
#
# To use this file, download it (right-click or shift-click on the
# link in Netscape) into a file named "Makefile".  You can then use
# the command "make" to compile your program.
#
# If you have trouble, it's probably because you downloaded using
# cut-and-paste, which clobbers tab characters.  See the comments
# after the line starting with "all:" for more information.
#

SHELL	=	/bin/sh

CXXFLAGS =	-g -Wall -W -pedantic
CXX	=	g++
CCC	=	$(CXX)
CCFLAGS	=	$(CXXFLAGS)

OBJECTS =	assign_02.o dumbreadline.o

all:	assign_02

assign_02:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o assign_02 $(OBJECTS)

assign_02.o:	dumbreadline.hh

dumbreadline.o: dumbreadline.hh

install:	assign_02
	cp -p assign_02 $$HOME/bin/check_style

clean:
	rm -f assign_02 $(OBJECTS)
