#include "Lab.h"
#include <iostream>

// Constructor
Lab::Lab(int course, const string& day, const string& start, 
          int duration, const string& room, const string& building, 
          const string& teacher, const string& group)
    : Lesson(course, day, start, duration, room, building, teacher, group) {
}

// Copy constructor
Lab::Lab(const Lab& other) : Lesson(other) {
}

// Assignment operator
Lab& Lab::operator=(const Lab& other) {
    if (this != &other) {
        Lesson::operator=(other);
    }
    return *this;
}

// Destructor
Lab::~Lab() {
}

// Implementation of pure virtual function
string Lab::getLessonType() const {
    return "Lab";
}

// Override print function
void Lab::printLesson() const {
    cout << "[LAB] ";
    Lesson::printLesson();
}
