/*
 * File:    intlist.cc
 * Authors: John Hearn and Russ Osborn
 * Date:    26 March, 2004
 * Course:  CS70, Spring 2004
 * Assignment #7
 *
 * This file implements the IntList class - a linked list of integers. Included
 * is the implementation of the IntListIterator and Node classes. The Node 
 * class is a subclass of the IntList class.
 *
 * The IntList class itself stores only pointers to the head and tail node of
 * the list. The Node class stores an integer value and a pointer to the next
 * node in the list. This implementation performs pushTail() and length() in
 * O(1) time.
 *
 */

#include "intlist.hh"
#include <stdlib.h>
#include <iostream>

using namespace std;

IntList::IntList()
  : len(0),
    head(NULL),
    tail(NULL)
{
}

IntList::IntList(const IntList& source)
  : len(0)
{
  for (IntListIterator i(source); i; ++i)	  // Iterate over the list
    {						  // being copied and push all
      pushTail(*i); 				  // values onto the new list.
    }
}

IntList::~IntList()
{
  while (len > 0)				  // Pop all values off the
    {						  // list, which also deletes
      popHead();				  // nodes used in the process.
    }
}

IntList& IntList::operator= (const IntList& source)
{
  while (len > 0)				  // Delete all data currently
    {						  // being stored on the list
      popHead();
    }

  for (IntListIterator i(source); i; ++i)	  // Copy new values onto list.
    {
      pushTail(*i);
    }  

  return *this;
}

int IntList::popHead()
{
  if(len > 0)					  // If there is data in the 
    {						  // list, delete the head
      int headValue = head->data;		  // node and return the value
      Node* temp = head;			  // it stored.
      head = head->next;
      delete temp;
      len--;
      return headValue;
    }
  cerr << "Attempt to pop empty IntList.  Aborting." << endl;
  abort();
}

int IntList::peekHead()
{
  if(len > 0)					  // If there is data in the 
    {						  // list, return the value
      int headValue = head->data;		  // stored in the head 
      return headValue;			          // node, but do not delete it
    }
  cerr << "Attempt to peek empty List.  Aborting." << endl;
  abort();
}

void IntList::pushHead(int value)
{
  if (len == 0)					  // If this is the first node
    {						  // in the list, create
      head = new Node;				  // the head and set pointers
      head->data = value;			  // appropriately.
      tail = head;
      len++;
      return;
    }
  Node* temp = new Node;
  temp->data = value;
  temp->next = head;
  head = temp;
  len++;
  return;
}

void IntList::pushTail(int value)
{
  if (len == 0)					  // If this is the first node
    {						  // in the list, create
      head = new Node;		                  // the head and set pointers
      head->data = value;			  // appropriately.
      tail = head;
      len++;
      return;
    }
  
  tail->next = new Node;			  // Otherwise, add a new node
  tail = tail->next;				  // to the tail of the list.
  tail->data = value;
  len++;
  return;
}

int IntList::length() const
{
  return len;
}

/* Node Nested Class Functions */

IntList::Node::Node()
  : next(NULL)
{
}

IntList::Node::~Node()
{
}

/* IntListIterator Class Functions */

IntListIterator::IntListIterator(const IntList& source)
  : target(source.head)
{
}

IntListIterator::IntListIterator(const IntListIterator& source)
  : target(source.target)
{
}

IntListIterator::~IntListIterator()
{
}

IntListIterator& IntListIterator::operator= (const IntListIterator& source)
{
  target = source.target;  
  return *this;
}

IntListIterator::operator bool() const
{
  return(target != NULL);			  // False if there are no more
						  // elements in the list.
}

IntListIterator& IntListIterator::operator++ ()
{
  if (!*this)
    {
      cerr << "Attempt to increment expired IntListIterator. Aborting.";
      cerr << endl;
      abort();
    }

  target = target->next;			  // Move to the next element
						  // in the list.
  return *this;
}

int& IntListIterator::operator* ()
{
  return target->data;				  // Give a reference to the
						  // actual data stored here.
}

