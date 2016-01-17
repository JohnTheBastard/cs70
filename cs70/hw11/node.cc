/*
 * File:       node.cc
 * Author:     John Hearn and Russ Osborn
 * Date:       23 April, 2004
 * CS70 Spring 2004 Assignment #11
 *
 * Implementation of the node class. Each node represents an element in a 
 * binary tree and is used by the Tree class. The node stores pointers to 
 * left and right children as well as a pointer to the data stored in the 
 * node. The node class supports standard operations (insert, remove, find)
 * for a binary tree so that these operations may be done recursively.
 * 
 */


#include <iostream>
#include <string>
#include <stdlib.h>
#include "tree.hh"
#include "node.hh"
#include "driver.hh"

using namespace std;


/* Node default constructor */

Node::Node(Driver licensedDriver_)      // Make a new node with 
  :licensedDriver(licensedDriver_),	// given driver as data.
   lChild(NULL),
   rChild(NULL)
{
  
}


/* Node destructor */
   
Node::~Node()				// Free memory used by this node.
{
  delete (&licensedDriver);
}


/* Node insert function */

bool Node::insert(Driver licensee)	// Put the new driver in the tree
{					// using recursion.
  if (licensee.prefix < licensedDriver.prefix)
    {					// If their prefix is less than
					// current driver's prefix

      if(lChild == NULL)		// If there is no left child, 
	{				// make left child new driver.
	  lChild = new Node(licensee);
	  return true;	  
	}
      else				// If there is a left child, insert
	{				// into the left subtree.
	  return lChild->insert(licensee);
	}
    }
  else if (licensee.prefix > licensedDriver.prefix)	
    {					// If prefix greater than current
					// driver's prefix...
      if(rChild == NULL)
	{				// If no right child, make right
	  rChild = new Node(licensee);	// child the new driver
	  return true;	  
	}
      else				// If there is a right child, 
	{				// insert into right subtree.
	  return rChild->insert(licensee);
	}
    }
  else					// If prefixes match, insert 
    {					// based on license number

      if (licensee.number < licensedDriver.number)	
	{				// If number less than current 
	  
	  if(lChild == NULL)		// If no left child, make new 
	    {				// new driver the left child
	      lChild = new Node(licensee);
	      return true;	  
	    }
	  else				// If left child exists, insert 
	    {				// in to left subtree.
	      return lChild->insert(licensee);
	    }
	}
      else				// otherwise, we know that 
	{				// license number is > current
	  if(rChild == NULL)
	    {				// If no right child, make new
					// driver the right child
	      rChild = new Node(licensee); 
	      return true;		
	    }
	  else				// otherwise, insert into right
	    {				// subtree
	      return rChild->insert(licensee);
	    }
	}
    }
  
  cerr << "Insertion failed.  Aborting." << endl; 
  abort();
  
  return false;
}



/* Node remove function */

bool Node::remove(char prefix, unsigned int number, Node* parent)
  /* 
   * This function workes essentially like the find function.
   * It search the tree for the node to delete and calls a 
   * helper function to actually perform the removal.
   */

{ 
  if (prefix < licensedDriver.prefix)	// Search first based on prefix
    {
      if(lChild == NULL)		// If we get to the bottom, we're
	{				// done.
	  cout << "Attempt to remove nonexistant driver " 
	       << prefix << number << endl;
	  return false;	  
	}
      else				// Search left subtree
	{
	  return lChild->remove(prefix, number, this);
	}
    }
  else if (prefix > licensedDriver.prefix) 
    {					// Check to the right of current
      if(rChild == NULL)		// If at the bottom, done
	{
	  cout << "Attempt to remove nonexistant driver " 
	       << prefix << number << endl;
	  return false;	  
	}
      else				// Otherwise search rt. subtree
	{
	  return rChild->remove(prefix, number, this);
	}
    }
  else					// Prefixes matched, search 
    {					// based on license number.
      if (number < licensedDriver.number)
	{				// Check left child...
	  if(lChild == NULL)
	    {				// If none, we're done
	      cout << "Attempt to remove nonexistant driver " 
		   << prefix << number << endl;
	      return false;
	    }
	  else				// Else, search left subtree.
	    {
	      return lChild->remove(prefix, number, this);
	    }
	}
      else if (number > licensedDriver.number) 
	{				// Check right child...
	  if(rChild == NULL)		// If none, we're done.
	    {
	      cout << "Attempt to remove nonexistant driver " 
		   << prefix << number << endl;
	      return false;
	    }
	  else				// Else, search rt. subtree
	    {
	      return rChild->remove(prefix, number, this);
	    }
	}
      
      if (parent->rChild == this)
	{
	  return removeHelper(parent->rChild); 
					// Both prefix and number are 
	}                               // equal... remove this node.
      else 
	{
	  return  removeHelper(parent->lChild);
	}
    }
  
  cerr << "Find failed.  Aborting." << endl;
  abort();
  return 0;
}


