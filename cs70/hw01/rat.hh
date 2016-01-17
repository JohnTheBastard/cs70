/*****************************************************************************\
 *                                                                            *
 * This sample code was provided for Assignment #1 for CS70 at Harvey Mudd    *
 * College, Spring 2004.  It was initially coded with poor style.  We have    *
 * modified the style of the code to make it easily understandable with as    *
 * few changes to the actual design as possible.                              *
 *                                                                            *
 * This is the header file for the rat class. It includes four constructors   *
 * and operator prototypes for standard arithmetic functions, stream i/o,     *
 * and gcd and normalize functions. All of these are implemented in the       *
 * rat.cc file.                                                               *
 *                                                                            *
 * We have chosen to leave many of the original author's comments intact,     *
 * with slight changes to improve readability and consistency with changes in *
 * variable naming.  The comments are recognizable as all-cap typeface.       *
 *                                                                            *
 * File: rat.hh                                                               *
 * Author: Unknown                                                            *
 * Date Created: Unknown                                                      *
 * Modified by: Russell Osborn, John Hearn                                    *
 * Date Modified: 25 January, 2004; 28 January, 2004                          *
 *                                                                            *
 \****************************************************************************/



using namespace	std;

class rat
 {
 public:

   // CONSTRUCTORS

   rat():top(0), bot(1)
     // default constructor -- given no values, set numerator to 0 
     // and denominator to 1.
   {}

   rat(int tt):top(tt), bot(1)
     // constructor for a single integer -- set the numerator to the 
     // given parameter and the denominator to 1.
   {}

   rat(int tt, int bb):top(tt), bot(bb)
     // constructor for a numerator/denominator pair -- set the 
     // numerator and denominator appropriately.
   {
     normalize();
   }

   rat(const rat &value):top(value.top), bot(value.bot)
     // constructor for another given rational -- set the numerator
     // and denominator identically.
   {}


   // ACCESSOR FUNCTIONS
   int numer() const
     // return the numerator
     // this function used when the values of the rational 
     // are not to be modified as opposed to simply using
     // 'top' and 'bot'
   {
     return top;
   }
   
   int denom() const
     // return the denominator
     // this function used when the values of the rational 
     // are not to be modified as opposed to simply using
     // 'top' and 'bot'
   {
     return bot; 
   }

   // ASSIGNMENTS
   void operator= (const rat &value)
   {
     top = value.top;
     bot = value.bot;
   }

   void operator+= (const rat &);

   // OTHER OPERATIONS
   operator double () const 
     // returns the rational casted as a double
   {
     double x;

     x =  ( top / (double) bot );

     return x;
   }

 
   const rat operator++ (int);
   // increments the rational
   
 
 private:
                      // DATA AREAS
   int top;           // numerator for the rational   
   int bot;           // denominator for the rational
   void	normalize();  // OPERATION USED INTERNALLY 
                      // used to reduce fractional representation
   static int gcd(int, int);
                      // Computes the gcd of an integer pair by Euclids Alg.
};

// Operator Prototypes (?)

const rat operator+ (const rat &, const rat &);
const rat operator- (const rat &, const rat &);
const rat operator* (const rat &, const rat &);
const rat operator/ (const rat &, const rat &);
const rat operator- (const rat &);

const rat abs(const rat&  num);

bool operator<	(const rat &, const rat &);
bool operator== (const rat &, const rat &);
bool operator>	(const rat &, const rat &);

ostream &operator<< (ostream &, const rat &);
istream &operator>> (istream &, rat &);
