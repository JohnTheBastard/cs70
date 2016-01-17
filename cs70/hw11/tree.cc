/*
 * File:       tree.cc
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


#include <iostream>
#include <string>
#include <stdlib.h>
#include "tree.hh"
#include "node.hh"
#include "driver.hh"


using namespace std;

Tree::Tree()				    // Default constructor starts 
  :root(NULL)				    // w/empty tree.
{

}




Tree::~Tree()				    // Free memory associated with tree
{
  while (root != NULL)
    {
      root->removeHelper(root);
    }
}




bool Tree::insert(char prefix,		    // Inserts a driver w/given info.
		  unsigned int number, 
		  string name, 
		  unsigned char month,
		  unsigned char day,
		  unsigned short year)
{

  if (root == NULL)			    // If the tree is empty, make
    {					    // this driver the root node.
      Driver* licensee = new Driver(prefix, number, name, month, day, year);
      root = new Node(*licensee);
      return true;
    }

  Driver* temp = root->find(prefix,number);
  
  if (!temp)
					    // If the driver isn't in the 
    					    // tree yet, create a data entry
    {
      Driver* licensee = new Driver(prefix, number, name, month, day, year);
      return root->insert(*licensee);	    // Put the new driver in the tree.
    }
  else					    // Driver already in tree
    {
      cout << "Attempt to re-insert existing driver: " << *temp << endl;
      return false;
    }
  
  
  cerr << "Insertion failed.  Aborting." << endl;
  abort();
  return false;
  
}





bool Tree::remove(char prefix,		    // Remove a driver based on their
		  unsigned int number)	    // license info.
{
  if (root == NULL)                         // If nobody in the tree, 
    {					    // we can't remove the driver.
      cout << "Attempt to remove nonexistant driver " 
	   << prefix << number << endl;
      return false;
    }
  
  if ((prefix == root->licensedDriver.prefix) && 
      (number == root->licensedDriver.number))
					    // Once we find the node to remove,
    {					    // call the helper to get rid
      return root->removeHelper(root);	    // of that node and maintain the 
    }                                       // tree structure.

  return root->remove(prefix, number, root);
}





Driver* Tree::find(char prefix,		    // Find a driver based on their
		   unsigned int number)	    // license info, returning by
{					    // reference.
  if (root == NULL)
    {
      cout << "Not found: " << prefix << number << endl;
      return NULL;
    }

  if (Driver* temp = root->find(prefix, number)) 
    {					    // If we can find, the driver
      cout << "Found: " << *temp << endl;   // in the tree, report that fact.
      return temp;
    }
  else					    // We didn't find the driver,
    {					    // report failure.
      cout << "Not found: " << prefix << number << endl;
      return NULL;
    }
}






bool Tree::ageCheck(char prefix,	    // Return true if the driver with
		    unsigned int number,    // given info was 21 on the 
		    unsigned char qMonth,   // date provided.
		    unsigned char qDay,
		    unsigned short qYear)
{
  if (root == NULL)
    {
      cout << "Not found: " << prefix << number << endl;
      return false;
    }


  if (Driver* temp = root->find(prefix, number))
    {					    // If we find our driver, compare
					    // birthday and query date.
      if (qYear - temp->year < 21)
	{				    // Compare based on year alone
	  cout << "Legal on " << (unsigned int) qMonth 
	       << "/" << (unsigned int) qDay 
	       << "/" << qYear 
	       << ": " << *temp << endl;
	  return true;
	}
      else if (qYear - temp->year > 21)	    // If definitely less than 21, 
	{				    // report this.
	  cout << "Not legal on " << (unsigned int) qMonth 
	       << "/" << (unsigned int) qDay 
	       << "/" << qYear 
	       << ": " << *temp << endl;
	  return false;
	}
      else				    // Year doesn't determine the 
	{				    // status, compare month.
	  if (temp->month < qMonth)
	    {
	      cout << "Legal on " << (unsigned int) qMonth 
		   << "/" << (unsigned int) qDay 
		   << "/" << qYear 
		   << ": " << *temp << endl;
	      return true;
	    }
	  else if (temp->month > qMonth)
	    {
	      cout << "Not legal on " << (unsigned int) qMonth 
		   << "/" << (unsigned int) qDay 
		   << "/" << qYear 
		   << ": " << *temp << endl;
	      return false;
	    }
	  else				    // Month doesn't determine age
	    {				    // status, check actual day.
	      if (temp->day <= qDay)
		{
		  cout << "Legal on " << (unsigned int) qMonth 
		       << "/" << (unsigned int) qDay 
		       << "/" << qYear 
		       << ": " << *temp << endl;
		  return true;
		}
	      else
		{
		  cout << "Not legal on " << (unsigned int) qMonth 
		       << "/" << (unsigned int) qDay 
		       << "/" << qYear 
		       << ": " << *temp << endl;
		  return false;
		}
	    }
	}
    }
  
  cout << "Not found: " << prefix << number << endl;
  return false;

}





void Tree::print()			    // Print tree in order
{
  cout << "Current database:" << endl;
  root->print();
  cout << "-----" << endl;
}


