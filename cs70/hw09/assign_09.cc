/*
 * File:       assign_09.cc
 * Authors:    John Hearn and Russ Osborn
 * Date:       14 April 2004
 * Course:     CS70, Spring 2004
 * Assignment #9
 *
 * This file contains the implementation of the encryption and decryption of
 * a Vignere cipher. It takes as input a flag specifying encryption (-e) or 
 * decryption (-d). When encrypting, a -g flag with argument n is also 
 * optional; it formats output so that n characters are printed and then a 
 * single whitespace is printed. 
 *
 * The code first processes arguments, then it opens the specified file name.
 * The user is then prompted for a password, which is stored in a ChunkyString.
 * The contents of the file are opened and are also put in a ChunkyString.
 * Then, the contents of the file are ciphered using the user-supplied
 * password. 
 *
 */

#include "chunkystring.hh"

#include <cassert>
#include <cstdlib>
#include <cctype>
#include <fstream>
#include <iostream>
#include <string>

/* Prototypes */

using namespace std;

int main(int argc, char* argv[]);

static int processOptions(int argc, char* argv[], 
	     bool& encryptFile, 
	     bool& decryptFile, 
	     int& groupSize);

static void usage(char* progname);     

ChunkyString* cipher(ChunkyString* password, 
		     ChunkyString* chunkedFile, 
		     bool encrypt);
					// Based on a user supplied password,
					// a file from input processed into an 
					// all-caps ChunkyString, and a 
					// boolean specifying encryption or
					// decryption, this function uses a 
					// Vignere cipher to encrypt the data.


ChunkyString* getPassword();		// Prompts the user for a password and
					// uppercases it, making memory for a
					// new ChunkyString and returning a 
					// pointer to it.

ChunkyString* fileReader(istream& file, bool encrypt);
					// Given an input file,and boolean 
					// specifying encryption/decryption,
					// makes memory for a new ChunkyString
					// and inserts the file contents.

static void output(ChunkyString* chunkOut, const int groupSize);
					// Outputs a given ChunkyString based
					// on the user given groupSize, 
					// ensuring groups are preserved across
					// new lines. If groupSize is 
					// unspecified, 0 is the default.


/* Functions */

int main( int argc, char* argv[])
{
  bool encryptFile;
  bool decryptFile;
  int groupSize;
  
  int argNo = processOptions(argc, argv, encryptFile, 
			     decryptFile, groupSize);
  ChunkyString* password; 
  ChunkyString* chunkedFile;
  
  ChunkyString* chunkOut;
  
  if (argNo >= argc || argNo < 2)		// Program will not run without 
    {												// at least one flag.  argNo is
      usage(argv[0]);						// only incremented when a flag is
    }												// encountered.
  
  
  assert(encryptFile != decryptFile);	// Make sure we processes arguments ok.
  
  
  
  for ( ; argNo < argc; ++argNo)			// For all files listed
    {

      ifstream currentFile(argv[argNo]);
      if (currentFile)						// If we were able to open the file,
	{												// decrypt it.
	  
	  password = getPassword();    
	  chunkedFile = fileReader(currentFile, encryptFile);
													// Read the file in to a ChunkyString

	  chunkOut = cipher(password, chunkedFile, encryptFile);
													// Encode or decode the file
	  output(chunkOut, groupSize);		// Output the ciphered text

	  currentFile.close();					// Done reading from the file.

	  delete password;						// Clean up memory allocated elsewhere
	  delete chunkedFile; 
	  
	}
      else										// If we can't open the file, complain
	{
	  cerr << "\nCouldn't open '" << argv[argNo] << "': "
	       << strerror(errno) << endl;
	}
    }
}


/*
 * The 'processOptions' and 'usage' functions that follow, as well as much 
 * of the associated commenting are modified from code written By Prof. 
 * Geoff Keunning for the file "assign_08.cc" provided for use in CS 70, 
 * Spring 2004, Assignment 8.
 *
 * -jhearn/rosborn 4/10/2004
 */

/*
 * Option processing.  Scan through the arguments, looking at
 * switches.  If we run into a non-switch argument, exit the function.
 *
 * If an illegal switch is detected, we will print a usage message to
 * cerr and terminate the program with an exit status of 2.
 *
 * The return value of this function is the number of the first
 * non-switch argument encountered.  If there are no non-switch
 * arguments, the return value equals argc.
 *
 */
