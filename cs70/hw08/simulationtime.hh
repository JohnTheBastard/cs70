#ifndef SIMULATIONTIME_HH
#define SIMULATIONTIME_HH

/*
 * CS 70, Spring 2004, Assignment 8
 *
 * Class for managing simulated time
 *
 * This file defines a class used to manage simulation time.
 * Simulation times are represented as integers, and in fact they
 * operate like integers in every respect except one: when printed,
 * any time greater than 59 seconds is shown in mm:ss or hh:mm:ss
 * form, where hh, mm, and ss are hours, minutes, and seconds,
 * respectively.
 */

#include <iostream>

/*
 * Table of contents: the following classes are defined in this file:
 */
class SimulationTime;

/*
 * SimulationTime class.  This class behaves like a (restricted)
 * integer, except printing is in ss, mm:ss, or hh:mm:ss format
 * depending on the value.  Not all integer operations are supported;
 * the functionality is limited to that which is actually needed by
 * the simulation.
 *
 * Although this class is named SimulationTime, it is also used to
 * store differences in simulation times.  There are two reasons for
 * this design decision.  First, it is sometimes somewhat difficult in
 * a simulation context to distinguish absolute times from time
 * differences.  For example, is the total run time of the simulation
 * a time difference (number of seconds from the start time, which
 * happens to be zero) or an absolute time (the time to terminate)?
 *
 * Second, as it turns out, it would be difficult to have a separate
 * SimulationTimeDiff class, because doing so would introduce a
 * chicken-and-egg problem with declaring the two classes.  Consider
 * the following operators:
 *
 *	SimulationTime - SimulationTime		(returns SimulationTimeDiff)
 *	SimulationTime + SimulationTimeDiff	(returns SimulationTime)
 *	SimulationTimeDiff + SimulationTIme	(returns SimulationTime)
 *
 * The first operator would require SimulationTimeDiff to be declared
 * first, while the third would require SimulationTime to be declared
 * first.  It would be possible to derive both from a common class,
 * but it seems excessive to go that far when one class will do, and
 * that "solution" is fraught with its own problems.  Fundamentally,
 * we're restricted by limitations of the C++ language.
 *
 * Rather than fight with the above issues, we use the SimulationTime
 * class to store both absolute and relative times.
 */
class SimulationTime
    {
    public:
	/*
	 * Constructors and destructors
	 */
			SimulationTime(int value_ = 0);
			SimulationTime(const SimulationTime& source);
			~SimulationTime();

	/*
	 * Overloaded operators.  In all cases the meaning of an
	 * operator is the same as its meaning on itegers.
	 */
	SimulationTime&	operator=(const SimulationTime& source);

	SimulationTime	operator+(SimulationTime rhs) const;
	SimulationTime	operator-(SimulationTime rhs) const;

	SimulationTime&	operator+=(SimulationTime rhs);
	SimulationTime&	operator-=(SimulationTime rhs);

	SimulationTime&	operator++();
	SimulationTime	operator++(int);

	bool		operator<(SimulationTime rhs) const;
	bool		operator<=(SimulationTime rhs) const;
	bool		operator==(SimulationTime rhs) const;
	bool		operator!=(SimulationTime rhs) const;
	bool		operator>=(SimulationTime rhs) const;
	bool		operator>(SimulationTime rhs) const;

	/*
	 * Pseudo-typecast operator, to make it easy to intermix
	 * SimulationTimes with integers.  Note that we could have
	 * named this function "operator int" and used C++
	 * typecasting.  However, it turns out that having both a
	 * from-integer constructor (above) and a to-integer typecast
	 * will often cause ambiguities.  Since we rarely need
	 * automatic typecasting *to* integers, we have chosen to make
	 * that operation slightly more difficult to avoid having to
	 * work around the ambiguities with explicit typecasts in many
	 * other placeds.
	 */
	int		toInt() const;	// Convert to integer

	/*
	 * Print function.  This function exists primarily to make it
	 * easier to write the overloaded output operator.  It prints
	 * the time in hh:mm:ss format, suppressing leading zero
	 * fields (and their associated colons).
	 */
	void		print(std::ostream& stream) const;
					// Print a SimulationTime
    private:
	int		value;		// Current value of the time
    };

/*
 * Output operator for SimulationTime.  See the print function for a
 * description of its behavior.
 */
std::ostream& operator<<(std::ostream& stream, SimulationTime time);

#endif // SIMULATIONTIME_HH
