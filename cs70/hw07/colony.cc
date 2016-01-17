#include "city.hh"
#include "colony.hh"
#include "mtwist.h"
#include "organism.hh"
#include "randistrs.h"
#include <cassert>
#include <cfloat>			// For DBL_MAX
#include <iostream>

/*
 * Public functions defined in this file:
 *
 *   Constructors and destructors:
 *	Colony::Colony(int size_, const Cities& cities_, InitOption howToInit);
 *	Colony::~Colony();
 *
 *   Overloaded operators:
 *	Organism& Colony::operator[](int which) const;
 *					// Find organism by index
 *
 *   Genetic functions:
 *	void Colony::evolve(bool debug, int selectionPoolSize,
 *			    double mutationProbability, int maxRandomSurvivors,
 *			    int maxGenerations);
 *					// Simulate multiple generations
 *	const Organism& Colony::findBest() const;
 *					// Find the best organism in the colony
 *	void Colony::naturalSelection(int selectionPoolSize,
 *				      int maxRandomSurvivors);
 *					// Perform natural selection
 *
 *   Helper functions:
 *	static int Colony::compareFitness(const void* first,
 *					  const void* second);
 *					// Compare fitness of two organisms
 */

using namespace std;

/*
 * Create an initial gene pool of randomly chosen organisms.
 */
Colony::Colony(
    int size_,				// Size of the colony to create
    const Cities& cities_,		// Cities to solve TSP on
    InitOption howToInit)		// Whether to use heuristic seeding
  : size(size_),
    organisms(new Organism[size_]),
    cities(cities_)
{
    assert(size > 0);

    /*
     * The organisms must be initialized to random gene lists.  Since
     * the default constructor can't do that properly (it doesn't know
     * the length), we must do it here.  This is a clear drawback of
     * C++ that could only be solved with a global variable.
     * Fortunately, the time to create the gene pool is minimal
     * compared to the time needed to solve the TSP problem.
     */
    for (int i = 0;  i < size;  ++i) {
	if (howToInit == HEURISTIC  &&  i == 0)
	    organisms[i].pickGenes(cities, Organism::FIXED_GENES);
	else
	    organisms[i].pickGenes(cities, Organism::RANDOM_GENES);
    }
}

Colony::~Colony()
{
    delete[] organisms;
}

Organism& Colony::operator[](
    int where)				// Where to index to
  const
{
    assert(where >= 0  &&  where < size);
					// Ensure a valid argument

    return organisms[where];
}

/*
 * Simulate a number of generations
 */
void Colony::evolve(
    bool debug,				// Produce debugging information
    int selectionPoolSize,		// Size of selection pool (see below)
    double mutationProbability,		// Probability of a mutation
    int maxRandomSurvivors,		// Maximum number of "unfit" survivors
					// ..to next generation
    int maxGenerations)			// Number of generations to run for
{
    /*
     * If we are debugging, we will print out the best candidate each
     * time a new one is found.  To do this, we must track the best
     * fitness found so far.  We start with the best set to the
     * largest possible number so that anything we find will be better.
     */
    double bestFitness = DBL_MAX;

    /*
     * Simulate the requested number of generations
     */
    for (int generation = 0;  generation < maxGenerations;  ++generation) {
	/*
	 * Choose the candidates who will survive until the next
	 * generation and put them at the front of the colony.
	 */
	naturalSelection(selectionPoolSize, maxRandomSurvivors);

	/*
	 * If we're debugging, report any improvement in the colony's
	 * best candidate.
	 */
	if (debug) {
	    const Organism& best = findBest();
	    double newFitness = best.getFitness();
	    if (newFitness < bestFitness) {
		bestFitness = newFitness;
		cerr << "Generation " << generation << ": " << bestFitness
		     << endl;
	    }
	}

        /*
         * Fill up the remainder of the colony with new organisms
         * whose parents have been selected from the survivors list.
         */
	for (int organism = selectionPoolSize;  organism < size;  ++organism) {
	    /*
	     * Choose two parents randomly.  Although we use the terms
	     * "father" and "mother", reproduction is actually
	     * asexual; any two organisms can be the parents.  We take
	     * the trouble to be sure the father and mother are
	     * actually different organisms.  The latter is managed by
	     * noting that there are N choices for the father, but
	     * only N-1 for the mother; we simply remap those N-1
	     * options so that they exclude the father but include all
	     * other organisms.
	     */
	    int father = rd_iuniform(0, selectionPoolSize);
	    int mother = rd_iuniform(0, selectionPoolSize - 1);
	    if (mother >= father)
		++mother;

	    /*
	     * Cross the parents at a randomly selected place in the
	     * gene string.  Note that this is a simplified
	     * approximation of the way real genetic crossover works.
	     */
	    organisms[organism] =
	      organisms[father].crossover(organisms[mother]);

	    /*
	     * Finally, introduce a random mutation (with small
	     * probability).  Again, this is a rough approximation of
	     * reality.
	     */
	    if (mt_drand() <= mutationProbability)
		organisms[organism].mutate();
	}
    }
}

