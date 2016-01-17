/*****************************************************************************\
 *                                                                            *
 * This sample code was provided for Assignment #1 for CS70 at Harvey Mudd    *
 * College, Spring 2004.  It was initially coded with poor style.  We have    *
 * modified the style of the code to make it easily understandable with as    *
 * few changes to the actual design as possible.                              *
 *                                                                            *
 * This file includes the operator declarations for the rat class, which is a *
 * rational number data structure.  This file implements standard arithmetic  *
 * functions as well as stream i/o, a greatest common divisor function, and a *
 * normalize function, which reduces the fraction.                            *
 *                                                                            *
 * We have chosen to leave many of the original author's comments intact,     *
 * with slight changes to improve readability and consistency with changes in *
 * variable naming.  The comments are recognizable as all-cap typeface.       *
 *                                                                            *
 * File: rat.cc                                                               *
 * Author: Unknown                                                            *
 * Date Created: Unknown                                                      *
 * Modified by: Russell Osborn, John Hearn                                    *
 * Date Modified: 25 January, 2004; 28 January, 2004                          *
 *                                                                            *
 \****************************************************************************/

/* 
   IMPLEMENTATION OF THE RAT PACKAGE. THIS CODE WAS BORROWED FROM VARIOUS
   SOURCES, PARTICULARLY [the original author's] DISCRETE MATH TEXTBOOK. 
*/

#include <iostream>
#include <stdexcept>
#include "rat.hh"

using namespace	std;   //We have no idea what this does.


/* Standard Arithmetic functions */

const rat operator+ (const rat &left, const rat &right)
  // return sum of two rationals
{

  rat x ( (left.numer() * right.denom()) + (right.numer() * left.denom()),
	  (left.denom() * right.denom()) );

  return x;

}

const rat operator- (const rat &left, const rat &right)
  // return difference of two rationals
{
    rat x ( (left.numer() * right.denom()) -  (right.numer() * left.denom()),
	    (left.denom() * right.denom()) );

    return x;
}

const rat operator* (const rat &left, const rat &right)
  // product of rationals
{

  rat x ( left.numer() * right.numer(), left.denom() * right.denom() );

  return x;

}

const rat operator/ (const rat &left, const rat &right)
  // division of rationals
{

  rat x ( left.numer() * right.denom(), left.denom() * right.numer() );

  return x;
}

const rat operator- (const rat &value)
  // change sign of rational
{

    rat x ( 0 - value.numer(), value.denom() );

    return x;
}

/* Comparison operators */

bool operator<	(const rat &left, const rat &right)
  // less than 
{

  bool u;

  u = (left.numer() * right.denom()) < (right.numer() * left.denom());

  return u;

}

bool operator>	(const rat &left, const rat &right)
  // greater than
{

  bool u;

  u = (left.numer() * right.denom()) > (right.numer() * left.denom());

  return u;

}

bool operator== (const rat &left, const rat &right)
  // equal to
{

  bool u;
  
  u = (left.numer() * right.denom()) == (right.numer() * left.denom());

  return u;

}

const rat rat::operator++ (int) 
  /*
    We don't understand the syntax of this function, 
    but it implements the increment operator by adding 
    the denominator to the numerator.
  */
{

    rat c(*this);
    // MAKE CHANGE
    top += bot;

    return c;

}


const rat abs(const rat &value)
  // absolute value of rational
{
    int newTop;
    int newBot = value.denom();

    if (value.numer()* value.denom() < 0)  
                                 // if the product of the numerator
                                 // and denominator is negative, change
                                 // the sign of the numerator.
      newTop = 0 - value.numer();

    return rat(newTop, newBot);

}

void rat::operator+= (const rat &value)
  // short hand implementation of add-value operator.
{
  
  // MODIFY BY ADDING RHS (right hand side)
  top = top*value.denom() + bot*value.numer();
  bot *= value.denom();
  
  // CALL normalize
  normalize();
  
}


int rat::gcd(int x, int y)
  // Greatest common divisor algorithm.  Presumably, this is
  // among the code taken from the Discrete math text.
  // We think this algorithm was invented by Euclid. Explanation by 
  // example given below.

  /* 
    Find gcd of 206 and 40:
    206 = 40 * 5 + 6  Find remainder when 206 is divided by 40.
    40  = 6  * 5 + 4  Find remainder when 40 is divided by previous remainder.
    6   = 4  * 1 + 2  Last non-zero remainder is the GCD.
    4   = 2  * 2 + 0  In this case, 2,
  */   
{

  // IT WORKS. DON'T CHANGE IT.

  if (x < 0)
    return gcd(-x, y);	
  else if (y < 0)
    return gcd(x, -y);

  if (x < y)
    return gcd(y, x);

  if (y == 0)
    return x;

  int remainder = x % y; 

  return gcd(y,remainder);

}

void rat::normalize()
  // reduce the fractional representation of the rational by
  // dividing the numerator and denominator by their greatest
  // common divisor.

  // We deleted some extraneous code from this function.
  // Since the algorithm was ported directly from assembly, it included
  // code to keep track of the sign, which is not necessary in higher
  // level programming.

{
  // NORMALIZE
  // ADAPTED THIS GOODY FROM TODD'S ASSEMBLY ROUTINE

  if (bot == 0)
    {
      throw range_error("zero denominator");
    }
  
  // SET denom TO GCD OF top/bot
  int divisor = gcd(top, bot);
  
  // DIVIDE BY GCD
  top /= divisor;
  bot /= divisor;
  
}


////////////////////////////////////////////////////////////
// OUTPUT AND INPUT OPERATORS //////////////////////////////
////////////////////////////////////////////////////////////

// We don't really know much about i/o streams in c++, so
// our comments will be minimal.

ostream & operator<< (ostream &out, const rat &value)
  // stream output operator
{ 

  out << value.numer() << '/' <<value.denom();
  return out;

}

istream & operator>> (istream &in, rat &value)
  //stream input operator
{  

  int top;
  int bot;
  // READ top
  in >> top;
  
  if (in.good())
    {
    // IF THERE IS A SLASH, READ THE NEXT NUMBER
    char c;
    if (!(in >> c)) 
      {
	in.clear(in.rdstate()&~ios::failbit);
	bot = 1;
      }
    else if (c == '/')
      {
	in >> bot;  // READ bot PART
      }
    else
      {
	in.putback(c);
	bot = 1;
      }
  }

  // SET value TO newVal
  rat newVal(top, bot);
  value = newVal;

  // RETURN THE STREAM
  return in;
}


