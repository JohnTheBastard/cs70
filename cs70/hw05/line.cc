/*
 * Name: Geoff Kuenning
 * Course: CS 70, Spring 2004
 * Assignment #5
 *
 * This file defines the implementation of the Line class, which
 * stores and manipulates lines of information that (might) have been
 * read from a text file.  See the header file for a complete
 * description.
 */

/*
 * Even though line.hh includes iostream, we will include it
 * ourselves.  Doing so is harmless, and makes it possible for line.hh
 * to change in the future without breaking this program.
 */
#include "line.hh"
#include <iostream>
#include <string.h>

using namespace std;

/*
 * Table of contents: the following functions are defined in this file.
 *
 *			LineGetter::LineGetter(istream& fromStream_);
 *			LineGetter::~LineGetter();
 * const Line*		LineGetter::getLine();	// Create and return a line
 * char*		LineGetter::expand(char* oldString,
 *			    unsigned int oldSize, unsigned int newSize)
 *			  const;
 *			Line::Line();		// Default constructor
 *			Line::Line(const Line& source);
 *					// Copy constructor
 *			Line::Line(int lineNumber_, const char* contents_,
 *			  bool hadNewline_);
 *			Line::~Line();
 * const Line&		Line::operator=(const Line& source);
 *				// Copy-assignment operator
 * ostream&		Line::print(ostream& stream, bool withNumber = false,
 *			    bool forceNewline = true)
 *			  const;
 * static const char*	Line::duplicate(const char* source);
 *					// Helper string-copier
 */

/*
 * Constructor for LineGetter.  This serves as both the default and
 * the from-a-stream constructor.  Note that the line numbering starts
 * at 1, not zero, and that the number kept in the class is the number
 * of the *next* line to be read.
 */
LineGetter::LineGetter(
    istream&		fromStream_)	// Stream that lines will come from
    : fromStream(fromStream_),
      nextNumber(1)
      {
      }

/*
 * Destructor for LineGetter.  Since we don't allocate anything with
 * "new", nothing needs to be done.
 */
LineGetter::~LineGetter()
    {
    }

/*
 * LineGetter factory function for creating Lines.  Each call to this
 * function reads one line from the input file, creates a Line object
 * to hold it, and returns the new Line object.  As is usual with
 * factory functions, it is the caller's responsiblity to later delete
 * the Line.
 *
 * If there is no more input available on the stream, this function
 * returns a NULL pointer.
 */
const Line* LineGetter::getLine()
    {
    /*
     * If an earlier EOF was hit, give up right away.  This code is
     * needed to handle the situation where the last line of the file
     * ended in a newline (in which case we already hit the EOF, but
     * still returned a line).  It also protects against users who
     * inadvertently try to read more lines after EOF is it.  (On some
     * operating systems, if this is not done, the program might need
     * more than one EOF before it would terminate.)
     */
    if (!fromStream)
	return NULL;

    /*
     * We will read from the input stream the hard way, expanding the
     * contents array as necessary.  Start by creating an array to
     * hold the contents.
     */
    char* line = new char[INITIAL_LENGTH];
    unsigned int maxLength = INITIAL_LENGTH;
    unsigned int length = 0;

    /*
     * Now loop, reading and collecting characters, until we either
     * hit EOF or reach a newline.
     */
    while (1)
	{
	/*
	 * Read a character; break out of the loop if we hit EOF.
	 * Skip over NUL ('\0') characters, since they would mess up
	 * our C-style strings.  If a newline is hit, create a line
	 * and return it to our caller.
	 */
	char nextCharacter;
	if (!fromStream.get(nextCharacter))
	    break;

	else if (nextCharacter == '\0')
	    continue;			// Ignore unsupported characters

	else if (nextCharacter == '\n')
	    {
	    /*
	     * A newline was hit.  Terminate the string with a NUL,
	     * create a Line, and return it.  We can terminate the
	     * string without expansion because we always make sure
	     * (below) that there's enough space for one more
	     * character.
	     */
	    line[length] = '\0';
	    Line* result = new Line(nextNumber, line, true);
	    nextNumber++;
	    return result;
	    }

	/*
	 * We only get here if there is another character that needs
	 * to be added to the line.  Make sure there is space, and
	 * then insert the character.  When checking the size, we
	 * remember to leave an extra slot so that there's room for
	 * the inevitable NUL character.
	 */
	if (length >= maxLength - 1)
	    {
	    unsigned int newMaxLength = maxLength * 2;
	    line = expand(line, length, newMaxLength);
	    maxLength = newMaxLength;
	    }

	/*
	 * We're finally ready to store the character.
	 */
	line[length] = nextCharacter;
	length++;
	}

    /*
     * We get here only if EOF was hit while trying to read the line.
     * If we didn't get any characters before the EOF, we will return
     * NULL right away (after cleaning up the line we didn't use).  If
     * we got characters, however, we will create a Line and return
     * it.  In the latter case, the line didn't end with a newline
     * character, so we have to set the "hadNewline" flag to false.
     */
    if (length == 0)
	{
	/*
	 * We hit EOF right away, so don't return a line.
	 */
	delete[] line;
	return NULL;
	}
    else
	{
	/*
	 * There was a line before the EOF, but it didn't end in a
	 * newline.  We need to terminate it with a NUL, which we can
	 * do safely because the code above made sure there was room
	 * for that extra character.
	 */
	line[length] = '\0';
	Line* result = new Line(nextNumber, line, false);
	nextNumber++;
	return result;
	}
    }

