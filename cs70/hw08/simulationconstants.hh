#ifndef SIMULATIONCONSTANTS_HH
#define SIMULATIONCONSTANTS_HH

/*
 * CS 70, Spring 2004, Assignment 8
 *
 * This file contains a few constants that are useful in the bar simulation.
 */

#include "simulationtime.hh"

/*
 * General things related to time: the number of seconds in an hour
 * and a minute.
 */
const int		SECS_IN_HOUR = 60 * 60;
const int		SECS_IN_MINUTE = 60;

/*
 * Defaults for simulation parameters.  All of these parameters can be
 * overridden using command-line switches.
 */
const double		DRINKER_ARRIVAL_TIME = 1200.0;
					// Interarrival time for solo drinkers
const double		SERVER_ARRIVAL_TIME = 300.0;
					// Interarrival time for table servers
const int		SERVER_MAX_DRINKS = 10;
					// Maximum drinks a server can carry
const double		SERVICE_TIME = 40.0;
					// Time needed to mix one drink
const SimulationTime	SIMULATION_RUN_TIME = 5 * SECS_IN_HOUR;
					// Total time simulation should run
const SimulationTime	SUSPENSION_PENALTY = 10;
					// Time penalty for suspending the
					// ..mixing of a drink

#endif // SIMULATIONCONSTANTS_HH
