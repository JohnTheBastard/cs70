/*****************************************************************************\
 *                                                                            *
 * This sample code was provided for Assignment #1 for CS70 at Harvey Mudd    *
 * College, Spring 2004.  It was initially coded with poor style.  We have    *
 * modified the style of the code to make it easily understandable with as    *
 * few changes to the actual design as possible.                              *
 *                                                                            *
 * This file is a test program used to verify the functionality of the rat    *
 * class (files rat.hh and rat.cc).                                           *
 *                                                                            *
 * We have chosen to leave many of the original author's comments intact,     *
 * with slight changes to improve readability and consistency with changes in *
 * variable naming.  The comments are recognizable as all-cap typeface.       *
 *                                                                            *
 * File: assign_01.cc                                                         *
 * Author: Unknown                                                            *
 * Modified by: Russell Osborn, John Hearn                                    *
 * Date Modified: 25 January, 2004; 28 January, 2004                          *
 *                                                                            *
 \****************************************************************************/

#include <iostream>
#include <stdexcept>
#include "rat.hh"

using namespace	std;

int main(int, char*[])
{
  // Declare four rationals for testing
  
  rat prb1 (1, 8);
  // prb2 IS 2 IN 3
  rat prb2 (2, 3);
  // prb(BOTH TOGETHER) IS PRODUCT
  rat prb3 = prb1 * prb2;
  // prb(EITHER INDEPENDENT) IS SUM
  rat prb4 = prb1 + prb2;
  
  // WHAT IS ANSWER?
  cout << "combined prob is " << prb3 << endl;
  cout << "independent prob is " << prb4 << endl;
  
  
  /* OTHER TEST */

  // Testing comparison and stream i/o operators

  cout << rat(24, 32) << endl;
  
  cout << rat(13) << endl;
  
  cout << rat(-24, -10) << endl;
  
  cout << rat(-24, 10) << endl;
  
  rat x;
  cout << x << endl;
  
  if(prb4 == (prb1 + prb2))
    {
      cout << "yes" << endl;
    }
  
  if(prb1 < prb2)
    {
      cout << "yup" << endl;
    }
  
  cout << prb4 - prb3 << endl;
  cout << prb2 * prb1 << ' ' << prb1 / prb2 << endl;


  /* test exception handling for divide by 0 case. */

  try
    { 
      rat z(1, 0);
      cout << z << endl;
    }
  catch(std::exception &e)
    {
      cout << e.what() << endl;
    }
  
  return 0;
}






