/*
 * File:       driver.cc
 * Author:     John Hearn and Russ Osborn
 * Date:       23 April, 2004
 * CS70 Spring 2004 Assignment #11
 *
 * Implementation for the driver class. This class stores information for a
 * driver in the DMV database. The data stored includes license number and 
 * prefix, the name of the driver, and the date of birth. Included in the class
 * is an output operator which prints the driver's information.
 *
 */
#include <iostream>
#include <string>
#include "driver.hh" 

Driver::Driver(char prefix_,		    // Make a new driver based on 
	       unsigned int number_,	    // info
	       std::string name_, 
	       unsigned char month_,
	       unsigned char day_,
	       unsigned short year_)
  :prefix(prefix_),
   number(number_),
   name(name_),
   month(month_),
   day(day_),
   year(year_)
{

}

Driver::~Driver()
{
  
}

std::ostream& operator<< (std::ostream& stream, 
			  Driver& licensedDriver)
{
  return stream << licensedDriver.prefix 
		<< licensedDriver.number << ' ' 
		<< licensedDriver.name << ' ' 
		<< (unsigned int) licensedDriver.month
		<< '/' << (unsigned int) licensedDriver.day 
		<< '/' << (unsigned int) licensedDriver.year;
}
