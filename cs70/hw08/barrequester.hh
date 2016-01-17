#ifndef BARREQUESTER_HH
#define BARREQUESTER_HH

/*
 * CS 70, Spring 2004, Assignment 8
 *
 * Classes for simulating bar customers.
 *
 * This file defines four classes (not in the order listed here):
 *
 *	BarRequester	Base class for functionality common to Drinker
 *			and TableServer.
 *	Drinker		Class used to represent individual drinkers.
 *	TableServer	Class used to represent a table server who
 *			asks for multiple drink orders to be filled at
 *			one time.
 *	TimeStatistics	Helper class used to record statistics on
 *			the service times seen by drinkers and table
 *			servers.
 */

#include "randistrs.h"
#include "simulationtime.hh"
#include <iostream>
#include <string>

/*
 * Table of contents: the following classes are defined in this file:
 */
class TimeStatistics;			// Track statistics on time parameters
class BarRequester;			// Base class for drink requesters
class Drinker;				// Class for individual drinkers
class TableServer;			// Class for table servers

/*
 * Time-statistics class.
 *
 * This class is used to accumulate statistics on anything that can be
 * represented as a SimulationTime.  The class provides a function to
 * record an observed time, and one to report statistical information
 * after all times have been recorded.  The statistics reported are
 * the mean, standard deviation, minimum, and maximum.
 */
class TimeStatistics
    {
    public:
			TimeStatistics();
			~TimeStatistics();
	void		recordTime(SimulationTime time);
					// Record a single observed time
	void		reportStatistics(std::ostream& stream,
			    const std::string& prefix)
			  const;	// Report useful recorded statisics

    private:
	/*
	 * Disable copy/assignment.
	 */
			TimeStatistics(const TimeStatistics& source);
	TimeStatistics&	operator=(const TimeStatistics& source);

	/*
	 * Private data
	 */
	SimulationTime	totalTime;	// Total of all times observed
	double		sumSquares;	// Sum of squares of observations
	SimulationTime	minTime;	// Minimum time observed
	SimulationTime	maxTime;	// Maximum time observed
	int		count;		// Count of observations gathered
    };

/*
 * Bar-requester class.
 *
 * This class is an abstract base class used to capture the
 * functionality common to people who ask the bartender to mix
 * drinks.
 *
 * A requester is represented by a scheduled start time (the time the
 * requester arrives at the bar to line up for service) and a
 * scheduled service time (the amount of time it will take to actually
 * mix the drinks).  When service is in progress, the requester also
 * records the actual start time (reflecting time spent in line) and
 * the remaining service time (reflecting any service already given).
 *
 * The class also provides a static setParameters function, so that
 * the mean service time and the suspension penalty can be set
 * externally.
 */
class BarRequester
    {
    public:
	/*
	 * Constructors and destructors.
	 */
			BarRequester();
			BarRequester(SimulationTime scheduledStartTime_,
			  SimulationTime serviceTime);
			BarRequester(const BarRequester& source);
	virtual		~BarRequester();

	/*
	 * Overloaded operators.
	 */
	BarRequester&	operator=(const BarRequester& source);

	/*
	 * Public member functions.
	 */
	virtual std::string
			type() const = 0;
					// Return requester type ("Drinker" or
					// .."Server")
	SimulationTime	dueTime() const;
					// Return the time service is supposed
					// ..to start
	virtual void	start(std::ostream& stream, SimulationTime when);
					// Record the beginning of service and
					// ..report it to a stream.  This
					// ..function can be called multiple
					// ..times if the requester has been
					// ..suspended.
	virtual void	finish(std::ostream& stream, SimulationTime when);
					// Record the end of service and report
					// ..it to a stream.
	void		giveService(SimulationTime howMuchService);
					// Provide an increment of service.
	bool		isDone() const;	// Test whether the requester is done
					// ..receiving service.
	void		suspend(std::ostream& stream, SimulationTime when);
					// Suspend service to a requester,
					// ..adjusting the remaining service
					// ..time to include the cost of
					// ..suspension.
	static void	setParameters(double serviceTime_,
			  const mt_distribution& servicePRNG,
			  SimulationTime suspensionPenalty_);
					// Record static parameters of the
					// ..simulation

    protected:
	SimulationTime	actualServiceTime(SimulationTime when) const;
					// Report the actual amount of time
					// ..that was needed to service this
					// ..request, including time spent
					// ..waiting in line.

	/*
	 * Protected data.
	 */
	static mt_distribution
			servicePRNG;	// PRNG for generating service times
	static double	serviceTime;	// Mean service time of a service
					// ..request
	static SimulationTime
			suspensionPenalty;
					// (Constant) time penalty for
					// ..suspending a request in
					// ..mid-service.

    private:
	/*
	 * Private data.
	 */
	SimulationTime	scheduledStartTime;
					// Time that service is supposed to
					// ..start; i.e., the time that this
					// ..request "arrives" at the bar.
					// ..Service may be delayed due to
					// ..queueing effects.
	SimulationTime	scheduledServiceTime;
					// Amount of time that "ought" to be
					// ..required to service this request.
					// ..This time can be increased by
					// ..queueing and by the cost of
					// ..suspension(s).
	SimulationTime	remainingServiceTime;
					// Amount of service time remaining
					// ..before this request is complete.
					// ..This value is decremented as
					// ..service is provided, and
					// ..incremented if the request is
					// ..suspended.
	SimulationTime	actualStartTime;
					// The simulation time when service was
					// ..actually begun on this request.
					// ..This may differ from the scheduled
					// ..start time due to queueing
					// ..effects.
    };

