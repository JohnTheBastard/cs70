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
   :courseCount(0),
    fullName(),
    hairColor()
    {
    /*
     * Initialize schedule array declared in header file 
     */
    for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++)
        {
        schedule[i] = NULL;
        }
    }

Student::~Student()		      // Destructor
    {
    dropAllCourses();
    }

/*
 * In liu of copy constructor, pseudo-constructor fills in student info. 
 */
void Student::fillInInfo(
    const string&	firstName_,
    const string&	lastName_,
    const string&	hairColor_)
    {
    fullName = lastName_ + ", " + firstName_;
    hairColor = hairColor_;
    return;
    }

/*
 * Return the student's full name in "last, first" format.  
 */
string Student::name()
    const
    {
    return fullName; 
    }

bool Student::hasMaximumLoad()		// True if can't add any more courses
    const
    {
    if (courseCount == MAX_COURSES_PER_STUDENT)
	return true;
    else
	return false;
    }

void Student::changeHairColor(		// Dye student's hair
    const string&	newColor)	// New color to give to hair
    {
    hairColor = newColor;
    return;
    }

bool Student::addCourseToSchedule(	// Insert course in schedule
    Course*		course)		// Course being added
    {
    if (course->isFull() || hasMaximumLoad())
                                        // Course is full or schedule is full
					// Do not add course to schedule
        return false;
    else                                
        {
        int             i;	        // Schedule index
	   
	for (i = 0; i < MAX_COURSES_PER_STUDENT; i++)
	    { 
            if (schedule[i] == course)
				        // If student is already enrolled
					// ...don't add course.
            return false;
            }
           
	 course->addStudent(this);
	 i = 0;
	 while (schedule[i] != NULL)
             ++i;
				        // Found first non-null pointer 
					// ...in schedule list.
	 schedule[i] = course;
	 ++courseCount;
	 return true;
	 }
    }

bool Student::removeCourseFromSchedule(	// Remove course from schedule
    Course*		course)		// Course being removed
    {
    for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++)
        {
	if (schedule[i] == course)
				        // Search schedule for course
					// ...and drop if found
	    {
	    schedule[i] = NULL;
            course->removeStudent(this);
            --courseCount;
            return true;
            }
	}
    return false;
    }

ostream& Student::print(		// Produce representation of student
    ostream&		targetStream)	// Stream to print to
    const
    {
    targetStream << fullName << ", " << hairColor << " hair" << "\n" << flush;

    if (courseCount == 1)
	targetStream << "\t" << courseCount << " course:" << flush;
    else
        targetStream << "\t" << courseCount << " courses:" << flush;

    for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++)
	{
        if (schedule[i] != NULL)
            {
				        // Print any courses in schedule
            targetStream << " ";
            schedule[i]->showName(targetStream);
            }
	}
    targetStream << "\n" << flush;
    return targetStream;
    }

void Student::dropAllCourses()		// Remove student from all enrollment
    {
    for (int i = 0; i < MAX_COURSES_PER_STUDENT; i++)
        {
	if (schedule[i] != NULL)
	    removeCourseFromSchedule(schedule[i]);
	}
    courseCount = 0;		        // This is a defensive measure.
				        // courseCount should already be zero.
    return;   
    }
