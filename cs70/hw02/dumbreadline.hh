#ifndef DUMBREADLINE_HH
#define DUMBREADLINE_HH

/*
 * Name: Geoff Kuenning
 * Course: CS 70, Spring 2004
 * Assignment #2
 *
 * Interface definition for the dumb read-line function.  This
 * function will read from a stream into a buffer provided by the
 * user.  The user must allocate the space and must specify the
 * maximum size.  The caller must check for EOF by testing the stream
 * for true/false or by calling the routines good, bad, or eof, all
 * according to taste.
 *
 * If a too-long line is encountered, the function will print a
 * message and abort the program.
 */

#include <iostream>

using namespace std;

extern void	dumbReadLine(istream& stream, char* buffer,
		  unsigned int maxSize);
					// Simple line-getting function

#endif // DUMBREADLINE_HH
