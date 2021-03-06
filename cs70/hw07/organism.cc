/*
 * Implementation of the organism class.  This class supports
 * organisms that can be used in a genetic algorithm.
 *
 * Note that some of the aspects of the organism (e.g., the findFitness
 * function) are specific to the problem being solved.
 *
 * Public functions defined in this file:
 *
 *   Constructors and destructors:
 *	Organism::Organism();
 *	Organism::Organism(const Cities& cities_);
 *	Organism::Organism(const Organism& source);
 *	Organism::~Organism();
 *
 *   Genetic functions:
 *	void Organism::mutate();	// Do the mutation operation (in place)
 *	Organism Organism::crossover(const Organism& parent2) const;
 *					// Do the crossover operation
 *	void Organism::pickGenes(const Cities& cities_,
 *				 PickGeneOption initMethod);
 *					// Choose a (possibly random) gene list
 *   Informational functions:
 *	double Organism::getFitness() const;
 *					// Return the organism's fitness
 *	bool Organism::approxEq(const Organism& rhs) const;
 *					// Compare fitnesses approximately
 *
 *   Overloaded operators:
 *	Organism& Organism::operator=(const Organism& source);
 *	bool Organism::operator==(const Organism& rhs) const;
 *	bool Organism::operator!=(const Organism& rhs) const;
 *	bool Organism::operator<(const Organism& rhs) const;
 *	bool Organism::operator>(const Organism& rhs) const;
 *	bool Organism::operator<=(const Organism& rhs) const;
 *	bool Organism::operator>=(const Organism& rhs) const;
 *	ostream& operator<<(ostream& stream, const Organism& source);
 *
 * Private functions defined in this file:
 *
 *   Genetic functions:
 *	void Organism::findFitness();	// Calculate fitness of an organism
 */

#include "city.hh"
#include "organism.hh"
#include "randistrs.h"
#include <algorithm>			// For swap()
#include <cassert>
#include <cfloat>			// For DBL_MAX
#include <cmath>
#include <iostream>

using namespace std;

/*
 * Threshold for declaring two floating-point values approximately
 * equal.  This basically gives the number of digits (10) to use in
 * the approximate comparison.
 */
const double APPROX_EQUALITY_THRESHOLD = 1e-10;

/*
 * Create a single new organism.  The primary purpose of this
 * constructor is to allow arrays to be made.
 */
Organism::Organism()
  : chromosome(),
    fitness(DBL_MAX),
    cities(NULL)
{
}

/*
 * Create an organism whose genes will be filled in later.
 */
Organism::Organism(const Cities& cities_)
  : chromosome(),
    fitness(DBL_MAX),
    cities(&cities_)
{
}

Organism::Organism(const Organism& source)
  : chromosome(source.chromosome),
    fitness(source.fitness),
    cities(source.cities)
{
}

Organism::~Organism()
{
}

/*
 * Perform in-place DNA mutation.  In most genetic algorithms,
 * mutation simply involves choosing a random new value for a gene.
 *
 * For TSP, however, we must ensure that all genes are unique, so
 * instead we swap two randomly chosen genes.  We do this by taking
 * advantage of the fact that the integer list iterator allows us to
 * change the integers in the list.  This approach is called Exchange
 * Mutation (EM) in Larranaga et al; it is not the best mutation
 * operator but is simple to implement.
 *
 * We also swap the actual values, rather than choosing the values at
 * particular positions.  This simplifies the code without changing
 * the randomness.
 */
void Organism::mutate()
{
    if (chromosome.length() <= 1)
	return;					// Swapping doesn't make sense

    int gene1 = rd_iuniform(0, chromosome.length());
    int gene2 = rd_iuniform(0, chromosome.length() - 1);
    if (gene2 >= gene1)
	++gene2;
    /*
     * Walk through the list, looking for the genes to replace.  When
     * we find them, take advantage of write access to the iterator to
     * do the replacement.
     */
    for (IntListIterator i(chromosome);  i;  ++i) {
	if (*i == gene1)
	    *i = gene2;
	else if (*i == gene2)
	    *i = gene1;
    }


    /*
     * Since we changed the chromosome, we must recalculate our fitness.
     */
    findFitness();
}

