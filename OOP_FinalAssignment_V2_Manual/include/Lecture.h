#ifndef LECTURE_H
#define LECTURE_H

#include "Lesson.h"

class Lecture : public Lesson {
public:
    // Constructor
    Lecture(int course, const string& day, const string& start, 
            int duration, const string& room, const string& building, 
            const string& teacher, const string& group);
    
    // Copy constructor
    Lecture(const Lecture& other);
    
    // Assignment operator
    Lecture& operator=(const Lecture& other);
    
    // Destructor
    ~Lecture();
    
    // Implementation of pure virtual function
    string getLessonType() const override;
    
    // Override print function if needed
    void printLesson() const override;
};

#endif
