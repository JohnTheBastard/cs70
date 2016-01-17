/*
 * File:       tree.hh
 * Author:     John Hearn and Russ Osborn
 * Date:       23 April, 2004
 * CS70 Spring 2004 Assignment #11
 *
 * Implementation of the tree class. The tree class consists of a single 
 * pointer to the root of a binary tree of Nodes. The Tree class acts as an
 * interface to the underlying nodes by allowing operations (insert, delete, 
 * find) to be performed on the tree.
 *
 */


#ifndef TREE_HH
#define TREE_HH

#include <iostream>
#include <string>
#include "driver.hh" 
#include "node.hh"

class Tree
{

  friend class Driver;

public:

  Tree();
  ~Tree();
  
  
  bool insert(char prefix,		// Insert based on data
	      unsigned int number, 
	      std::string name, 
	      unsigned char month,
	      unsigned char day,
	      unsigned short year);
  
  bool remove(char prefix,		// Remove (hopefully) existing driver
	      unsigned int number);
  
  Driver* find(char prefix,		// Lookup driver
	       unsigned int number);
  
  bool ageCheck(char prefix,
		unsigned int number,
		unsigned char qMonth,	// Month of query
		unsigned char qDay,	// Day of query
		unsigned short qYear);	// Year of query
  
  void print();

  
private:

  Node* root;				// Pointer to root of binary tree


};



#endif
