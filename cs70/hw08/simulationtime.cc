/*
 * CS 70, Spring 2004, Assignment 8
 *
 * Implementation of the SimulationTime class.
 *
 * This class is used to manage simulation time.  Simulation times are
 * represented as integers and operate like integers in every respect
 * except when printed.
 */

#include "simulationconstants.hh"
#include "simulationtime.hh"
#include <iostream>

using namespace std;

/*
 * Constructors: from-argument and copy.
 */
SimulationTime::SimulationTime(
    int			value_)		// Value to initialize from
  : value(value_)
    {
    }

SimulationTime::SimulationTime(
    const SimulationTime&
			source)		// Time to copy from
  : value(source.value)
    {
    }

/*
 * Destructor.
 */
SimulationTime::~SimulationTime()
    {
    }

/*
 * Assignment operator.
 */
SimulationTime& SimulationTime::operator=(
    const SimulationTime&
			source)		// Source to assign from
    {
    if (this != &source)
	value = source.value;
    return *this;
    }

/*
 * Addition and subtraction operators.
 */
SimulationTime SimulationTime::operator+(
    SimulationTime	rhs)		// Value to add
  const
    {
    return SimulationTime(value + rhs.value);
    }

SimulationTime SimulationTime::operator-(
    SimulationTime	rhs)		// Value to subtract
  const
    {
    return SimulationTime(value - rhs.value);
    }

/*
 * Addition and subtraction assignment operators.
 */
SimulationTime& SimulationTime::operator+=(
    SimulationTime	rhs)		// Value to add
    {
    value += rhs.value;
    return *this;
    }

SimulationTime& SimulationTime::operator-=(
    SimulationTime	rhs)		// Value to subtract
    {
    value -= rhs.value;
    return *this;
    }

/*
 * Pre- and postincrement operators.
 */
SimulationTime& SimulationTime::operator++()
    {
    value++;
    return *this;
    }

SimulationTime SimulationTime::operator++(
    int			)		// Dummy argument for C++
    {
    SimulationTime old = *this;
    value++;
    return old;
    }

/*
 * Comparison operators.  All six operators are provided, but only
 * with SimulationTime as both operands.  C++'s automatic typecasting
 * will convert right-operand integers if necessary.
 */
bool SimulationTime::operator<(
    SimulationTime	rhs)		// Value to compare
  const
    {
    return value < rhs.value;
    }

bool SimulationTime::operator<=(
    SimulationTime	rhs)		// Value to compare
  const
    {
    return value <= rhs.value;
    }

bool SimulationTime::operator==(
    SimulationTime	rhs)		// Value to compare
  const
    {
    return value == rhs.value;
    }

bool SimulationTime::operator!=(
    SimulationTime	rhs)		// Value to compare
  const
    {
    return value != rhs.value;
    }

bool SimulationTime::operator>=(
    SimulationTime	rhs)		// Value to compare
  const
    {
    return value >= rhs.value;
    }

bool SimulationTime::operator>(
    SimulationTime	rhs)		// Value to compare
  const
    {
    return value > rhs.value;
    }

/*
 * Print function.  This prints the time in hh:mm:ss format, with
 * leading zero fields (and the associated colons) suppressed.
 */
void SimulationTime::print(
    ostream&		stream)		// Stream to print to
  const
    {
    bool haveDoneSomething = false;	// True if something has already been
					// ..printed
    int valueCopy = value;		// Copy of value, so we can modify it

    /*
     * If the time is over 59:59, produce the hours field.  After
     * doing so, get rid of the hours part of the value.
     */
    if (valueCopy >= SECS_IN_HOUR)
	{
	stream << valueCopy / (SECS_IN_HOUR) << ':';
	valueCopy %= SECS_IN_HOUR;
	haveDoneSomething = true;
	}

    /*
     * Do the same thing for the minutes.  Note that if we already
     * generated an hours field, we always have to output the minutes.
     */
    if (valueCopy >= SECS_IN_MINUTE  ||  haveDoneSomething)
	{
	/*
	 * If we generated an hours part, make sure we generate a
	 * two-digit minutes part.  Incidentally, the 2-digit forcing
	 * function could be done with the C++ "setw" and "setfill"
	 * manipulators, but those functions are so clumsy to use that
	 * it's easier to do the work by hand in this case.
	 */
	if (haveDoneSomething  &&  valueCopy < SECS_IN_MINUTE * 10)
	    stream << '0';

	stream << valueCopy / SECS_IN_MINUTE << ':';

	valueCopy %= SECS_IN_MINUTE;
	haveDoneSomething = true;
	}

    /*
     * Generate the seconds field.  If we already produced a minutes
     * field, we must again insist on producing 2 digits.
     */
    if (haveDoneSomething  &&  valueCopy < 10)
	stream << '0';
    stream << valueCopy;
    }

/*
 * Convert a SimulationTime to an integer.  As discussed in the header
 * file, this is not implemented as a typecast operator because it
 * would lead to ambiguities in certain situations.
 */
int SimulationTime::toInt()
  const
    {
    return value;
    }

/*
 * Output operator.  This operator depends on the print function above
 * (q.v.) to do its work.
 */
ostream& operator<<(
    ostream&		stream,		// Stream to write to
    SimulationTime	time)		// SimulationTime to be written
    {
    time.print(stream);
    return stream;
    }
