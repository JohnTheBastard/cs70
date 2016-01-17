/*
 * Name: Geoff Kuenning
 * Course: CS 70, Spring 2004
 * Assignment #7
 *
 * Copyright 2004, Geoff Kuenning.
 *
 * This file contains the main program for the assignment.
 *
 * The program implements a very simple genetic algorithm, using
 * straightforward (but inefficient) data structures.  The algorithm
 * solves the Traveling Salesrep Problem through a process of natural
 * selection.
 *
 * Usage:
 *
 *	assign_07 [options] file ...
 *
 * In its simplest form, the program is invoked with a one or more
 * positive integer arguments.  For each argument, a random population
 * is generated and an evolutionary simulation searches for a value
 * that, when squared, is as close to the given integer as possible.
 *
 * Switches can be used to modify the program behavior as follows:
 *
 *	-d	Produce debugging output, showing the execution
 *		parameters, and showing the fitness and generation
 *		number each time a better individual is found.
 *	-g n	Specify the number of generations to run for (default 50).
 *	-h	Use a trivial heuristic to seed the first generation.
 *		The heuristic simply generates one organism that has
 *		the cities in the same order as the input file.  If
 *		the input is grouped in some reasonable manner (e.g.,
 *		by state or country), this is likely to produce a
 *		better initial tour and thus a better solution.
 *	-m n	Specify the probability of genetic mutation (default
 *		0.001).  Each time a new organism is generated, it is
 *		mutated with a probability equal to this value.
 *	-p n	Specify the size of the population of organisms
 *		(default 1000).  This is the number of organisms that
 *		compete to survive into the next generation.
 *	-r n	Specify the maximum number of random survivors into
 *		the next generation (default 2).  In each generation,
 *		up to this number of the fittest organisms are
 *		replaced by randomly chosen ones.
 *	-s n	Specify the size of the selection pool (default 10).
 *		This is the number of organisms that survive into the
 *		next generation.  Note that surviving gives an
 *		organism a chance to become a parent, but does not
 *		guarantee it.  The difference between the -p value and
 *		the -s value is that the latter controls the number of
 *		new organisms produced at the beginning of each
 *		generation.  The smaller this value, the "harsher" the
 *		environment:  only the best organisms survive.  As a
 *		general rule, a harsh environment is more likely to
 *		produce good results.
 *	-S n	Specify a seed for the random-number generator.  If no
 *		random seed is given, one is derived from the time of
 *		day.
 *	-v	Produce verbose output.  Causes the list of cities to
 *		be included along with the fitness achieved.
 *
 * The input file contains a list of cities and latitudes and
 * longitudes, separated by whitespace.  The Earth is assumed to be
 * spherical, and great-circle distances will be calculated.  Each
 * city has a name (which may not contain whitespace) that is followed
 * by the latitude and longitude in decimal degrees.  South latitudes
 * and west longitudes are negative.
 *
 * Each organism in the colony is a route, represented for
 * convenience as a linked list of city numbers.  The representation
 * is very handy for doing genetic crossover and mutation, and is easy
 * to understand, but it is quite inconvenient for use in squaring a
 * number.  The reader should note that the representation is quite
 * inefficient in both space and time.  Serious users of genetic
 * algorithms put great effort into efficient representations, because
 * they may simulate many hundreds of thousands of generations, each
 * with thousands or millions of organisms.
 *
 * The crossover and mutation algorithms are taken from P. Larranaga,
 * C. M. H. Kujipers, R. H. Murga, I. Inza and S. Dizdarevic, "Genetic
 * Algorithm for the Travelling Salesman Problem: A Review of
 * Representations and Operations," Artificial Intelligent Review,
 * vol. 13, no. 2, pp. 129-170, April 1999.  We use the OX1 crossover
 * algorithm and the EM mutator.
 * 
 * Many other aspects of this program are simplified in other ways.
 * The program should not be taken as an example of how to implement a
 * genetic algorithm correctly, although it can be useful in
 * understanding the general method.
 */

