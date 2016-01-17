 /****************************************************************************\
  * File:  registrar.cc                                                       *
  * Modified by: Russ Osborn, John Hearn                                      *
  * Date: 11 February, 2004                                                   *
  * Original Author: Geoff Kuenning                                           *
  * Course: CS 70, Spring 2004                                                *
  * Assignment #3                                                             *
  *                                                                           *
  * This file defines the interface to the Registrar_DB class.  This          *
  * class supports a simple registrar's database for a small college.         *
  * The database can track students and classes, and can handle minor         *
  * changes in the student records, addition of new students and              *
  * classes, and changes in enrollment.                                       *
  *                                                                           *
 \****************************************************************************/

#include "course.hh"
#include "registrar.hh" 
#include "student.hh"
#include <iostream>
#include <string>

using namespace std;

/****************************************************************************\ 
 * Table of Contents: the following routines are defined in this file:      *
 *                                                                          *
 *			Registrar_DB::Registrar_DB(int maxStudents_,        *
 *			  int maxCourses_);                                 *
 *			Registrar_DB::~Registrar_DB();                      *
 * bool			Registrar_DB::canAcceptStudents() const;            *
 *					// True if room for more students   *
 * bool			Registrar_DB::canAcceptCourses() const;             *
 *					// True if room for more courses    *
 * bool			Registrar_DB::newStudent(const string& firstName,   *
 *			  const string& lastName, const string& hairColor); *
 *					// Add a new student to DB          *
 * bool			Registrar_DB::newCourse(const string& department,   *
 *			  int courseNumber, const string& meetingTime,      *
 *			  int maxEnrollment);                               *
 *					// Add a new course to DB           *
 * Student*		Registrar_DB::findStudentByName(                    *
 *			  const string& firstName, const string& lastName)  *
 *			    const;                                          *
 *					// Given a student name, find him/her
 * Course*		Registrar_DB::findCourseByName(                     *
 *			  const string& department, int courseNumber)       *
 *			    const;                                          *
 *					// Given a course name, find it     *
 * ostream&		Registrar_DB::showStudents(ostream& stream);        *
 *					// Display all students             *
 * ostream&		Registrar_DB::showCourses(ostream& stream);         *
 *					// Display all courses              *
 \**************************************************************************/

Registrar_DB::Registrar_DB(
    int			maxStudents_,
    int			maxCourses_)
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    {
      /*
       *
       * Pseudocode for Registrar Constructor
       * Initialize maxStudents and maxCourses to the parameters given.
       *
       * Create studentArray of size maxStudents+1 (so that we can just 
       * index this array with the numStudents variable). 
       * Set the students pointer to the first element in this array.
       *
       * Follow a similar sequence for the coursesArray.
       * Set the courses pointer to the first element of this array.
       *
       * Initialize numStudents and numCourses to 0.
       *
       */ 
    }

Registrar_DB::~Registrar_DB()
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

bool Registrar_DB::canAcceptStudents()	// True if room for more students
    const
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

bool Registrar_DB::canAcceptCourses()	// True if room for more courses
    const
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    }

bool Registrar_DB::newStudent(
    const string&	firstName,
    const string&	lastName,
    const string&	hairColor)
    {
      /* Pseudocode for newStudent function */
       
        if (!canAcceptStudents())
          print error (too many students in database)
        else
          Increment numStudents.
          students[numStudents] = Student()
          fillInInfo for this student using given parameters
       
    }

bool Registrar_DB::newCourse(
    const string&	department,
    int			courseNumber,
    const string&	meetingTime,
    int			maxEnrollment)
    {

      /* Pseudocode for newCourse function */
       
        if (!canAcceptCourses())
          print error (too many courses in database)
        else
          Increment numCourses.
          courses[numCourses] = Course()
          fillInInfo for this course using given parameters
       
       

    }

Student* Registrar_DB::findStudentByName(
    const string&	firstName,
    const string&	lastName)
    const
    {
      /* Pseudocode for findStudentByName function */
       
         string fullName = lastName + ", " + firstName.
         for all students in student array
            check if students[position].name equals fullName.
               if true, return address of students[position].
       
         If not found, print error message.
       
       
    }

Course* Registrar_DB::findCourseByName(
    const string&	department,
    int			courseNumber)
    const
    {
      /* Pseudocode for findCourseByName function */
       
        for all courses in course array
          check if courses[position].courseIs(parameters) returns true
            if yes, return address of courses[position]
       
        If not found, print error message.
       
    }

ostream& Registrar_DB::showStudents(	// Display all students
    ostream&		stream)
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    return stream;
    }

ostream& Registrar_DB::showCourses(     // Display all courses
    ostream&		stream)
    {
    // ADD STUFF (PSEUDOCODE NOT NEEDED)
    return stream;
    }
