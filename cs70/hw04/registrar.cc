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
   :students(new Student[maxStudents_]),
    courses(new Course[maxCourses_]),
    maxStudents(maxStudents_),
    maxCourses(maxCourses_),
    numStudents(0),
    numCourses(0)
    {

    }

Registrar_DB::~Registrar_DB()
    {
    /* 
     * Destructor: free memory allocated in constructor.
     */
    delete[] courses;
    delete[] students;
    }

bool Registrar_DB::canAcceptStudents()	// True if room for more students
    const
    {
    if (numStudents < maxStudents)
        return true;
    else
        return false;
    }

bool Registrar_DB::canAcceptCourses()	// True if room for more courses
    const
    {
    if (numCourses < maxCourses)
        return true;
    else
        return false;
    }

bool Registrar_DB::newStudent(
    const string&	firstName,
    const string&	lastName,
    const string&	hairColor)
    {
    if (canAcceptStudents() && !findStudentByName(firstName, lastName))
				        // If maximum students has not been
					// ...reached and student does not
					// ...already exist add student
              	                        // ...and initialize with
				        // ...parameters given.
        {
	students[numStudents].fillInInfo(firstName, lastName, hairColor);
	++numStudents;
	return true;
	}
    else
        return false;
    }

bool Registrar_DB::newCourse(
    const string&	department,
    int			courseNumber,
    const string&	meetingTime,
    int			maxEnrollment)
    {
    if (canAcceptCourses() && !findCourseByName(department, courseNumber))
				        // If maximum courses has not been
					// ...reached and course does not
					// ...already exist add course
                                        // ...and initialize with
				        // ...parameters given.
	{
	courses[numCourses].fillInInfo(department, courseNumber,
				       meetingTime, maxEnrollment);
	++numCourses;
	return true;
	}
    else
        return false;
    }

Student* Registrar_DB::findStudentByName(
    const string&	firstName,
    const string&	lastName)
    const
    {
    string fullName = lastName + ", " + firstName;

    /*
     * If student found in array, return address of student.
     */
    for (int i = 0; i < numStudents; i++)
        {
	if (students[i].name() == fullName)
	    {
	    return &students[i];
            }
        }

    /*
     * If student is not found, return NULL.  The calling function
     * handles error output.
     */
    return NULL;
    }

Course* Registrar_DB::findCourseByName(
    const string&	department,
    int			courseNumber)
    const
    {
    /*
     * If course found in array, return address of course.
     */
    for (int i = 0; i < numCourses; i++)
        {
        if (courses[i].courseIs(department, courseNumber))
            {
            return &courses[i];
            }
        }

    /*
     * If course is not found, return NULL.  The calling function
     * handles error output.
     */
    return NULL;
       
    }

ostream& Registrar_DB::showStudents(	// Display all students
    ostream&		stream)
    {
    for (int i = 0; i < numStudents; i++)
	{
        students[i].print(stream);
        }
    return stream;
    }

ostream& Registrar_DB::showCourses(     // Display all courses
    ostream&		stream)
    {
    for (int i = 0; i < numCourses; i++)
	{
	courses[i].print(stream);
        }
    return stream;
    }
