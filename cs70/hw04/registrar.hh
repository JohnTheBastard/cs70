 /****************************************************************************\
  * File:  registrar.hh                                                       *
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
  \***************************************************************************/


#ifndef REGISTRAR_HH
#define REGISTRAR_HH

#include <iostream>
#include <string>

using namespace std;

/*
 * Table of contents: the following classes are defined in this file.
 */
class Registrar_DB;

/*
 * The following classes are used but not defined in this file.
 */
class Course;
class Student;

/*
 * Interface to the registrar's database.
 */
class Registrar_DB
    {
    public:
	/*
	 * Constructors and destructors.
	 *
	 * To create a new registrar database, you must specify upper
	 * limits on numbers of students and courses, because this
	 * database doesn't dynamically extend itself when it is full.
	 * So the constructor takes maxima for both students and
	 * courses.
	 */
			Registrar_DB(int maxStudents_, int maxCourses_);
			~Registrar_DB();

	/*
	 * Informational functions.
	 */
	bool		canAcceptStudents() const;
					// True if room for more students
	bool		canAcceptCourses() const;
					// True if room for more courses

	/*
	 * Functions for inserting students and courses.
	 */
	bool		newStudent(const string& firstName,
			  const string& lastName, const string& hairColor);
					// Add a new student to DB.  Returns
					// ..true if add was successful, false
					// ..otherwise (e.g., if student
					// ..already existed or there was no
					// ..room).
	bool		newCourse(const string& department,
			  int courseNumber, const string& meetingTime,
			  int maxEnrollment);
					// Add a new course to DB.  Returns
					// ..true if add was successful, false
					// ..otherwise (e.g., if course already
					// ..existed or there was no room).
	/*
	 * Functions for locating students and courses.
	 */
	Student*	findStudentByName(const string& firstName,
			  const string& lastName) const;
					// Given a student name, find him/her.
					// ..Returns NULL if not found.
	Course*		findCourseByName(const string& department,
			  int courseNumber) const;
					// Given a course name, find it.
					// ..Returns NULL if not found.

	/*
	 * Display functions.
	 */
	ostream&	showStudents(ostream& stream);
					// Display all students
	ostream&	showCourses(ostream& stream);
					// Display all courses
    private:
	/*
	 * Disable copy/assignment.
	 */
			Registrar_DB(const Registrar_DB& source);
	Registrar_DB&	operator=(const Registrar_DB& source);

	/*
	 * Private data
	 */
	Student*	students;	// Array of all known students
	Course*		courses;	// Array of all known courses
	int		maxStudents;	// Maximum number of students
					// ..allowed in this database
	int		maxCourses;	// Maximum number of courses
					// ..allowed in this database
	int		numStudents;	// Current number of students
	int		numCourses;	// Current number of courses
    };

#endif // REGISTRAR_HH
