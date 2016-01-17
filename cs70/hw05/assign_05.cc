/*
 * Name: Geoff Kuenning
 * Course: CS 70, Spring 2004
 * Assignment #5
 *
 * Simple file-shuffling program
 *
 * This program provides a simple file-shuffling command.  If invoked
 * with no arguments, it reads an entire file from the standard input
 * device, shuffles it, and writes the scrambled file to standard
 * output.  Optionally, it can instead read from one or more files
 * named on the command line (concatenating them together like 'cat').
 *
 * Usage:
 *
 * ./shuffle [-d] [-n] [-s n] [file] ...
 *
 * The specified file(s) (or standard input if none are given) are
 * read and saved internally.  After all files have been swallowed,
 * they are shuffled and the result is written to standard output.
 *
 * Options:
 *
 *	-d	Give debugging output.
 *	-n	Print a line number before each output line.  The line
 *		numbers are assigned as the files are read, and are
 *		not reset between files.  Thus, the line numbers will
 *		be unique, but will appear on the output in a
 *		scrambled order.
 *	-s n	Specify a numeric random seed for the pseudo
 *		random-number generator.  This allows the program to
 *		produce reproducible results, which is useful in
 *		testing.
 */

#include "line.hh"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <errno.h>
#include <stdlib.h>
#include <sys/time.h>

using namespace std;

/*
 * Table of Contents: the following functions are defined in this file
 */
int			main(int argc, char* argv[]);
					// Shuffle lines of a file
static int		processOptions(int argc, char* argv[],
			  bool& showLineNumbers);
					// Process option arguments
static void		usage(char* progname);
					// Issue a usage message
static void		setRandomSeed();
					// Set a "random" random seed
static int		doShuffling(int argc, char* argv[], int firstArg,
			  bool showLineNumbers);
					// Do the actual shuffling of files
static void		snagFile(istream& file, vector<Line>& whereToPutIt);
					// Read a file into a vector
static void		randomize(vector<Line>* array);
					// Randomize an array of Lines

/*
 * The random-seed function needs to know the size of the machine
 * word.  It also needs to run the pseudorandom-number generator
 * (PRNG) for a few cycles to make sure it has begun a unique
 * sequence.  These two constants define those values.
 */
const unsigned int	LONG_BITS = sizeof(long) * 8;
					// Number of bits in a long
const unsigned int	RAN_ITERS = 10;	// No. of iterations for random init

/*
 * Main driver for the shuffling program
 */
int main(
    int			argc,		// Argument count
    char*		argv[])		// Argument vector
    {
    bool		showLineNumbers = false;
					// True to show line nos. on output

    /*
     * Process all option arguments.  Note that "showLineNumbers" is a
     * reference parameter, which means that the function can modify
     * it.  The return value is the number of the first non-option
     * argument on the command line.
     */
    int argNo = processOptions(argc, argv, showLineNumbers);

    /*
     * At this point, any arguments that are left are the names of
     * files to be processed.  The shuffling routine will return a
     * success/failure indication (nonzero for failure).
     */
    return doShuffling(argc, argv, argNo, showLineNumbers);
    }

/*
 * Option processing.  Scan through the arguments, looking at
 * switches.  If we run into a non-switch argument, exit the function.
 *
 * If an illegal switch is detected, we will print a usage message to
 * cerr and terminate the program with an exit status of 2.
 *
 * The return value of this function is the number of the first
 * non-switch argument encountered.  If there are no non-switch
 * arguments, the return value equals argc.
 *
 * The reader will note that this function is fairly long, even though
 * it does relatively little.  That is generally true of
 * switch-processing functions, and it is not considered bad style so
 * long as the function limits itself to simple operations such as
 * setting Boolean flags.
 */
