#
# This is a sample Makefile for CS70, Spring 2004, Homework Assignment 5
#
# To use this file, download it (right-click or shift-click on the
# link in Netscape) into a file named "Makefile".  You can then use
# the command "make" to compile your program.
#
# If you have trouble, it's probably because you downloaded using
# cut-and-paste, which clobbers tab characters.  See the comments
# after the line starting with "all:" for more information.
#

#
# As you might guess, any line beginning with "#" is a comment that
# will be ignored by "make".  I have provided extensive comments to
# help you understand how to build your own Makefiles.
#

#
# Most makefiles begin with macro definitions.  Some macros are built
# in to "make", while others are for your own use.  The most important
# built-in macros, by far, are SHELL and CXXFLAGS.  You should *always*
# set SHELL to /bin/sh, for historical reasons having to do with a
# misguided version of make that is not yet quite obsolete.
#
SHELL	=	/bin/sh

#
# As you can see, a macro definition consists of the macro name
# (underscores are permissible), an equals sign, and a value.  By
# convention, macro names are always written in all-uppercase.  Make
# ignores whitespace around the equals sign, so that you can format
# your definition nicely.  Once it sees a printing character, however,
# it assigns the rest of the line to the macro, including blanks.  So,
# for example, you can specify several options to the C++ compiler in
# CXXFLAGS:
#
CXXFLAGS =	-g -Wall -W -Wshadow -pedantic

#
# It turns out that Turing follows different standards depending on
# what version of make you use.  To work around this problem, we'll
# redefine a couple of macros used by the "other" make so you'll get
# the same effects for all versions.
#
CXX	=	g++
CCC	=	$(CXX)
CCFLAGS	=	$(CXXFLAGS)

#
# The flags above ask the compiler to include debugging information
# that will be useful with the gdb debugger ("-g"), to give you all
# the warning messages it possibly can ("-Wall"), and to stick to the
# ANSI C++ standard ("-pedantic").  Make already knows how to compile
# C++ programs, and it will automatically pass the above flags to the
# compiler when it is invoked.  (If you don't define CXXFLAGS, it will
# default to "-O", which optimizes the program for fast execution but
# may make it harder to debug.)
#

#
# After defining the "standard" macros, it is often useful to define
# helper macros.  For example:
#
OBJECTS =	assign_05.o line.o

#
# The macro above defines the "object" files that will be part of the
# final program.  Here, there is only one file, assign_05.o, as given
# in the assignment.  Note that we don't specify the source file
# here.  Make is smart enough to find it on its own.
#

#
# Now we are ready to give "rules" for building the program.  By
# convention, the first rule is named "all", and it tells make how to
# build everything in the directory.  If you just type "make" with no
# arguments, it will execute the first rule it finds, so "make" is the
# same as "make all" -- compile everything.
#

all:	assign_05

#
# The rule above is a very simple one: it simply says that to make
# "all", you should make "assign_05".  Now we tell make how to do
# that.  As it happens, make is able to figure out on its own how to
# build assign_05 from assign_05.cc.  But we'll give it a rule anyway,
# just to illustrate how it's done.
#
# This time, there are two pieces.  First, we say that assign_05
# should be built whenever the object file changes.  Then we tell make
# exactly how to build assign_05.  $(CXX) is a built-in macro that is
# set to the C++ compiler.  $(xxx) is the way you get at the value of
# a macro named "xxx".
#
# There is one more tricky thing here: the second line below MUST
# begin with an ASCII tab character (hex 09).  A string of blanks does
# NOT have the same meaning.  This is an unfortunate holdover from the
# early days of Unix; it's a stupid restriction but we're now stuck
# with it for historical reasons.  If your Makefile doesn't work, it's
# probably because you didn't follow the downloading instructions, and
# the tab got converted into a string or blanks.
#
assign_05:	$(OBJECTS)
	$(CXX) $(CXXFLAGS) -o assign_05 $(OBJECTS)

#
# We don't need to tell make exactly *how* to create assign_05.o,
# because it has built-in rules for that.  In fact, we don't even need
# to tell it that assign_05.cc exists, because it will figure this
# out.
#

#
# The one thing remaining is to tell make the relationships between
# header files and .cc files.  The implication is that whenever a
# header file changes, all of the cc files that use it need to be
# recompiled.  We do this by saying that the .o files should be
# rebuilt whenever the header file changes.
#
assign_05.o:	line.hh

line.o:		line.hh

#
# It is conventional in Makefiles to include a couple of special
# targets named "install" and "clean".  The "install" target is used
# to install a program in its final destination, such as your "bin"
# directory.  It lists "assign_05" as a dependency to make sure that
# the version installed is the latest one.  The $$HOME notation is
# make's way of expanding a shell variable (in this case, your home
# directory).  We will install the program under the name "shuffle",
# which is a much more meaningful name than "assign_05".
#
install:	assign_05
	cp -p assign_05 $$HOME/bin/shuffle

#
# By convention, the "clean" target cleans things up by deleting
# unwanted files.  Some programmers like to include core files, editor
# backup files, etc., in the list, but in the simplest form "clean"
# only gets rid of the files generated by make.  Since "clean" doesn't
# list any dependencies, the commands will always be executed when you
# type "make clean".
#
clean:
	rm -f assign_05 $(OBJECTS)

#
# That's all there is to it!  If you take out all the comments, you
# will find that this is a very small file, yet it is enough to allow
# make to manage your homework assignment and relieve you of the
# details of remembering how to run the compiler.  In future
# assignments, make will do even more for you, so it's good to get
# into the habit of using it correctly right away.
#
