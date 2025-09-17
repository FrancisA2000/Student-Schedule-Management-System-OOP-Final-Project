#ifndef LESSON_H
#define LESSON_H

#include <string>
using namespace std;

class Lesson {
 protected:
  int courseId;
  string day;
  string startTime;
  int duration;
  string classroom;
  string building;
  string teacher;
  string groupId;

 public:
  // Constructor
  Lesson(int course, const string& lessonDay, const string& start, int dur,
         const string& room, const string& bldg, const string& instructor,
         const string& group);

  // Copy constructor
  Lesson(const Lesson& other);

  // Assignment operator
  Lesson& operator=(const Lesson& other);

  // Virtual destructor
  virtual ~Lesson();

  // Getters
  int getCourseId() const;
  string getDay() const;
  string getStartTime() const;
  int getDuration() const;
  string getClassroom() const;
  string getBuilding() const;
  string getTeacher() const;
  string getGroupId() const;

  // Setters
  void setCourseId(int course);
  void setDay(const string& lessonDay);
  void setStartTime(const string& start);
  void setDuration(int dur);
  void setClassroom(const string& room);
  void setBuilding(const string& bldg);
  void setTeacher(const string& instructor);
  void setGroupId(const string& group);

  // Pure virtual function - makes this class abstract
  virtual string getLessonType() const = 0;

  // Virtual function for printing lesson details
  virtual void printLesson() const;
};

#endif