static int processOptions(
    int			argc,		// Argument count passed to main
    char*		argv[],		// Argument vector passed to main
    bool&		showLineNumbers)
					// MODIFIED: set to true if line
					// ..numbers should be
					// ..displayed, false
					// ..otherwise
    {
    /*
     * Before beginning the loop, we set defaults for all options.  By
     * doing the defaults in a single place, it is easy to change the
     * defaults later if necessary.
     *
     * One of the defaults is the random seed.  If the -s switch
     * appears, we can then clobber the random seed with the specified
     * value.
     */
    showLineNumbers = false;
    setRandomSeed();			// Pick a seed for the PRNG


    /*
     * Note that the loop index is modified inside the body of
     * the loop; this is generally poor style but is common in
     * argument processing.  In this case, the modification is done
     * inside the processing of the "s" switch.
     */
    for (int argNo = 1;  argNo < argc;  argNo++)
	{
	if (argv[argNo][0] != '-')	// End of switches?  If so, exit
	    return argNo;		// ..option processing

	/*
	 * By convention, the special switch option "--" means that
	 * all following arguments are non-option arguments, even if
	 * they start with a hyphen.  If we run into that switch, we
	 * must skip over it but then exit the option-processing loop.
	 *
	 * We use C++ strings in the comparison here primarily for
	 * illustrative purposes.  Note that we must explicitly
	 * typecast one of the arguments of the == operator to
	 * "string" or the comparison won't work -- and we WILL NOT
	 * get any compiler error messages!
	 */
	if (argv[argNo] == string("--"))
	    {
	    argNo++;			// Skip over the --
	    return argNo;		// Go to non-option argument processing
	    }

	/*
	 * All other switch options should consist of a single hyphen,
	 * an alphabetic character, and nothing else.  At this point
	 * we know that the hyphen exists.  Check to make sure that
	 * there is exactly one character following it.
	 */
	if (argv[argNo][2] != '\0')
	    usage(argv[0]);

	/*
	 * Process switch options.  We do this in a "switch" statement
	 * so that it is easy to add new options later.
	 */
	switch (argv[argNo][1])
	    {
	    case 'd':
		{
		struct timeval tv;
		struct timezone tz;
		(void) gettimeofday (&tv, &tz);
		cerr << tv.tv_sec << '.' << tv.tv_usec << endl;
		break;
		}
	    case 'n':			// -n: number lines on output
		{
		showLineNumbers = true;
		break;
		}

	    case 's':			// -s n: specify random seed
		{
		/*
		 * Processing an option that has an argument is a bit
		 * tricky.  First, we must make sure that there is
		 * actually a following argument.  Second, we must
		 * process that argument appropriately (in this case,
		 * by converting it to a "long" and passing it to
		 * srand48).  Third, we must make sure that the
		 * option-processing loop does not try to treat the
		 * argument as a switch itself.  We do that by
		 * incrementing the loop index, as noted in the
		 * comments before the loop.  The first and third
		 * operations are combined.
		 */
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		/*
		 * Convert the next argument to a long (atol) and pass
		 * it to srand48.  If the next argument isn't an
		 * integer, atol will probably return zero.  But
		 * that's OK, because the behavior of the program will
		 * still be reproducible.
		 */
		srand48(atol(argv[argNo]));
		break;
		}

	    default:			// Default: unrecognized option
		{
		usage(argv[0]);
		break;
		}
	    }
	}

    /*
     * We can only get here if we ran out of arguments.  Return argc
     * to the caller, indicating that there are no non-option
     * arguments.
     */
    return argc;
    }

/*
 * Issue a usage message to cerr, and terminate the program with a
 * failure status.
 */
static void usage(			// Issue a usage message
    char*		progname)	// Name we were executed under
    {
    cerr << "Usage: " << progname << " [-n] [-s n] [file] ...\n";
    cerr << "Switches:\n";
    cerr << "\t-d\tGive debugging output\n";
    cerr << "\t-n\tAdd line numbers to the printed output.\n";
    cerr << "\t-s n\tSpecify the random seed to be used.\n";
    exit(2);
    }

/*
 * Set up a "random" random seed.  This routine works by using the
 * time of day to initialize the pseudorandom number generator.  Then,
 * since the time of day tends to be a fairly predictable number, it
 * cycles the PRNG a few times to make sure that it has entered a
 * unique part of its sequence.
 *
 * On a machine that supports /dev/random, a better way to initialize
 * the PRNG would be to read from that device.  However, doing so would
 * complicate the code without adding instructional value.
 */
static void setRandomSeed()
    {
    /*
     * Get the time of day in seconds and microseconds.  Note that the
     * system clock may not have microsecond resolution, so there is
     * no guarantee that all of the microsecond bits are significant
     */
    struct timeval tv;			// Time of day, for getting usecs
    struct timezone tz;			// Dummy for gettimeofday
    (void) gettimeofday (&tv, &tz);

    /*
     * Calculate a 48-bit random seed, and use it (seed48) to
     * initialize the random-number generator.  The seed is composed
     * of the Unix time, in microsecond resolution, converted to a
     * 48-bit number.  We would have to do some pretty hairy integer
     * arithmetic with 32-bit numbers to get a correct seed value, and
     * frankly it's just not worth it.  (This is a case where access
     * to assembly language would greatly simplify the task, but that
     * would be very non-portable.)  That leaves us with two choices:
     * (1) use the "long long" type that g++ provides, but which is
     * not part of the C++ standard (yet), or (2) use double-precision
     * arithmetic to calculate intermediate results.  The latter
     * option has the advantage that a double supports 56 bits in its
     * mantissa, so it can handle the problem, but it is also
     * portable.  We'll do it that way.
     *
     * However, in using double-precision arithmetic we must be
     * careful to avoid variations in the way different machines
     * convert numbers that are larger than the maximum integer.  Some
     * truncate; others set the result to the largest integer.  So
     * we're careful to be sure that our results are never larger than
     * the largest representable integer.
     *
     * It's worth noting that the microsecond-adjusted time is
     * actually about 52 bits wide (the Unix time is up to 32 bits,
     * and the microseconds take just a hair under 20 bits).  Thus,
     * the first truncation must discard the top 4 bits.  That's not a
     * problem, since those bits are the most slowly varying of all.
     *
     * Also note that in setting seed16[2], we couldn't use a single
     * integral divisor because its value would overflow the size of a
     * long.  That's why we use a const double and divide twice.  An
     * alternative would be to type in the value of 2^32 by hand as a
     * double, which would be easy but slightly less readable.  Since
     * this routine only executes once, the time lost in doing the
     * calculation dynamically is irrelevant.
     */
    double wideSeed = (unsigned long)tv.tv_sec * 1000000.0 + tv.tv_usec;
    unsigned short seed16[3];
    const double divisor = 1ul << 16;
    unsigned long topBits = (unsigned long)(wideSeed / divisor / divisor);
    seed16[2] = (unsigned short)topBits;
    wideSeed -= topBits * divisor * divisor;
    seed16[1] = (unsigned short)(wideSeed / divisor);
    wideSeed -= seed16[1] * divisor;
    seed16[0] = (unsigned short)wideSeed;

    seed48(seed16);

    /*
     * The above calculation will tend to generate a random seed that
     * has predictable values in some bits and unpredictable values in
     * others.  Cycle the PRNG a few times so that we will have
     * departed from the predictable part of the sequence.
     */
    for (unsigned int i = 0;  i < RAN_ITERS;  i++)
	drand48();
    }

