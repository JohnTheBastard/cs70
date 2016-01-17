/*
 * File:       assign_11.cc
 * Author:     John Hearn and Russ Osborn
 * Date:       23 April, 2004
 * CS70 Spring 2004 Assignment #11
 * 
 * This program reads and processes a binary file of commands and 
 * associated data (licensed drivers and various information about them)
 * and uses the tree class to store and manipulate the data. In this
 * instance data is sorted by license number in a binary search tree.  
 * It can be inserted, deleted, printed, and searched for.  It is also
 * possible to check the age of a driver to determine eligibility to
 * purchase and drink alcohol. 
 *
 */



#include <iostream>
#include <string>
#include "tree.hh"

using namespace std;

void binaryProcessor(char command, Tree* database);

/*
 * The main function merely executes a while loop that calls the
 * binaryProcessor function as long as there is data available to be
 * read from cin.
 */


int main(int, char*)
{
  char command;                         // The command to process.
  Tree* database = new Tree;		// The tree to store sorted data.

  while (cin.read((char*) &command, sizeof command))
					// Read as many characters as are
					// necessary to store the command.
    {
      binaryProcessor(command, database);
    }
  
  return 0;

}

void binaryProcessor(char command, Tree* database)
{
  
  if (command == 'p')			// print
    {
      database->print();
      return;
    }

  
  char prefix;				// License number prefix.
  unsigned int number;			// Numeric component of license number.

  cin.read((char*) &prefix, sizeof prefix);
  if(!cin)				// Read prefix.
    {
      cerr << "Bad prefix input.  Aborting." << endl;
      abort();
    }
  
  cin.read((char*) &number, sizeof number);
  if(!cin)				// Read number.
    {
      cerr << "Bad number input.  Aborting." << endl;
      abort();
    }

  unsigned int nameLength;		// Length in characters of 
					// ...driver's name.
  string name;				// Driver's name.

  if (command == 'i')			// Command insert.
    {
      
      cin.read((char*) &nameLength, sizeof nameLength);
      if(!cin)				// Read nameLength.
	{
	  cerr << "'i' bad nameLength input.  Aborting." << endl;
	  abort();
	}
      
      char* buffer = new char[nameLength];
      
      cin.read(buffer, nameLength);
      if(!cin)				// Read nameLength characters
					// from the input file/
	{
	  cerr << "'i' bad name input.  Aborting." << endl;
	  abort();
	}
      
      name = buffer;			// Store characters in name.
      delete[] buffer;
    }

  unsigned char month;			// (i) birth month or (a) query month
  unsigned char day;			// birth or query day
  unsigned short year;			// birth or query year

  if (command == 'i' || command == 'a')	// insert or ageCheck
    {

      cin.read((char*) &month, sizeof month);
      if(!cin)				// Read month.
	{
	  cerr << "'i' or 'a' bad month input.  Aborting." << endl;
	  abort();			
	}
      
      cin.read((char*) &day, sizeof day);
      if(!cin)				// Read day.
	{
	  cerr << "'i' or 'a' bad day input.  Aborting." << endl;
	  abort();
	}
      
      cin.read((char*) &year, sizeof year);
      if(!cin)				// Read year.
	{
	  cerr << "'i' or 'a' bad prefix input.  Aborting." << endl;
	  abort();
	}
    }

  switch (command)
	{
	case 'i':			// insert
	  {
	    database->insert(prefix, number, name, month, day, year);
	    return;
	  }
	case 'r':			// remove
	  {
	    database->remove(prefix, number);
	    return;
	  }
	case 'f':			// find
	  {
	    database->find(prefix, number);
	    return;
	  }
	case 'a':			// age check
	  {
	    database->ageCheck(prefix, number, month, day, year);
	    return;
	  }
	}
  cerr << "Switch failed. Aborting." << endl;
  abort();
}