/*
 * Perform DNA crossover.  There are several ways to do crossover in a
 * genetic algorithm.  The simplest is to choose a random position and
 * copy from the first parent up to that position, then copy from the
 * second parent after that point.  A slightly more realistic approach
 * is to walk down both parents, switching which one is copied at
 * random points (with a fairly low probability of switching).
 *
 * Neither of these algorithms works for TSP, because for TSP we must
 * ensure that every gene is unique.  Instead, we use the Order
 * Crossover (OX1) method from Larranaga, et al.  In this method, we
 * choose two random "cut points" in the organism.  Genes between the
 * cut points come from the first parent; those outside the cut points
 * come from the second parent but are not allowed to duplicate those
 * between the cut points.  To simplify coding, we take advantage of
 * the fact that all circular shifts of a tour have identical fitness.
 * That allows us to put the stuff from between the cut points at the
 * beginning of the output list.
 *
 * We use a scratch array of bools to keep track of which genes we
 * have seen.
 */
Organism Organism::crossover(
    const Organism& parent2)		// Second parent's gene list
  const
{
    assert(chromosome.length() == parent2.chromosome.length());

    Organism result(*cities);		// Where we'll build the crossover

    int nGenes = chromosome.length();
    bool* seen = new bool[nGenes];
    for (int i = 0;  i < nGenes;  ++i)
	seen[i] = false;

    /*
     * Choose cut points.  See the mother/father selection code in
     * solveTSP for a similar algorithm.  If the second cut precedes
     * the first, swap them.
     */
    int firstCut = rd_iuniform(0, nGenes);
    int secondCut = rd_iuniform(0, nGenes - 1);
    if (firstCut > secondCut)
	swap(firstCut, secondCut);
    else				// secondCut >= firstCut
	++secondCut;

    /*
     * Copy genes from between the cut points in the first parent (*this).
     */
    int position = 0;
    for (IntListIterator i(chromosome);  i;  ++i, ++position) {
	if (position >= secondCut)
	    break;
	else if (position >= firstCut) {
	    result.chromosome.pushTail(*i);
	    seen[*i] = true;
	}
    }

    /*
     * Copy genes from outside the cut points in the second parent,
     * skipping duplicates.  Start by going from the second cut point
     * to the end.
     */
    position = 0;
    IntListIterator i(parent2.chromosome);
    for (position = 0;  position < secondCut;  ++position)
	++i;
    for (  ;  i;  ++i) {
	if (!seen[*i]) {
	    result.chromosome.pushTail(*i);
	    seen[*i] = true;
	}
    }

    /*
     * Now go from the beginning up to the second cut point.
     */
    position = 0;
    for (IntListIterator j(parent2.chromosome);
      j  &&  position < secondCut;
      ++position, ++j) {
	if (!seen[*j]) {
	    result.chromosome.pushTail(*j);
	    seen[*j] = true;
	}
    }

    delete[] seen;

    /*
     * Make sure we got the right number of genes in the final result.
     */
    assert(result.chromosome.length() == chromosome.length());

    result.findFitness();		// Figure out how fit it is
    return result;
}

/*
 * Initialize an organism with a gene list.  If the non-random method
 * is used, the genes are created in numerical order.  This way
 * seeding heuristics can be used by simply sorting the gene list.
 */
