#include "AdvancedScheduleAnalytics.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <sstream>
#include <thread>

// Modern color definitions for professional output
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string DIM = "\033[2m";

// Text colors
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

// Background colors for highlights
const string BG_BLUE = "\033[44m";
const string BG_GREEN = "\033[42m";
const string BG_RED = "\033[41m";
const string BG_YELLOW = "\033[43m";
const string BG_MAGENTA = "\033[45m";
const string BG_CYAN = "\033[46m";

/**
 * @brief Minimal constructor - the 3 main functions are in separate files
 */
AdvancedScheduleAnalytics::AdvancedScheduleAnalytics()
    : courses_ptr(nullptr), schedules_ptr(nullptr), courseLessons_ptr(nullptr) {
  // Basic initialization only
  cout << BRIGHT_GREEN << "[SYSTEM] Advanced Analytics Engine ready" << RESET
       << endl;
}

/**
 * @brief Constructor with data references
 */
AdvancedScheduleAnalytics::AdvancedScheduleAnalytics(
    const vector<Course>& courses, const vector<Schedule>& schedules,
    const map<int, vector<shared_ptr<Lesson>>>& courseLessons)
    : courses_ptr(&courses),
      schedules_ptr(&schedules),
      courseLessons_ptr(&courseLessons) {
  cout << BRIGHT_GREEN
       << "[SYSTEM] Advanced Analytics Engine initialized with data" << RESET
       << endl;
}

/**
 * @brief Set data references for analytics operations
 */
void AdvancedScheduleAnalytics::setDataReferences(
    const vector<Course>& courses, const vector<Schedule>& schedules,
    const map<int, vector<shared_ptr<Lesson>>>& courseLessons,
    AddLessonFunction addCallback, RemoveLessonFunction removeCallback,
    CreateScheduleFunction createCallback,
    ReloadSystemDataFunction reloadCallback) {
  courses_ptr = &courses;
  schedules_ptr = &schedules;
  courseLessons_ptr = &courseLessons;
  addLessonCallback = addCallback;
  removeLessonCallback = removeCallback;
  createScheduleCallback = createCallback;
  reloadSystemData = reloadCallback;

  // Build course cache for faster lookups
  courseCache.clear();
  for (const Course& course : courses) {
    courseCache[course.getCourseId()] = course;
  }
}

/**
 * @brief Destructor - Clean up async tasks
 */
AdvancedScheduleAnalytics::~AdvancedScheduleAnalytics() {
  // Wait for any pending async tasks to complete
  for (auto& task : asyncTasks) {
    if (task.valid()) {
      task.wait();
    }
  }
}

vector<string> AdvancedScheduleAnalytics::parseCSVLine(
    const string& line) const {
  vector<string> result;
  stringstream ss(line);
  string field;
  bool inQuotes = false;
  string currentField;

  for (char c : line) {
    if (c == '"') {
      inQuotes = !inQuotes;
    } else if (c == ',' && !inQuotes) {
      result.push_back(currentField);
      currentField.clear();
    } else {
      currentField += c;
    }
  }
  result.push_back(currentField);

  return result;
}

void AdvancedScheduleAnalytics::displayProgressAnimation(
    const string& operation, int steps) const {
  cout << BRIGHT_BLUE << operation << RESET << " ";
  for (int i = 0; i < steps; i++) {
    cout << BRIGHT_GREEN << "#" << RESET;
    cout.flush();
    this_thread::sleep_for(chrono::milliseconds(120));
  }
  cout << BRIGHT_GREEN << BOLD << " COMPLETE!" << RESET << endl;
}

// =================== UTILITY METHODS FOR SEPARATED FILES ===================

int AdvancedScheduleAnalytics::timeStringToMinutes(
    const string& timeStr) const {
  if (timeStr.empty()) return 0;

  stringstream ss(timeStr);
  string hourStr, minuteStr;

  if (getline(ss, hourStr, ':') && getline(ss, minuteStr)) {
    try {
      int hours = stoi(hourStr);
      int minutes = stoi(minuteStr);
      return hours * 60 + minutes;
    } catch (const exception&) {
      return 0;
    }
  }

  return 0;
}

string AdvancedScheduleAnalytics::minutesToTimeString(int minutes) const {
  int hours = minutes / 60;
  int mins = minutes % 60;
  return (hours < 10 ? "0" : "") + to_string(hours) + ":" +
         (mins < 10 ? "0" : "") + to_string(mins);
}

