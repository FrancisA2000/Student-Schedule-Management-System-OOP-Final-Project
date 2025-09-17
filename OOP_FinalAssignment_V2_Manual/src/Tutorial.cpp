#include "Tutorial.h"
#include <iostream>

// Constructor
Tutorial::Tutorial(int course, const string& day, const string& start, 
                   int duration, const string& room, const string& building, 
                   const string& teacher, const string& group)
    : Lesson(course, day, start, duration, room, building, teacher, group) {
}

// Copy constructor
Tutorial::Tutorial(const Tutorial& other) : Lesson(other) {
}

// Assignment operator
Tutorial& Tutorial::operator=(const Tutorial& other) {
    if (this != &other) {
        Lesson::operator=(other);
    }
    return *this;
}

// Destructor
Tutorial::~Tutorial() {
}

// Implementation of pure virtual function
string Tutorial::getLessonType() const {
    return "Tutorial";
}

// Override print function
void Tutorial::printLesson() const {
    cout << "[TUTORIAL] ";
    Lesson::printLesson();
}