static int processOptions(
	     int argc,							// Argument count passed to main
	     char* argv[],					// Argument vector passed to main
	     bool& encryptFile,				// Set true for file encryption
	     bool& decryptFile,				// Set true for file decryption
	     int& groupSize)					// Size of character groups for
												// ...file encryption
{
  encryptFile = false;
  decryptFile = false;
  groupSize = 0;
  
  /*
   * Note that the loop index is modified inside the body of
   * the loop; this is generally poor style but is common in
   * argument processing.  In this case, the modification is done
   * inside the processing of the "g" switch.
   */
  for (int argNo = 1;  argNo < argc;  argNo++)
    {
      if (argv[argNo][0] != '-')	// End of switches?  If so, exit
	return argNo;			// ..option processing
      
      /*
       * All other switch options should consist of a single hyphen,
       * an alphabetic character, and nothing else.  At this point
       * we know that the hyphen exists.  Check to make sure that
       * there is exactly one character following it.
       */
      if (argv[argNo][1] == '\0'  ||  argv[argNo][2] != '\0')
	usage(argv[0]);
      
      /*
       * Process switch options.  We do this in a "switch" statement
       * so that it is easy to add new options later.
       */
      switch (argv[argNo][1])
	{
	case '-':			// --: end of arguments
	  {
	    /*
	     * By convention, the special switch option "--" means that
	     * all following arguments are non-option arguments, even if
	     * they start with a hyphen.  If we run into that switch, we
	     * must skip over it but then exit the option-processing loop.
	     */
	    argNo++;								// Skip over the --
	    return argNo;							// Go to non-option argument processing
	  }
	  
	case 'd':									// -d: decrypt <file>
	  {
	    if (groupSize || encryptFile)
	      {
		usage(argv[0]);
	      }
	    
	    decryptFile = true;
	    
	    break;
	  }
	case 'e':									// -e: encrypt <file>
	  {
	    if (decryptFile)
	      {
		usage(argv[0]);
	      }
	    
	    encryptFile = true;
	    
	    break;
	  }
	case 'g':									// -g n: group sizing
	  {
	    if (++argNo >= argc)				// Make sure there's an argument
	      usage(argv[0]);					// ..if not, complain and exit
	    
	    /*
	     * Convert the next argument to an integer (strtol) and
	     * save it.  If *lastValid isn't '\0', the conversion failed.
	     */
	    char* lastValid;
	    groupSize = strtol(argv[argNo], &lastValid, 0);
	    if (*lastValid != '\0')
	      usage(argv[0]);
	    break;
	  }
	default:											// Default: unrecognized option
	  {
	    usage(argv[0]);
	    break;
	  }
	}
    }
  
  /*
   * We can only get here if we ran out of arguments.  Return argc
   * to the caller, indicating that there are no non-option
   * arguments.
   */
  return argc;
}


/*
 * Issue a usage message to cerr, and terminate the program with a
 * failure status.
 */
static void usage(char* progname)			// Issue a usage message with
														// filename executed on
{
    cerr << "\nUsage: " << progname
	 << " [-d] [-e] [-g n] <file>\n";
    cerr << "\nSwitches:\n";
    cerr << "\t-d\tDecrypt <file>\n" 
	 << "\t\tCannot be used in conjunction with -e or -g switch.\n";
    cerr << "\t-e\tEncrypt <file>\n"
	 << "\t\tCannot be used in conjunction with -d switch. \n";
    cerr << "\t-g n\tSpecify size of character grouping during encryption.\n"
	 << "\t\tUsed in conjunction with -e switch.\n";
    cerr << "\t<file>\tSpecify file to encrypt or decrypt. \n";
    exit(2);
}


