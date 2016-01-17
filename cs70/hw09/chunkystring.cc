/*
 * File:     chunkystring.cc
 * Authors:  John Hearn and Russ Osborn
 * Date:     9 April, 2004
 * Course:   CS70, Spring 2004
 * Assignment #9
 *
 * This file implements the ChunkyString class which is a linked list
 * of chunks, each storing a portion of a string. Included is the header for 
 * the ChunkyStringIterator class.
 *
 * The ChunkyString class stores a single larger string inside a linked list of
 * chunks, each of which contain an array of characters. The ChunkyString 
 * class acts as if it were a regular string, supporting a copy constructor, 
 * assignment operator, concatenation of single characters, etc.
 *
 */

#include "chunkystring.hh"
#include <iostream>
#include <stdlib.h>

using namespace std;


ChunkyString::ChunkyString()	        // Default constructor
  :len(0),
   chunkList()
{
}

ChunkyString::ChunkyString(const ChunkyString& source)
  :len(source.len),							// Copy Constructor
  chunkList(source.chunkList)
{
}

ChunkyString::~ChunkyString()
{
}

ChunkyString& ChunkyString::operator= (const ChunkyString& source)
  {												// Assignment operator
    len = source.len;						// Note that real work is done by
    chunkList = source.chunkList;		// List assignment operator.
    return *this;
  }

ChunkyString& ChunkyString::operator+= (const char addition)
													// Add the addition to the end of the
													// ChunkyString 
{
  
  if ( (len % CHUNKSIZE) != 0)			// If we don't need a new Chunk to 
													// store the additional character,
    {												// just add it to the last chunk
      Chunk& lastChunk = chunkList.peekTail();
      if (lastChunk.length < CHUNKSIZE)
	{
	  lastChunk.value[(lastChunk.length)++] = addition;
	}
    }
  else											// If we do need a new Chunk,
    {												// make one and update the List.
      Chunk newChunk;
      newChunk.value[newChunk.length] = addition;
      newChunk.length = 1;
      chunkList.pushTail(newChunk);
    }

  len++;

  return *this;
  
}

int ChunkyString::length()
{
  return(len);
}

ostream& ChunkyString::print(ostream& stream) const
															// Prints the contents of this ChunkyString to the given stream.
{
  for (ChunkyStringIterator i(*this); i; ++i)
    stream<<*i;

  return stream;
}

/* Chunk Nested Class Functions */

ChunkyString::Chunk::Chunk()
  : length(0)
{
}

ChunkyString::Chunk::~Chunk()
{
}

ChunkyString::Chunk& 
    ChunkyString::Chunk::operator= (const ChunkyString::Chunk& source)
												// Assignment for individual Chunks: Copies all of the source's data to itself
{
  for(int i=0; i<source.length; i++)
  {
    value[i] = source.value[i];
  }
  
  length = source.length;
  return *this;
}

/* ChunkyStringIterator Functions */
  
ChunkyStringIterator::ChunkyStringIterator(const ChunkyString& source)
  :chunkIterator(source.chunkList),			// Start the iterator at the beginning
   index(0)											// of the source ChunkyString.
{
}

ChunkyStringIterator::~ChunkyStringIterator()
{
}

ChunkyStringIterator::operator bool() const
{
  if (!chunkIterator)	        
    {
      return false;								// Avoids the possible incrementation 
    }													// of an expired listIterator, which
														// could be NULL if we've reached
														// the end of our ChunkyString without
														// reaching the end of a node.

  ListIterator<ChunkyString::Chunk> nextChunkItr(chunkIterator);
  ++nextChunkItr;									// Look at the chunk after the current
														// one...
  
  return (nextChunkItr || (index < (*chunkIterator).length));
														// If both the next Chunk is NULL and
														// we are at the end of the current 
														// Chunk, this iterator is expired.

}

ChunkyStringIterator& ChunkyStringIterator::operator++ ()
{
  if (!*this)										// If we can't increment, complain.
    {
      cerr << "Attempt to increment expired ChunkyStringIterator. Aborting.";
      cerr << endl;
      abort();
    }

  if (index < (*chunkIterator).length - 1 ) 
    {													// If there's still data in this 
      ++index;										// Chunk, just move forward in its 
    }													// array.
  else
    {													// Otherwise, move to the next chunk.
      ++chunkIterator;
      index = 0;
    }
 
  return *this;
}

char& ChunkyStringIterator::operator* () const
														// Return the character value that the
														// iterator is pointing at.
{
  return (*chunkIterator).value[index];
}

void ChunkyStringIterator::reinitialize(ChunkyString chunkS)
														// Reset this iterator to the beginning
														// of the source ChunkyString.
{
  ListIterator<ChunkyString::Chunk> temp(chunkS.chunkList);
  index = 0;
  chunkIterator = temp;
}