/*
 * Drinker class.
 *
 * This class is derived from the bar-requester class.  It represents
 * a request from an individual drinker (such as a customer sitting at
 * the bar).
 *
 * An individual drinker is identical to a requester.  The only
 * special data is the private static information needed to collect
 * statistics.
 */
class Drinker : public BarRequester
    {
    public:
	/*
	 * Constructors and destructors.
	 */
			Drinker();
	explicit	Drinker(SimulationTime scheduledStartTime_);
			Drinker(const Drinker& source);
	virtual		~Drinker();

	/*
	 * Overloaded operators.
	 */
	Drinker&	operator=(const Drinker& source);

	/*
	 * Virtual functions overridden from the base class.
	 */
	virtual std::string
			type() const;
					// Return requester type ("Drinker")
	virtual void	start(std::ostream& stream, SimulationTime when);
					// Record the beginning of service and
					// ..report it to a stream.  This
					// ..function can be called multiple
					// ..times if the drinker has been
					// ..suspended.
	virtual void	finish(std::ostream& stream, SimulationTime when);
					// Record the end of service and report
					// ..it to a stream.

	/*
	 * Static member functions.
	 */
	static std::ostream&
			reportStatistics(std::ostream& stream);
					// Report cumulative simulation
					// ..statistics to a stream.

    private:
	/*
	 * Static (statistical) data.
	 */
	static int	totalServices;	// Total number of drinkers served.
					// ..This is equal to the length of the
					// ..drinker queue when the barMonkey
					// ..simulation function is called.
	static TimeStatistics
			serviceStats;	// General statistics about the service
					// ..provided to drinkers.
    };

/*
 * TableServer class.
 *
 * This class is derived from the bar-requester class.  It represents
 * a request from a table server (who may be filling orders from a
 * number of customers at various tables around the bar).
 *
 * A table server is an extension of a simple requester.  In
 * particular, the server must record the number of drinks involved as
 * well as the other requester information.  The class as a whole must
 * track both the number of times service is provided, and the total
 * number of drinks mixed.
 */
class TableServer : public BarRequester
    {
    public:
	/*
	 * Constructors and destructors.
	 */
			TableServer();
			TableServer(SimulationTime scheduledStartTime_,
			  int nDrinks_);
			TableServer(const TableServer& source);
	virtual		~TableServer();

	/*
	 * Overloaded operators.
	 */
	TableServer&	operator=(const TableServer& source);

	/*
	 * Class-specific functions.
	 */
	int		drinkCount() const;
					// Return the number of drinks that the
					// ..server is ordering.

	/*
	 * Virtual functions overridden from the base class.
	 */
	virtual std::string
			type() const;
					// Return requester type ("Server")
	virtual void	start(std::ostream& stream, SimulationTime when);
					// Record the beginning of service and
					// ..report it to a stream.
	virtual void	finish(std::ostream& stream, SimulationTime when);
					// Record the end of service and report
					// ..it to a stream.

	/*
	 * Static member functions.
	 */
	static std::ostream&
			reportStatistics(std::ostream& stream);
					// Report cumulative simulation
					// ..statistics to a stream.
			
    private:
	/*
	 * Private member functions.
	 */
	SimulationTime	drinkServiceTime(int nDrinks);
					// Calculate the service time needed to
					// ..mix N drinks, found by summing the
					// ..(random) service time for each
					// ..individual  drink.

	/*
	 * Private data.
	 */
	int		nDrinks;	// Number of drink orders this server
					// ..needs to fill.

	/*
	 * Static (statistical) data.
	 */
	static int	totalServices;	// Total number of table servers
					// ..served.  This is equal to the
					// ..length of the table server queue
					// ..when the barMonkey simulation
					// ..function is called.
	static int	totalDrinksServed;
					// Total number of drinks served by all
					// ..servers.  This is equal to the sum
					// ..of the nDrinks fields of all
					// ..servers on the queue when the
					// ..barMonkey simulation function is
					// ..called.
	static TimeStatistics
			serviceStats;	// General statistics about the service
					// ..provided to drinkers.
    };

#endif // BARREQUESTER_HH