void AdvancedScheduleAnalytics::TimeSlotMatrix::setTimeSlot(int day, int hour,
                                                            int minute,
                                                            int duration) {
  if (day < 0 || day >= DAYS_PER_WEEK) return;

  int startSlot = (hour * 60 + minute) / 15;  // 15-minute slots
  int endSlot = startSlot + (duration / 15);

  for (int slot = startSlot; slot < endSlot && slot < 64; slot++) {
    timeMatrix[day] |= (1ULL << slot);
  }
}

bool AdvancedScheduleAnalytics::TimeSlotMatrix::hasConflict(
    int day, int hour, int minute, int duration) const {
  if (day < 0 || day >= DAYS_PER_WEEK) return false;

  int startSlot = (hour * 60 + minute) / 15;
  int endSlot = startSlot + (duration / 15);

  for (int slot = startSlot; slot < endSlot && slot < 64; slot++) {
    if (timeMatrix[day] & (1ULL << slot)) {
      return true;
    }
  }

  return false;
}

double AdvancedScheduleAnalytics::TimeSlotMatrix::getUtilizationRatio() const {
  int totalSlots = 0;
  int usedSlots = 0;

  for (int day = 0; day < DAYS_PER_WEEK; day++) {
    totalSlots += 64;  // 64 slots per day (16 hours * 4 slots per hour)

    // Count set bits manually (cross-platform solution)
    uint64_t bits = timeMatrix[day];
    int count = 0;
    while (bits) {
      count += bits & 1;
      bits >>= 1;
    }
    usedSlots += count;
  }

  return totalSlots > 0 ? static_cast<double>(usedSlots) / totalSlots : 0.0;
}

vector<shared_ptr<Lesson>> AdvancedScheduleAnalytics::loadLessonsForCourse(
    int courseId, const string& lessonType) const {
  vector<shared_ptr<Lesson>> lessons;

  if (!courseLessons_ptr) return lessons;

  auto courseIt = courseLessons_ptr->find(courseId);
  if (courseIt != courseLessons_ptr->end()) {
    for (const auto& lesson : courseIt->second) {
      if (lesson &&
          (lessonType.empty() || lesson->getLessonType() == lessonType)) {
        lessons.push_back(lesson);
      }
    }
  }

  return lessons;
}

bool AdvancedScheduleAnalytics::tryAddCompleteCourse(int scheduleId,
                                                     int courseId,
                                                     bool quietMode) const {
  if (!addLessonCallback) {
    if (!quietMode) {
      cout << BRIGHT_RED << "No add lesson callback available" << RESET << endl;
    }
    return false;
  }

  // Try to add different lesson types with their group IDs
  vector<string> lessonTypes = {"lectures", "tutorials", "labs"};
  int successCount = 0;

  for (const string& lessonType : lessonTypes) {
    vector<shared_ptr<Lesson>> lessons =
        loadLessonsForCourse(courseId, lessonType);
    if (!lessons.empty()) {
      bool typeAdded = false;

      // Try different group IDs for this lesson type
      vector<string> groupIds;
      if (lessonType == "lectures") {
        groupIds = {"L1", "L2", "L3"};
      } else if (lessonType == "tutorials") {
        groupIds = {"T1", "T2", "T3"};
      } else if (lessonType == "labs") {
        groupIds = {"LB1", "LB2", "LB3"};
      }

      // Try each group ID until one works
      for (const string& groupId : groupIds) {
        bool groupAdded = addLessonCallback(scheduleId, courseId, groupId);
        if (groupAdded) {
          typeAdded = true;
          successCount++;
          break;  // Successfully added this lesson type, move to next type
        }
      }
    }
  }

  return successCount > 0;
}

