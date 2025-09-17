#include "Lesson.h"

#include <iostream>

// Constructor
Lesson::Lesson(int course, const string& lessonDay, const string& start,
               int dur, const string& room, const string& bldg,
               const string& instructor, const string& group)
    : courseId(course),
      day(lessonDay),
      startTime(start),
      duration(dur),
      classroom(room),
      building(bldg),
      teacher(instructor),
      groupId(group) {}

// Copy constructor
Lesson::Lesson(const Lesson& other)
    : courseId(other.courseId),
      day(other.day),
      startTime(other.startTime),
      duration(other.duration),
      classroom(other.classroom),
      building(other.building),
      teacher(other.teacher),
      groupId(other.groupId) {}

// Assignment operator
Lesson& Lesson::operator=(const Lesson& other) {
  if (this != &other) {
    courseId = other.courseId;
    day = other.day;
    startTime = other.startTime;
    duration = other.duration;
    classroom = other.classroom;
    building = other.building;
    teacher = other.teacher;
    groupId = other.groupId;
  }
  return *this;
}

// Virtual destructor
Lesson::~Lesson() {}

// Getters
int Lesson::getCourseId() const { return courseId; }

string Lesson::getDay() const { return day; }

string Lesson::getStartTime() const { return startTime; }

int Lesson::getDuration() const { return duration; }

string Lesson::getClassroom() const { return classroom; }

string Lesson::getBuilding() const { return building; }

string Lesson::getTeacher() const { return teacher; }

string Lesson::getGroupId() const { return groupId; }

// Setters
void Lesson::setCourseId(int course) { courseId = course; }

void Lesson::setDay(const string& lessonDay) { day = lessonDay; }

void Lesson::setStartTime(const string& start) { startTime = start; }

void Lesson::setDuration(int dur) { duration = dur; }

void Lesson::setClassroom(const string& room) { classroom = room; }

void Lesson::setBuilding(const string& bldg) { building = bldg; }

void Lesson::setTeacher(const string& instructor) { teacher = instructor; }

void Lesson::setGroupId(const string& group) { groupId = group; }

// Virtual function for printing lesson details
void Lesson::printLesson() const {
  cout << "Course: " << courseId << " | ";
  cout << "Type: " << getLessonType() << " | ";
  cout << "Day: " << day << " | ";
  cout << "Time: " << startTime << " | ";
  cout << "Duration: " << duration << "h | ";
  cout << "Room: " << classroom << " " << building << " | ";
  cout << "Teacher: " << teacher << " | ";
  cout << "Group: " << groupId << endl;
}
