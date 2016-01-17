 /****************************************************************************\
 * File:    assign_02_verbose.cc                                              *
 * Authors: John Hearn and Russ Osborn                                        *
 * Date created: 29 January 2004                                              *
 * Date modified: 30 January 2004; 2 February 2004                            *
 *                                                                            *
 * This program is designed to automatically read through code and note some  *
 * kinds of potential style problems. The user is notified of lines longer    *
 * than MAX_LENGTH, lines with sequences of capital letters and enclosed      *
 * space longer than MAX_CAPS, instances of the keyword 'goto', and lines     *
 * with poorly formatted '//' comments (those lacking surrounding             *
 * whitespace).                                                               *
 *                                                                            *
 * Input is any text file, presumably C++ source code. The file name is       *
 * provided on the command line with usage:                                   *
 *                                                                            *
 *	bash$ ./assign_02 < "FILENAME".                                       *
 *                                                                            *
 * Output is printed to the screen. Each time a style violation is found, the *
 * line number and type of the violation are printed.                         *
 *                                                                            *
 * Additional information about the program is available in the accompanying  *
 * README file.                                                               *
 *                                                                            *
 * Note: Debugging functionality has been stripped from this file.  See       *
 * assign_02_verbose.cc for debugging functionality.                          *
 *                                                                            *
 \****************************************************************************/


#include <ctype.h>
#include <iostream.h>
#include "dumbreadline.hh"

/* Function prototypes for this file. */

void lenCheck(int lineNum, char buffer[]);
// Used to check that no line exceeds MAX_LENGTH characters
// (including tab spacing).

void capsCheck(int lineNum, char buffer[]);
// Used to check that no string of capitals and enclosed whitespace exceeds
// MAX_CAPS.

void gotoCheck(int lineNum, char buffer[]);
// Used to check for possible 'goto' statements.

void commentCheck(int lineNum, char buffer[]);
// Used to ensure that every '//' comment has whitespace around it.

const int MAX_LENGTH = 80;      // Set maximum line length to 80 characters.
const int MAX_CAPS = 10;        // Set maximum consecutive capitals allowed.
const int BUFF_SIZE = 1024;     // Set input stream length to 1024.
const int TAB_WIDTH = 8;        // Set default tab width.

int main()
{
  char buffer[BUFF_SIZE];
  int lineNum = 0;
  
  dumbReadLine(cin, buffer, BUFF_SIZE);
                                        // Read line from file to buffer.
  
  while (cin)
    {
      lineNum++;
 
      lenCheck(lineNum, buffer);        // Make sure line length < MAX_LENGTH
      
      capsCheck(lineNum, buffer);       // make sure no string of consecutive
                                        // caps and enclosed whitespace
                                        // exceeds MAX_CAPS.

      gotoCheck(lineNum, buffer);        // search for possible 
                                        // 'goto' statements.

      commentCheck(lineNum, buffer);    // Make sure all '//' comments are 
                                        // surrounded by whitespace.
       
      dumbReadLine(cin, buffer, BUFF_SIZE);
                                        // Read line from file to buffer.  
  }
  
  return(1);

}

/* 
 * Checks the length of the buffer, reporting an error message if it exceeds
 * MAX_LENGTH characters.
 */

void lenCheck(int lineNum, char buffer[])
{
  
  int currentPosition = 0;                     // Keeps track of how
                                               // much of the buffer
                                               // has been counted.

  int charCount = 0;                           // Counts the width of 
                                               // the current buffer.

  while (buffer[currentPosition] != '\n')      // While not at the end of
                                               // the buffer, keep counting
                                               // the buffer length.
    {
      if (buffer[currentPosition] != '\t')     // If the current char not tab.
	{
	  charCount++;                         // Increment the known length.
	}
      else                                     // Current character is a tab.
	{
	  if (currentPosition == 0)            // If in the first space, 
             	                               // tab will insert 
                                   	       // TAB_WIDTH spaces.
	    {
	      charCount += TAB_WIDTH;
	    }
	  else
	    {
	      charCount += TAB_WIDTH - (charCount % TAB_WIDTH);
       	                                       // Amount of space inserted
	                                       // by a tab will be the width
   	                                       // of a tab minus the number of
	                                       // spaces ahead of the last 
	                                       // tab stop.
	    }
	}

      currentPosition++;

    }
  if (charCount > MAX_LENGTH)                 // If the maximum allowed length
                                              // is exceeded, print warning.
    {
      cout << "Line " << lineNum << " is too long: ";  
      cout << charCount << " characters." << endl;
    }

  return;

}