/*
 * String expansion routine.  This routine is called when more space
 * is needed in a string.  It allocates the requested amount of space,
 * copies the data into the new area, and frees the old area (which
 * must have been created with new[]).
 *
 * The return value is a pointer to the new area.  The caller is
 * responsible for arranging that this area eventually gets freed.
 *
 * Note that neither the input nor the output string is
 * null-terminated.  The copying is controlled by the length
 * variables.
 */
char* LineGetter::expand(
    char*		oldString,	// String to be expanded
    unsigned int	oldSize,	// Number of valid chars in old string
    unsigned int	newSize)	// Amount of space wanted in new string
    const
    {
    /*
     * We'll start out with a couple of sanity checks.  If the old
     * string was a NULL pointer, we still create a new string but we
     * set the old size to zero so that we won't try to copy any
     * bytes.  If the new string is smaller than the old one, we use
     * the smaller size so that we can be sure not to read past the
     * end of the old string.
     */
    if (oldString == NULL)
	oldSize = 0;			// Make sure we don't copy anything
    else if (newSize < oldSize)
	oldSize = newSize;		// Make sure we don't copy too much

    /*
     * Now we're ready to do the real work.  It's pretty simple: we
     * allocate a new string, copy over the bytes, and then get rid of
     * the old one.
     */
    char* newString = new char[newSize];
    for (unsigned int i = 0;  i < oldSize;  i++)
	newString[i] = oldString[i];
    delete[] oldString;
    return newString;
    }

/*
 * Default constructor for Line.  By setting the contents to NULL we
 * indicate that the line is empty.  Note that it might be better to
 * create a 1-byte character array for "contents" and initialize it to
 * a zero-length string, so that the rest of the code doesn't have to
 * deal with the possibility of a NULL pointer.  In that case, the
 * print rouutine would have to detect an empty Line by looking for a
 * zero line number.
 */
Line::Line()
  : lineNumber(0),
    contents(NULL),
    hadNewline(false)
  {
  }

/*
 * Copy constructor for creating a Line from another Line.  The only
 * hard part is the contents, because we need to allocate our own
 * space for the copy.  This part is done by "duplicate", which also
 * returns the pointer we will need for initializing "contents".  The
 * result is that everything can be done in the colon initializer,
 * even though a loop is involved.
 */
Line::Line(
    const Line&		source)		// Line to be copied
    : lineNumber(source.lineNumber),
      contents(duplicate(source.contents)),
      hadNewline(source.hadNewline)
    {
    }

/*
 * Special from-arguments constructor for use by LineGetter::getLine.
 * The "contents_" field must have been allocated with new[], and the
 * constructor takes over ownership of that array from the caller.
 * For that reason, we DO NOT call "duplicate" (if we did, we would
 * have a memory leak).
 */
Line::Line(
    int			lineNumber_,	// Number to assign to line
    const char*		contents_,	// Contents of the line
    bool		hadNewline_)	// Whether it had a newline
    : lineNumber(lineNumber_),
      contents(contents_),
      hadNewline(hadNewline_)
    {
    }

/*
 * Destructor for Line.  Since "contents" was allocated with "new[]",
 * we must delete[] it.
 */
Line::~Line()
    {
    delete[] contents;
    }

/*
 * Copy-assignment operator for Line.  This is similar to the copy
 * constructor, except (a) we can't use a colon initializer, and (b)
 * we have to get rid of the old "contents", whatever they were,
 * before we create the new value using "duplicate".  Note that, since
 * it is legal to delete a NULL pointer, we don't have to write a
 * special check for that case.
 */
const Line& Line::operator=(
    const Line&		source)		// Line to assign from
    {
    if (this == &source)
	return *this;

    lineNumber = source.lineNumber;
    hadNewline = source.hadNewline;

    delete[] contents;			// Get rid of old contents

    contents = duplicate(source.contents);
    return *this;
    }

/*
 * Print a Line.  Optionally, this function can prefix the line with
 * its line number and force a newline to follow (even if the original
 * didn't have one).
 *
 * If a line number is prefixed, it is followed by a colon and a tab
 * character.
 *
 * If the line doesn't exist (indicated by a NULL contents pointer),
 * nothing at all is printed.
 */
ostream& Line::print(
    ostream&		stream,		// Stream to print on
    bool		withNumber,	// True to prefix with a line number
					// ..(default is no numbering)
    bool		forceNewline)	// True to force newline afterwards
					// ..(default is to force the newline)
    const
    {
    if (contents == NULL)		// Don't print nonexistent lines
	return stream;

    if (withNumber)			// Produce line number if needed
	stream << lineNumber << ":\t";

    stream << contents;

    if (hadNewline  ||  forceNewline)	// Produce trailing newline if needed
	stream << '\n';

    return stream;
    }

/*
 * Helper function for duplicating a string.  This performs almost
 * the same function as the C library strdup(), but it works with new
 * rather than malloc so that it is safe even with an unsafe library.
 * Also, it will deal properly with a NULL source.
 *
 * On entry, "source" is a NUL-terminated string.  The function
 * allocates space to hold a copy of the string, and copies the string
 * into that space (including the NUL terminator.  A pointer to the
 * new space is then returned to the caller.  The caller is
 * responsible for delete[]ing the string sometime in the future.
 */
const char* Line::duplicate(
    const char*		source)		// String to be duplicated
    {
    if (source == NULL)			// If source is NULL, do nothing
	return NULL;

    /*
     * Create space to hold the string and the trailing NUL byte.  We
     * create it as a writable string so that we can do the actual
     * copying without difficulty.
     */
    char* copy = new char[strlen(source) + 1];

    /*
     * Now do the copying and return the result.  A side effect of the
     * return is that the string will be turned into a constant
     * string, so that the caller can't accidentally change it
     * further.
     */
    strcpy(copy, source);
    return copy;
    }