#include "city.hh"
#include "colony.hh"
#include "mtwist.h"
#include "organism.hh"
#include "randistrs.h"
#include <fstream>
#include <iostream>

using namespace std;

/*
 * Table of contents: the following functions are defined in this file
 */
int main(int argc, char* argv[]);	// Main genetic algorithm
static int processOptions(int argc, char* argv[], bool& debug, bool& verbose,
			  bool& heuristic,
			  int& colonySize, int& selectionPoolSize,
			  double& mutationProbability,
			  int& maxRandomSurvivors, int& maxGenerations);
					// Process option arguments
static void usage(char* programName);
					// Issue a usage message
static void solveTSP(istream& stream, bool debug, bool verbose, bool heuristic,
		     int colonySize, int selectionPoolSize,
		     double mutationProbability, int maxRandomSurvivors,
		     int maxGenerations);
					// Find square root genetically

/*
 * Defaults for switch-selectable arguments.
 */
const int COLONY_SIZE = 1000;		// -p: Number of organisms in
					// ..population
const int MAX_GENERATIONS = 50;		// -g: Number of generations
const int MAX_RANDOM_SURVIVORS = 2;	// -r: Number of random survivors to
					// ..next generation.
const double MUTATION_PROBABILITY = 0.001;
					// -m: Probability of mutation
const int SELECTION_POOL_SIZE = 10;	// -s: Number of survivors to
					// ..next generation.

/*
 * Main driver for testing DNA recombination.  This function processes
 * all option arguments, generates a pool of organisms, and then
 * applies the rules of natural selection to try to find the one that
 * best satisfies the fitness criterion.  In this case, a fit organism
 * is one that minimizes the distance a salesrep must travel.
 */
