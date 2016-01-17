#include <iostream>

using namespace std;

/*
 * Name: Geoff Kuenning
 * Course: CS 70, Spring 2004
 * Assignment #2
 *
 * Simple (and dumb) function to read a line of input and return it to
 * the caller.  The caller must provide the buffer that will receive
 * the line, and must tell us the size of the buffer.
 *
 * This routine does NOT return any EOF indication.  It is the
 * caller's responsibility to check for EOF on the stream.
 *
 * If a line is encountered that is longer than the buffer provided,
 * we simply print a message and abort the program.  It is this
 * behavior that qualifies the routine as "dumb".
 */

/*
 * Table of contents: the following routines are defined in this file.
 */
void		dumbReadLine(istream& stream, char buffer[],
		  unsigned int maxSize);

/*
 * Dumb read-line function.  See above for specifications.
 */
void dumbReadLine(
    istream&		stream,		// Stream to read from
    char		buffer[],	// Buffer to read into
    unsigned int	maxSize)	// Size of the buffer
    {
    /*
     * Make sure the caller didn't give us a too-small buffer.
     */
    if (maxSize < 2)
	{
	cerr << "dumbReadLine:  maxSize is ridiculously small.\n";
	exit(1);
	}

    /*
     * Character-collection loop.  We simply read characters, stuffing
     * them in the buffer, until we hit EOF or a newline.  If the
     * buffer overflows, we abort.
     */
    unsigned int i = 0;
    char ch;
    while (stream.get(ch))
	{
	if (ch == '\r')
	    continue;			// Strip carriage returns

	/*
	 * Check for buffer overflow
	 */
	if (i >= maxSize - 1)
	    {
	    cerr << "dumbReadLine:  line too long (over " << maxSize
	      << " characters).\n";
	    exit(1);
	    }

	/*
	 * We now know there is room in the buffer.  Store the
	 * character and check to see if it was a newline (end-of-line
	 * character).  If so, finish off the buffer with a NUL and
	 * return to the caller.
	 */
	buffer[i++] = ch;
	if (ch == '\n')
	    {
	    buffer[i] = '\0';		// Add NUL terminator
	    return;
	    }
	}

    /*
     * We only get here if end-of-file was encountered while reading
     * the line.  If we got no characters before the EOF, then
     * everything was fine and we're done.  If we got characters, then
     * the last line was partial (no newline at the end) and we need
     * to do some trickery to deliver that line to the caller.
     */
    if (i == 0)
	{
	/*
	 * EOF was hit, and we didn't put any characters in the
	 * buffer.  Just return, and let the caller check for EOF.
	 */
	return;
	}
    else
	{
	/*
	 * EOF was hit, but we had read some characters before we got
	 * there.  Undo the EOF so that the caller won't see it.  This
	 * code is a bit of C++ magic.
	 */
	stream.clear(stream.rdstate() & ~ios::failbit);

	/*
	 * Because this is a dumb implementation, we will guarantee
	 * the caller that the last line will be terminated by a
	 * newline, even if there wasn't one in the input.  We are
	 * guaranteed that there is room because when the loop exited,
	 * i was at most maxSize - 2.
	 */
	buffer[i++] = '\n';
	buffer[i] = '\0';		// Add NUL terminator
	}
    }
