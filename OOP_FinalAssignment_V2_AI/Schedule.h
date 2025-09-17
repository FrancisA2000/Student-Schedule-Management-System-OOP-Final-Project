#ifndef SCHEDULE_H
#define SCHEDULE_H

#include <map>
#include <memory>
#include <vector>

#include "Course.h"
#include "Lesson.h"
using namespace std;

class Schedule {
 private:
  int scheduleId;
  vector<shared_ptr<Lesson>> lessons;
  static map<int, Course> courseDatabase;  // Static course lookup

 public:
  // Constructor
  Schedule(int id);

  // Copy constructor
  Schedule(const Schedule& other);

  // Assignment operator
  Schedule& operator=(const Schedule& other);

  // Destructor
  ~Schedule();

  // Getters
  int getScheduleId() const;
  const vector<shared_ptr<Lesson>>& getLessons() const;

  // Setters
  void setScheduleId(int id);

  // Add lesson to schedule
  void addLesson(shared_ptr<Lesson> lesson);

  // Remove lesson from schedule by course and group
  bool removeLesson(int courseId, const string& groupId);

  // Check if there's a conflict with new lesson
  bool hasConflict(const Lesson& newLesson) const;

  // Print schedule in table format
  void printSchedule() const;

  // Get lessons for specific day
  vector<shared_ptr<Lesson>> getLessonsForDay(const string& day) const;

  // Clear all lessons
  void clearSchedule();

  // Get total credits for the schedule
  int getTotalCredits() const;

  // Get number of lessons
  int getLessonCount() const;

  // Save schedule to CSV
  void saveToCSV(const string& filename) const;

  // Load schedule from CSV
  void loadFromCSV(const string& filename);

  // Static methods for course database
  static void setCourseDatabase(const vector<Course>& courses);
  static string getCourseNameById(int courseId);
};

#endif