int main(
    int argc,				// Argument count
    char* argv[])			// Argument vector
{
    int colonySize;			// Number of organisms to simulate
    bool debug;				// True to write debugging output
    bool heuristic;			// True to use seeding heuristic
    int maxGenerations;			// Number of generations to run
    double mutationProbability;		// Probability of a mutation
    int maxRandomSurvivors;		// Maximum number of "unfit" survivors
					// ..to next generation
    int selectionPoolSize;		// Number of survivors to next
					// ..generation
    bool verbose;			// True to produce verbose output

    /*
     * Process all option arguments.  Note that the last four
     * arguments are reference parameters, which means that the
     * function can modify them.  The return value is the number of
     * the first non-option argument on the command line.
     */
    int argNo = processOptions(argc, argv, debug, verbose, heuristic,
			       colonySize, selectionPoolSize,
			       mutationProbability, maxRandomSurvivors,
			       maxGenerations);

    /*
     * If no file names are specified, solve the TSP from standard input.
     */
    if (argNo >= argc)
	solveTSP(cin, debug, verbose, heuristic, colonySize, selectionPoolSize,
		 mutationProbability, maxRandomSurvivors, maxGenerations);
    else {
	/*
	 * Solve the TSP separately for each file
	 */
	for (  ;  argNo < argc;  ++argNo) {
	    ifstream currentFile(argv[argNo]);
	    if (currentFile)
		solveTSP(currentFile, debug, verbose, heuristic, colonySize,
			 selectionPoolSize, mutationProbability,
			 maxRandomSurvivors, maxGenerations);
	    else
		cerr << "Couldn't open '" << argv[argNo] << "': "
		     << strerror(errno) << endl;
	}
    }

    return 0;
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
    int argc,				// Argument count passed to main
    char* argv[],			// Argument vector passed to main
    bool& debug,			// MODIFIED: set true if debugging
					// ..enabled
    bool& verbose,			// MODIFIED: set true if verbosity
					// ..enabled
    bool& heuristic,			// MODIFIED: set true if initial
					// ..heuristic enabled
    int& colonySize,			// MODIFIED: set to number of
					// ..organisms to simulate
    int& selectionPoolSize,		// MODIFIED: set to number of
					// ..survivors into next generation
    double& mutationProbability,	// MODIFIED: set to probability of
					// ..genetic mutations
    int& maxRandomSurvivors,		// MODIFIED: set to maximum number of
					// ..randomly-chosen survivors
    int& maxGenerations)		// MODIFIED: set to number of
					// ..generations to simulate
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
    debug = false;
    verbose = false;
    heuristic = false;
    colonySize = COLONY_SIZE;
    selectionPoolSize = 0;
    maxGenerations = MAX_GENERATIONS;
    mutationProbability = MUTATION_PROBABILITY;
    selectionPoolSize = SELECTION_POOL_SIZE;
    maxRandomSurvivors = MAX_RANDOM_SURVIVORS;

    mt_seed();				// Pick a seed for the PRNG

    /*
     * Note that the loop index is modified inside the body of
     * the loop; this is generally poor style but is common in
     * argument processing.  In this case, the modification is done
     * inside the processing of the "s" switch.
     *
     * Also note that this code is illustrative.  Most modern programs
     * use getopt(3) or getopt_long(3) to help process options in a
     * standard fashion.
     */
    int argNo;
    for (argNo = 1;  argNo < argc;  ++argNo) {
	if (argv[argNo][0] != '-')	// End of switches?  If so, exit
	    break;			// ..option processing

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
	if (argv[argNo] == string("--")) {
	    ++argNo;			// Skip over the --
	    break;			// Go to non-option argument processing
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
	switch (argv[argNo][1]) {
	    case 'd':			// -d: Turn on debugging
		debug = true;
		break;

	    case 'g':			// -g n: specify generations
		{
		/*
		 * Processing an option that has an argument is a bit
		 * tricky.  First, we must make sure that there is
		 * actually a following argument.  Second, we must
		 * process that argument appropriately (in this case,
		 * by converting it to an integer and saving it in
		 * maxGenerations).  Third, we must make sure that the
		 * option-processing loop does not try to treat the
		 * argument as a switch itself.  We do that by
		 * incrementing the loop index, as noted in the
		 * comments before the loop.  The first and third
		 * operations are combined.
		 */
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		/*
		 * Convert the next argument to an integer (strtol) and
		 * save it.  If *lastValid isn't '\0', the conversion failed.
		 */
		char* lastValid;
		maxGenerations = strtol(argv[argNo], &lastValid, 10);
		if (*lastValid != '\0')
		    usage(argv[0]);
		break;
		}

	    case 'h':			// -h: use heuristic seeding
		heuristic = true;
		break;

	    case 'm':			// -m n: specify mutation probability
		{
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		/*
		 * Convert the next argument to a double (strtod) and
		 * save it.  If *lastValid isn't '\0', the conversion failed.
		 */
		char* lastValid;
		mutationProbability = strtod(argv[argNo], &lastValid);
		if (*lastValid != '\0')
		    usage(argv[0]);
		break;
		}

	    case 'p':			// -p n: specify colony size
		{
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		char* lastValid;
		colonySize = strtol(argv[argNo], &lastValid, 10);
		if (*lastValid != '\0')
		    usage(argv[0]);
		break;
		}

	    case 'r':			// -r n: specify max random survivors
		{
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		char* lastValid;
		maxRandomSurvivors = strtol(argv[argNo], &lastValid, 10);
		if (*lastValid != '\0')
		    usage(argv[0]);
		break;
		}

	    case 's':			// -s n: specify selection pool size
		{
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		char* lastValid;
		selectionPoolSize = strtol(argv[argNo], &lastValid, 10);
		if (*lastValid != '\0')
		    usage(argv[0]);
		break;
		}

	    case 'S':			// -S n: specify random seed
		{
		if (++argNo >= argc)	// Make sure there's an argument
		    usage(argv[0]);	// ..if not, complain and exit

		char* lastValid;
		long randomSeed = strtol(argv[argNo], &lastValid, 10);
		if (*lastValid != '\0')
		    usage(argv[0]);

		mt_seed32new(randomSeed);
					// Set seed to specified value
		break;
		}

	    case 'v':
		{
		verbose = true;
		break;
		}

	    default:			// Default: unrecognized option
		usage(argv[0]);
		break;
	}
    }

    /*
     * Check the legality of invocation.  Now that we have extracted
     * all of the user's options, make sure that they are legal and
     * that they are consistent with each other.  If any problems are
     * detected, either abort the program or correct them.
     */
    if (maxGenerations <= 0) {
	cerr << "Number of generations must be positive.\n";
	usage(argv[0]);
    }
    if (colonySize <= 0) {
	cerr << "Colony size must be positive.\n";
	usage(argv[0]);
    }
    if (selectionPoolSize > colonySize) {
	cerr
	  << "Selection pool cannot be bigger than colony;  reducing it to "
	  << colonySize << endl;
	selectionPoolSize = colonySize;
    }
    if (maxRandomSurvivors > selectionPoolSize) {
	cerr
	  << "Random survivors cannot be bigger than selection pool; reducing "
	  << "it to " << selectionPoolSize << endl;
	maxRandomSurvivors = selectionPoolSize;
    }

    if (debug) {
	cerr << maxGenerations << " generations\n";
	cerr << colonySize << " organisms\n";
	cerr << selectionPoolSize << " survive in each generation\n";
	cerr << maxRandomSurvivors << " survive randomly\n";
	cerr << mutationProbability << " probability of mutation" << endl;
    }

    /*
     * Argument processing is done.  The calling function needs to
     * know how many arguments we swallowed so that it can tell where
     * the non-option arguments begin.  So we return the number of the
     * first non-option argument.
     */
    return argNo;
}

