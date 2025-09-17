#ifndef TUTORIAL_H
#define TUTORIAL_H

#include "Lesson.h"

class Tutorial : public Lesson {
public:
    // Constructor
    Tutorial(int course, const string& day, const string& start, 
             int duration, const string& room, const string& building, 
             const string& teacher, const string& group);
    
    // Copy constructor
    Tutorial(const Tutorial& other);
    
    // Assignment operator
    Tutorial& operator=(const Tutorial& other);
    
    // Destructor
    ~Tutorial();
    
    // Implementation of pure virtual function
    string getLessonType() const override;
    
    // Override print function if needed
    void printLesson() const override;
};

#endif
