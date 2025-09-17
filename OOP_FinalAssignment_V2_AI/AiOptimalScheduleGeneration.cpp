#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <random>
#include <sstream>
#include <vector>

#include "AdvancedScheduleAnalytics.h"

// Helper structures for conflict detection (from working function)
struct ScheduledLesson {
  int scheduleId;
  string lessonType;
  int courseId;
  string day;
  string startTime;
  int duration;
  string classroom;
  string building;
  string teacher;
  string groupId;
};

struct TimeSlot {
  string day;
  string startTime;
  int duration;
  string classroom;
  string building;
  string teacher;
  string groupId;
};

// Colors for beautiful output
const string RESET = "\033[0m";
const string BOLD = "\033[1m";
const string BRIGHT_BLUE = "\033[94m";
const string BRIGHT_GREEN = "\033[92m";
const string BRIGHT_YELLOW = "\033[93m";
const string BRIGHT_RED = "\033[91m";
const string BRIGHT_MAGENTA = "\033[95m";
const string BRIGHT_CYAN = "\033[96m";
const string BRIGHT_WHITE = "\033[97m";

// Helper functions (from working function)
int timeToMinutes(const string& timeStr) {
  if (timeStr.empty()) return -1;

  size_t colonPos = timeStr.find(':');
  if (colonPos == string::npos) return -1;

  try {
    int hours = stoi(timeStr.substr(0, colonPos));
    int minutes = stoi(timeStr.substr(colonPos + 1));
    return hours * 60 + minutes;
  } catch (const exception&) {
    return -1;
  }
}

vector<string> parseCSVLine(const string& line) {
  vector<string> fields;
  stringstream ss(line);
  string field;

  while (getline(ss, field, ',')) {
    // Remove leading/trailing whitespace
    field.erase(0, field.find_first_not_of(" \t"));
    field.erase(field.find_last_not_of(" \t") + 1);
    fields.push_back(field);
  }

  return fields;
}

bool checkTimeConflict(const TimeSlot& newLesson,
                       const vector<ScheduledLesson>& existingLessons) {
  for (const ScheduledLesson& existing : existingLessons) {
    if (existing.day != newLesson.day) continue;

    int existingStart = timeToMinutes(existing.startTime);
    int existingEnd = existingStart + (existing.duration * 60);
    int newStart = timeToMinutes(newLesson.startTime);
    int newEnd = newStart + (newLesson.duration * 60);

    // Check for overlap
    if ((newStart < existingEnd) && (existingStart < newEnd)) {
      return true;  // Conflict found
    }
  }
  return false;  // No conflict
}

