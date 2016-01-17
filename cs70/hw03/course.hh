 /****************************************************************************\
  * File:  course.hh                                                          *
  * Modified by: Russ Osborn, John Hearn                                      *
  * Date: 11 February, 2004                                                   *
  * Original Author: Geoff Kuenning                                           *
  * Course: CS 70, Spring 2004                                                *
  * Assignment #3                                                             *
  *                                                                           *
  * This file defines the interface to the Course class, which is an          *
  * auxiliary class used by the Registrar_DB class (see registrar.hh).        *
  *                                                                           *
  * The Course class can track a single course by name.  It allows the        *
  * course to be created and named, students to be enrolled and               *
  * deenrolled, and the current course data to be printed.                    *
  *                                                                           *
  \***************************************************************************/


#ifndef COURSE_HH
#define COURSE_HH

#include <iostream>
#include <string>

using namespace std;

/*
 * Table of contents: the following classes are defined in this file.
 */
class Course;

/*
 * The following classes are used but not defined in this file.
 */
class Student;

/*
 * Interface to the Course class.
 */
class Course
    {
    public:
	/*
	 * Constructors and destructors.  We provide a zero-argument
	 * constructor so that arrays of courses can be created.  For
	 * the current assignment, no other constructor is needed.
	 */
			Course();
			~Course();

	/*
	 * Pseudo-constructor.  This is used to add information to a
	 * course after it already exists, assuming that the course
	 * was created as part of an array.  It is very similar to an
	 * assignment operator, which hasn't yet been covered in CS70.
	 */
	void		fillInInfo(const string& department_,
			  int courseNumber_, const string& meetingTime_,
			  int maxEnrollment_);
					// Set or change course info

	/*
	 * Informational functions.
	 */
	bool		isFull() const;
					// Return true if course is at
					// ..maximum enrollment
	bool		courseIs(const string& testDepartment, int testNumber)
			  const;
					// Return true if course department
					// ..and number match parameters

	/*
	 * Modification functions.
	 */
	bool		addStudent(Student* enrollee);
					// Add a student to the course,
					// ..possibly also adding the course
					// ..to student's schedule
					// ..[depending on implementation].
					// ..Return true if add was
					// ..successful, false if not done
					// ..(e.g., student already in
					// ..course)
	bool		removeStudent(Student* dropper);
					// Remove a student from the course,
					// ..possibly also removing the
					// ..course from student's schedule
					// ..[depending on implementation].
					// ..Returns true if remove was
					// ..successful, false otherwise
					// ..(e.g., student wasn't in the
					// ..course)
	void		changeMeetingTime(const string& newMeetingTime);
					// Reschedule course

	/*
	 * Display functions.
	 */
	ostream&	showName(ostream& targetStream) const;
					// Display course name on a stream
	ostream&	print(ostream& targetStream) const;
					// Display all course info on a stream
    private:
	/*
	 * Disable copy/assignment.
	 */
			Course(const Course& source);
	Course&		operator=(const Course& source);

	/*
	 * Private functions.
	 */
        
        /* Pseudocode for Private functions: */
	
 	void kickOutStudents()		// Deenroll students
	 
	/* End pseudocode */ 

	/*
	 * Private data.
	 */
        
        /* Pseudocode for Private data members. */
	 
	string         departmentName
	integer        departmentCourseNumber
	integer        numStudentsEnrolled
	 
	/* End pseudocode */
	 
	string		meetingTime;	// e.g., "MWF 9"
	int		maxEnrollment;	// Maximum size of the roster
	Student**	roster;		// Students enrolled in course
					// ..kept alphabetized by name
    };

#endif // COURSE_HH
