# DO NOT DELETE
# Makefile for CS70, Spring 2004, Homework Assignment 8
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
OBJECTS =	assign_08.o \
		barrequester.o \
		mtwist.o \
		simulationtime.o \
		randistrs.o

all:	assign_08

assign_08:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o assign_08 $(OBJECTS)

#
# If you want the resulting program in ~/bin, type "make install"
#
install:	all
	cp -p assign_08 $$HOME/bin/tsp

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
	rm -f assign_08 $(OBJECTS) Makefile.bak

#
# DO NOT DELETE THIS LINE
#
# Automatically generated dependencies follow.
#
assign_08.o: assign_08.cc barrequester.hh randistrs.h mtwist.h \
  simulationtime.hh list.hh list.icc simulationconstants.hh
barrequester.o: barrequester.cc barrequester.hh randistrs.h mtwist.h \
  simulationtime.hh simulationconstants.hh
simulationtime.o: simulationtime.cc simulationconstants.hh \
  simulationtime.hh