/*
 * Find the best organism in a colony.  "Best" is defined simply as
 * the organism with the lowest fitness.  In case of ties, the
 * first-found is chosen.
 */
const Organism& Colony::findBest()
  const
{
    const Organism* best = &organisms[0];
					// "best" is the best found so far

    for (int i = 1;  i < size;  ++i) {
	if (organisms[i] < *best)
	    best = &organisms[i];
    }

    return *best;
}

/*
 * Do natural selection by choosing the organisms that have the
 * greatest fitness and placing them into a preselected section of
 * the colony.
 *
 * The selection is done by the simple expedient of sorting the
 * organisms by fitness, with the most fit placed first, but with a
 * little bit of randomness thrown in.
 */
void Colony::naturalSelection(
    int selectionPoolSize,		// Size of selection pool
    int maxRandomSurvivors)		// Maximum no. of random survivors
{
    /*
     * Selection boils down to just sorting.  I'm old-fashioned, so
     * I'm going to use qsort.  Frankly, I think the interface is
     * better than the STL sort monstrosity.  See "man qsort" for more
     * information.
     */
    qsort(organisms, size, sizeof organisms[0], compareFitness);

    /*
     * In real genetics, the most-fit organisms sometimes don't
     * survive, and instead lesser-fit ones reproduce.  This turns out
     * to be a good thing, because it introduces important variance
     * into the genetic pool.  We'll randomize things very slightly to
     * simulate this behavior.
     *
     * Note that this simulation is poor in that all unfit organisms
     * have an equal chance to survive.  A better implementation would
     * make the probability of survival dependent on the fitness.
     */
    int randomSurvivors = rd_iuniform(0, maxRandomSurvivors);
    for (int i = 0;  i < randomSurvivors;  ++i) {
	int firstSlot = rd_iuniform(0, selectionPoolSize);
	int secondSlot = rd_iuniform(0, size);
	if (firstSlot != secondSlot)
	    swap(organisms[firstSlot], organisms[secondSlot]);
    }
}

/*
 * Compare the fitness of two organisms, returning a result useful
 * with qsort: <0 means the first organism precedes the second, 0
 * means they are equal, and >0 means the second organism should go
 * first.  Because of how qsort works, the arguments are of type void*
 * even though they point to integer lists.  Note that lower fitnesses
 * are considered to be better.
 */
int Colony::compareFitness(
    const void* first,			// Items to compare
    const void* second)			// ...
{
    const Organism& firstOrganism = *(const Organism*)first;
    const Organism& secondOrganism = *(const Organism*)second;

    /*
     * Figure out who goes first.  We can't just return the difference
     * between the two fitnesses, because then we would produce the
     * wrong result if the difference couldn't be represented by an
     * integer.
     */
    if (firstOrganism < secondOrganism)
	return -1;
    else if (firstOrganism > secondOrganism)
	return 1;
    else
	return 0;
}
