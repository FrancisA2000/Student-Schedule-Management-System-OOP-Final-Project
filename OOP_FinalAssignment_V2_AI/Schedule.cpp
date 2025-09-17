#include "Schedule.h"

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>

#include "Course.h"
#include "Lab.h"
#include "Lecture.h"
#include "Tutorial.h"

// Initialize static member
map<int, Course> Schedule::courseDatabase;

// Constructor
Schedule::Schedule(int id) : scheduleId(id) {}

// Copy constructor
Schedule::Schedule(const Schedule& other)
    : scheduleId(other.scheduleId), lessons(other.lessons) {}

// Assignment operator
Schedule& Schedule::operator=(const Schedule& other) {
  if (this != &other) {
    scheduleId = other.scheduleId;
    lessons = other.lessons;
  }
  return *this;
}

// Destructor
Schedule::~Schedule() {}

// Getters
int Schedule::getScheduleId() const { return scheduleId; }

const vector<shared_ptr<Lesson>>& Schedule::getLessons() const {
  return lessons;
}

// Setters
void Schedule::setScheduleId(int id) { scheduleId = id; }

// Add lesson to schedule
void Schedule::addLesson(shared_ptr<Lesson> lesson) {
  if (lesson != nullptr) {
    lessons.push_back(lesson);
  }
}

// Remove lesson from schedule by course and group
bool Schedule::removeLesson(int courseId, const string& groupId) {
  auto it = find_if(lessons.begin(), lessons.end(),
                    [courseId, groupId](const shared_ptr<Lesson>& lesson) {
                      return lesson->getCourseId() == courseId &&
                             lesson->getGroupId() == groupId;
                    });

  if (it != lessons.end()) {
    lessons.erase(it);
    return true;
  }
  return false;
}

// Check if there's a conflict with new lesson
bool Schedule::hasConflict(const Lesson& newLesson) const {
  for (const auto& lesson : lessons) {
    if (lesson->getDay() == newLesson.getDay()) {
      // Simple time conflict check (this can be improved)
      if (lesson->getStartTime() == newLesson.getStartTime()) {
        return true;
      }
    }
  }
  return false;
}

