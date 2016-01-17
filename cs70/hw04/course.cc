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
   :departmentName(),
    departmentCourseNum(0),
    numStudentsEnrolled(0),
    meetingTime(),
    maxEnrollment(0),
    roster(NULL)
    {
    /*
     * Initialize roster array declared in header file.
     */
    roster = new Student*[maxEnrollment];
    for (int i = 0; i < maxEnrollment; i++) 
         roster[i] = NULL;
    }

Course::~Course()
    {
    /*
     * Before deleting the course, we must deenroll all students.
     */
    kickOutStudents();
    delete[] roster; 
    }

/*
 * In liu of copy constructor, pseudo-constructor fills in course info. 
 */
void Course::fillInInfo(
    const string&	department_,
    int			courseNumber_,
    const string&	meetingTime_,
    int			maxEnrollment_)
    {
    departmentName = department_;
    departmentCourseNum = courseNumber_;
    maxEnrollment = maxEnrollment_;
    meetingTime = meetingTime_;
    return;  
    }

bool Course::isFull()		         // True if can't add any more students
    const
    {
    if (numStudentsEnrolled >= maxEnrollment)
        return true;
    else
        return false;
    }

bool Course::courseIs(		        // True if dept name, number 
				        // ..and course name match  
    const string&	testDepartment,	// Department to check
    int			testNumber)     // Course number to check
    const
    {
    if (departmentName == testDepartment && departmentCourseNum == testNumber)
        return true;
    else
        return false;
    }

bool Course::addStudent(
    Student*		enrollee)
    {
    /*
     * This function will be called by the Student class' function
     * addCourseToSchedule, so we do not need to modify the student's 
     * schedule from here. As a defensive programming measure, we will
     * check that the course's maximum enrollment level is not exceeded.
     */
    if (isFull() || enrollee->hasMaximumLoad())
        return false;
     
    /*
     * Check all enrolled students to make sure student not already enrolled
     */
    int                 i;
    for (i = 0; i < numStudentsEnrolled; i++)
        {
	if (roster[i] == enrollee)
	    return false;
	}
    /*
     * i is re-initialized to the number of students enrolled.  
     * The while loop begins at the end of the roster and shifts enrolled
     * students who follow the student to be inserted lexigraphically. 
     * The new student is then assigned to the now open spot in the roster.
     */

    i = numStudentsEnrolled;       
    while ((i > 0) && (roster[i-1]->name() > enrollee->name()))
        {
	roster[i] = roster[i-1];        // shift the current student down in
	                                // ...the roster.
	--i;
        }

    /*
     * i marks the point of insertion for the new student and all latter
     * students have been shifted. 
     */
    roster[i] = enrollee;
    ++numStudentsEnrolled;
    return true;
    }

bool Course::removeStudent(
    Student*		dropper)	 // Student who is dropping the course
    {
    /* 
     * Check all enrolled students to make sure student is already enrolled
     */
    int                 i;
    for (i = 0; i < numStudentsEnrolled; i++)
        {
	if (roster[i] == dropper)
	    break;			 // student is found in the roster
	}

    if (i == numStudentsEnrolled)
        return false;                    // student not found in the roster
    
    /*
     * i gives the position of the dropper in the roster array
     */
    for(int k = i; k < numStudentsEnrolled - 1; k++)
	roster[k] = roster[k+1];         // shift students up in the array if
                                         // ...they come after the dropping
					 // ...student
    roster[numStudentsEnrolled] = NULL;
    --numStudentsEnrolled;
    return true;
    }

void Course::changeMeetingTime(	         // Enter new course meeting time 
    const string&	newMeetingTime)  // New course meeting time
    {
    meetingTime = newMeetingTime;
    return;
    }

ostream& Course::showName(	         // Print department, number,
				         // ...and course name
    ostream&		targetStream)
    const
    {
    targetStream << departmentName << departmentCourseNum;
    return targetStream;
    }

ostream& Course::print(		         // Print complete course information
    ostream&		targetStream)
    const
    {
    showName(targetStream);
    targetStream << " meets " << meetingTime << "\n" << flush;
    targetStream << "\t" << "Roster:" << "\n" << flush;
    for (int i = 0; i < numStudentsEnrolled; i++)
        targetStream << "\t" << roster[i]->name() << "\n" << flush;
    return targetStream;
    }

void Course::kickOutStudents()		// Deenroll all students
    {
    /*
     * As long as roster is not empty, remove first student from roster.
     * (Recall that the roster is alphabetized and compacted.)
     */
    while(numStudentsEnrolled > 0) 
        {
        roster[0]->removeCourseFromSchedule(this);
        }
    return;
    }
