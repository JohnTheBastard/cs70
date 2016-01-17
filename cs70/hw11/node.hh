/*
 * File:       node.hh
 * Author:     John Hearn and Russ Osborn
 * Date:       23 April, 2004
 * CS70 Spring 2004 Assignment #11
 *
 * Header file for the node class. Each node represents an element in a 
 * binary tree and is used by the Tree class. The node stores pointers to 
 * left and right children as well as a pointer to the data stored in the 
 * node. The node class supports standard operations (insert, remove, find)
 * for a binary tree so that these operations may be done recursively.
 *
 */


#ifndef NODE_HH
#define NODE_HH

#include "driver.hh" 

class Node
{

  friend class Tree;

public:
  
  Node(Driver licensedDriver);
  ~Node();
  
  bool insert(Driver licensee);	        // Insert into tree
					// rooted at this node.
  bool remove(char prefix, unsigned int number, Node* parent);
					// Remove from tree rooted at this node
  bool removeHelper(Node* &child);
  
  Driver* find(char prefix, unsigned int number);
					// Look up node with given license.
  Node* findMinParent();
					// Find the parent of the minium 
					// node in the tree rooted at this node

  void print();				// Print the tree (inorder traversal)

private:
  
  Driver licensedDriver;		// Actual data
  Node* lChild;				// pointers to child subtrees
  Node* rChild;
  
};

#endif