/*
 * Checks strings of capital letters (and whitespace enclosed by them) to 
 * ensure their length doesn't exceed MAX_CAPS.
 */

void capsCheck(int lineNum, char buffer[])
  /*
   * This function finds the largest string of consecutive capitals and
   * whitespace and then prints a warning message if that length is larger
   * than MAX_CAPS.
   *
   * The function first finds the first capital in a candidate string.
   * The next character is examined. If it is a capital, the capital string
   * length counter is incremented. If it is whitespace (but not the newline
   * character), the amount of whitespace is counted. If the first character
   * after the whitespace is a capital, then the amount of whitespace is added
   * to the capital string length. Otherwise, the string length counter is
   * finalized and compared to the maximum capital length seen in the buffer.
   *
   * When the end of the buffer is reached, the maximum capital length value 
   * is compared to MAX_CAPS and a warning message is printed when appropriate.
   */
{
  
  int currentPosition = 0;                    // stores the index of the 
                                              // character currently being
                                              // tested from the buffer
                                              // array.
  
  int maxCapCount = 0;                        // stores the length of the
                                              // longest string of consecutive
                                              // capitols and enclosed white
                                              // space in the current buffer.
  
  int currentCapCount = 0;                    // counts the number of
                                              // characters in the immediate
                                              // string of consecutive 
                                              // capitol letters and enclosed
                                              // white space.
  
  int spaceCount = 0;                         // counts the number of
                                              // consecutive spaces, to be
                                              // added to capsCount if they
                                              // are enclosed.
  

  while (buffer[currentPosition] != '\n')    
    /*
     * Loop over all characters in the buffer, searching for a capital
     * which starts a string of capitals and enclosed whitespace.
     */
    {

      if (isupper(buffer[currentPosition]))
	/* Check each character to see if it is upper case. */
	{
	  currentCapCount = 1;                  // Found the first capital in
	                                        // a series of unknown length
	  
	  while (isupper(buffer[currentPosition + currentCapCount]) ||
		 isspace(buffer[currentPosition + currentCapCount]) &&
		 buffer[currentPosition + currentCapCount] != '\n') 
	    /* 
	     * As long as the next characters are either capitals or whitespace
	     * (but not the end-of-line character), keep incrementing the
	     * known length of the capital string and checking the next
	     * character.
	     */
	    {
	      if (isupper(buffer[currentPosition + currentCapCount]))
		/* Current character is a capital letter. */
		{
		  currentCapCount++;
		}
	      else
		/* Current character is a space. */
		{
		  while ( (isspace(buffer[currentPosition + 
				         currentCapCount + 
				         spaceCount])) &&
			  (buffer[currentPosition +
				 currentCapCount +
			         spaceCount] != '\n') )
		    /*
		     * As long as whitespace characters are together
		     * keep incrementing spaceCount.
		     */
		    {
		      spaceCount++;
		    }
		  
		  /* Reached the end of the consecutive whitespace. */
		  
		  if (isupper(buffer[currentPosition +
				    currentCapCount +
				    spaceCount]))
		    /* 
		     * If character enclosing space is a capital letter,
		     * add spaceCount to currentCapCount.
		     */
		    {
		      currentCapCount += spaceCount;
		    }    
		  else
		    /*
		     * If the character after the whitespace is not a capital
		     * exit the larger while loop because this whitespace
		     * should not be included in the length of the string of 
		     * capitals.
		     */
		    {
		      break;
		    }
		}
	    }
	}
      if (currentCapCount > maxCapCount)
	/*
	 * If the just recorded length of capital letters string is larger than
	 * the largest such string found so far, make the maximum length
	 * found equal to the length just found.
	 */
	{
	  maxCapCount = currentCapCount;
	}
 
      currentPosition += currentCapCount;       // Increase the position of 
                                                // the current position by
                                                // the length of the capital
                                                // string just discovered.
      /* Reset counter variables to zero */

      currentCapCount = 0;
      spaceCount = 0;
      currentPosition++;
    }
  if (maxCapCount > MAX_CAPS)
    /*
     * If the longest string of capitals found is greater than the allowed
     * amount, print the warning statement.
     */
    {
      cout << "Line " << lineNum << " has a string of " << maxCapCount;
      cout << " capital letters and spaces." << endl;
    }

  return;
}

