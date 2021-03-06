/*
 * File:     list.hh
 * Authors:  John Hearn and Russ Osborn
 * Date:     26 March, 2004
 * Modified: 1 April, 2004
 * Course:   CS70, Spring 2004
 * Assignment #8
 *
 * This file defines the interface to the List class - a templated linked list
 * that will accept any type of object. Included are the headers for the 
 * ListIterator class,and the Node class (a subclass of the List class).
 *
 * The List class itself stores only pointers to the head and tail node of
 * the list. The Node class stores an object and a pointer to the next
 * node in the list. This implementation performs pushTail() and length() in
 * O(1) time.
 *
 */


#ifndef LIST_HH
#define LIST_HH

template <typename OBJECT>
class List;

template <typename OBJECT>
class ListIterator;

template <typename OBJECT>
class List
{
  friend class ListIterator<OBJECT>;

public:
  
    List();				  // Default Constructor
    List(const List& source);		  // Copy Constructor
    ~List();				  // Destructor

    List& operator= (const List& source); // Assignment operator
  
    OBJECT popHead();			  // remove head and return it
    OBJECT peekHead();			  // return head value 
    void pushHead(OBJECT value);
    void pushTail(OBJECT value);	  // push value onto list in O(1)
    int length() const;			  // return list length in O(1)

private:
  
    int len;

    class Node				  // Node subclass of the linked
					  // list class... stores actual
					  // data
    {
    public:
        OBJECT data;			  // Stored value
        Node* next;			  // Pointer to next element
    
        Node();				  // Default Constructor
       ~Node();				  // Destructor
    
    };
  
    Node* head;				  // Pointer to first node in list
    Node* tail;				  // Pointer to last node in list
  
};

template <typename OBJECT>
class ListIterator			  // Iterator class declarations
{
public:

    ListIterator(const List<OBJECT>& source); 
					  // Create an iterator for the
					  // give source list
    ListIterator(const ListIterator<OBJECT>& source);
					  // Copy constructor
    ~ListIterator();
  
    ListIterator<OBJECT>& operator= (const ListIterator<OBJECT>& source);
  
    operator bool() const;		  // Returns true if the iterator
					  // is still valid (more elements
					  // in the list)
  
    ListIterator<OBJECT>& operator++ ();  // Iterates to the next element
					  // in the list
  
    OBJECT& operator* ();		  // Returns the value stored
					  // at this point in the list
					  // so that it may be examined
					  // or modified
  
private:

    typename List<OBJECT>::Node* target;  // Current position in the list
  
};

#include "list.icc"			  // Template to include
#endif
