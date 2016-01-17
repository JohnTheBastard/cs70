 /****************************************************************************\
  * File:  student.hh                                                         *
  * Modified by: Russ Osborn, John Hearn                                      *
  * Date: 11 February, 2004                                                   *
  * Original Author: Geoff Kuenning                                           *
  * Course: CS 70, Spring 2004                                                *
  * Assignment #3                                                             *
  *                                                                           *
  * This file defines the interface to the Student class, which is an         *
  * auxiliary class used by the Registrar_DB class (see registrar.hh).        *
  *                                                                           *
  * The Student class describes a single college student.  It allows          *
  * the student to be created and his/her hair color to be changed at         *
  * will.  It also supports adding courses to the student's schedule          *
  * and removing them later.                                                  *
  *                                                                           *
  \***************************************************************************/

#ifndef STUDENT_HH
#define STUDENT_HH

#include <iostream>
#include <string>

using namespace std;

/*
 * Table of contents: the following classes are defined in this file.
 */
class Student;

/*
 * The following classes are used but not defined in this file.
 */
class Course;

/*
 * Interface to the Student class.
 */
class Student
    {
    public:
	/*
	 * Constructors and destructors.  We provide a zero-argument
	 * constructor so that arrays of students can be created.  For 
	 * the current assignment, no other constructor is needed.
	 */
			Student();
			~Student();
	/*
	 * Pseudo-constructor.  This is used to add information to a
	 * preexisting student, assuming that the student was created
	 * as part of an array.  It is very similar to an assignment
	 * operator, which hasn't yet been covered in CS70.
	 */
	void		fillInInfo(const string& firstName_,
			  const string& lastName_, const string& hairColor_);
					// Fill in student info
	/*
	 * Information functions.
	 */
	string		name() const;	// Return student's full name in
					// .."last, first" format.
					// ..See implementation for an
					// ..explanation of why it
					// ..doesn't return "const
					// ..string&"
	bool		hasMaximumLoad() const;
					// True if can't add any more courses
	/*
	 * Modification functions.
	 */
	void		changeHairColor(const string& newColor);
					// Dye student's hair
	bool		addCourseToSchedule(Course* course);
					// Insert course in schedule;
					// ..also adds student to course if
					// ..needed.  Returns true if course
					// ..was added, false otherwise
					// ..(e.g., if course was already
					// ..there, or no room)
	bool		removeCourseFromSchedule(Course* course);
					// Remove course from schedule;
					// ..also removes student from
					// ..course if needed.  Returns true
					// ..if course was removed, false
					// ..otherwise (e.g., if course
					// ..wasn't in schedule)
	/*
	 * Display functions.
	 */
	ostream&	print(ostream& targetStream) const;
					// Produce representation of student
    private:
	/*
	 * Disable copy/assignment.
	 */
			Student(const Student& source);
	Student&	operator=(const Student& source);

	/*
	 * Private functions.
	 */

        void            dropAllCourses();
			         	// Drops student from all courses
					// ...in which s/he is enrolled.
  
	/*
	 * Private data.
	 */
        enum			
	    {
	    MAX_COURSES_PER_STUDENT = 5	// Limit on courses a student can take
	    };

	
	int             courseCount;    // Keeps track of # of courses
	 			        // student is enrolled in
	 
	string		fullName;	// Last name, first name
	string		hairColor;	// Current color of student's hair
	Course*		schedule[MAX_COURSES_PER_STUDENT];
					// Pointers to all the courses being
					// ..taken by this student
    };

#endif // STUDENT_HH
