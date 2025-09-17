#include "Lecture.h"
#include <iostream>

// Constructor
Lecture::Lecture(int course, const string& day, const string& start, 
                 int duration, const string& room, const string& building, 
                 const string& teacher, const string& group)
    : Lesson(course, day, start, duration, room, building, teacher, group) {
}

// Copy constructor
Lecture::Lecture(const Lecture& other) : Lesson(other) {
}

// Assignment operator
Lecture& Lecture::operator=(const Lecture& other) {
    if (this != &other) {
        Lesson::operator=(other);
    }
    return *this;
}

// Destructor
Lecture::~Lecture() {
}

// Implementation of pure virtual function
string Lecture::getLessonType() const {
    return "Lecture";
}

// Override print function
void Lecture::printLesson() const {
    cout << "[LECTURE] ";
    Lesson::printLesson();
}
