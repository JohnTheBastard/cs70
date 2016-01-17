#
# Sample Makefile for CS70, Spring 2004, Homework Assignment 7
#
SHELL	=	/bin/sh

#
# Don't change these flags
#
CXXFLAGS =	-g -Wall -W -pedantic -DMT_NO_LONGLONG

# Compatability with Sun's make
CXX	=	g++
CCC	=	$(CXX)
CCFLAGS	=	$(CXXFLAGS)

#
# If you add new files to the program, list them (as .o files) in OBJECTS
#
OBJECTS =	assign_07.o \
		city.o \
		colony.o \
		intlist.o \
		mtwist.o \
		organism.o \
		randistrs.o

all:	assign_07

assign_07:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o assign_07 $(OBJECTS)

#
# If you want a traveling-salesman solver in ~/bin, type "make install"
#
install:	all
	cp -p assign_07 $$HOME/bin/tsp

#
# Magic rule to build dependencies.  It is worth studying how this works.
# Note that the doubled dollar sign ($$) in the sed command is
# translated to a single dollar by make, so sed sees only one dollar
# sign.
#
depend:
	mv Makefile Makefile.bak
	sed '/^# DO NOT DELETE THIS LINE/,$$d' Makefile.bak > Makefile
	echo '# DO NOT DELETE THIS LINE' >> Makefile
	echo '#' >> Makefile
	echo '# Automatically generated dependencies follow.' >> Makefile
	echo '#' >> Makefile
	$(CXX) -MM *.cc >> Makefile

clean:
	rm -f assign_07 $(OBJECTS) Makefile.bak

#
# DO NOT DELETE THIS LINE
#
# Automatically generated dependencies follow.
#
assign_07.o: assign_07.cc city.hh colony.hh mtwist.h organism.hh \
  intlist.hh randistrs.h
city.o: city.cc city.hh
colony.o: colony.cc city.hh colony.hh mtwist.h organism.hh intlist.hh \
  randistrs.h
intlist.o: intlist.cc intlist.hh
organism.o: organism.cc city.hh organism.hh intlist.hh randistrs.h \
  mtwist.h
testprogram.o: testprogram.cc intlist.hh