// Print schedule in enhanced format with rich details
void Schedule::printSchedule() const {
  // Define modern color scheme
  const string RESET = "\033[0m";
  const string BOLD = "\033[1m";
  const string DIM = "\033[2m";

  const string BRIGHT_BLUE = "\033[94m";
  const string BRIGHT_GREEN = "\033[92m";
  const string BRIGHT_YELLOW = "\033[93m";
  const string BRIGHT_RED = "\033[91m";
  const string BRIGHT_MAGENTA = "\033[95m";
  const string BRIGHT_CYAN = "\033[96m";
  const string BRIGHT_WHITE = "\033[97m";
  const string CYAN = "\033[36m";
  const string GREEN = "\033[32m";
  const string YELLOW = "\033[33m";
  const string RED = "\033[31m";
  const string WHITE = "\033[37m";
  const string GRAY = "\033[90m";

  // Background colors for course blocks
  const string BG_BLUE = "\033[44m";
  const string BG_GREEN = "\033[42m";
  const string BG_RED = "\033[41m";
  const string BG_YELLOW = "\033[43m";
  const string BG_MAGENTA = "\033[45m";
  const string BG_CYAN = "\033[46m";

  cout << "\n"
       << CYAN << BOLD
       << "===================================================================="
          "===========\n"
          "                              SCHEDULE "
       << scheduleId
       << "\n"
          "===================================================================="
          "==========="
       << RESET << endl;

  if (lessons.empty()) {
    cout << BRIGHT_YELLOW
         << "\n                        No lessons in this schedule.\n"
         << RESET << endl;
    cout << BRIGHT_CYAN
         << "  [TIP] Use 'Add <schedule_id> <course_id> <group_id>' to add "
            "lessons"
         << RESET << endl;
    cout << CYAN
         << "=================================================================="
            "============="
         << RESET << endl;
    return;
  }

  // Calculate statistics
  int totalCredits = getTotalCredits();
  int totalLessons = static_cast<int>(lessons.size());
  int totalWeeklyHours = 0;
  map<string, int> lessonTypeCounts;

  for (const auto& lesson : lessons) {
    totalWeeklyHours += lesson->getDuration();
    lessonTypeCounts[lesson->getLessonType()]++;
  }

  int weeklyHours = totalWeeklyHours / 60;
  int weeklyMins = totalWeeklyHours % 60;

  // Display statistics
  cout << BRIGHT_GREEN << "Total Credits: " << BOLD << totalCredits << RESET
       << " | " << BRIGHT_BLUE << "Weekly Load: " << weeklyHours << "h "
       << weeklyMins << "m" << RESET << " | " << BRIGHT_MAGENTA
       << "Total Lessons: " << totalLessons << RESET << endl;

  cout << GREEN << "Lectures: " << lessonTypeCounts["Lecture"] << RESET << " | "
       << YELLOW << "Tutorials: " << lessonTypeCounts["Tutorial"] << RESET
       << " | " << RED << "Labs: " << lessonTypeCounts["Lab"] << RESET << endl;

  cout << CYAN
       << "===================================================================="
          "==========="
       << RESET << endl;

  // Create time grid
  vector<string> days = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                         "Thursday", "Friday", "Saturday"};

  // Find time range (dynamic based on actual lesson times)
  int startHour = 24, endHour = 0;
  for (const auto& lesson : lessons) {
    int lessonStart = stoi(lesson->getStartTime().substr(0, 2));
    int duration = lesson->getDuration();
    int lessonEnd =
        lessonStart + (duration / 60) + ((duration % 60) > 0 ? 1 : 0);

    if (lessonStart < startHour) startHour = lessonStart;
    if (lessonEnd > endHour) endHour = lessonEnd;
  }

  // Expand range slightly for better visibility
  startHour = max(7, startHour - 1);
  endHour = min(23, endHour + 1);

  // Create grid: [hour][day] = lessons in that slot
  map<int, map<string, vector<shared_ptr<Lesson>>>> grid;

  // Fill the grid
  for (const auto& lesson : lessons) {
    int lessonStartHour = stoi(lesson->getStartTime().substr(0, 2));
    int lessonStartMin = stoi(lesson->getStartTime().substr(3, 2));
    int duration = lesson->getDuration();

    // Calculate how many hour slots this lesson spans
    int slotsNeeded = (duration / 60) + ((duration % 60) > 0 ? 1 : 0);

    // Add lesson to appropriate time slots
    for (int i = 0; i < slotsNeeded; i++) {
      int currentHour = lessonStartHour + i;
      if (currentHour >= startHour && currentHour <= endHour) {
        grid[currentHour][lesson->getDay()].push_back(lesson);
      }
    }
  }

  // Course color mapping - assign unique colors to each course
  map<int, string> courseColors;
  vector<string> bgColors = {BG_BLUE, BG_GREEN,  BG_MAGENTA,
                             BG_CYAN, BG_YELLOW, BG_RED};
  int colorIndex = 0;

  set<int> uniqueCourses;
  for (const auto& lesson : lessons) {
    uniqueCourses.insert(lesson->getCourseId());
  }

  for (int courseId : uniqueCourses) {
    courseColors[courseId] = bgColors[colorIndex % bgColors.size()];
    colorIndex++;
  }

  // Print timetable header
  cout << "\n" << BRIGHT_WHITE << BOLD;
  cout << setw(7) << "Time" << " |";
  for (const string& day : days) {
    cout << setw(13) << day.substr(0, 9) << setw(10) << " |";
  }
  cout << RESET << endl;

  // Print separator
  cout << GRAY << string(8, '=') << "+";
  for (size_t i = 0; i < days.size(); i++) {
    cout << string(22, '=') << "+";
  }
  cout << RESET << endl;

  // Print grid rows with multi-line lesson details
  for (int hour = startHour; hour <= endHour; hour++) {
    // For each hour slot, we'll print multiple lines to show all details
    vector<vector<vector<string>>> dayLines(
        days.size());  // Each day can have multiple lines, each line can wrap
    int maxLines = 1;  // Track maximum lines needed for this hour slot

    // Prepare the content for each day
    for (size_t dayIndex = 0; dayIndex < days.size(); dayIndex++) {
      const string& day = days[dayIndex];

      if (grid[hour][day].empty()) {
        // Empty slot - just one empty line
        dayLines[dayIndex].push_back(vector<string>{""});
        dayLines[dayIndex].push_back(vector<string>{""});
        dayLines[dayIndex].push_back(vector<string>{""});
      } else if (grid[hour][day].size() == 1) {
        // Single lesson - show all details with word wrapping
        auto lesson = grid[hour][day][0];
        string courseColor = courseColors[lesson->getCourseId()];

        // Line 1: Course ID and Name with word wrapping
        string courseId = to_string(lesson->getCourseId());
        string courseName = Schedule::getCourseNameById(lesson->getCourseId());
        string fullLine1 = courseId + " " + courseName;

        // Word wrap function for line 1
        vector<string> wrappedLine1;
        if (fullLine1.length() <= 20) {
          wrappedLine1.push_back(fullLine1);
        } else {
          // Try to break at word boundaries
          string remaining = fullLine1;
          while (remaining.length() > 20) {
            size_t breakPoint = remaining.rfind(' ', 20);
            if (breakPoint == string::npos || breakPoint < 5) {
              // No good break point, just cut at 20 chars
              breakPoint = 20;
            }
            wrappedLine1.push_back(remaining.substr(0, breakPoint));
            remaining = remaining.substr(breakPoint);
            if (remaining[0] == ' ')
              remaining = remaining.substr(1);  // Remove leading space
          }
          if (!remaining.empty()) {
            wrappedLine1.push_back(remaining);
          }
        }

        // Line 2: Type, Group and Room
        string lessonType =
            lesson->getLessonType().substr(0, 3);  // Lec/Tut/Lab
        string groupId = lesson->getGroupId();
        string room = lesson->getClassroom();
        string fullLine2 = lessonType + " G:" + groupId + " " + room;

        vector<string> wrappedLine2;
        if (fullLine2.length() <= 20) {
          wrappedLine2.push_back(fullLine2);
        } else {
          // Break at logical points
          string part1 = lessonType + " G:" + groupId;
          string part2 = room;
          if (part1.length() <= 20) {
            wrappedLine2.push_back(part1);
            wrappedLine2.push_back(part2);
          } else {
            wrappedLine2.push_back(fullLine2.substr(0, 20));
            if (fullLine2.length() > 20) {
              wrappedLine2.push_back(fullLine2.substr(20));
            }
          }
        }

        // Line 3: Teacher and Building
        string teacher = lesson->getTeacher();
        string building = lesson->getBuilding();
        string fullLine3 = teacher + " (" + building + ")";

        vector<string> wrappedLine3;
        if (fullLine3.length() <= 20) {
          wrappedLine3.push_back(fullLine3);
        } else {
          // Try to break at teacher name boundary
          if (teacher.length() <= 15) {
            wrappedLine3.push_back(teacher);
            wrappedLine3.push_back("(" + building + ")");
          } else {
            // Break teacher name
            size_t breakPoint = teacher.rfind(' ', 15);
            if (breakPoint != string::npos && breakPoint > 3) {
              wrappedLine3.push_back(teacher.substr(0, breakPoint));
              wrappedLine3.push_back(teacher.substr(breakPoint + 1) + " (" +
                                     building + ")");
            } else {
              wrappedLine3.push_back(teacher.substr(0, 15));
              string remainder = teacher.substr(15) + " (" + building + ")";
              if (remainder.length() > 20) {
                wrappedLine3.push_back(remainder.substr(0, 20));
                if (remainder.length() > 20) {
                  wrappedLine3.push_back(remainder.substr(20));
                }
              } else {
                wrappedLine3.push_back(remainder);
              }
            }
          }
        }

        dayLines[dayIndex].push_back(wrappedLine1);
        dayLines[dayIndex].push_back(wrappedLine2);
        dayLines[dayIndex].push_back(wrappedLine3);

        // Update max lines for this hour slot
        int totalLines = static_cast<int>(
            wrappedLine1.size() + wrappedLine2.size() + wrappedLine3.size());
        maxLines = max(maxLines, totalLines);

      } else {
        // Multiple lessons (conflict)
        vector<string> line1 = {
            "CONFLICT (" + to_string(grid[hour][day].size()) + " lessons)"};

        string courses = "";
        for (size_t i = 0; i < grid[hour][day].size() && i < 3; i++) {
          if (i > 0) courses += ", ";
          courses += to_string(grid[hour][day][i]->getCourseId());
        }
        if (grid[hour][day].size() > 3) courses += ", more";

        vector<string> line2;
        if (courses.length() <= 20) {
          line2.push_back(courses);
        } else {
          // Word wrap the course list
          string remaining = courses;
          while (remaining.length() > 20) {
            size_t breakPoint = remaining.rfind(',', 20);
            if (breakPoint == string::npos) breakPoint = 20;
            line2.push_back(remaining.substr(0, breakPoint));
            remaining = remaining.substr(breakPoint);
            if (remaining[0] == ',' || remaining[0] == ' ')
              remaining = remaining.substr(1);
          }
          if (!remaining.empty()) {
            line2.push_back(remaining);
          }
        }

        vector<string> line3 = {"Check schedule!"};

        dayLines[dayIndex].push_back(line1);
        dayLines[dayIndex].push_back(line2);
        dayLines[dayIndex].push_back(line3);

        int totalLines =
            static_cast<int>(line1.size() + line2.size() + line3.size());
        maxLines = max(maxLines, totalLines);
      }
    }

    // Flatten all wrapped lines into a single array per day
    vector<vector<string>> flatDayLines(days.size());
    for (size_t dayIndex = 0; dayIndex < days.size(); dayIndex++) {
      for (const auto& section : dayLines[dayIndex]) {
        for (const string& line : section) {
          flatDayLines[dayIndex].push_back(line);
        }
      }
      // Pad with empty lines to match maxLines
      while (flatDayLines[dayIndex].size() < (size_t)maxLines) {
        flatDayLines[dayIndex].push_back("");
      }
    }

    // Print the time slot with dynamic number of lines
    for (int lineIndex = 0; lineIndex < maxLines; lineIndex++) {
      // Time column (only show time on first line)
      if (lineIndex == 0) {
        string timeStr = (hour < 10 ? "0" : "") + to_string(hour) + ":00";
        cout << BRIGHT_CYAN << BOLD << setw(7) << timeStr << RESET << " |";
      } else {
        cout << setw(7) << " " << " |";
      }

      // Day columns
      for (size_t dayIndex = 0; dayIndex < days.size(); dayIndex++) {
        const string& day = days[dayIndex];
        string content = (lineIndex < flatDayLines[dayIndex].size())
                             ? flatDayLines[dayIndex][lineIndex]
                             : "";

        if (grid[hour][day].empty()) {
          // Empty slot
          cout << setw(22) << " " << "|";
        } else if (grid[hour][day].size() == 1) {
          // Single lesson with color
          auto lesson = grid[hour][day][0];
          string courseColor = courseColors[lesson->getCourseId()];
          cout << courseColor << WHITE << BOLD << " " << setw(20) << left
               << content << " " << RESET << "|";
        } else {
          // Conflict
          cout << BG_RED << WHITE << BOLD << " " << setw(20) << left << content
               << " " << RESET << "|";
        }
      }
      cout << endl;
    }

    // Add separator after each time slot
    cout << GRAY << string(8, '-') << "+";
    for (size_t i = 0; i < days.size(); i++) {
      cout << string(22, '-') << "+";
    }
    cout << RESET << endl;

    // Add thicker separator every 3 hours
    if ((hour - startHour + 1) % 3 == 0 && hour != endHour) {
      cout << GRAY << string(8, '=') << "+";
      for (size_t i = 0; i < days.size(); i++) {
        cout << string(22, '=') << "+";
      }
      cout << RESET << endl;
    }
  }

  // Legend
  cout << "\n" << BRIGHT_WHITE << BOLD << "LEGEND:" << RESET << endl;
  cout << BRIGHT_CYAN << "Each lesson box shows:" << RESET << endl;
  cout << "  Line 1: Course ID + Course Name" << endl;
  cout << "  Line 2: Type + Group + Room" << endl;
  cout << "  Line 3: Instructor + Building" << endl;

  cout << "\n" << BRIGHT_WHITE << BOLD << "COURSE COLORS:" << RESET << endl;
  set<int> displayedCourses;
  int legendCount = 0;
  for (const auto& lesson : lessons) {
    if (displayedCourses.find(lesson->getCourseId()) ==
        displayedCourses.end()) {
      string courseColor = courseColors[lesson->getCourseId()];
      string courseName = Schedule::getCourseNameById(lesson->getCourseId());

      // Format the legend entry nicely
      cout << courseColor << WHITE << BOLD << " " << setw(6) << left
           << lesson->getCourseId() << " " << RESET;
      cout << courseName.substr(0, 20);  // Truncate long names

      displayedCourses.insert(lesson->getCourseId());
      legendCount++;

      if (legendCount % 2 == 0) {
        cout << endl;
      } else {
        cout << "   ";  // Add spacing between columns
      }
    }
  }
  if (legendCount % 2 != 0) cout << endl;

  cout << "\n"
       << BG_RED << WHITE << BOLD << " CONFLICT " << RESET
       << " = Multiple courses scheduled at same time" << endl;

  cout << CYAN
       << "===================================================================="
          "==========="
       << RESET << endl;
}

