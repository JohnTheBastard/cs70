/*
 * File:       assign_10.cc
 * Author:     John Hearn and Russ Osborn
 * Date:       17 April, 2004
 * CS70 Spring 2004 Assignment #10
 *
 * This program implements a basic spell checker based on the HashTable class.
 * The user specifies a dictionary file to be used on the command line, and 
 * then input is read from stdin. When an unrecognized word is found, it is
 * output with possible corrections. These corrections include all words in the
 * dictionary that differ by a single character from the input word.
 *
 */



#include <fstream>
#include <iostream>
#include <string>
#include "hashtable.hh"

using namespace std;

HashTable* getFile(char* fileName);
 
void corrections(HashTable& dictionary, 
		 string word, 
		 List<string*>& correctList);

int main(int, char* argv[])
{
  
  string word;
  
  HashTable* dictionary = getFile(argv[1]); // Read the argument file in as
					    // the dictionary
  List<string*> correctList;


  (*dictionary).printStats();		    // See hash table performance 
					    // after dictionary is read
  while (cin >> word)
    {					    // Read words from stdin, 
      if (isupper(word[0]))		    // capitalize, and then look them
	{				    // up in the dictionary.
	  word[0] = tolower(word[0]);
	}
      if ((*dictionary).lookUp(word) == NULL)
	{				    // If a word is not found,
					    // generate and print corrections.
	  corrections(*dictionary, word, correctList);
	  
	  cout << word << ":";
	  
	  while (correctList.length() > 0) 
	    {
	      cout << " " << *correctList.popHead();
	      
	    }
	  
	  cout << endl;
	}
    }
  
  delete dictionary;  
  
  return 0;
}


HashTable* getFile(char* filename)	    // Read the contents of the 
{					    // specified file in and return
  ifstream currentfile(filename);	    // a hash table filled with the
  string key;				    // contents.
  HashTable* dictionary = new HashTable;
  
  if (currentfile)
    {
      while (currentfile >> key)
	{
	  if (isupper(key[0]))		    // Ensure words are lower case
	    {
	      key[0] = tolower(key[0]);
	    }
	  (*dictionary).insert(key);
	}
    }
  
  return dictionary;
}

void corrections(HashTable& dictionary,	    // Based on word, a string not
		 string word,		    // found in the dictionary, 
		 List<string*>& correctList)// generate a list of corrections
{					    // and return pointers to them
  int wordLen = word.length();		    // in the form of a linked list.

  string potentialWord;

  for (int i = 0; i < wordLen; i++)	    // Assume only error is a single
    {					    // character typo.
      potentialWord = word;
      for (char j = 'a'; j <= 'z'; j++)
	{
	  potentialWord[i] = j;
	  if (dictionary.lookUp(potentialWord) != NULL)
	    {
	      correctList.pushTail(dictionary.lookUp(potentialWord));
	    }
	}
    }
}

