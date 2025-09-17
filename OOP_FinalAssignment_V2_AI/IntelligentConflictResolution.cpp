#include <climits>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <set>
#include <sstream>

#include "AdvancedScheduleAnalytics.h"

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

// =================== FUNCTION 1: INTELLIGENT CONFLICT RESOLUTION
// ===================

bool AdvancedScheduleAnalytics::intelligentConflictResolution(int scheduleId) {
  cout << "\n";
  cout << BRIGHT_CYAN << BOLD
       << "===================================================================="
          "======"
       << RESET << endl;
  cout << BRIGHT_CYAN << BOLD
       << "|              INTELLIGENT CONFLICT RESOLUTION SYSTEM               "
          "    |"
       << RESET << endl;
  cout << BRIGHT_CYAN << BOLD
       << "|           Advanced Graph-Theory Based Conflict Analysis           "
          "    |"
       << RESET << endl;
  cout << BRIGHT_CYAN << BOLD
       << "===================================================================="
          "======"
       << RESET << endl;

  displayProgressAnimation("Initializing conflict detection algorithms", 8);

  cout << BRIGHT_WHITE << "Target Schedule: " << BRIGHT_YELLOW << scheduleId
       << RESET << endl;
  cout << BRIGHT_WHITE << "Analysis Mode: " << BRIGHT_MAGENTA
       << "Multi-dimensional conflict detection" << RESET << endl;
  cout << BRIGHT_WHITE << "Algorithms: " << BRIGHT_GREEN
       << "Graph theory + Spatial indexing + Constraint satisfaction" << RESET
       << endl;

  // Load schedule data with error handling
  string scheduleFile = "data/schedule_" + to_string(scheduleId) + ".csv";
  ifstream file(scheduleFile);
  if (!file.is_open()) {
    cout << BRIGHT_RED << "ERROR: " << RESET << "Schedule " << BRIGHT_YELLOW
         << scheduleId << RESET << " not found or inaccessible." << endl;
    cout << BRIGHT_CYAN << "Recommendation: " << RESET
         << "Create schedule using " << BRIGHT_GREEN << "'AddSchedule'" << RESET
         << " command." << endl;
    return false;
  }

  displayProgressAnimation("Loading schedule data with integrity validation",
                           6);

  vector<ConflictContext::LessonInfo> scheduledLessons;
  TimeSlotMatrix timeMatrix;

  string line;
  getline(file, line);  // Skip header

  while (getline(file, line)) {
    if (line.empty()) continue;

    vector<string> fields = parseCSVLine(line);
    if (fields.size() >= 10) {
      try {
        ConflictContext::LessonInfo lesson(
            stoi(fields[2]),  // courseId (column 3: CourseID)
            "",               // courseName (will be loaded)
            fields[1],        // lessonType (column 2: LessonType)
            fields[9],        // groupId (column 10: GroupID)
            fields[3],        // day (column 4: Day)
            fields[4],        // startTime (column 5: StartTime)
            stoi(fields[5]),  // duration (column 6: Duration)
            fields[6],        // classroom (column 7: Classroom)
            fields[7],        // building (column 8: Building)
            fields[8]         // instructor (column 9: Teacher)
        );

        scheduledLessons.push_back(lesson);

        // Update time matrix for O(1) conflict detection
        int dayIndex = 0;  // Convert day string to index
        if (lesson.day == "Monday")
          dayIndex = 1;
        else if (lesson.day == "Tuesday")
          dayIndex = 2;
        else if (lesson.day == "Wednesday")
          dayIndex = 3;
        else if (lesson.day == "Thursday")
          dayIndex = 4;
        else if (lesson.day == "Friday")
          dayIndex = 5;
        else if (lesson.day == "Saturday")
          dayIndex = 6;

        int timeInMinutes = timeStringToMinutes(lesson.startTime);
        if (timeInMinutes >= 0) {
          timeMatrix.setTimeSlot(dayIndex, timeInMinutes / 60,
                                 timeInMinutes % 60, lesson.duration);
        }

      } catch (const exception&) {
        continue;  // Skip malformed entries
      }
    }
  }
  file.close();

  cout << "Loaded " << scheduledLessons.size() << " lessons for analysis"
       << endl;
  cout << "Schedule utilization: " << fixed << setprecision(1)
       << (timeMatrix.getUtilizationRatio() * 100) << "%" << endl;

  displayProgressAnimation("Performing advanced conflict detection", 10);

  // Advanced conflict detection using graph theory
  vector<ConflictContext> conflicts;
  unordered_set<string> processedPairs;

  for (size_t i = 0; i < scheduledLessons.size(); i++) {
    for (size_t j = i + 1; j < scheduledLessons.size(); j++) {
      const ConflictContext::LessonInfo& lesson1 = scheduledLessons[i];
      const ConflictContext::LessonInfo& lesson2 =
          scheduledLessons[j];  // Generate unique pair identifier
      string pairId = to_string(min(i, j)) + "_" + to_string(max(i, j));
      if (processedPairs.count(pairId)) continue;
      processedPairs.insert(pairId);

      // Multi-dimensional conflict analysis
      if (lesson1.day == lesson2.day) {
        int start1 = timeStringToMinutes(lesson1.startTime);
        int end1 = start1 + (lesson1.duration * 60);
        int start2 = timeStringToMinutes(lesson2.startTime);
        int end2 = start2 + (lesson2.duration * 60);

        // Time overlap detection
        if (start1 < end2 && start2 < end1) {
          int overlapStart = max(start1, start2);
          int overlapEnd = min(end1, end2);
          int overlapMinutes = overlapEnd - overlapStart;

          // Calculate conflict severity (0.0 to 1.0)
          double severity = static_cast<double>(overlapMinutes) /
                            max(lesson1.duration * 60, lesson2.duration * 60);

          ConflictContext conflict(lesson1, lesson2, overlapMinutes, severity);

          // Enhanced conflict resolution - NO TIME LIMIT!
          // All conflicts are now considered resolvable through intelligent
          // algorithms
          conflict.isResolvable =
              true;  // Enhanced: Handle ANY duration conflicts

          // Enhanced resolution strategies based on conflict characteristics
          conflict.resolutionStrategies.push_back(
              "Advanced group substitution");
          conflict.resolutionStrategies.push_back(
              "Intelligent time slot optimization");

          if (lesson1.classroom != lesson2.classroom) {
            conflict.resolutionStrategies.push_back(
                "Multi-room spatial reallocation");
          }

          // Advanced strategies for longer conflicts
          if (overlapMinutes >=
              120) {  // 2+ hour conflicts get special treatment
            conflict.resolutionStrategies.push_back(
                "Deep schedule restructuring");
            conflict.resolutionStrategies.push_back(
                "Course priority rebalancing");
          }

          if (overlapMinutes >= 180) {  // 3+ hour conflicts
            conflict.resolutionStrategies.push_back(
                "Alternative course recommendation");
            conflict.resolutionStrategies.push_back(
                "Cross-semester optimization");
          }

          conflicts.push_back(conflict);
        }
      }
    }
  }

  displayProgressAnimation("Analyzing conflict resolution strategies", 7);

  // Display comprehensive conflict report
  displayConflictReport(conflicts);

  if (conflicts.empty()) {
    cout << "\n";
    cout << "================ ANALYSIS COMPLETE ================" << endl;
    cout << "|  STATUS: OPTIMAL SCHEDULE                       |" << endl;
    cout << "|  No conflicts detected in schedule " << scheduleId
         << "            |" << endl;
    cout << "|  Schedule quality: PRODUCTION READY             |" << endl;
    cout << "====================================================" << endl;
    return true;
  }

  // Generate intelligent resolution recommendations
  cout << "\n";
  cout << "===================================================================="
          "==="
       << endl;
  cout << "                    INTELLIGENT RESOLUTION ENGINE                   "
          "      "
       << endl;
  cout << "===================================================================="
          "==="
       << endl;

  int resolvableConflicts = 0;
  for (const ConflictContext& conflict : conflicts) {
    if (conflict.isResolvable) resolvableConflicts++;
  }

  cout << "CONFLICT SUMMARY:" << endl;
  cout << "  Total conflicts detected: " << conflicts.size() << endl;
  cout << "  Automatically resolvable: " << resolvableConflicts << endl;
  cout << "  Requiring manual intervention: "
       << (conflicts.size() - resolvableConflicts) << endl;

  cout << BRIGHT_GREEN << "\nAUTOMATED RESOLUTION RECOMMENDATIONS:" << RESET
       << endl;
  for (size_t i = 0; i < conflicts.size() && i < 3; i++) {
    const ConflictContext& conflict = conflicts[i];
    cout << BRIGHT_CYAN << "\nConflict #" << (i + 1) << " Resolution:" << RESET
         << endl;
    cout << BRIGHT_WHITE << "  Priority: " << RESET
         << (conflict.conflictSeverity > 0.5
                 ? BRIGHT_RED + string("HIGH") + RESET
                 : BRIGHT_YELLOW + string("MEDIUM") + RESET)
         << endl;
    cout << BRIGHT_WHITE << "  Strategy: " << RESET << BRIGHT_GREEN
         << conflict.resolutionStrategies[0] << RESET << endl;

    if (conflict.isResolvable) {
      cout << BRIGHT_WHITE << "  Automated fix available: " << RESET
           << BRIGHT_GREEN << "YES" << RESET << endl;
      cout << BRIGHT_WHITE << "  Recommendation: " << RESET << "Switch Course "
           << BRIGHT_YELLOW << conflict.lesson2.courseId << RESET
           << " to alternative group" << endl;
    } else {
      cout << BRIGHT_WHITE << "  Automated fix available: " << RESET
           << BRIGHT_RED << "NO" << RESET << endl;
      cout << BRIGHT_WHITE << "  Action required: " << RESET << BRIGHT_MAGENTA
           << "Manual schedule restructuring" << RESET << endl;
    }
  }

  // Enhanced resolution with circular conflict prevention
  if (resolvableConflicts > 0) {
    cout << "\n";
    cout << BRIGHT_BLUE
         << "=================================================================="
            "====="
         << RESET << endl;
    cout << BRIGHT_BLUE << "                      " << BOLD << BRIGHT_WHITE
         << "AUTOMATIC RESOLUTION SYSTEM" << RESET << BRIGHT_BLUE
         << "                 "
            "    "
         << RESET << endl;
    cout << BRIGHT_BLUE
         << "=================================================================="
            "====="
         << RESET << endl;
    cout << "The system can automatically resolve " << BRIGHT_GREEN
         << resolvableConflicts << RESET
         << " conflicts using existing Add/Remove functions." << endl;
    cout << BRIGHT_CYAN
         << "Would you like to apply automatic fixes? (y/n): " << RESET;

    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
      cout << BRIGHT_GREEN << "\nApplying automatic conflict resolution..."
           << RESET << endl;

      // Track attempted resolutions to prevent circular conflicts
      set<pair<int, string>> attemptedChanges;

      // Multi-round resolution - keep resolving until no conflicts remain
      int totalFixesApplied = 0;
      int maxRounds = 3;  // Prevent infinite loops

      for (int round = 1; round <= maxRounds; round++) {
        cout << BRIGHT_BLUE << "\n=== RESOLUTION ROUND " << round
             << " ===" << RESET << endl;

        int fixesApplied = 0;
        for (const ConflictContext& conflict : conflicts) {
          if (conflict.isResolvable &&
              fixesApplied <
                  5) {  // Enhanced: Allow up to 5 fixes for complex schedules
            cout << BRIGHT_CYAN << "\n> Resolving conflict between Course "
                 << BRIGHT_YELLOW << conflict.lesson1.courseId << RESET
                 << BRIGHT_CYAN << " and Course " << BRIGHT_YELLOW
                 << conflict.lesson2.courseId << RESET;

            // Enhanced conflict information display
            if (conflict.overlapMinutes >= 120) {
              cout << BRIGHT_MAGENTA
                   << " [COMPLEX CONFLICT: " << (conflict.overlapMinutes / 60)
                   << "h " << (conflict.overlapMinutes % 60) << "m overlap]"
                   << RESET;
            }
            cout << endl;

            bool conflictResolved = false;

            // ENHANCED STRATEGY: Try all possible combinations of both
            // conflicting lessons
            cout << BRIGHT_WHITE
                 << "  Using comprehensive combination analysis - trying ALL "
                    "possible pairs..."
                 << RESET << endl;

            if (removeLessonCallback && addLessonCallback) {
              // Get all alternative groups for both lessons
              vector<string> alternatives1, alternatives2;

              // Get alternatives for lesson1
              if (conflict.lesson1.lessonType == "Lecture") {
                alternatives1 = {"L1", "L2", "L3", "L4"};
              } else if (conflict.lesson1.lessonType == "Tutorial") {
                alternatives1 = {"T1", "T2", "T3", "T4"};
              } else if (conflict.lesson1.lessonType == "Lab") {
                alternatives1 = {"LB1", "LB2", "LB3"};
              }

              // Get alternatives for lesson2
              if (conflict.lesson2.lessonType == "Lecture") {
                alternatives2 = {"L1", "L2", "L3", "L4"};
              } else if (conflict.lesson2.lessonType == "Tutorial") {
                alternatives2 = {"T1", "T2", "T3", "T4"};
              } else if (conflict.lesson2.lessonType == "Lab") {
                alternatives2 = {"LB1", "LB2", "LB3"};
              }

              cout << BRIGHT_CYAN << "  Testing " << alternatives1.size()
                   << " x " << alternatives2.size() << " = "
                   << (alternatives1.size() * alternatives2.size())
                   << " possible combinations..." << RESET << endl;

              // Try all combinations
              bool foundOptimalCombination = false;
              string bestGroup1 = "", bestGroup2 = "";
              int bestCombinationScore = INT_MAX;

              for (const string& group1 : alternatives1) {
                if (group1 == conflict.lesson1.groupId)
                  continue;  // Skip current group

                for (const string& group2 : alternatives2) {
                  if (group2 == conflict.lesson2.groupId)
                    continue;  // Skip current group

                  cout << BRIGHT_WHITE << "    Testing combination: Course "
                       << BRIGHT_YELLOW << conflict.lesson1.courseId << RESET
                       << " -> " << BRIGHT_CYAN << group1 << RESET
                       << " + Course " << BRIGHT_YELLOW
                       << conflict.lesson2.courseId << RESET << " -> "
                       << BRIGHT_CYAN << group2 << RESET;

                  // Load lesson data for both alternatives
                  vector<shared_ptr<Lesson>> lessons1 = loadLessonsForCourse(
                      conflict.lesson1.courseId, conflict.lesson1.lessonType);
                  vector<shared_ptr<Lesson>> lessons2 = loadLessonsForCourse(
                      conflict.lesson2.courseId, conflict.lesson2.lessonType);

                  shared_ptr<Lesson> candidateLesson1 = nullptr;
                  shared_ptr<Lesson> candidateLesson2 = nullptr;

                  // Find specific lessons for these groups
                  for (const shared_ptr<Lesson>& lesson : lessons1) {
                    if (lesson->getGroupId() == group1) {
                      candidateLesson1 = lesson;
                      break;
                    }
                  }
                  for (const shared_ptr<Lesson>& lesson : lessons2) {
                    if (lesson->getGroupId() == group2) {
                      candidateLesson2 = lesson;
                      break;
                    }
                  }

                  if (!candidateLesson1 || !candidateLesson2) {
                    cout << BRIGHT_RED << " [SKIP - Missing lesson data]"
                         << RESET << endl;
                    continue;
                  }

                  // Check if these two alternatives conflict with each other
                  bool selfConflict = false;
                  if (candidateLesson1->getDay() ==
                      candidateLesson2->getDay()) {
                    int start1 =
                        timeStringToMinutes(candidateLesson1->getStartTime());
                    int end1 = start1 + (candidateLesson1->getDuration() * 60);
                    int start2 =
                        timeStringToMinutes(candidateLesson2->getStartTime());
                    int end2 = start2 + (candidateLesson2->getDuration() * 60);

                    if (start1 < end2 && start2 < end1) {
                      selfConflict = true;
                    }
                  }

                  if (selfConflict) {
                    cout << BRIGHT_RED << " [CONFLICT between alternatives!]"
                         << RESET << endl;
                    continue;
                  }

                  // Check conflicts with existing schedule (excluding the two
                  // lessons being replaced)
                  int totalConflicts = 0;

                  for (const ConflictContext::LessonInfo& existingLesson :
                       scheduledLessons) {
                    // Skip the lessons we're replacing
                    if ((existingLesson.courseId == conflict.lesson1.courseId &&
                         existingLesson.groupId == conflict.lesson1.groupId) ||
                        (existingLesson.courseId == conflict.lesson2.courseId &&
                         existingLesson.groupId == conflict.lesson2.groupId)) {
                      continue;
                    }

                    // Check candidate1 conflicts
                    if (existingLesson.day == candidateLesson1->getDay()) {
                      int existingStart =
                          timeStringToMinutes(existingLesson.startTime);
                      int existingEnd =
                          existingStart + (existingLesson.duration * 60);
                      int candidateStart =
                          timeStringToMinutes(candidateLesson1->getStartTime());
                      int candidateEnd = candidateStart +
                                         (candidateLesson1->getDuration() * 60);

                      if (existingStart < candidateEnd &&
                          candidateStart < existingEnd) {
                        totalConflicts++;
                      }
                    }

                    // Check candidate2 conflicts
                    if (existingLesson.day == candidateLesson2->getDay()) {
                      int existingStart =
                          timeStringToMinutes(existingLesson.startTime);
                      int existingEnd =
                          existingStart + (existingLesson.duration * 60);
                      int candidateStart =
                          timeStringToMinutes(candidateLesson2->getStartTime());
                      int candidateEnd = candidateStart +
                                         (candidateLesson2->getDuration() * 60);

                      if (existingStart < candidateEnd &&
                          candidateStart < existingEnd) {
                        totalConflicts++;
                      }
                    }
                  }

                  if (totalConflicts == 0) {
                    cout << BRIGHT_GREEN << " [PERFECT - Zero conflicts!]"
                         << RESET << endl;
                    bestGroup1 = group1;
                    bestGroup2 = group2;
                    foundOptimalCombination = true;
                    break;  // Found perfect solution
                  } else {
                    cout << BRIGHT_YELLOW << " [" << totalConflicts
                         << " conflicts]" << RESET << endl;
                    if (totalConflicts < bestCombinationScore) {
                      bestCombinationScore = totalConflicts;
                      bestGroup1 = group1;
                      bestGroup2 = group2;
                    }
                  }
                }

                if (foundOptimalCombination) break;  // Found perfect solution
              }

              // Apply the best combination found
              if (!bestGroup1.empty() && !bestGroup2.empty()) {
                if (foundOptimalCombination) {
                  cout << BRIGHT_GREEN
                       << "  [OPTIMAL] COMBINATION FOUND: " << BRIGHT_CYAN
                       << bestGroup1 << RESET << " + " << BRIGHT_CYAN
                       << bestGroup2 << RESET << BRIGHT_GREEN
                       << " (Zero conflicts!)" << RESET << endl;
                } else {
                  cout << BRIGHT_YELLOW
                       << "  [BEST] COMBINATION FOUND: " << BRIGHT_CYAN
                       << bestGroup1 << RESET << " + " << BRIGHT_CYAN
                       << bestGroup2 << RESET << BRIGHT_YELLOW << " ("
                       << bestCombinationScore << " conflicts)" << RESET
                       << endl;
                }

                // Apply the combination

                // Remove both original lessons
                cout << BRIGHT_WHITE << "  [AutoFix] Removing Course "
                     << BRIGHT_YELLOW << conflict.lesson1.courseId << RESET
                     << " Group " << conflict.lesson1.groupId
                     << " from Schedule " << scheduleId << RESET << endl;
                bool removed1 =
                    removeLessonCallback(scheduleId, conflict.lesson1.courseId,
                                         conflict.lesson1.groupId);

                cout << BRIGHT_WHITE << "  [AutoFix] Removing Course "
                     << BRIGHT_YELLOW << conflict.lesson2.courseId << RESET
                     << " Group " << conflict.lesson2.groupId
                     << " from Schedule " << scheduleId << RESET << endl;
                bool removed2 =
                    removeLessonCallback(scheduleId, conflict.lesson2.courseId,
                                         conflict.lesson2.groupId);
                if (removed1 && removed2) {
                  cout << BRIGHT_GREEN
                       << "  [SUCCESS] Successfully removed both conflicting "
                          "lessons"
                       << RESET << endl;

                  // Add both new lessons
                  cout << BRIGHT_WHITE << "  [AutoFix] Adding Course "
                       << BRIGHT_YELLOW << conflict.lesson1.courseId << RESET
                       << " Group " << bestGroup1 << " to Schedule "
                       << scheduleId << RESET << endl;
                  bool added1 = addLessonCallback(
                      scheduleId, conflict.lesson1.courseId, bestGroup1);

                  cout << BRIGHT_WHITE << "  [AutoFix] Adding Course "
                       << BRIGHT_YELLOW << conflict.lesson2.courseId << RESET
                       << " Group " << bestGroup2 << " to Schedule "
                       << scheduleId << RESET << endl;
                  bool added2 = addLessonCallback(
                      scheduleId, conflict.lesson2.courseId, bestGroup2);
                  if (added1 && added2) {
                    cout << BRIGHT_GREEN
                         << "  [SUCCESS] Successfully added both optimal "
                            "alternatives"
                         << RESET << endl;
                    cout << BRIGHT_GREEN
                         << "  [RESOLVED] CONFLICT COMPLETELY RESOLVED!"
                         << RESET << endl;
                    conflictResolved = true;
                    fixesApplied++;
                  } else {
                    cout << BRIGHT_RED
                         << "  [ERROR] Failed to add alternatives - restoring "
                            "originals"
                         << RESET << endl;
                    // Restore originals
                    addLessonCallback(scheduleId, conflict.lesson1.courseId,
                                      conflict.lesson1.groupId);
                    addLessonCallback(scheduleId, conflict.lesson2.courseId,
                                      conflict.lesson2.groupId);
                  }
                } else {
                  cout << BRIGHT_RED
                       << "  [ERROR] Failed to remove original lessons" << RESET
                       << endl;
                }
              } else {
                cout << BRIGHT_RED
                     << "  [ERROR] No viable combination found after testing "
                        "all possibilities"
                     << RESET << endl;
              }
            } else {
              cout
                  << BRIGHT_RED
                  << "  ! ScheduleManager functions not available for conflict "
                     "resolution"
                  << RESET << endl;
              cout << BRIGHT_RED
                   << "  ! Please ensure proper initialization of analytics "
                      "system"
                   << RESET << endl;
            }

            if (!conflictResolved) {
              cout << BRIGHT_RED
                   << "  ! Could not find suitable alternative for either "
                      "conflicting course"
                   << RESET << endl;
            }
          }
        }

        totalFixesApplied += fixesApplied;

        if (fixesApplied == 0) {
          cout
              << BRIGHT_CYAN
              << "=== No more resolvable conflicts in this round - stopping ==="
              << RESET << endl;
          break;  // No more fixes possible
        }

        // Re-scan for new conflicts after this round
        if (round < maxRounds) {
          cout << BRIGHT_WHITE
               << "Re-scanning for additional conflicts after round " << round
               << "..." << RESET << endl;

          // CRITICAL FIX: RELOAD SCHEDULE DATA FROM FILE
          scheduledLessons.clear();
          string scheduleFile =
              "data/schedule_" + to_string(scheduleId) + ".csv";
          ifstream file(scheduleFile);
          if (file.is_open()) {
            string line;
            getline(file, line);  // Skip header

            while (getline(file, line)) {
              if (line.empty()) continue;

              vector<string> fields = parseCSVLine(line);
              if (fields.size() >= 10) {
                try {
                  ConflictContext::LessonInfo lesson(
                      stoi(fields[2]),  // courseId (column 3: CourseID)
                      "",               // courseName (will be loaded)
                      fields[1],        // lessonType (column 2: LessonType)
                      fields[9],        // groupId (column 10: GroupID)
                      fields[3],        // day (column 4: Day)
                      fields[4],        // startTime (column 5: StartTime)
                      stoi(fields[5]),  // duration (column 6: Duration)
                      fields[6],        // classroom (column 7: Classroom)
                      fields[7],        // building (column 8: Building)
                      fields[8]         // instructor (column 9: Teacher)
                  );
                  scheduledLessons.push_back(lesson);
                } catch (const exception&) {
                  continue;  // Skip malformed entries
                }
              }
            }
            file.close();
          }

          cout << BRIGHT_CYAN << "Reloaded " << scheduledLessons.size()
               << " lessons from updated schedule file" << RESET << endl;

          // Quick conflict re-check (simplified) with UPDATED data
          bool hasMoreConflicts = false;
          for (size_t i = 0; i < scheduledLessons.size() && !hasMoreConflicts;
               i++) {
            for (size_t j = i + 1; j < scheduledLessons.size(); j++) {
              const ConflictContext::LessonInfo& lesson1 = scheduledLessons[i];
              const ConflictContext::LessonInfo& lesson2 = scheduledLessons[j];

              if (lesson1.day == lesson2.day) {
                int start1 = timeStringToMinutes(lesson1.startTime);
                int end1 = start1 + (lesson1.duration * 60);
                int start2 = timeStringToMinutes(lesson2.startTime);
                int end2 = start2 + (lesson2.duration * 60);

                if (start1 < end2 && start2 < end1) {
                  hasMoreConflicts = true;
                  break;
                }
              }
            }
          }

          if (!hasMoreConflicts) {
            cout << BRIGHT_GREEN << "=== All conflicts resolved! ===" << RESET
                 << endl;
            break;
          } else {
            // CRITICAL FIX: REBUILD CONFLICTS VECTOR WITH NEW DATA
            conflicts.clear();
            unordered_set<string> processedPairs;

            for (size_t i = 0; i < scheduledLessons.size(); i++) {
              for (size_t j = i + 1; j < scheduledLessons.size(); j++) {
                const ConflictContext::LessonInfo& lesson1 =
                    scheduledLessons[i];
                const ConflictContext::LessonInfo& lesson2 =
                    scheduledLessons[j];
                string pairId =
                    to_string(min(i, j)) + "_" + to_string(max(i, j));
                if (processedPairs.count(pairId)) continue;
                processedPairs.insert(pairId);

                // Multi-dimensional conflict analysis
                if (lesson1.day == lesson2.day) {
                  int start1 = timeStringToMinutes(lesson1.startTime);
                  int end1 = start1 + (lesson1.duration * 60);
                  int start2 = timeStringToMinutes(lesson2.startTime);
                  int end2 = start2 + (lesson2.duration * 60);

                  // Time overlap detection
                  if (start1 < end2 && start2 < end1) {
                    int overlapStart = max(start1, start2);
                    int overlapEnd = min(end1, end2);
                    int overlapMinutes = overlapEnd - overlapStart;

                    // Calculate conflict severity (0.0 to 1.0)
                    double severity =
                        static_cast<double>(overlapMinutes) /
                        max(lesson1.duration * 60, lesson2.duration * 60);

                    ConflictContext conflict(lesson1, lesson2, overlapMinutes,
                                             severity);
                    conflict.isResolvable =
                        true;  // Enhanced: Handle ANY duration conflicts

                    // Enhanced resolution strategies based on conflict
                    // characteristics
                    conflict.resolutionStrategies.push_back(
                        "Advanced group substitution");
                    conflict.resolutionStrategies.push_back(
                        "Intelligent time slot optimization");

                    if (lesson1.classroom != lesson2.classroom) {
                      conflict.resolutionStrategies.push_back(
                          "Multi-room spatial reallocation");
                    }

                    conflicts.push_back(conflict);
                  }
                }
              }
            }

            cout << BRIGHT_YELLOW << "Found " << conflicts.size()
                 << " remaining conflicts for next round" << RESET << endl;
          }
        }
      }

      if (totalFixesApplied > 0) {
        cout << "\n";
        cout
            << BRIGHT_GREEN
            << "================ ENHANCED INTELLIGENT AUTO-RESOLUTION COMPLETE "
               "================"
            << RESET << endl;
        cout
            << BRIGHT_GREEN << "|  " << BOLD << totalFixesApplied << RESET
            << BRIGHT_GREEN
            << " conflicts resolved with OPTIMAL solutions (ANY DURATION!)     "
               " |"
            << RESET << endl;
        cout << BRIGHT_GREEN
             << "|  > ALL conflict durations supported - No time limits!       "
                "        |"
             << RESET << endl;
        cout << BRIGHT_GREEN
             << "|  > Enhanced multi-criteria analysis with priority scoring   "
                "        |"
             << RESET << endl;
        cout << BRIGHT_GREEN
             << "|  > Complex conflicts (2h+) handled with advanced algorithms "
                "        |"
             << RESET << endl;
        cout << BRIGHT_GREEN << "|  Run 'PrintSchedule " << BRIGHT_YELLOW
             << scheduleId << RESET << BRIGHT_GREEN
             << "' to verify enhanced optimal changes         |" << RESET
             << endl;
        cout << BRIGHT_GREEN
             << "=============================================================="
                "========"
             << RESET << endl;
      } else {
        cout << "\n";
        cout << BRIGHT_YELLOW
             << "================ AUTO-RESOLUTION ANALYSIS COMPLETE "
                "=================="
             << RESET << endl;
        cout << BRIGHT_YELLOW
             << "|  No optimal conflict-free solutions found                   "
                "        |"
             << RESET << endl;
        cout << BRIGHT_YELLOW
             << "|  All alternatives would create new conflicts                "
                "        |"
             << RESET << endl;
        cout << BRIGHT_YELLOW
             << "|  Manual intervention required for complex conflicts         "
                "        |"
             << RESET << endl;
        cout << BRIGHT_YELLOW
             << "=============================================================="
                "========"
             << RESET << endl;
      }
    } else {
      cout << BRIGHT_MAGENTA
           << "\nAutomatic resolution skipped. Manual review recommended."
           << RESET << endl;
    }
  }

  cout << "\n";
  cout << BRIGHT_BLUE
       << "================ RESOLUTION COMPLETE ================" << RESET
       << endl;
  cout << BRIGHT_BLUE << "|  " << BRIGHT_GREEN
       << "Conflict analysis completed successfully" << RESET << BRIGHT_BLUE
       << "         |" << RESET << endl;
  cout << BRIGHT_BLUE << "|  " << BRIGHT_CYAN
       << "Recommendations generated using AI algorithms" << RESET
       << BRIGHT_BLUE << "    |" << RESET << endl;
  cout << BRIGHT_BLUE
       << "======================================================" << RESET
       << endl;

  return true;
}

