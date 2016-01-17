/*
 *
 * A simple program for calculating industrial grade primes.
 *
 * John Hearn
 * Mar 22, 2004
 *
 * Discrete HW#6 Problem 6
 *
 * For reasons I wont be able to ascertain before this assignment is due,
 * I cannot run the executable a.out created when compiled.  I don't
 * mean I run it and it crashes.  I mean I type ./a and get the unix 
 * "Command not found." message. So, I don't know if it works or not.
 * 
 * This would print out real primes as well as industrial primes.  My
 * next step was to integrate a seive of Eristosthenes to find all the 
 * primes below 1000, and only print p if it wasnt found in the list.  
 * Not elegant, but effective.  However, it seems useless to include
 * added functionality if I can't run the program to test it.  
 * 
 * My c++ and unix skills are mediocre at best, and I don't currently
 * have anyone at my disposal to get help, so I am just turning in 
 * the program as it stands for partial credit.
 * 
 */

#include <iostream>
#include <stdexcept>

using namespace	std;

int main()
{
  // We want to find numbers p of the form 2^p mod p = 2.
  // For our purposes, we are only looking for 561 < p < 1000.
  
  int p = 563;
  int x = 8;
  
  while (p < 1000)
    {
      p += 2;
      x = (4*x) % p;

      if (x == 2) 
	{
	  cout << p << endl;
	}
    }
  return 1;
}
