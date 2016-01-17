 /****************************************************************************\
  * File:  course.cc                                                          *
  * Modified by: Russ Osborn, John Hearn                                      *
  * Date: 11 February, 2004                                                   *
  * Original Author: Geoff Kuenning                                           *
  * Course: CS 70, Spring 2004                                                *
  * Assignment #3                                                             *
  *                                                                           *
  * This file implements the interface to the Student class, which is an      *
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
 *			Course::Course();                                     *
 *			Course::Course(const string department_,              *
 *			  int courseNumber_, const string meetingTime_,       *
 *			  int maxEnrollment_);                                *
 *					// Construct from parameters          *
 *			Course::~Course();                                    *
 * void			Course::fillInInfo(const string department_,          *
 *			  int courseNumber_, const string meetingTime_,       *
 *			  int maxEnrollment_);                                *
 *					// Set or change course info          *
 * bool			Course::isFull() const;                               *
 *					// Return true if course is at        *
 *					// ..maximum enrollment               *
 * bool			Course::courseIs(const string testDepartment,         *
 *			  int testNumber) const;                              *
 *					// Return true if course department   *
 *					// ..and number match parameters      *
 * bool			Course::addStudent(Student* enrollee);                *
 *					// Add a student to the course,       *
 *					// ..possibly also adding the course  *
 *					// ..to student's schedule            *
 *					// ..[depending on implementation].   *
 *					// ..Return true if add was           *
 *					// ..successful, false if not done    *
 *					// ..(e.g., student already in        *
 *					// ..course)                          *
 * void			Course::removeStudent(Student* dropper);              *
 *					// Remove a student from the course,  *
 *					// ..possibly also removing the       *
 *					// ..course from student's schedule   *
 *					// ..[depending on implementation].   *
 *					// ..Returns true if remove was       *
 *					// ..successful, false otherwise      *
 *					// ..(e.g., student wasn't in the     *
 *					// ..course)                          *
 * void			Course::changeMeetingTime(string newMeetingTime);     *
 *					// Reschedule course                  *
 * ostream&		Course::showName(ostream& targetStream) const;        *
 *					// Display course name on a stream    *
 * ostream&		Course::print(ostream& targetStream) const;           *
 *					// Display all course info on a stream*
 * void			Course::kickOutStudents();                            *
 *					// Deenroll all students              *
\*****************************************************************************/

Course::Course()
    : department(),
      courseNumber(0),
      meetingTime(),
      maxEnrollment(0),
      roster(NULL)
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

Course::~Course()
    {
    /*
     * Before deleting the course, we must deenroll all students.
     */
    kickOutStudents()

    }

void Course::fillInInfo(
    const string&	department_,
    int			courseNumber_,
    const string&	meetingTime_,
    int			maxEnrollment_)
    {
    /* Pseudocode for fillInInfo */
    
      departmentName = department_
      departmentCourseNumber = courseNumber_
      
      maxEnrollment = maxEnrollment_
      meetingTime = meetingTime_
      
      roster = new Student*[maxEnrollment]
      for (int i = 0; i < maxEnrollment; ++i) 
	roster[i] = NULL
      
    }

bool Course::isFull()
    const
    {
     /* Pseudocode for isFull */

      if numStudentsEnrolled >= maxEnrollment
         return true
      else
         return false
     
     
    }

bool Course::courseIs(
    const string&	testDepartment,
    int			testNumber)
    const
    {
    /* Pseudocode for courseIs */
     
      Compare parameters to appropriate private data members
      if both match, return true.
      else return false.
     
     
    }

bool Course::addStudent(
    Student*		enrollee)
    {
      /* Pseudocode for addStudent */
      
      /*
       * This function will be called by the Student class' function
       * addCourseToSchedule, so we do not need to modify the student's 
       * schedule from here. As a defensive programming measure, we will
       * check that the course's maximum enrollment level is not exceeded.
       */

     
      if isFull()
         return error, class full
     
      // Check all enrolled students to make sure student not already enrolled
      for(i=0; i < numStudentsEnrolled; i++)
        if(roster[i] == enrollee)
	  return error, already enrolled   
     

    /*
     * i is re-initialized to the number of students enrolled.  The while loop
     * begins at the end of the roster and shifts enrolled students who 
     * follow the student to be inserted lexigraphically.  The new student
     * is then assigned to the now open spot in the roster.
     */

      int i = numStudentsEnrolled

      while(roster[i]->name() > enrollee.name())
	roster[i+1] = roster[i]        // shift the current student down in
	                               // the roster.
	--i;

      /*
       * i marks the point of insertion for the new student and all latter
       * students have been shifted. 
       */
      roster[i] = enrollee
     
      ++numStudents

    }

bool Course::removeStudent(
    Student*		dropper)	// Student who is dropping the course
    {
    /* Pseudocode for removeStudent */
     
                                        // Check all enrolled students to make
					// sure student is already enrolled
      for(i=0; i < numStudentsEnrolled; i++)
        if(roster[i] == dropper)
	  break			        // student is found in the roster

      if i == numStudentsEnrolled
         return false, student not found in the roster.

      
				        // i gives the position of the
					// dropper in the roster array
      for(k = i; k < numStudentsEnrolled - 1; k++)
	roster[k] = roster[k+1];        // shift students up in the array if
                                        // they come after the dropping student

      roster[numStudents] = NULL;

      --numStudents

      return true
    }

void Course::changeMeetingTime(
    const string&	newMeetingTime)
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

ostream& Course::showName(
    ostream&		targetStream)
    const
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    return targetStream;
    }

ostream& Course::print(
    ostream&		targetStream)
    const
    {
    /* Pseudocode for print */
     
      Print course department and number.
     
      for (i = 0; i < numStudentsEnrolled; i++)
         Print out names of these students
     
     
    return targetStream;
    }

void Course::kickOutStudents()		// Deenroll all students
    {
    /* Pseudocode for kickOutStudents */
     
      While(students enrolled) 
         call roster[0]->removeCourseFromSchedule
      
      delete roster
     
    }