/*
 * Checks for the string 'goto' surrounded by non alpha-numerics.
 */

void gotoCheck(int lineNum, char buffer[])
  /*
   * This function searches the buffer for a lower-case 'g'.  If found,
   * the function checks to see if the g is followed by 'oto'. It also
   * ensures that the 'goto' is not preceded or followed by alpha-
   * numeric characters.
   */
{
  int currentPosition = 0;                     // Keeps track of how
                                               // much of the buffer
                                               // has been counted.

  while (buffer[currentPosition] != '\n')      // While not at the end of
                                               // the buffer, keep counting
                                               // the buffer length.
    {
      if (buffer[currentPosition] == 'g')
	/* Occurrence of 'g' begins goto test structure. */
	{
	  if (currentPosition != 0)
	    /* 
	     * Avoid out of array bounds testing for alpha-numeric. 
	     * The if structure below references buffer[currentPosition - 1]).
	     * If currentPosition = 0, this will result in an error.  
	     */
	    {
	      if ( (!isalnum(buffer[currentPosition - 1])) && 
		                               // Previous char is not
		                               // alpha-numeric.
		  (buffer[currentPosition + 1] == 'o') &&
		  (buffer[currentPosition + 2] == 't') &&
		  (buffer[currentPosition + 3] == 'o') &&
		                               // Appearance of 'goto'.
		  (!isalnum(buffer[currentPosition + 4])) )
	 	                               // Trailing char is not
		                               // alpha-numeric.
	
		/* Test for occurrence of possible 'goto' command. */
		{
		  cout << "Line " << lineNum;
		  cout << " seems to contain a goto statement." << endl;
		                               // Print warning message.
		  return;

		}
	    }
          else
	    /* 'g' is first character on the line. */
	    {
	      if ( (buffer[currentPosition + 1] == 'o') &&
		   (buffer[currentPosition + 2] == 't') &&
		   (buffer[currentPosition + 3] == 'o') &&
		                               // Appearance of 'goto'.
		   (!isalnum(buffer[currentPosition + 4])) )
	 	                               // Trailing char is not
		                               // alpha-numeric.
		
		/* Test for occurrence of possible 'goto' command. */
		{
		  cout << "Line " << lineNum;
		  cout << " seems to contain a goto statement." << endl;
		                               // Print warning message.
		  return;

		}
	    }
	}

      currentPosition++;

    }  
  
  return;

}

/*
 * Checks for all '//' comments, reporting an error when they are not
 * surrounded by whitespace.
 */

void commentCheck(int lineNum, char buffer[])
{
  int currentPosition = 0;                     // Keeps track of how
                                               // much of the buffer
                                               // has been counted.

  while (buffer[currentPosition] != '\n')      // While not at the end of
                                               // the buffer, keep counting
                                               // the buffer length.
    {
      if (buffer[currentPosition] == '/')
	/* Occurrence of '/' begins comment test structure. */
	{
	  if (currentPosition != 0)
	    /* 
	     * Avoid out of array bounds testing for whitespace. 
	     * The if structure below references buffer[currentPosition - 1]).
	     * If currentPosition = 0, this will result in an error.  
	     */
	    {
	      if ( ((!isspace(buffer[currentPosition - 1])) ||
		    (!isspace(buffer[currentPosition + 2]))) &&
		                           // Appearance of '//' not 
		                           // surrounded by whitespace.
		  (buffer[currentPosition + 1] == '/')   )
		/*
		 * Test for possible occurrences of '//' not 
		 * surrounded by whitespace.
		 */
		{ 
		  cout << "Line " << lineNum;
		  cout << " contains a // comment without";
		  cout << " surrounding whitespace." << endl;

		  return;                 // Found an unspaced comment, done.

		}
	    }
	  else
	    /* '/' found at the beginning of the buffer. */
	    {
	      if ( (buffer[currentPosition + 1] == '/')   &&
		   (!isspace(buffer[currentPosition + 2]))  )
		                           // Appearance of '//' not 
		                           // surrounded by whitespace.
		/* 
		 * Test for possible occurrences of '//' not 
		 * surrounded by whitespace.
		 */
		{ 
		  cout << "Line " << lineNum;
		  cout << " contains a // comment without";
		  cout << " surrounding whitespace." << endl;

		  return;                 // Found an unspaced comment, done.

		}
	    }
	}

      currentPosition++;

    }

  return;

}