// Get lessons for specific day
vector<shared_ptr<Lesson>> Schedule::getLessonsForDay(const string& day) const {
  vector<shared_ptr<Lesson>> dayLessons;
  for (const auto& lesson : lessons) {
    if (lesson->getDay() == day) {
      dayLessons.push_back(lesson);
    }
  }
  return dayLessons;
}

// Clear all lessons
void Schedule::clearSchedule() { lessons.clear(); }

// Get number of lessons
int Schedule::getLessonCount() const {
  return static_cast<int>(lessons.size());
}

// Save schedule to CSV
void Schedule::saveToCSV(const string& filename) const {
  ofstream file(filename);
  if (file.is_open()) {
    file << "ScheduleID,LessonType,CourseID,Day,StartTime,Duration,Classroom,"
            "Building,Teacher,GroupID"
         << endl;
    for (const auto& lesson : lessons) {
      file << scheduleId << "," << lesson->getLessonType() << ","
           << lesson->getCourseId() << "," << lesson->getDay() << ","
           << lesson->getStartTime() << "," << lesson->getDuration() << ","
           << lesson->getClassroom() << "," << lesson->getBuilding() << ","
           << lesson->getTeacher() << "," << lesson->getGroupId() << endl;
    }
    file.close();
  }
}

// Load schedule from CSV
void Schedule::loadFromCSV(const string& filename) {
  ifstream file(filename);
  if (file.is_open()) {
    string line;
    getline(file, line);  // Skip header

    while (getline(file, line)) {
      if (line.empty()) continue;

      stringstream ss(line);
      string item;
      vector<string> tokens;

      while (getline(ss, item, ',')) {
        tokens.push_back(item);
      }

      if (tokens.size() >= 10) {
        try {
          // Parse with correct field order:
          // ScheduleID,LessonType,CourseID,Day,StartTime,Duration,Classroom,Building,Teacher,GroupID
          int schedId = stoi(tokens[0]);   // ScheduleID
          string lessonType = tokens[1];   // LessonType
          int courseId = stoi(tokens[2]);  // CourseID
          string day = tokens[3];          // Day
          string startTime = tokens[4];    // StartTime
          int duration = stoi(tokens[5]);  // Duration
          string classroom = tokens[6];    // Classroom
          string building = tokens[7];     // Building
          string teacher = tokens[8];      // Teacher
          string groupId = tokens[9];      // GroupID

          // Create appropriate lesson objects based on type
          shared_ptr<Lesson> lesson;
          if (lessonType == "Lecture") {
            lesson =
                make_shared<Lecture>(courseId, day, startTime, duration,
                                     classroom, building, teacher, groupId);
          } else if (lessonType == "Tutorial") {
            lesson =
                make_shared<Tutorial>(courseId, day, startTime, duration,
                                      classroom, building, teacher, groupId);
          } else if (lessonType == "Lab") {
            lesson = make_shared<Lab>(courseId, day, startTime, duration,
                                      classroom, building, teacher, groupId);
          }

          if (lesson && schedId == scheduleId) {
            lessons.push_back(lesson);
          }
        } catch (const exception&) {
          // Skip malformed lines
          continue;
        }
      }
    }
    file.close();
  }
}

// Get total credits for the schedule
int Schedule::getTotalCredits() const {
  set<int> uniqueCourses;
  for (const auto& lesson : lessons) {
    uniqueCourses.insert(lesson->getCourseId());
  }
  // For now, assume each course is 3 credits (this would need to be calculated
  // properly with course data)
  return static_cast<int>(uniqueCourses.size() * 3);
}

// Static methods for course database
void Schedule::setCourseDatabase(const vector<Course>& courses) {
  courseDatabase.clear();
  for (const auto& course : courses) {
    courseDatabase[course.getCourseId()] = course;
  }
}

string Schedule::getCourseNameById(int courseId) {
  auto it = courseDatabase.find(courseId);
  if (it != courseDatabase.end()) {
    return it->second.getName();
  }
  return "Course " + to_string(courseId);  // Fallback if not found
}