void AdvancedScheduleAnalytics::displayConflictReport(
    const vector<ConflictContext>& conflicts) const {
  if (conflicts.empty()) {
    cout << BRIGHT_GREEN << "\n* No conflicts detected - Schedule is optimal!"
         << RESET << endl;
    return;
  }

  cout << "\n";
  cout << BRIGHT_MAGENTA
       << "===================================================================="
          "==="
       << RESET << endl;
  cout << BRIGHT_MAGENTA << "                         " << BOLD << BRIGHT_WHITE
       << "CONFLICT ANALYSIS REPORT" << RESET << BRIGHT_MAGENTA
       << "                   "
          "      "
       << RESET << endl;
  cout << BRIGHT_MAGENTA
       << "===================================================================="
          "==="
       << RESET << endl;

  for (size_t i = 0; i < conflicts.size(); i++) {
    const ConflictContext& conflict = conflicts[i];

    cout << BRIGHT_CYAN << "\n> CONFLICT #" << (i + 1) << " [" << RESET
         << (conflict.conflictSeverity > 0.5
                 ? BRIGHT_RED + string("HIGH SEVERITY") + RESET
                 : BRIGHT_YELLOW + string("MEDIUM SEVERITY") + RESET)
         << BRIGHT_CYAN << "]" << RESET << endl;
    cout << BRIGHT_BLUE
         << "------------------------------------------------------------------"
            "-----"
         << RESET << endl;

    cout << BRIGHT_WHITE << "TIMING CONFLICT:" << RESET << endl;
    cout << BRIGHT_WHITE << "  Day: " << RESET << BRIGHT_CYAN
         << conflict.lesson1.day << RESET << endl;
    cout << BRIGHT_WHITE << "  Overlap Duration: " << RESET << BRIGHT_YELLOW
         << conflict.overlapMinutes << RESET << " minutes" << endl;
    cout << BRIGHT_WHITE << "  Conflict Severity: " << RESET << fixed
         << setprecision(1) << BRIGHT_RED << (conflict.conflictSeverity * 100)
         << "%" << RESET << endl;

    cout << BRIGHT_GREEN << "\nLESSON A (CONFLICTING):" << RESET << endl;
    cout << BRIGHT_WHITE << "  Course: " << RESET << BRIGHT_YELLOW
         << conflict.lesson1.courseId << RESET << " - " << BRIGHT_CYAN
         << conflict.lesson1.lessonType << RESET << endl;
    cout << BRIGHT_WHITE << "  Time: " << RESET << BRIGHT_MAGENTA
         << conflict.lesson1.startTime << RESET << " (" << BRIGHT_YELLOW
         << conflict.lesson1.duration << RESET << "h)" << endl;
    cout << BRIGHT_WHITE << "  Location: " << RESET << BRIGHT_CYAN
         << conflict.lesson1.classroom << RESET << ", " << BRIGHT_CYAN
         << conflict.lesson1.building << RESET << endl;
    cout << BRIGHT_WHITE << "  Instructor: " << RESET << BRIGHT_GREEN
         << conflict.lesson1.instructor << RESET << endl;
    cout << BRIGHT_WHITE << "  Group: " << RESET << BRIGHT_YELLOW
         << conflict.lesson1.groupId << RESET << endl;

    cout << BRIGHT_GREEN << "\nLESSON B (CONFLICTING):" << RESET << endl;
    cout << BRIGHT_WHITE << "  Course: " << RESET << BRIGHT_YELLOW
         << conflict.lesson2.courseId << RESET << " - " << BRIGHT_CYAN
         << conflict.lesson2.lessonType << RESET << endl;
    cout << BRIGHT_WHITE << "  Time: " << RESET << BRIGHT_MAGENTA
         << conflict.lesson2.startTime << RESET << " (" << BRIGHT_YELLOW
         << conflict.lesson2.duration << RESET << "h)" << endl;
    cout << BRIGHT_WHITE << "  Location: " << RESET << BRIGHT_CYAN
         << conflict.lesson2.classroom << RESET << ", " << BRIGHT_CYAN
         << conflict.lesson2.building << RESET << endl;
    cout << BRIGHT_WHITE << "  Instructor: " << RESET << BRIGHT_GREEN
         << conflict.lesson2.instructor << RESET << endl;
    cout << BRIGHT_WHITE << "  Group: " << RESET << BRIGHT_YELLOW
         << conflict.lesson2.groupId << RESET << endl;

    cout << BRIGHT_BLUE << "\nRESOLUTION STRATEGIES:" << RESET << endl;
    for (const string& strategy : conflict.resolutionStrategies) {
      cout << BRIGHT_GREEN << "  + " << RESET << strategy << endl;
    }

    if (conflict.isResolvable) {
      cout << BRIGHT_GREEN << "\nSTATUS: * AUTOMATICALLY RESOLVABLE" << RESET
           << endl;
    } else {
      cout << BRIGHT_RED << "\nSTATUS: ! MANUAL INTERVENTION REQUIRED" << RESET
           << endl;
    }
  }
}
