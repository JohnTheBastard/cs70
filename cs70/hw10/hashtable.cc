/*
 * File:       hashtable.cc
 * Author:     John Hearn and Russ Osborn
 * Date:       17 April, 2004
 * CS70 Spring 2004 Assignment #10
 *
 * This file implements the HashTable class. It uses separate chaining to
 * handle collisions and employs the FNV hash function. The class supports 
 * insertion, lookups, assignment, copy construction, and output of statistics.
 * The underlying data structure used is a pointer to an array of pointers to 
 * Lists of strings.
 *
 */

#include <string>
#include <iostream>
#include "list.hh"
#include "hashfuncs.hh"
#include "hashtable.hh"

using namespace std;

HashTable::HashTable()
  :tableSize(0),
   maxSize(INIT_MAX_SIZE),
   collisions(0),
   maxChain(0),
   numExpansions(0),
   loadFactor(0),
   table(new List<string>*[INIT_MAX_SIZE])
{
  for (long i= 0; i < maxSize; i++)       // Set all buckets in the hash table
    {					  // to NULL.
      table[i] = NULL;
    }
}


HashTable::HashTable(const HashTable& source)
  :tableSize(source.tableSize),		  // Copy all the parameters from the
   maxSize(source.maxSize),		  // source hash table.
   collisions(source.collisions),
   maxChain(source.maxChain),
   numExpansions(source.numExpansions),
   loadFactor(source.loadFactor),
   table(new List<string>*[source.maxSize])
{
  for (long i = 0; i < maxSize; i++)
    {
      if (source.table[i] != NULL)	  // Use the List copy constructor to
	{				  // copy all buckets of the hash table
	  table[i] = new List<string>(*source.table[i]);
	}
      else
	{
	  table[i] = NULL;
	}
    }
}

HashTable::~HashTable()
{
  for (long i = 0; i < maxSize; i++)	  // delete all memory that the hash
    {					  // table was using
      if (table[i] != NULL)
	{
	  delete table[i];
	  table[i] = NULL;
	}
    }
}


HashTable& HashTable::operator= (const HashTable& source)
{
  tableSize = source.tableSize;		  // Assign the current hash table to
  collisions = source.collisions;	  // the source, being sure to delete
  maxChain = source.maxChain;		  // the memory allocated for the 
  loadFactor = source.loadFactor;	  // current hash table.
  numExpansions = source.numExpansions;
  
  for (long i = 0; i < maxSize; i++)
    {
      if (table[i] != NULL)
	{
	  delete table[i];
	  table[i] = NULL;
	}
    }
  delete table;

  table = new List<string>*[source.maxSize];

  for (long i = 0; i < source.maxSize; i++)
    {					  // Copy buckets of source hash table
      if (source.table[i] != NULL)
	{
	  table[i] = new List<string>(*source.table[i]);
	}
      else
	{
	  table[i] = NULL;
	}
    }

  maxSize = source.maxSize;
  return *this;
}

  
bool HashTable::insert(string key)
{
  unsigned int  hashValue = hashStringFNV(key) % maxSize;
  
  if (lookUp(key) != NULL)		  // If the key isn't already in the 
    {					  // table
      return false;
    }
  
  if (loadFactor > MAX_LOAD_FACTOR)	  // Check to see if we need to expand
    {					  // the hash table to maintain the
      ++numExpansions;			  // desired load factor
      doubleSize();
    }
  
  hashValue = hashStringFNV(key) % maxSize;
  
  if (table[hashValue] == NULL)		  // If no collision (no List in this
    {					  // hash position), make a List for
      table[hashValue] = new List<string>;// the position
					  
      if (maxChain == 0)
	{
	  maxChain = 1;
	}
    }
  else					  // If we collide with an already 
    {					  // existing list, increment 
      ++collisions;			  // the number of collisions.

      maxChain = 2;
    }
  
  (*table[hashValue]).pushTail(key);	  // Put the key string on to the list
  
  tableSize++;				  // Increment the size of the table
  
  loadFactor = (double)tableSize / maxSize;
					  // Recalculate the load factor.
  return true;
}

  
string* HashTable::lookUp(string key)
// Return true if we find the key string already in the table.
{
  unsigned int hashValue = hashStringFNV(key) % maxSize;
  
  if (table[hashValue] != NULL)		  // If there's a list in our hash 
    {					  // spot, iterate through it.
      for (ListIterator<string> i(*table[hashValue]); i; ++i)
	{
	  if (*i == key)
	    {
	      return &(*i);
	    }
	}
    }
  
  return NULL;				  // Value not in the table.
}

void HashTable::printStats()
{

  cerr << numExpansions << " expansions, "
       << "load factor " << loadFactor 
       << ", " << collisions << " collisions, "
       << "longest chain " << maxChain << endl;

}
  
void HashTable::doubleSize()
// When we exceed our specified max load limit, we need to resize the table
// to handle more strings and keep collisions down.
{
  unsigned int  hashValue;
  long newMax = getPrime();
  
  collisions = 0;
  maxChain = 0;
  
  List<string>** tempTable = new List<string>*[newMax];
					  // Make new pointer to array of 
					  // pointers to List<string*>'s

  for(long i = 0; i < newMax; i++)	  // Initialize pointers to NULL.
    {
      tempTable[i] = NULL;
    }


  for (long i = 0; i < maxSize; i++)	  // Examine the values stored in the
    {					  // table already and rehash them 
      if (table[i] != NULL)		  // in to the new, larger table.
	{
	  for (ListIterator<string> j(*table[i]); j; ++j)
	    {
	      hashValue = hashStringFNV(*j) % newMax;
	      
	      if (tempTable[hashValue] == NULL)
		{
		  tempTable[hashValue] = new List<string>;
		}
	      
	      (*tempTable[hashValue]).pushTail(*j);
	    } 
	}
    }

  for (long i = 0; i < maxSize; i++)	  // delete all memory used for old
    {					  // smaller hash table.
      if (table[i] != NULL)
	{
	  delete table[i];
	  table[i] = NULL;
	}
    }
  
  table = tempTable;			  // Set this table to be the newly
  maxSize = newMax;			  // resized table.
}
  
long HashTable::getPrime()
// Return a prime roughly twice the size of the current table size.
{
  /* Primes: 
   *   53, 107, 223, 443, 883, 1759, 3511, 7013, 14009, 28001,
   *   56003, 111997, 224011, 448003, 896003, 1889999
   */
  
  switch (maxSize)
    {
    case 53: return 107;
      
    case 107: return 223;

    case 223: return 443;

    case 443: return 883;

    case 883: return 1759;

    case 1759: return 3511;

    case 3511: return 7013;

    case 7013: return 14009;

    case 14009: return 28001;

    case 28001: return 56003;

    case 56003: return 111997;

    case 111997: return 224011;

    case 224011: return 448003;

    case 448003: return 896003;

    case 896003: return 1889999;
    }
  
  return 0;
}
