#ifndef COLONY_HH
#define COLONY_HH

/*
 * Definition of the Colony class.  A Colony is a collection of
 * organisms related to a problem to be solved.  It provides functions
 * for simulating natural selection.  It also allows access to the
 * individual organisms in the colony.
 */

/*
 * Classes defined in this file:
 */
class Colony;

/*
 * Classes used, but not defined, in this file:
 */
class Cities;
class Organism;

/*
 * The Colony class defines a collection of organisms.  Public
 * functions allow natural selection to be performed and the best
 * organism in a colony to be located.  An operator[] allows the
 * colony to be trated like an array.
 */
class Colony {
    public:
	/*
	 * Options to the Colony constructor
	 */
	enum InitOption {
	    HEURISTIC,			// Initialize heuristically
	    RANDOM			// Initialize randomly
	};

	Colony(int size_, const Cities& cities_, InitOption howToInit);

	~Colony();

	Organism& operator[](int which) const;
					// Find organism by index

	/*
	 * Genetic functions
	 */
	void evolve(bool debug, int selectionPoolSize,
	  double mutationProbability, int maxRandomSurvivors,
	  int maxGenerations);
					// Simulate multiple generations
	const Organism& findBest() const;
					// Find the best organism in the colony

    private:
	/*
	 * Disable copy and assignment
	 */
	Colony(const Colony& source);
	Colony& operator=(const Colony& source);

	/*
	 * Internal genetic functions
	 */
	void naturalSelection(int selectionPoolSize, int maxRandomSurvivors);
					// Perform natural selection

	/*
	 * Fitness-comparison function, for use with qsort.  The
	 * function must be static, and the arguments must be given as
	 * void*.  See the qsort(3) man page for more information.
	 */
	static int compareFitness(const void* first, const void* second);
					// Compare fitness of two organisms
	/*
	 * Private data
	 */
	int size;			// Size of the colony
	Organism* organisms;		// Array of organisms in the colony
	const Cities& cities;		// Cities to solve problem on
};

#endif // COLONY_HH