bool AdvancedScheduleAnalytics::tryAddCompleteCourseBulletproof(
    int scheduleId, int courseId, bool quietMode) const {
  if (!addLessonCallback) {
    if (!quietMode) {
      cout << BRIGHT_RED << "No add lesson callback available" << RESET << endl;
    }
    return false;
  }

  // Try to add different lesson types with their group IDs
  vector<pair<string, string>> lessonGroups = {
      {"lectures", "L1"},  {"lectures", "L2"},  {"lectures", "L3"},
      {"tutorials", "T1"}, {"tutorials", "T2"}, {"tutorials", "T3"},
      {"labs", "LB1"},     {"labs", "LB2"},     {"labs", "LB3"}};

  int successCount = 0;
  int totalAttempts = 0;

  // Try each lesson type (lectures, tutorials, labs)
  vector<string> lessonTypes = {"lectures", "tutorials", "labs"};

  for (const string& lessonType : lessonTypes) {
    vector<shared_ptr<Lesson>> lessons =
        loadLessonsForCourse(courseId, lessonType);
    if (!lessons.empty()) {
      totalAttempts++;
      bool typeAdded = false;

      // Try different group IDs for this lesson type
      vector<string> groupIds;
      if (lessonType == "lectures") {
        groupIds = {"L1", "L2", "L3"};
      } else if (lessonType == "tutorials") {
        groupIds = {"T1", "T2", "T3"};
      } else if (lessonType == "labs") {
        groupIds = {"LB1", "LB2", "LB3"};
      }

      // Try each group ID until one works
      for (const string& groupId : groupIds) {
        bool groupAdded = addLessonCallback(scheduleId, courseId, groupId);
        if (groupAdded) {
          typeAdded = true;
          if (!quietMode) {
            cout << BRIGHT_GREEN << "  Added " << lessonType << " group "
                 << groupId << " for course " << courseId << RESET << endl;
          }
          break;  // Successfully added this lesson type, move to next type
        }
      }

      if (typeAdded) {
        successCount++;
      } else if (!quietMode) {
        cout << BRIGHT_YELLOW << "  Conflict detected for all " << lessonType
             << " groups of course " << courseId << RESET << endl;
      }
    }
  }

  // Return true if we successfully added at least some lessons
  bool result = (successCount > 0 && successCount == totalAttempts);

  if (!quietMode) {
    if (result) {
      cout << BRIGHT_GREEN << "Successfully added course " << courseId << " ("
           << successCount << "/" << totalAttempts << " lesson types)" << RESET
           << endl;
    } else if (successCount > 0) {
      cout << BRIGHT_YELLOW << "Partially added course " << courseId << " ("
           << successCount << "/" << totalAttempts << " lesson types)" << RESET
           << endl;
    } else {
      cout << BRIGHT_RED << "Failed to add course " << courseId
           << " - all lesson types have conflicts" << RESET << endl;
    }
  }

  // Return true if we added at least one lesson type successfully
  return successCount > 0;
}

bool AdvancedScheduleAnalytics::tryDirectLessonAddition(int scheduleId,
                                                        int courseId) const {
  // Direct CSV file approach when callbacks fail
  // This is a backup method that directly reads lesson files and writes to
  // schedule

  vector<string> lessonTypes = {"lectures", "tutorials", "labs"};
  int addedLessons = 0;

  string scheduleFilename = "data/schedule_" + to_string(scheduleId) + ".csv";

  for (const string& lessonType : lessonTypes) {
    string lessonFilename =
        "data/" + to_string(courseId) + "_" + lessonType + ".csv";

    ifstream lessonFile(lessonFilename);
    if (!lessonFile.is_open()) {
      continue;  // Skip if lesson file doesn't exist
    }

    // Read the lesson file
    string line;
    getline(lessonFile, line);  // Skip header

    bool foundLesson = false;
    while (getline(lessonFile, line) && !foundLesson) {
      if (line.empty()) continue;

      vector<string> fields = parseCSVLine(line);
      if (fields.size() >= 8) {
        // Format:
        // courseId,day,startTime,duration,classroom,building,teacher,groupId
        string day = fields[1];
        string startTime = fields[2];
        string duration = fields[3];
        string classroom = fields[4];
        string building = fields[5];
        string teacher = fields[6];
        string groupId = fields[7];

        // Create lesson type name
        string lessonTypeName =
            lessonType.substr(0, lessonType.length() - 1);  // Remove 's'
        if (lessonTypeName == "lecture")
          lessonTypeName = "lectures";
        else if (lessonTypeName == "tutorial")
          lessonTypeName = "tutorials";
        else if (lessonTypeName == "lab")
          lessonTypeName = "labs";

        // Write directly to schedule file
        ofstream scheduleFile(scheduleFilename, ios::app);
        if (scheduleFile.is_open()) {
          scheduleFile << scheduleId << "," << lessonTypeName << "," << courseId
                       << "," << day << "," << startTime << "," << duration
                       << "," << classroom << "," << building << "," << teacher
                       << "," << groupId << endl;
          scheduleFile.close();
          addedLessons++;
          foundLesson = true;
        }
      }
    }
    lessonFile.close();
  }

  return addedLessons > 0;
}

// NOTE: The three main functions are implemented in separate files:
// - intelligentConflictResolution() -> IntelligentConflictResolution.cpp
// - aiOptimalScheduleGeneration() -> AiOptimalScheduleGeneration.cpp
// - advancedAcademicAnalytics() -> AdvancedAcademicAnalytics.cpp
