#ifndef SCHEDULE_MANAGER_H
#define SCHEDULE_MANAGER_H

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#include "AdvancedScheduleAnalytics.h"
#include "Course.h"
#include "Lab.h"
#include "Lecture.h"
#include "Schedule.h"
#include "Tutorial.h"

using namespace std;

/**
 * @class ScheduleManager
 * @brief Professional Schedule Management System
 *
 * This class provides a comprehensive solution for managing academic schedules,
 * including course management, schedule creation, and advanced analytics.
 */
class ScheduleManager {
 private:
  // Core Data Members
  vector<Course> courses;
  vector<Schedule> schedules;
  map<int, vector<shared_ptr<Lesson>>> courseLessons;
  int currentCourseIndex = 0;
  AdvancedScheduleAnalytics analytics;

  // ================= MODERN COLOR PALETTE =================
  // Text Formatting
  static const string RESET;
  static const string BOLD;
  static const string DIM;
  static const string ITALIC;
  static const string UNDERLINE;

  // Standard Colors
  static const string BLACK;
  static const string RED;
  static const string GREEN;
  static const string YELLOW;
  static const string BLUE;
  static const string MAGENTA;
  static const string CYAN;
  static const string WHITE;
  static const string GRAY;

  // Bright/Vibrant Colors
  static const string BRIGHT_RED;
  static const string BRIGHT_GREEN;
  static const string BRIGHT_YELLOW;
  static const string BRIGHT_BLUE;
  static const string BRIGHT_MAGENTA;
  static const string BRIGHT_CYAN;
  static const string BRIGHT_WHITE;

  // Background Colors
  static const string BG_BLACK;
  static const string BG_RED;
  static const string BG_GREEN;
  static const string BG_YELLOW;
  static const string BG_BLUE;
  static const string BG_MAGENTA;
  static const string BG_CYAN;
  static const string BG_WHITE;
  static const string BG_GRAY;

 public:
  /**
   * @brief Constructor - Initializes the Schedule Management System
   */
  ScheduleManager();

  // ================= CORE DATA MANAGEMENT =================
  /**
   * @brief Load course data from CSV file
   */
  void loadCourses();

  /**
   * @brief Load lesson data from specific file
   * @param filename Path to the lesson file
   * @param type Type of lesson (lectures, tutorials, labs)
   */
  void loadLessonsFromFile(const string& filename, const string& type);

  /**
   * @brief Load all lesson data for all courses
   */
  void loadLessons();

  /**
   * @brief Load existing schedules from storage
   */
  void loadSchedules();

  /**
   * @brief Save current schedules to storage
   */
  void saveSchedules();

  /**
   * @brief Save individual schedule to file in comprehensive format
   * @param scheduleId The schedule ID to save
   */
  void saveScheduleToFile(int scheduleId);

  /**
   * @brief Load lesson data for existing schedules
   */
  void loadScheduleData();

  // ================= DISPLAY FUNCTIONS =================
  /**
   * @brief Display detailed information for a specific course
   * @param courseId The course ID to display
   */
  void printCourse(int courseId);

  /**
   * @brief Display all available courses with professional formatting
   */
  void printCourses();

  /**
   * @brief Display next 10 courses in pagination
   */
  void printNext10Courses();

  /**
   * @brief Display detailed information for a specific schedule
   * @param scheduleId The schedule ID to display
   */
  void printSchedule(int scheduleId);

  /**
   * @brief Display all created schedules with overview
   */
  void printSchedules();

  // ================= SCHEDULE MANAGEMENT =================
  /**
   * @brief Create a new schedule
   * @param specificId Optional specific ID for the schedule
   * @return ID of the created schedule, or -1 if failed
   */
  int addSchedule(int specificId = -1);

  /**
   * @brief Remove an existing schedule
   * @param scheduleId ID of the schedule to remove
   */
  void removeSchedule(int scheduleId);

  /**
   * @brief Add a lesson to a specific schedule
   * @param scheduleId Target schedule ID
   * @param courseId Course ID of the lesson
   * @param groupId Group ID of the lesson
   * @return true if successful, false otherwise
   */
  bool addLessonToSchedule(int scheduleId, int courseId, const string& groupId);

  /**
   * @brief Remove a lesson from a specific schedule
   * @param scheduleId Target schedule ID
   * @param courseId Course ID of the lesson
   * @param groupId Group ID of the lesson
   * @return true if successful, false otherwise
   */
  bool removeLessonFromSchedule(int scheduleId, int courseId,
                                const string& groupId);

  /**
   * @brief Get schedule object by ID
   * @param scheduleId The schedule ID to retrieve
   * @return Pointer to schedule or nullptr if not found
   */
  Schedule* getScheduleById(int scheduleId);

  /**
   * @brief Perform automatic conflict resolution
   * @param scheduleId Target schedule ID
   * @param courseId Course ID involved in conflict
   * @param oldGroup Current group causing conflict
   * @param newGroup Proposed new group to resolve conflict
   * @return true if resolution successful, false otherwise
   */
  bool performAutomaticConflictResolution(int scheduleId, int courseId,
                                          const string& oldGroup,
                                          const string& newGroup);

  // ================= USER INTERFACE =================
  /**
   * @brief Display the main menu with modern styling
   */
  void showMainMenu();

  /**
   * @brief Display help information
   */
  void showHelp();

  /**
   * @brief Main application loop
   */
  void run();

  // ================= UTILITY FUNCTIONS =================
  /**
   * @brief Calculate end time based on start time and duration
   * @param startTime Start time in HH:MM format
   * @param duration Duration in minutes
   * @return End time in HH:MM format
   */
  string calculateEndTime(const string& startTime, int duration) const;

  // ================= TESTING FRAMEWORK =================
  /**
   * @brief Display formatted test header
   * @param testName Name of the test being performed
   */
  void printTestHeader(const string& testName);

  /**
   * @brief Display formatted test result
   * @param testCase Name of the test case
   * @param passed Whether the test passed
   * @param details Additional test details
   */
  void printTestResult(const string& testCase, bool passed,
                       const string& details = "");

  /**
   * @brief Display test summary with statistics
   * @param total Total number of tests
   * @param passed Number of passed tests
   * @param failed Number of failed tests
   */
  void printTestSummary(int total, int passed, int failed);

  /**
   * @brief Run comprehensive system test suite
   */
  void runComprehensiveSystemTest();

  /**
   * @brief Run advanced analytics test suite
   */
  void runAdvancedAnalyticsTest();
};

#endif  // SCHEDULE_MANAGER_H
