# DO NOT DELETE
# Makefile for CS70, Spring 2004, Homework Assignment 9
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
OBJECTS =	hashtable.o \
		hashfuncs.o \
		assign_10.o

all:	assign_10

assign_10:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o assign_10 $(OBJECTS)

#
# If you want the resulting program in ~/bin, type "make install"
#
install:	all
	cp -p assign_10 $$HOME/bin/tsp

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
	rm -f assign_10 $(OBJECTS) Makefile.bak

#
# DO NOT DELETE THIS LINE
#
# Automatically generated dependencies follow.
#
assign_10.o: assign_10.cc hashtable.hh list.hh list.icc hashfuncs.hh
hashfuncs.o: hashfuncs.cc hashfuncs.hh
hashtable.o: hashtable.cc list.hh list.icc hashfuncs.hh hashtable.hh
testprogram.o: testprogram.cc hashtable.hh list.hh list.icc hashfuncs.hh