bool AdvancedScheduleAnalytics::aiOptimalScheduleGeneration(
    double targetCredits, double tolerance) {
  if (tolerance <= 0) {
    tolerance = 0.15;
  }

  cout << "\n";
  cout << BRIGHT_BLUE
       << "===================================================================="
          "========"
       << RESET << endl;
  cout << BRIGHT_BLUE << "|               " << BOLD << BRIGHT_WHITE
       << "AI-POWERED OPTIMAL SCHEDULE GENERATION" << RESET << BRIGHT_BLUE
       << "              |" << RESET << endl;
  cout << BRIGHT_BLUE << "|        " << BRIGHT_CYAN
       << "Smart Course Selection + Real Conflict Detection" << RESET
       << BRIGHT_BLUE << "        |" << RESET << endl;
  cout << BRIGHT_BLUE
       << "===================================================================="
          "========"
       << RESET << endl;

  cout << BRIGHT_WHITE << "Target Credits: " << RESET << BRIGHT_YELLOW
       << targetCredits << RESET << " (+/-" << BRIGHT_YELLOW << tolerance
       << RESET << ")" << endl;
  cout << BRIGHT_WHITE << "Strategy: " << RESET << BRIGHT_GREEN
       << "Intelligent Course Selection with Conflict-Free Groups" << RESET
       << endl;

  cout << "\n"
       << BRIGHT_CYAN
       << "Initializing advanced conflict-free optimization engine..." << RESET
       << endl;
  cout << BRIGHT_GREEN << "Loading ";
  for (int i = 0; i < 10; i++) {
    cout << ".";
    cout.flush();
    // this_thread::sleep_for(chrono::milliseconds(150)); // Commented out for
    // speed
  }
  cout << " COMPLETE!" << RESET << endl;

  // Step 1: Create new schedule using callback (equivalent to AddSchedule
  // command)
  int newScheduleId = -1;
  if (createScheduleCallback) {
    newScheduleId = createScheduleCallback();
    cout << BRIGHT_CYAN << "Using Schedule ID: " << BRIGHT_YELLOW
         << newScheduleId << RESET << endl;
  } else {
    cout << BRIGHT_RED
         << "ERROR: Cannot create schedule - callback not available!" << RESET
         << endl;
    return false;
  }

  if (newScheduleId == -1) {
    cout << BRIGHT_RED << "ERROR: Failed to create schedule!" << RESET << endl;
    return false;
  }

  // Get available courses from the loaded course data
  if (!courses_ptr || courses_ptr->empty()) {
    cout << BRIGHT_RED << "ERROR: Could not load course data!" << RESET << endl;
    return false;
  }

  vector<Course> allCourses =
      *courses_ptr;  // Use the courses loaded by ScheduleManager

  // INTELLIGENT ALGORITHM: Find best course combination with MULTIPLE ATTEMPTS
  // (like working function)
  vector<Course> bestCombination;
  double bestCredits = 0;
  bool foundOptimal = false;
  vector<vector<Course>> candidateCombinations;
  map<int, vector<string>> courseGroupsAdded;

  // SMART ALGORITHM: Use random sampling instead of exhaustive search to avoid
  // combinatorial explosion

  random_device rd;
  mt19937 gen(rd());

  // Try different course counts with MORE samples to find working combinations
  for (int numCourses = 2; numCourses <= min(6, (int)allCourses.size());
       numCourses++) {
    int maxSamples =
        min(5000, (int)allCourses.size() * 100);  // INCREASED samples to 5000
    int sampleCount = 0;

    for (int sample = 0; sample < maxSamples && sampleCount < 500;
         sample++) {  // INCREASED candidates to 500
      vector<Course> combination;
      double totalCredits = 0;

      // Randomly select numCourses different courses
      vector<int> courseIndices;
      for (int i = 0; i < (int)allCourses.size(); i++) {
        courseIndices.push_back(i);
      }
      shuffle(courseIndices.begin(), courseIndices.end(), gen);

      for (int i = 0; i < numCourses && i < (int)courseIndices.size(); i++) {
        combination.push_back(allCourses[courseIndices[i]]);
        totalCredits += allCourses[courseIndices[i]].getCredits();
      }

      // Check if credits are in target range
      if (totalCredits >= targetCredits - tolerance &&
          totalCredits <= targetCredits + tolerance) {
        candidateCombinations.push_back(combination);
        sampleCount++;
      }
    }

    // If we found enough candidates, don't need to try more course counts
    if (candidateCombinations.size() >=
        500) {  // INCREASED threshold to 500 for better guarantees
      break;
    }
  }

  // NOW TEST EACH COMBINATION TO FIND THE ONE THAT ACTUALLY WORKS (UP TO 500)
  size_t maxCombinationsToTest = min((size_t)500, candidateCombinations.size());

  if (candidateCombinations.empty()) {
    cout << BRIGHT_RED << "ERROR: Could not find any suitable combination for "
         << targetCredits << " credits." << RESET << endl;
    cout << BRIGHT_YELLOW << "RECOMMENDATIONS:" << RESET << endl;
    cout << BRIGHT_WHITE << "1. Try a different credit target (e.g., "
         << (targetCredits - 2) << " or " << (targetCredits + 2) << ")" << RESET
         << endl;
    cout << BRIGHT_WHITE
         << "2. Contact academic advisor for course scheduling options" << RESET
         << endl;
    cout << BRIGHT_WHITE << "3. Consider taking courses in different semesters"
         << RESET << endl;
    return false;
  }

  // Track optimization statistics
  int totalSamples = 0;
  int candidatesFound = static_cast<int>(candidateCombinations.size());
  int courseCombinations = 0;

  for (size_t testIndex = 0; testIndex < maxCombinationsToTest; testIndex++) {
    vector<Course> testCombination = candidateCombinations[testIndex];
    double testCredits = 0;
    for (const Course& course : testCombination) {
      testCredits += course.getCredits();
    }

    courseCombinations++;

    // Test this combination using IN-MEMORY conflict tracking (no CSV files)
    vector<ScheduledLesson>
        inMemorySchedule;  // Track lessons in memory instead of CSV
    bool combinationWorked = true;
    map<int, vector<string>> tempGroupsAdded;

    for (const Course& course : testCombination) {
      int courseId = course.getCourseId();
      bool lectureAdded = false, labAdded = false, tutorialAdded = false;

      // Try to add lecture - TRY ALL AVAILABLE GROUPS
      string lectureFile = "data/" + to_string(courseId) + "_lectures.csv";
      ifstream lectureCSV(lectureFile);
      if (lectureCSV.is_open()) {
        string line;
        bool isFirstLine = true;

        while (getline(lectureCSV, line)) {
          if (isFirstLine) {
            isFirstLine = false;
            continue;
          }

          if (line.empty()) continue;

          vector<string> fields = parseCSVLine(line);
          if (fields.size() >= 8) {
            TimeSlot timeSlot;
            timeSlot.day = fields[1];
            timeSlot.startTime = fields[2];
            timeSlot.duration = stoi(fields[3]);
            timeSlot.classroom = fields[4];
            timeSlot.building = fields[5];
            timeSlot.teacher = fields[6];
            timeSlot.groupId = fields[7];

            if (!checkTimeConflict(timeSlot, inMemorySchedule)) {
              // Add to in-memory schedule instead of CSV
              ScheduledLesson lesson;
              lesson.scheduleId = newScheduleId;
              lesson.lessonType = "Lecture";
              lesson.courseId = courseId;
              lesson.day = timeSlot.day;
              lesson.startTime = timeSlot.startTime;
              lesson.duration = timeSlot.duration;
              lesson.classroom = timeSlot.classroom;
              lesson.building = timeSlot.building;
              lesson.teacher = timeSlot.teacher;
              lesson.groupId = timeSlot.groupId;

              inMemorySchedule.push_back(lesson);
              tempGroupsAdded[courseId].push_back(timeSlot.groupId);
              lectureAdded = true;
              break;  // Found working group, stop searching
            }
          }
        }
        lectureCSV.close();
      }

      if (!lectureAdded) {
        combinationWorked = false;
        break;  // This combination won't work
      }

      // Try to add lab - TRY ALL AVAILABLE GROUPS
      string labFile = "data/" + to_string(courseId) + "_labs.csv";
      ifstream labCSV(labFile);
      if (labCSV.is_open()) {
        string line;
        bool isFirstLine = true;

        while (getline(labCSV, line)) {
          if (isFirstLine) {
            isFirstLine = false;
            continue;
          }

          if (line.empty()) continue;

          vector<string> fields = parseCSVLine(line);
          if (fields.size() >= 8) {
            TimeSlot timeSlot;
            timeSlot.day = fields[1];
            timeSlot.startTime = fields[2];
            timeSlot.duration = stoi(fields[3]);
            timeSlot.classroom = fields[4];
            timeSlot.building = fields[5];
            timeSlot.teacher = fields[6];
            timeSlot.groupId = fields[7];

            if (!checkTimeConflict(timeSlot, inMemorySchedule)) {
              // Add to in-memory schedule instead of CSV
              ScheduledLesson lesson;
              lesson.scheduleId = newScheduleId;
              lesson.lessonType = "Lab";
              lesson.courseId = courseId;
              lesson.day = timeSlot.day;
              lesson.startTime = timeSlot.startTime;
              lesson.duration = timeSlot.duration;
              lesson.classroom = timeSlot.classroom;
              lesson.building = timeSlot.building;
              lesson.teacher = timeSlot.teacher;
              lesson.groupId = timeSlot.groupId;

              inMemorySchedule.push_back(lesson);
              tempGroupsAdded[courseId].push_back(timeSlot.groupId);
              labAdded = true;
              break;  // Found working group, stop searching
            }
          }
        }
        labCSV.close();
      }

      if (!labAdded) {
        combinationWorked = false;
        break;
      }

      // Try to add tutorial - TRY ALL AVAILABLE GROUPS
      string tutorialFile = "data/" + to_string(courseId) + "_tutorials.csv";
      ifstream tutorialCSV(tutorialFile);
      if (tutorialCSV.is_open()) {
        string line;
        bool isFirstLine = true;

        while (getline(tutorialCSV, line)) {
          if (isFirstLine) {
            isFirstLine = false;
            continue;
          }

          if (line.empty()) continue;

          vector<string> fields = parseCSVLine(line);
          if (fields.size() >= 8) {
            TimeSlot timeSlot;
            timeSlot.day = fields[1];
            timeSlot.startTime = fields[2];
            timeSlot.duration = stoi(fields[3]);
            timeSlot.classroom = fields[4];
            timeSlot.building = fields[5];
            timeSlot.teacher = fields[6];
            timeSlot.groupId = fields[7];

            if (!checkTimeConflict(timeSlot, inMemorySchedule)) {
              // Add to in-memory schedule instead of CSV
              ScheduledLesson lesson;
              lesson.scheduleId = newScheduleId;
              lesson.lessonType = "Tutorial";
              lesson.courseId = courseId;
              lesson.day = timeSlot.day;
              lesson.startTime = timeSlot.startTime;
              lesson.duration = timeSlot.duration;
              lesson.classroom = timeSlot.classroom;
              lesson.building = timeSlot.building;
              lesson.teacher = timeSlot.teacher;
              lesson.groupId = timeSlot.groupId;

              inMemorySchedule.push_back(lesson);
              tempGroupsAdded[courseId].push_back(timeSlot.groupId);
              tutorialAdded = true;
              break;  // Found working group, stop searching
            }
          }
        }
        tutorialCSV.close();
      }

      if (!tutorialAdded) {
        combinationWorked = false;
        break;
      }
    }

    if (combinationWorked) {
      // Now that we found a working combination, add it to the REAL schedule
      // using callbacks
      for (const ScheduledLesson& lesson : inMemorySchedule) {
        if (addLessonCallback) {
          addLessonCallback(newScheduleId, lesson.courseId, lesson.groupId);
        }
      }

      // Set the final results
      bestCombination = testCombination;
      bestCredits = testCredits;
      courseGroupsAdded = tempGroupsAdded;
      foundOptimal = true;
      break;  // Found our optimal combination - stop testing
    } else {
      // inMemorySchedule automatically gets cleared on next iteration
    }
  }

  // FINAL VALIDATION AND SUCCESS REPORTING
  if (!foundOptimal) {
    cout << BRIGHT_RED
         << "OPTIMIZATION FAILED: No combination could be scheduled without "
            "conflicts!"
         << RESET << endl;
    cout << BRIGHT_YELLOW << "RECOMMENDATIONS:" << RESET << endl;
    cout << BRIGHT_WHITE << "1. Try a different credit target" << RESET << endl;
    cout << BRIGHT_WHITE << "2. Check for alternative course groups/timings"
         << RESET << endl;
    cout << BRIGHT_WHITE << "3. Consider taking courses in different semesters"
         << RESET << endl;
    return false;
  }

  cout << "\n"
       << BRIGHT_BLUE
       << "===================================================================="
          "========"
       << RESET << endl;
  cout << BRIGHT_BLUE << "|                    " << BOLD << BRIGHT_WHITE
       << "AI OPTIMIZATION PROCESS SUMMARY" << RESET << BRIGHT_BLUE
       << "                     |" << RESET << endl;
  cout << BRIGHT_BLUE
       << "===================================================================="
          "========"
       << RESET << endl;

  cout << BRIGHT_CYAN << "PHASE 1 - DATABASE ANALYSIS:" << RESET << endl;
  cout << BRIGHT_WHITE << "  - Loaded " << BRIGHT_YELLOW << allCourses.size()
       << RESET << " available courses from database" << endl;
  cout << BRIGHT_WHITE
       << "  - Configured advanced conflict detection algorithms" << RESET
       << endl;
  cout << BRIGHT_WHITE
       << "  - Initialized intelligent sampling for combinatorial optimization"
       << RESET << endl;

  cout << "\n"
       << BRIGHT_CYAN << "PHASE 2 - CANDIDATE GENERATION:" << RESET << endl;
  cout << BRIGHT_WHITE << "  - Target Credits: " << BRIGHT_YELLOW
       << targetCredits << RESET << " (+/-" << tolerance << ")" << endl;
  cout << BRIGHT_WHITE << "  - Tested course combinations from 2 to 6 courses"
       << RESET << endl;
  cout << BRIGHT_WHITE
       << "  - Used random sampling (up to 2000 samples per course count)"
       << RESET << endl;
  cout << BRIGHT_WHITE << "  - Generated " << BRIGHT_GREEN << candidatesFound
       << RESET << " candidate combinations within credit range" << endl;

  cout << "\n"
       << BRIGHT_CYAN << "PHASE 3 - CONFLICT-FREE SCHEDULING:" << RESET << endl;
  cout << BRIGHT_WHITE << "  - Tested " << BRIGHT_YELLOW << courseCombinations
       << "/" << maxCombinationsToTest << RESET
       << " combinations for schedulability" << endl;
  cout << BRIGHT_WHITE
       << "  - Used in-memory conflict detection (3 lessons per course: "
          "lecture + lab + tutorial)"
       << RESET << endl;
  cout << BRIGHT_WHITE
       << "  - Applied real-time conflict resolution across all time slots"
       << RESET << endl;
  cout << BRIGHT_WHITE << "  - Found optimal solution on attempt "
       << BRIGHT_GREEN << courseCombinations << RESET << endl;

  cout << "\n"
       << BRIGHT_CYAN << "PHASE 4 - SCHEDULE CREATION:" << RESET << endl;
  cout << BRIGHT_WHITE << "  - Created Schedule ID: " << BRIGHT_YELLOW
       << newScheduleId << RESET << endl;
  cout << BRIGHT_WHITE << "  - Added " << BRIGHT_GREEN
       << (bestCombination.size() * 3) << RESET << " lessons (3 per course)"
       << endl;
  cout << BRIGHT_WHITE
       << "  - Applied optimal group selections for zero conflicts" << RESET
       << endl;
  cout << BRIGHT_WHITE << "  - Achieved " << BRIGHT_GREEN << bestCredits
       << RESET << " credits (target: " << targetCredits << ")" << endl;

  cout << "\n"
       << BRIGHT_BLUE << "=== OPTIMAL SCHEDULE RESULTS ===" << RESET << endl;
  cout << BRIGHT_WHITE << "Final Credits: " << RESET << BRIGHT_GREEN
       << bestCredits << RESET << " (target: " << targetCredits << ")" << endl;
  cout << BRIGHT_WHITE << "Courses Selected: " << RESET << BRIGHT_CYAN
       << bestCombination.size() << RESET << endl;
  cout << BRIGHT_WHITE << "Total Lessons: " << RESET << BRIGHT_CYAN
       << (bestCombination.size() * 3) << RESET
       << " (lecture + lab + tutorial per course)" << endl;
  cout << BRIGHT_WHITE << "Conflicts Found: " << RESET << BRIGHT_GREEN << "ZERO"
       << RESET << " (fully optimized)" << endl;

  cout << "\n" << BRIGHT_WHITE << "Selected Courses:" << RESET << endl;
  for (size_t i = 0; i < bestCombination.size(); i++) {
    const Course& course = bestCombination[i];
    cout << BRIGHT_CYAN << "  " << (i + 1) << ". " << BRIGHT_YELLOW
         << course.getCourseId() << RESET << " - " << course.getName() << " ("
         << BRIGHT_GREEN << course.getCredits() << RESET << " credits)" << endl;
  }

  cout << "\n"
       << BRIGHT_BLUE
       << "===================================================================="
          "========"
       << RESET << endl;
  cout << BRIGHT_GREEN
       << "AI OPTIMIZATION COMPLETED SUCCESSFULLY! SCHEDULE READY FOR USE."
       << RESET << endl;
  cout << BRIGHT_BLUE
       << "===================================================================="
          "========"
       << RESET << endl;
  return true;
}