/*
 * Print a usage message and exit.  As is conventional, the exit
 * status is 2 if there is a usage error.
 */
static void usage(
    char* programName)			// Name we were run under
{
    cerr << "Usage:\t" << programName <<
      " [-d] [-g n] [-h] [-m n] [-p n] [-r n] [-s n] [-S n] [-v] file ...\n";
    cerr << "Switches:\n";
    cerr << "\t-d\tProduce debugging outout.\n";
    cerr << "\t-g n\tGive number of generations to run for.\n";
    cerr << "\t-h\tUse heuristic seeding.\n";
    cerr << "\t-m n\tGive probability of mutation.\n";
    cerr << "\t-p n\tGive population size.\n";
    cerr << "\t-r n\tGive number of randomly-chosen survivors.\n";
    cerr << "\t-s n\tGive selection pool size (number of survivors).\n";
    cerr << "\t-S n\tSet random seed\n";
    exit(2);
}

/*
 * Use a genetic algorithm to solve the traveling-salesrep problem.
 * The algorithm operates by repeatedly generating "organisms" that
 * are then tested for "fitness".  In this case, a fit organism is one
 * where the total distance traveled is small.  In each generation,
 * genetic crossover and mutation are used to produce new organisms.
 * In addition, the fittest organisms survive unchanged into the next
 * generation.
 */
static void solveTSP(
    istream& stream,			// Stream with city names & coordinates
    bool debug,				// Produce debugging information
    bool verbose,			// Produce verbose output
    bool heuristic,			// Use seeding heuristic
    int colonySize,			// Size of colony
    int selectionPoolSize,		// Size of selection pool (see below)
    double mutationProbability,		// Probability of a mutation
    int maxRandomSurvivors,		// Maximum number of "unfit" survivors
					// ..to next generation
    int maxGenerations)			// Number of generations to run for
{
    /*
     * Read the cities into an array.  The array is allocated with
     * new[], and it is our responsibility to delete it at exit.
     */
    Cities cities(stream);

    /*
     * Start with a random population of the chosen size.
     */
    Colony colony(colonySize, cities,
      heuristic ? Colony::HEURISTIC : Colony::RANDOM);

    /*
     * Do evolution
     */
    colony.evolve(debug, selectionPoolSize, mutationProbability,
      maxRandomSurvivors, maxGenerations);

    /*
     * Report the best organism.
     */
    const Organism& winner = colony.findBest();
    if (verbose)
	cout << winner << '\n';		// Bug: winner puts out own newline
					// ..so the above double-spaces

    cout << "Total distance: " << winner.getFitness() << endl;
}