/*
 * Do the actual shuffling of files.  This function reads the input
 * (either standard input or the list of files), shuffles them, and
 * prints the results.
 *
 * Some notes for CS70 students: it would be much more efficient to
 * have "entireInput" be declared as a vector<Line*> rather than
 * vector<Line>.  (Doing so would require changes to several places in
 * the program.)  Similarly, it would be stylistically better for
 * "randomize" to accept "entireInput" as a reference parameter,
 * rather than taking it as a pointer.  I have chosen to use a pointer
 * because it lets me illustrate some debugger features.
 * 
 * Since this
 * program will usually be used on relatively small files, the
 * inefficiency and non-C++-ish style won't matter.  However, if you
 * plan to use it in the future, you might wish to modify it after you
 * understand how to use pointers and references more effectively.
 */
static int doShuffling(
    int			argc,		// Argument count given to main
    char*		argv[],		// Argument vector given to main
    int			firstArg,	// First argument to process
    bool		showLineNumbers)
					// True to show line nos. on output
    {
    int			failure = 0;	// NZ if something went wrong

    /*
     * If there are no filenames, then swallow the standard input.
     * Otherwise, open each file in turn and add it to "entireInput".
     */
    vector<Line> entireInput;
    if (firstArg == argc)
	{
	/*
	 * No more arguments: read from standard input.
	 */
	snagFile(cin, entireInput);
	}
    else
	{
	/*
	 * Read from each remaining argument in turn.
	 */
	for (int argNo = firstArg;  argNo < argc;  argNo++)
	    {
	    /*
	     * Open the next file.  If we can't open it, issue a message.
	     */
	    ifstream nextFile(argv[argNo]);
	    if (!nextFile)
		{
		/*
		 * We couldn't open the file.  Issue an error message
		 * (the strerror call will produce an explanation;
		 * it's defined in errno.h).  Then set the failure
		 * flag for later and continue with the next file.
		 */
		cerr << "Couldn't open '" << argv[argNo] << "': "
		  << strerror(errno) << endl;
		failure = 1;
		continue;
		}

	    /*
	     * The open succeeded.  Read the file.
	     */
	    snagFile(nextFile, entireInput);

	    /*
	     * At the end of the loop, "nextFile" will be
	     * automatically destroyed.  As a side effect, the
	     * associated file will be closed.  So we don't have to
	     * worry about that detail.
	     */
	    }
	}

    /*
     * We have now read the input.  Randomize it.
     */
    randomize(&entireInput);

    /*
     * Finally, print the randomized data.
     */
    for (unsigned int i = 0;  i < entireInput.size();  i++)
	entireInput[i].print(cout, showLineNumbers);

    return failure;
    }

/*
 * Read a file, using a LineGetter, and append the lines gotten from
 * it onto the end of a vector.  We pass the vector by reference for
 * efficiency; that also allows us to modify the vector directly
 * instead of returning it as the result of the function.
 */
static void snagFile(
    istream&		file,		// Open file to read from
    vector<Line>&	whereToPutIt)	// Vector to put the file into
    {
    LineGetter grabber(file);
    const Line* nextLine;
    while ((nextLine = grabber.getLine()) != NULL)
	{
	whereToPutIt.push_back(*nextLine);
	delete nextLine;
	}
    }


/*
 * Randomize an array of Lines.  The array passed as a parameter is
 * shuffled, and is returned as the value of the function.
 *
 * The shuffling algorithm is a fairly standard one: for each entry in
 * the array, we pick some other entry at random and swap the original
 * with that one.  This will usually mean that some elements are moved
 * more than once, but that is not a serious drawback.
 */
static void randomize (			// Randomize the line table
    vector<Line>*	array)		// Array to be randomized
    {
    /*
     * Shuffle the array by swapping each line with a second randomly
     * selected line.
     */
    for (unsigned int i = 0;  i < array->size();  i++)
	{
	/*
	 * Pick a random guy to swap with.  "Swap" is a templated STL
	 * function; it can be used on any type.
	 */
	unsigned int j = (unsigned int) (drand48() * array->size());
	if (i != j)
	    swap((*array)[i], (*array)[j]);
	}
    }
