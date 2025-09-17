#ifndef LAB_H
#define LAB_H

#include "Lesson.h"

class Lab : public Lesson {
 public:
  // Constructor
  Lab(int course, const string& day, const string& start, int duration,
      const string& room, const string& building, const string& teacher,
      const string& group);

  // Copy constructor
  Lab(const Lab& other);

  // Assignment operator
  Lab& operator=(const Lab& other);

  // Destructor
  ~Lab();

  // Implementation of pure virtual function
  string getLessonType() const override;

  // Override print function if needed
  void printLesson() const override;
};

#endif