ChunkyString* cipher(ChunkyString* password, 
		     ChunkyString* chunkedFile, 
		     bool encryptFile)
{
  char fTemp;
  char pTemp;
  
  ChunkyStringIterator iPass(*password);
  ChunkyStringIterator iChunkFile(*chunkedFile);
  
  
  while (iChunkFile)							// While more of the ChunkyString
    {												// remains to be decoded...
      
      if (!iPass)								// If we're about to run over the end
	{												// of our password, reset to the start
	  iPass.reinitialize(*password);
	} 
      
      fTemp = *iChunkFile;
      pTemp = *iPass;
      
      /* Convert characters to 0-26 range */
      if (isalpha(fTemp))					// fTemp is a capital letter
	{
	  fTemp -= 'A';
	}
      else										// fTemp is a '.'
	{
	  fTemp = 26; 
	}
      
      if (isalpha(pTemp))					// pTemp is a capital letter
	{
	  pTemp -= 'A';
	}
      else										// pTemp is a '.'
	{
	  pTemp = 26;
	}
      
      /* Perform encryption or decryption */
      if (encryptFile)
	{
	  fTemp += pTemp;
	}
      else
	{
	  fTemp -= pTemp;								// Decode this single character
	  fTemp += 27; 
	}
      
      fTemp %= 27;
      
      
      /* Convert back to capital letters */
      if (encryptFile && (fTemp == 26))	// If we're encrypting whitespace,
	{													// place a '.' in the output
	  fTemp = '.';
	}
      else if (fTemp == 26)					// If we're decrypting whitespace,
	{													// print a space.
	  fTemp = ' ';
	}
      else
	{
	  fTemp += 'A';
	}
      
      *iChunkFile = fTemp;
      
      ++iPass;										// Move to the next character.
      ++iChunkFile;
    }
  
  return chunkedFile;
  
}

ChunkyString* getPassword()					// Get the password used for 
{														// encryption or decryption
  char nextChar;
  ChunkyString* password = new ChunkyString;
  
  cerr << "Password: ";
  
  while (cin.get(nextChar) && nextChar != '\n')
    {													// Keep reading the password
      if (isalpha(nextChar))					// until a newline character.
	{
	  *password += toupper(nextChar);
	}
      else											// Convert non-alpha numerics to 
	{													// the default '.' character.
	  *password += '.';
	}
    }

  if ( (*password).length() == 0)			// If the password is empty, prompt
    {													// for a new one.
      cerr << "Invalid password: "
	   << "password must be at least one character long." << endl;
      
      return getPassword();
    }
  
  return password;
}

ChunkyString* fileReader(istream& file, bool encrypt)
														// Using the file to be ciphered,
														// creates a new ChunkyString, and
														// reads the file in to the string.
{
  char nextChar;
  ChunkyString* chunkedFile = new ChunkyString;
  
  while (file.get(nextChar))					// While there is more file to read
    {
      if (isalpha(nextChar))					// Convert to upper case if alpha num.
	{
	  *chunkedFile += toupper(nextChar);
	}
      else if (encrypt)							// If we're encrypting, put a '.' for
	{													// non alpha numerics
	  *chunkedFile += '.';
	}
      else if (nextChar == '.')				// If we're decrypting and see a '.'
	{													// which is a placeholder for ' ', 
	  *chunkedFile += '.';						// put a '.' in the string. If a blank
	}													// is read (b/c of -g flag)
    }													// ignore it.
  
  return chunkedFile;
}

static void output(ChunkyString* chunkOut, const int groupSize)
														// Outputs a ChunkyString obeying the group size constraint passed in.
														// Note that if no size is given, the default size will be 0.
{
  
  const int MAX_LINE_WIDTH = 70;
  ChunkyStringIterator chunkItr(*chunkOut);

  
  if (groupSize == 0)							// If no groups, just print one line
    {
      while (chunkItr)
	{
	  cout << *chunkItr;
	  ++chunkItr;
	}
    }
  else												// Group size is less than a line, 
    {													// put new lines so groups aren't 
														// broken up.
      
      int groupsPerLine = MAX_LINE_WIDTH / (groupSize + 1);
				        // Add 1 to group size to account
					// for spacing.

      if (groupsPerLine == 0)		// Ensure at least one group printed
	{				// per line (important if groupSize
	  ++groupsPerLine;		// exceeds MAX_LINE_WIDTH.
	}
     
      
      while (chunkItr)
	{
	  for (int i = 0; i < groupsPerLine; i++) 
	    {
	      for (int j = 0; (j < groupSize) && chunkItr; j++)
		{
		  cout << *chunkItr;
		  ++chunkItr;
		}
	      cout << (' ');
	    }
	  cout << '\n';
	}
    }
  
  cout << endl;

}
