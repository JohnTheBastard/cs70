 /****************************************************************************\
  * File:  student.cc                                                         *
  * Modified by: Russ Osborn, John Hearn                                      *
  * Date: 11 February, 2004                                                   *
  * Original Author: Geoff Kuenning                                           *
  * Course: CS 70, Spring 2004                                                *
  * Assignment #3                                                             *
  *                                                                           *
  * This file implementss the interface to the Student class, which is an     *
  * auxiliary class used by the Registrar_DB class (see registrar.hh).        *
  *                                                                           *
  * The Student class describes a single college student.  It allows          *
  * the student to be created and his/her hair color to be changed at         *
  * will.  It also supports adding courses to the student's schedule          *
  * and removing them later.                                                  *
  *                                                                           *
 \****************************************************************************/

#include "course.hh"
#include "student.hh"
#include <iostream>
#include <string>

using namespace std;

/*****************************************************************************\
 * Table of Contents: the following routines are defined in this file:        *
 *                                                                            *
 *			Student::Student();                                   *
 *			Student::~Student();                                  *
 * void			Student::fillInInfo(const string& firstName_,         *
 *			  const string& lastName_, const string& hairColor_); *
 *					// Fill in student info               *
 * string		Student::name() const;                                *
 *					// Return student's full name         *
 * bool			Student::hasMaximumLoad() const;                      *
 *					// True if can't add any more courses *
 * void			Student::changeHairColor(const string& newColor);     *
 *					// Dye student's hair                 *
 * bool			Student::addCourseToSchedule(Course* course);         *
 *					// Insert course in schedule;         *
 *					// ..also adds student to course if   *
 *					// ..needed.  Returns true if course  *
 *					// ..was added, false otherwise       *
 *					// ..(e.g., if course was already     *
 *					// ..there, or no room)               *
 * bool			Student::removeCourseFromSchedule(Course* course);    *
 *					// Remove course from schedule;       *
 *					// ..also removes student from        *
 *					// ..course if needed.  Returns true  *
 *					// ..if course was removed, false     *
 *					// ..otherwise (e.g., if course       *
 *					// ..wasn't in schedule)              *
 * ostream&		Student::print(ostream& targetStream) const;          *
 *					// Produce representation of student  *
 * void			Student::dropAllCourses();                            *
 *					// Remove student from all enrollment *
 \****************************************************************************/

Student::Student()
    : fullName(),
      hairColor(),
      courseCount(0)
    {
      /* Pseudocode for student constructor */
       
        // initialize schedule array declared in header file
       
        for (i=0:MaxCourses; i++)
           schedule[i] = null
       
    }

Student::~Student()
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

/*
 * Pseudo-constructor to fill in student information after the fact.
 * This should really be done using a copy constructor and assignment
 * operator, but I'm not 100% sure we'll have covered them in time.
 */
void Student::fillInInfo(
    const string&	firstName_,
    const string&	lastName_,
    const string&	hairColor_)
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

/*
 * Return the student's full name in "last, first" format.  Depending
 * on how this function is implemented, it might be possible to return
 * a "const string&", which would make it more efficient.  However,
 * the "string" return type declaration is necessary if the full name
 * is constructed "on the fly", and causes no harm in other
 * situations, so I have chosen that form.
 */
string Student::name()			// Return student's full name
    const
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

bool Student::hasMaximumLoad()		// True if can't add any more courses
    const
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

void Student::changeHairColor(		// Dye student's hair
    const string&	newColor)	// New color to give to hair
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

bool Student::addCourseToSchedule(	// Insert course in schedule
    Course*		course)		// Course being added
    {
      /* Pseudocode for addCourseToSchedule */
       
        if (!course->isFull() && !student.hasMaximumLoad())
           course->addStudent(&this)
       
           for (i = 0; i < maxCourses, i++)
	     if (schedule[i] = course)
              return error: student already enrolled
       
           i = 0

	   while (schedule[i] != null)
              i++
           Found first non-null pointer in schedule list
           Set that element in schedule = course
        else return error
           (course is full or schedule is full)
       
        ++courseCount
       
    }

bool Student::removeCourseFromSchedule(	// Remove course from schedule
    Course*		course)		// Course being removed
    {
      /* Pseudocode for removeCourseFromSchedule */
        
        course->removeStudent(&this)
       
        for (i = 0; i < MaxCourses; i++)
	  if (schedule[i] == course) 
	    schecule[i] = NULL;
	    --courseCount
	    return true

	return false
    }

ostream& Student::print(		// Produce representation of student
    ostream&		targetStream)	// Stream to print to
    const
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    // THIS ROUTINE SHOULD PRODUCE THE FULL STUDENT OUTPUT: NAME, HAIR
    // COLOR, AND COURSE LIST
    return targetStream;
    }

void Student::dropAllCourses()		// Remove student from all enrollment
    {
      /* Pseudocode for dropAllCourses */
        
        for(i=0; i < maxCourses; i++)
           if (schedule[i] != null)
              removeCourseFromSchedule
       
        courseCount = 0  //this is a defensive measure, should already be zero
       
    }