void Organism::pickGenes(
    const Cities& cities_,		// Cities to pick genes for
    PickGeneOption initMethod)		// True to use random method
{
    cities = &cities_;			// Organism needs to remember cities

    int nGenes = cities->size();

    assert(nGenes >= 0);

    if (nGenes == 0)
	return;				// Nothing needs to be done

    /*
     * Picking the genes is complicated by the fact that in TSP, every
     * entry must be unique.  For efficiency, we build an array of
     * unique numbers (genes), shuffle them using the algorithm from
     * assignment 5, and then put them on the list.
     *
     * We only do the shuffling if initMethod is RANDOM.
     */
    int* newGenes = new int[nGenes];
    for (int i = 0;  i < nGenes;  ++i)
	newGenes[i] = i;

    if (initMethod == RANDOM_GENES) {
	for (int i = nGenes - 1;  i > 0;  --i) {
	    /*
	     * Pick a random gene to swap with.
	     */
	    int j = rd_iuniform(0, i + 1);
	    if (i != j)
		swap(newGenes[i], newGenes[j]);
	}
    }

    /*
     * Put the genes on the organism's gene list.
     */
    for (int i = 0;  i < nGenes;  ++i)
	chromosome.pushTail(newGenes[i]);

    delete[] newGenes;

    findFitness();			// Figure out our fitness
}

/*
 * Return the fitness of an organism.  This function is needed only
 * because the colony needs to know the fitness of the best organism
 * in each generation.  In an alternative implementation, there would
 * only be functions to print and compare fitnesses.  However, as it
 * turns out, not having this function would require the IntList class
 * to have an assignment operator!
 */
double Organism::getFitness() const	// Return the organism's fitness
{
    return fitness;
}

/*
 * Approximate comparison.  This compares fitnesses for approximate
 * equality, to compensate for the fact that floating-point addition
 * is inaccurate.
 */
bool Organism::approxEq(
    const Organism&	rhs)		// What to compare against
  const
{
    return abs(fitness - rhs.fitness) < fitness * APPROX_EQUALITY_THRESHOLD;
}

/*
 * Organism assignment.
 */
Organism& Organism::operator=(const Organism& source)
{
    chromosome = source.chromosome;
    fitness = source.fitness;
    cities = source.cities;
    return *this;
}

/*
 * Comparison functions.  These use strict equality.  Everything is
 * built in terms of == and <.
 */
bool Organism::operator==(
    const Organism& rhs)		// Organism to compare against
  const
{
    return fitness == rhs.fitness;
}

bool Organism::operator!=(
    const Organism& rhs)		// Organism to compare against
  const
{
    return !(*this == rhs);
}

bool Organism::operator<(
    const Organism& rhs)		// Organism to compare against
  const
{
    return fitness < rhs.fitness;
}

bool Organism::operator>(
    const Organism& rhs)		// Organism to compare against
  const
{
    return rhs < *this;
}

bool Organism::operator<=(
    const Organism& rhs)		// Organism to compare against
  const
{
    return !(rhs < *this);
}

bool Organism::operator>=(
    const Organism& rhs)		// Organism to compare against
  const
{
    return !(*this < rhs);
}

/*
 * I/O functions.  Printing an organism involves printing each of its
 * cities, with a newline after each.  This algorithm is appropriate
 * for a big data structure, but has the minor drawback that a newline
 * is generated after the last organism, which is inconsistent with
 * the way C++ prints small objects.  Such is life.
 */
ostream& operator<<(ostream& stream, const Organism& source)
{
    for (IntListIterator i(source.chromosome);  i;  ++i)
	stream << (*source.cities)[*i] << endl;
    return stream;
}

/*
 * Calculate the fitness of an organism.  The fitness is defined as
 * the total distance a salesrep would travel (including the distance
 * back to the beginning of the tour).
 */
void Organism::findFitness()
{
    if (cities == NULL) {
	fitness = DBL_MAX;
	return;
    }

    int nCities = cities->size();
    assert(nCities == chromosome.length());

    fitness = 0.0;
    if (nCities <= 1)			// If 0 or 1 cities, fitness always 0
	return;

    IntListIterator i(chromosome);
    const City& firstCity = (*cities)[*i];
    const City* previousCity = &firstCity;

    /*
     * We increment i so that it points to the second city and we can
     * then calculate its distance from the first.
     */
    for (++i;  i;  ++i) {
	fitness += (*cities)[*i].distanceFrom(*previousCity);
	previousCity = &(*cities)[*i];
    }

    fitness += firstCity.distanceFrom(*previousCity);
}
