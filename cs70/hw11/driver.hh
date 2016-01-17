/*
 * File:       driver.hh
 * Author:     John Hearn and Russ Osborn
 * Date:       23 April, 2004
 * CS70 Spring 2004 Assignment #11
 *
 * Header file for the driver class. This class stores information for a
 * driver in the DMV database. The data stored includes license number and 
 * prefix, the name of the driver, and the date of birth. Included in the class
 * is an output operator which prints the driver's information.
 */

#ifndef DRIVER_HH
#define DRIVER_HH

#include <iostream>
#include <string>

class Driver
{

  friend class Tree;
  friend class Node;

public:
  
  Driver(char prefix,			// Build driver based on info
	 unsigned int number, 
	 std::string name, 
	 unsigned char month,
	 unsigned char day,
	 unsigned short year);
  ~Driver();
  
  friend std::ostream& operator<< (std::ostream& stream, 
				   Driver& licensedDriver);
  
private:
  char prefix;
  unsigned int number; 
  std::string  name; 
  unsigned char month;
  unsigned char day;
  unsigned short year;
};

#endif
