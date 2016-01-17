#ifndef ORGANISM_HH
#define ORGANISM_HH

/*
 * Class to support organisms in the genetic algorithm.  Most of the
 * problem-specific code is in this class.  An organism is basically
 * just a list of genes that define a trial solution to the problem in
 * question.  We provide output (but not input) of organisms, a
 * function to set an organism to a random set of genes, and functions
 * to compare organisms and discover their fitness.  There are also
 * public functions to implement the two operations of genetic
 * recombination: mutation and crossover.
 *
 * For most genetic algorithms, these last two functions are generic.
 * However, for the Traveling Salesrep Problem, they must be
 * specific to the problem, because duplicate genes are a violation of
 * the rules.
 */

#include "intlist.hh"
#include <iostream>

/*
 * Classes defined in this file
 */
class Organism;

/*
 * Classes mentioned, but not defined, in this file:
 */
class Cities;
class City;

/*
 * The Organism class implements a single organism within a colony.
 * The genetic functions implement the process of breeding.  The
 * comparison operators allow the fitness of two organisms to be
 * compared.
 */
class Organism {
    public:
	Organism();
	explicit Organism(const Cities& cities_);
	Organism(const Organism& source);

	~Organism();

	/*
	 * Options to the pickGenes function
	 */
	enum PickGeneOption {
	    FIXED_GENES,		// Set genes to numbers 0-n
	    RANDOM_GENES		// Choose genes randomly
	};

	/*
	 * Genetic functions
	 */
	void mutate();			// Do the mutation operation (in place)
	Organism crossover(const Organism& parent2) const;
					// Do the crossover operation
	void pickGenes(const Cities& cities_, PickGeneOption initMethod);
					// Choose a (possibly random) gene list

	/*
	 * Informational functions
	 */
	double getFitness() const;	// Return the organism's fitness
	bool approxEq(const Organism& rhs) const;
					// Compare fitnesses approximately

	/*
	 * Overloaded operators.  The comparison operators all compare
	 * fitnesses.
	 */
	Organism& operator=(const Organism& source);

	bool operator==(const Organism& rhs) const;
	bool operator!=(const Organism& rhs) const;
	bool operator<(const Organism& rhs) const;
	bool operator>(const Organism& rhs) const;
	bool operator<=(const Organism& rhs) const;
	bool operator>=(const Organism& rhs) const;

	friend std::ostream& operator<<(std::ostream& stream,
	  const Organism& source);

    private:
	/*
	 * Private functions
	 */
	void findFitness();		// Calculate fitness of an organism

	/*
	 * Private data
	 */
	IntList chromosome;		// Genes in the organism
	double fitness;			// Current fitness of the organism
	const Cities* cities;		// Cities the organism refers to
};

#endif // ORGANISM_HH
