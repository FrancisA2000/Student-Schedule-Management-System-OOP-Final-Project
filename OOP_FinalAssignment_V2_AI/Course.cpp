#include "Course.h"

#include <iostream>

// Default constructor
Course::Course()
    : courseId(0),
      name(""),
      credits(0),
      examDateA(""),
      examDateB(""),
      lecturer("") {}

// Constructor
Course::Course(int id, const string& courseName, double courseCredits,
               const string& dateA, const string& dateB,
               const string& courseLecturer)
    : courseId(id),
      name(courseName),
      credits(courseCredits),
      examDateA(dateA),
      examDateB(dateB),
      lecturer(courseLecturer) {}

// Copy constructor
Course::Course(const Course& other)
    : courseId(other.courseId),
      name(other.name),
      credits(other.credits),
      examDateA(other.examDateA),
      examDateB(other.examDateB),
      lecturer(other.lecturer) {}

// Assignment operator
Course& Course::operator=(const Course& other) {
  if (this != &other) {
    courseId = other.courseId;
    name = other.name;
    credits = other.credits;
    examDateA = other.examDateA;
    examDateB = other.examDateB;
    lecturer = other.lecturer;
  }
  return *this;
}

// Destructor
Course::~Course() {}

// Getters
int Course::getCourseId() const { return courseId; }

string Course::getName() const { return name; }

double Course::getCredits() const { return credits; }

string Course::getExamDateA() const { return examDateA; }

string Course::getExamDateB() const { return examDateB; }

string Course::getLecturer() const { return lecturer; }

// Setters
void Course::setCourseId(int id) { courseId = id; }

void Course::setName(const string& courseName) { name = courseName; }

void Course::setCredits(double courseCredits) { credits = courseCredits; }

void Course::setExamDateA(const string& dateA) { examDateA = dateA; }

void Course::setExamDateB(const string& dateB) { examDateB = dateB; }

void Course::setLecturer(const string& courseLecturer) {
  lecturer = courseLecturer;
}

// Print course details
void Course::printCourse() const {
  cout << "Course ID: " << courseId << endl;
  cout << "Name: " << name << endl;
  cout << "Credits: " << credits << endl;
  cout << "Exam Date A: " << examDateA << endl;
  cout << "Exam Date B: " << examDateB << endl;
  cout << "Lecturer: " << lecturer << endl;
  cout << "------------------------" << endl;
}
