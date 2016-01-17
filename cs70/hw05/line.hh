#ifndef LINE_HH

/*
 * Name: Geoff Kuenning
 * Course: CS 70, Fall 2002
 * Assignment #5
 *
 * This file defines the interface to the LineGetter and Line classes.
 *
 * The LineGetter class provides a simple facility for getting Lines
 * from a file.  The Line class stores and manipulates lines of
 * information that (might) have been read from a text file.  Both
 * classes are very simple class, so simple that they probably
 * wouldn't be used in a non-instructional setting.  In addition, Line
 * duplicates some of the functionality of the STL string class.
 */

#include <iostream>

/*
 * Table of Contents: the following classes are defined in this file.
 */
class LineGetter;
class Line;

/*
 * The following classes are used but not defined in this file.
 */
// (none)

/*
 * The LineGetter class.  This class provides only a constructor,
 * destructor, and getLine function.  The constructor automatically
 * attaches the LineGetter to a specified input stream.
 *
 * The heart of the class is the getLine function, which is a factory
 * function that produces Lines.  Each time getLine is called, a line
 * is read from the input stream specified when the LineGetter was
 * constructed.  (A "line" is defined as all characters up to the next
 * newline; the newline is swallowed as well.)  The line that was read
 * is numbered and used to construct a Line, which is returned to the
 * caller.  The Line is allocated with "new", and it is the caller's
 * responsibility to delete it later.
 *
 * When EOF is reached, getLine will return a NULL pointer to indicate
 * that fact.
 */
class LineGetter
    {
    public:
	/*
	 * Standard functions
	 */
	explicit	LineGetter(std::istream& fromStream_ = std::cin);
					// Construction from a stream
					// ..also serves as default constructor
					// ..because a default value is given
					// ..for fromStream, so it can be
					// ..called with no arguments
			~LineGetter();

	/*
	 * Factory functions
	 */
	const Line*	getLine();	// Create and return a line

    private:
	/*
	 * Disable copy and assignment.  It doesn't make sense to copy
	 * a LineGetter, because then if you got something from one
	 * LineGetter, the other one would wind up with incorrect line
	 * numbering.  Disabling copy and assignment isn't perfect,
	 * because somebody could still construct two LineGetters from
	 * the same input stream, but it's better than nothing.  There
	 * are ways to make the LineGetter handle the problem of
	 * multiple streams (and to allow copying and assignment), but
	 * they would complicate the class a LOT.  "Object-oriented
	 * programming protects you from Murphy, not Machiavelli."
	 */
			LineGetter(const LineGetter& source);
					// Copy constructor
	const LineGetter&
			operator=(const LineGetter& source);
					// Copy-assignment operator

	/*
	 * Private functions
	 */
	char*		expand(char* oldString, unsigned int oldSize,
			    unsigned int newSize)
			  const;
					// Expand an array

	/*
	 * Private data
	 */
	std::istream&	fromStream;	// Stream we take input from.
	unsigned int	nextNumber;	// Number of next line to read

	/*
	 * Internal constants
	 *
	 * Note: g++ now insists on using "static const unsigned int".
	 * Some older compilers don't like that construct, and instead
	 * insist on using the enum.  If your compiler complains about
	 * "static const int", try changing the 1 to a 0 in the
	 * following line.
	 */
						#if 1
	static const unsigned int INITIAL_LENGTH = 16;
					// Initial length of a line, including
					// ..space for the NUL byte
						#else // 1
	enum
	    {
	    INITIAL_LENGTH = 16		// Initial length of a line, including
					// ..space for the NUL byte
	    };
						#endif // 1
    };

/*
 * The Line class.  This class holds lines read from a file.  As well
 * as the contents of the line (stored as a C-style string), it tracks
 * the line number and whether the line originally ended in a newline.
 * The newline is never stored internally, but it will be
 * reconstructed on output if necessary.
 *
 * Because the Line class is intended for instructional purposes, it
 * is very simple and doesn't provide very many functions.  Besides
 * the constructors, destructor, and the copy-assignment operator, the
 * only function is a print routine that displays the line on a
 * stream.  The print function can optionally prefix the line with its
 * line number, and can be told to print a newline after the line even
 * if it didn't originally end with a newline.  (This latter option is
 * the default).
 */
class Line
    {
	/*
	 * Friends
	 *
	 * LineGetter::getLine is a friend so that it can use a
	 * special internal constructor that is not available to
	 * outsiders.  This is an example of encapsulation: by making
	 * the internal constructor private, outsiders can't know how
	 * the line number and newline status are stored, and can't
	 * accidentally call the constructor with a string that wasn't
	 * allocated by "new[]".
	 */
	friend const Line*
			LineGetter::getLine();

    public:
	/*
	 * Standard functions.  There is also a private constructor,
	 * listed below, for use by LineGetter::getLine().
	 */
			Line();		// Default constructor
			Line(const Line& source);
					// Copy constructor
			~Line();
	const Line&	operator=(const Line& source);
					// Copy-assignment operator

	/*
	 * Other public functions
	 */
	std::ostream&	print(std::ostream& stream, bool withNumber = false,
			    bool forceNewline = true)
			  const;
					// Print the line on a stream

    private:
	/*
	 * Private constructor for use by LineGetter::getline
	 */
			Line(int lineNumber_, const char* contents_,
			  bool hadNewline_);

	/*
	 * Helper function that copies strings.  This is identical to
	 * the C library "strdup" function, except that it uses
	 * "new[]" instead of "malloc".  Most C++ libraries provide a
	 * C++-style "strdup", but we will use our own anyway just in
	 * case.
	 */
	static const char*
			duplicate(const char* source);
					// Create a duplicate of a string
	/*
	 * Private data
	 */
	unsigned int	lineNumber;	// The line number in the source file
	const char*	contents;	// The actual contents of the line,
					// ..with the trailing newline
					// (if any) deleted
	bool		hadNewline;	// True if line ended with a newline
					// ..when we read it in
    };

#endif // LINE_HH