/* Helper function used by the remove function. */

bool Node::removeHelper(Node* &child)
  /*
   * This function is responsible for actually removing a node once 
   * it has been located. The if-else structure determines and treats 
   * the various conditions relating the node for deletion to its parent
   * and children. 
   */ 

{
  
  if (child->lChild == NULL)
    {
      if (child->rChild == NULL)	    

	/* child->lChild == NULL  &&  child->rChild == NULL */

	{
	  delete child;			// Delete the desired node
	  child = NULL;			// Move the actual pointer
					// which is the parent's 
	  return true;			// rChild or lChild pointer
	}                               // (we can do this b/c it was
				        // passed by reference)

      else
	
	/* child->lChild == NULL  &&  child->rChild != NULL */
	
	{
	  Node* temp = child;		// Delete the desired node and
	  child = child->rChild;	// move the parent's appropriate
					// child pointer by changing the 
	  delete temp;			// reference to the pointer 
	  return true;			// we are given.
	}                              
    }
  else					    
    {
      if (child->rChild == NULL)
	
	/* child->lChild != NULL  &&  child->rChild == NULL */
	
	{
	  Node* temp = child;		// Similar to above, delete 
	  child = child->lChild;	// desired node and change
					// appropriate parent's pointer
	  delete temp;			// by changing its reference. 
	  return true;
	}  
      else 

	/* child->lChild != NULL &&  child->rChild != NULL */

	{
	  Node* rMin;
	  Node* rMinParent = child->rChild->findMinParent();
	  Node* temp = child;

	  if (rMinParent == 0)
	    {
	      rMinParent = child;
	      rMin = child->rChild;
	      rMin->lChild = child->lChild;
	    }
	  else
	    {
	      rMin = rMinParent->lChild;
	      rMinParent->lChild = rMin->rChild;
	      rMin->lChild = child->lChild;
	      rMin->rChild = child->rChild;
	    }
	  
	  
	  child = rMin;			// Change appropriaate parent's
					// pointer by changing its 
	  delete temp;			// reference
	  return true;
	  
	}
    }    
  
  cerr << "RemoveHelper failed.  Aborting." << endl;
  abort();
  return false;

}


Driver* Node::find(char prefix,
			 unsigned int number)
{
  
  if (prefix < licensedDriver.prefix)	    // Search based on prefix.
    {					    // Check left subtree
      if(lChild == NULL)		    // If we're at the bottom 
	{				    // and not found, we're done.
	  return NULL;	  
	}
      else				    // Otherwise search left subtree
	{
	  return lChild->find(prefix, number);
	}
    }
  else if (prefix > licensedDriver.prefix)  // Check right subtree
    {
      if(rChild == NULL)		    // If at bottom, we're done
	{
	  return NULL;	  
	}
      else				    // Otherwise search right subtree
	{
	  return rChild->find(prefix, number);
	}
    }
  else					    // Prefixes matched, search on 
    {					    // license number
      if (number < licensedDriver.number)   // Check left subtree
	{
	  if(lChild == NULL)		    // If at bottom, done.
	    {
	      return NULL;
	    }
	  else				    // Otherwise search left subtree
	    {
	      return lChild->find(prefix, number);
	    }
	}
      else if (number > licensedDriver.number) 
	{				    // Check right subtree
	  if(rChild == NULL)
	    {				    // If at bottom, done
	      return NULL;
	    }
	  else				    // Otherwise search right subtree
	    {
	      return rChild->find(prefix, number);
	    }
	}
      return &licensedDriver;		    // Both prefix and number are 
    }                                       // equal, we've found a match
  
  cerr << "Find failed.  Aborting." << endl;
  abort();
  return NULL;
}



Node* Node::findMinParent()
{
  if (lChild == NULL)
    {
      return NULL;
    }
  else if (lChild->lChild == NULL)
    {
      return this;
    }
  else
    {
      return lChild->findMinParent();
    }
}


void Node::print()
{
  if (this != NULL)
    {
      if (lChild != NULL)
	{
	  lChild->print();
	}
      
      cout << licensedDriver << endl;
      
      if(rChild != NULL)
	{
	  rChild->print();
	}
    }
}





