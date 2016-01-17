/*
 * File:       hashtable.hh
 * Author:     John Hearn and Russ Osborn
 * Date:       17 April, 2004
 * CS70 Spring 2004 Assignment #10
 *
 * This file defines the interface for the HashTable class. It uses separate
 * chaining to handle collisions and employs the FNV hash function.
 * The class supports insertion, lookups, assignment, copy construction, 
 * and output of statistics. The underlying data structure used is a pointer
 * to an array of pointers to Lists of strings.
 *
 */

#ifndef HASHTABLE_HH
#define HASHTABLE_HH

#include <string>
#include <iostream>
#include "list.hh"
#include "hashfuncs.hh"

const int INIT_MAX_SIZE = 53;		  // Set initial table size to 53
const double MAX_LOAD_FACTOR = 0.5;	  // Ratio of empty spaces to items
					  // in table is at least 2:1.
class HashTable
{

public:
  
  HashTable();
  HashTable(const HashTable& source);
  ~HashTable();
  
  HashTable& operator= (const HashTable& source); 
				          // Hash table assignment,
					  // empties current table and then
					  // copies source table.

  bool insert(string key);		  // Puts the key string into the 
					  // hash table, returning true upon
					  // successful insertion.

  string* lookUp(string key);		  // Check to see if the key string is
					  // in the table. If it is not, a 
					  // NULL pointer is returned.

  void printStats();			  // Print hash table statistics.
  
private:
  
  long tableSize;
  long maxSize;
  int collisions;
  int maxChain;				  // Either 0 if table empty, 
					  // 1 if table has no collisions, or
					  // 2 if there is at least one 
					  // collision.
  int numExpansions;

  double loadFactor;
  
  List<string>** table;			  // Pointer to an array of pointers
					  // to lists. Dynamically sized.
  void doubleSize();
  
  long getPrime();

};

#endif
