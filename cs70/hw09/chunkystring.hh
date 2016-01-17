/*
 * File:     chunkystring.hh
 * Authors:  John Hearn and Russ Osborn
 * Date:     9 April, 2004
 * Course:   CS70, Spring 2004
 * Assignment #9
 *
 * This file defines the interface to the ChunkyString class which is a linked
 * list of chunks, each storing a portion of a string. Included is the header
 * for the ChunkyStringIterator class.
 *
 * The ChunkyString class stores a single larger string inside a linked list of
 * chunks, each of which contain an array of characters. The ChunkyString 
 * class acts as if it were a regular string, supporting a copy constructor,
 * assignment operator, concatenation of single characters, etc.
 *
 */

#ifndef CHUNKYSTRING_HH
#define CHUNKYSTRING_HH

#include <iostream>
#include "list.hh"

const int CHUNKSIZE = 4;	        // Default number of characters to be
											  // stored in a single chunk.

class ChunkyString
{
  friend class ChunkyStringIterator;

public:
 
  ChunkyString();
  ChunkyString(const ChunkyString& source);
												// Copy Constructor takes another 
												// ChunkyString
  ~ChunkyString();
  
  ChunkyString& operator= (const ChunkyString& source);
  ChunkyString& operator+= (const char addition);
											// Add a single character to the end
											// of the chunky string.
  
  int length();

  ostream& print(ostream& stream) const;

private:

  int len;

  class Chunk							// Stores individual portions of the
  {										// ChunkyString (the actual data).
  public: 

    Chunk();
    ~Chunk();
    Chunk& operator= (const Chunk& source);
    int length;
    char value[CHUNKSIZE];
  };

  List<Chunk> chunkList;

};

class ChunkyStringIterator
{

public:
  
  ChunkyStringIterator(const ChunkyString& source);

  ~ChunkyStringIterator();

  operator bool() const;					// True if the iterator can be 
													// incremented still
  ChunkyStringIterator& operator++ ();

  char& operator* () const;				// Return value of the iterator by
													// reference so that it can be modified

  void reinitialize(ChunkyString chunkS);
													// Allows an iterator to be set to the
													// beginning of the given ChunkyString
  
private:

  ListIterator<ChunkyString::Chunk> chunkIterator;
													// Tracks which Chunk we're on
  int index;									// Tracks location in Chunk's data.

};

#endif
