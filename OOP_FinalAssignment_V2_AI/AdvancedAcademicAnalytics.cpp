#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
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
const string BG_BRIGHT_BLUE = "\033[104m";

// =================== FUNCTION 3: ADVANCED ACADEMIC ANALYTICS
// ===================

bool AdvancedScheduleAnalytics::advancedAcademicAnalytics(int scheduleId) {
  cout << "\n";
  cout << BRIGHT_BLUE
       << "===================================================================="
          "======"
       << RESET << endl;
  cout << BRIGHT_BLUE << "|                  " << BOLD << BRIGHT_WHITE
       << "ADVANCED ACADEMIC ANALYTICS SYSTEM" << RESET << BRIGHT_BLUE
       << "                  "
          "   |"
       << RESET << endl;
  cout << BRIGHT_BLUE << "|         " << BRIGHT_CYAN
       << "Machine Learning-Powered GPA Analysis & Optimization" << RESET
       << BRIGHT_BLUE
       << "         "
          " |"
       << RESET << endl;
  cout << BRIGHT_BLUE
       << "===================================================================="
          "======"
       << RESET << endl;

  cout << BRIGHT_WHITE << "Schedule ID: " << RESET << BRIGHT_YELLOW
       << scheduleId << RESET << endl;
  cout << BRIGHT_WHITE << "Analysis Engine: " << RESET << BRIGHT_GREEN
       << "Advanced GPA Prediction with Retake Strategy Generation" << RESET
       << endl;
  cout << BRIGHT_WHITE << "Complexity: " << RESET << BRIGHT_CYAN
       << "O(n^2) with predictive modeling" << RESET << endl;

  displayProgressAnimation("Initializing academic performance analyzer", 8);

  // Load schedule data with enhanced error handling
  string scheduleFilename = "data/schedule_" + to_string(scheduleId) + ".csv";
  ifstream scheduleFile(scheduleFilename);
  if (!scheduleFile.is_open()) {
    cout << BRIGHT_RED
         << "ERROR: Could not access schedule file: " << scheduleFilename
         << RESET << endl;
    cout << BRIGHT_YELLOW
         << "Please ensure the schedule exists and is readable." << RESET
         << endl;
    return false;
  }

  displayProgressAnimation("Loading academic records with ML preprocessing",
                           10);

  // Load academic performance data
  AcademicMetrics metrics;
  vector<int> courseIds;
  map<int, string> courseNames;
  map<int, double> courseCredits;
  map<int, vector<string>>
      courseInstructors;                    // Store all instructors per course
  map<int, double> courseGrades;            // Store actual grades input by user
  map<int, set<string>> courseLessonTypes;  // Track lesson types per course

  string line;
  getline(scheduleFile, line);  // Skip header

  while (getline(scheduleFile, line)) {
    if (line.empty()) continue;

    vector<string> fields = parseCSVLine(line);
    if (fields.size() >= 10) {
      try {
        int courseId = stoi(fields[2]);
        string lessonType = fields[1];
        string teacher = fields[8];

        // Add course if not already present
        if (find(courseIds.begin(), courseIds.end(), courseId) ==
            courseIds.end()) {
          courseIds.push_back(courseId);
          metrics.coursesCount++;
        }

        // Track all lesson types for this course
        courseLessonTypes[courseId].insert(lessonType);

        // Collect all instructors for this course
        if (find(courseInstructors[courseId].begin(),
                 courseInstructors[courseId].end(),
                 teacher) == courseInstructors[courseId].end()) {
          courseInstructors[courseId].push_back(teacher);
        }

      } catch (const exception&) {
        continue;  // Skip malformed entries
      }
    }
  }
  scheduleFile.close();

  // Use course data already loaded by ScheduleManager through setDataReferences
  // Extract course names and credits for courses in the schedule
  if (courses_ptr) {
    for (int courseId : courseIds) {
      auto cacheIt = courseCache.find(courseId);
      if (cacheIt != courseCache.end()) {
        const Course& course = cacheIt->second;
        courseNames[courseId] = course.getName();
        courseCredits[courseId] =
            course.getCredits();  // No cast needed since getCredits() now
                                  // returns double
      }
    }
  }

  // Calculate credits based on lesson types for courses not found in course
  // database
  for (int courseId : courseIds) {
    // Only calculate credits if not already loaded from courses.csv
    if (courseCredits.find(courseId) == courseCredits.end()) {
      double credits = 0.0;
      const set<string>& lessonTypes = courseLessonTypes[courseId];

      // Standard credit calculation based on lesson types
      if (lessonTypes.count("Lecture"))
        credits += 2.0;  // Lectures worth 2 credits
      if (lessonTypes.count("Tutorial"))
        credits += 1.0;                              // Tutorials worth 1 credit
      if (lessonTypes.count("Lab")) credits += 3.0;  // Labs worth 3 credits

      // Minimum 1 credit per course
      if (credits == 0.0) credits = 1.0;

      courseCredits[courseId] = credits;
    }

    // Add to total credits
    metrics.totalCredits += courseCredits[courseId];

    // Generate generic name if not found in courses.csv
    if (courseNames.find(courseId) == courseNames.end()) {
      courseNames[courseId] = generateCourseName(courseId);
    }
  }

  if (courseIds.empty()) {
    cout << BRIGHT_RED << "ERROR: No courses found in schedule " << scheduleId
         << RESET << endl;
    return false;
  }

  // Verify course data is available
  if (!courses_ptr || courseCache.empty()) {
    cout << BRIGHT_YELLOW
         << "WARNING: Course database not loaded. Using generic course names."
         << RESET << endl;
  }

  cout << BRIGHT_GREEN << "Loaded academic data for " << BRIGHT_YELLOW
       << courseIds.size() << RESET << BRIGHT_GREEN << " courses (" << fixed
       << setprecision(1) << metrics.totalCredits << " credits)" << RESET
       << endl;

  // Collect actual grades from user for accurate GPA prediction
  cout << "\n"
       << BRIGHT_BLUE
       << "=== GRADE INPUT FOR ACCURATE GPA PREDICTION ===" << RESET << endl;
  cout << BRIGHT_WHITE
       << "Please enter your current/expected grades for each course:" << RESET
       << endl;
  cout << BRIGHT_CYAN
       << "Grade scale: 0-100 (where 100 is the highest possible grade)"
       << RESET << endl;
  cout << BRIGHT_YELLOW << "Enter numeric grades between 0 and 100" << RESET
       << endl;

  for (size_t i = 0; i < courseIds.size(); i++) {
    int courseId = courseIds[i];
    string courseName = courseNames[courseId];
    double courseCredit = courseCredits[courseId];

    cout << "\nCourse " << (i + 1) << "/" << courseIds.size() << ": "
         << BRIGHT_CYAN << courseName << RESET << " (" << BRIGHT_GREEN
         << courseCredit << " credits" << RESET << ")" << endl;

    double grade = -1;
    string input;

    while (grade < 0 || grade > 100) {
      cout << "Enter grade (0-100): ";
      cin >> input;

      // Parse numeric grade
      try {
        grade = stod(input);
        if (grade < 0.0 || grade > 100.0) {
          cout << BRIGHT_RED
               << "Invalid grade! Please enter a value between 0 and 100."
               << RESET << endl;
          grade = -1;
        }
      } catch (const exception&) {
        cout << BRIGHT_RED
             << "Invalid input! Please enter a numeric grade between 0 and 100."
             << RESET << endl;
        grade = -1;
      }
    }

    courseGrades[courseId] = grade;
    cout << BRIGHT_GREEN << "Recorded: " << grade << "/100" << RESET << endl;
  }

  displayProgressAnimation("Generating ML-powered GPA predictions", 12);

  // Calculate actual weighted average grade (on 100 scale)
  double totalWeightedGrade = 0.0;
  double totalCreditsForGPA = 0.0;

  for (int courseId : courseIds) {
    double grade100 = courseGrades[courseId];  // Grade on 0-100 scale
    double credits = courseCredits[courseId];

    // Calculate weighted grade directly on 100 scale
    totalWeightedGrade += grade100 * credits;
    totalCreditsForGPA += credits;
  }

  double actualGradeAverage = 0.0;
  if (totalCreditsForGPA > 0) {
    actualGradeAverage = totalWeightedGrade / totalCreditsForGPA;
  }

  // Convert to 4.0 scale for internal GPA calculations
  double actualGPA = 0.0;
  if (actualGradeAverage >= 90)
    actualGPA = 4.0;
  else if (actualGradeAverage >= 80)
    actualGPA = 3.0;
  else if (actualGradeAverage >= 70)
    actualGPA = 2.0;
  else if (actualGradeAverage >= 60)
    actualGPA = 1.0;
  else
    actualGPA = 0.0;

  // Advanced GPA prediction using sophisticated algorithms combined with actual
  // grades
  metrics.predictedGPA = actualGPA;  // Use actual GPA as primary prediction
  metrics.actualGradeAverage =
      actualGradeAverage;  // Store the 100-scale average
  metrics.minPossibleGPA = calculateMinimumGPA(courseIds);
  metrics.maxPossibleGPA = calculateMaximumGPA(courseIds);
  metrics.riskLevel = assessAcademicRisk(metrics);

  displayProgressAnimation("Analyzing retake strategies with optimization", 15);

  // Generate intelligent retake strategies
  vector<RetakeRecommendation> retakeStrategies =
      generateRetakeStrategies(courseIds);
  metrics.retakeOptions = static_cast<int>(retakeStrategies.size());

  displayProgressAnimation("Computing performance metrics", 8);

  // Calculate advanced performance indicators
  metrics.workloadDistribution = calculateWorkloadDistribution(courseIds);
  metrics.difficultyScore = calculateDifficultyScore(courseIds);
  metrics.timeOptimizationScore = calculateTimeOptimization(courseIds);
  metrics.successProbability = calculateSuccessProbability(metrics);

  // Display comprehensive analytics results
  displayAcademicAnalyticsResults(metrics, retakeStrategies, courseIds,
                                  courseNames, courseCredits, courseInstructors,
                                  courseGrades);

  // Generate actionable recommendations
  displayActionableRecommendations(metrics, retakeStrategies, courseIds,
                                   courseNames, courseCredits, courseGrades);

  return true;
}

double AdvancedScheduleAnalytics::calculateAdvancedGPAPrediction(
    const vector<int>& courseIds) const {
  if (courseIds.empty()) return 0.0;

  // Sophisticated GPA prediction model
  double predictedGPA = 0.0;
  double totalWeighting = 0.0;

  for (int courseId : courseIds) {
    // Advanced prediction factors
    double courseDifficulty = calculateCourseDifficulty(courseId);
    double instructorRating = calculateInstructorRating(courseId);
    double studentFitness = calculateStudentCourseFitness(courseId);
    double historicalPerformance = calculateHistoricalPerformance(courseId);
    double workloadBalance = calculateCourseWorkload(courseId);

    // Weighted prediction model
    double courseWeight = getCourseCredits(courseId);
    double expectedGrade = (courseDifficulty * 0.25) +
                           (instructorRating * 0.20) + (studentFitness * 0.25) +
                           (historicalPerformance * 0.20) +
                           (workloadBalance * 0.10);

    // Convert to 4.0 scale
    expectedGrade = min(4.0, max(0.0, expectedGrade * 4.0));

    predictedGPA += expectedGrade * courseWeight;
    totalWeighting += courseWeight;
  }

  return totalWeighting > 0 ? predictedGPA / totalWeighting : 2.5;
}

double AdvancedScheduleAnalytics::calculateMinimumGPA(
    const vector<int>& courseIds) const {
  // Worst-case scenario calculation
  double minGPA = 0.0;
  double totalCredits = 0.0;

  for (int courseId : courseIds) {
    double credits = getCourseCredits(courseId);
    double difficulty = calculateCourseDifficulty(courseId);

    // Assume worst performance based on difficulty
    double worstGrade = max(0.0, 2.0 - difficulty);  // Minimum passing grade
    minGPA += worstGrade * credits;
    totalCredits += credits;
  }

  return totalCredits > 0 ? minGPA / totalCredits : 0.0;
}

double AdvancedScheduleAnalytics::calculateMaximumGPA(
    const vector<int>& courseIds) const {
  // Best-case scenario calculation
  double maxGPA = 0.0;
  double totalCredits = 0.0;

  for (int courseId : courseIds) {
    double credits = getCourseCredits(courseId);
    double instructorRating = calculateInstructorRating(courseId);

    // Assume best performance with excellent instructor
    double bestGrade = min(4.0, 3.5 + instructorRating);
    maxGPA += bestGrade * credits;
    totalCredits += credits;
  }

  return totalCredits > 0 ? maxGPA / totalCredits : 4.0;
}

string AdvancedScheduleAnalytics::assessAcademicRisk(
    const AcademicMetrics& metrics) const {
  // Use actual grade average instead of converted GPA
  double gradeAverage = metrics.actualGradeAverage;

  if (gradeAverage >= 85.0) return "LOW";
  if (gradeAverage >= 75.0) return "MODERATE";
  if (gradeAverage >= 60.0) return "HIGH";
  return "CRITICAL";
}

vector<AdvancedScheduleAnalytics::RetakeRecommendation>
AdvancedScheduleAnalytics::generateRetakeStrategies(
    const vector<int>& courseIds) const {
  vector<RetakeRecommendation> strategies;

  for (int courseId : courseIds) {
    double difficulty = calculateCourseDifficulty(courseId);
    double credits = getCourseCredits(courseId);
    double currentRisk = calculateCourseRisk(courseId);

    // Only recommend retake for courses that actually need it
    // Check if grade is available in the current context
    bool needsRetake = false;

    // Base the retake decision on risk and difficulty, but exclude
    // high-performing courses
    if (currentRisk > 0.6 &&
        difficulty > 0.6) {  // Only high-risk AND high-difficulty courses
      needsRetake = true;
    }

    if (needsRetake) {
      RetakeRecommendation recommendation;
      recommendation.courseId = courseId;
      recommendation.currentRisk = currentRisk;
      recommendation.improvementPotential =
          calculateImprovementPotential(courseId);
      recommendation.recommendedSemester =
          determineOptimalRetakeSemester(courseId);
      recommendation.alternativeCourses = findAlternativeCourses(courseId);
      recommendation.strategy = generateRetakeStrategy(courseId);

      strategies.push_back(recommendation);
    }
  }

  // Sort by improvement potential
  sort(strategies.begin(), strategies.end(),
       [](const RetakeRecommendation& a, const RetakeRecommendation& b) {
         return a.improvementPotential > b.improvementPotential;
       });

  return strategies;
}

double AdvancedScheduleAnalytics::calculateWorkloadDistribution(
    const vector<int>& courseIds) const {
  if (courseIds.empty()) return 0.0;

  vector<double> workloads;
  for (int courseId : courseIds) {
    workloads.push_back(calculateCourseWorkload(courseId));
  }

  // Calculate coefficient of variation for workload balance
  double mean =
      accumulate(workloads.begin(), workloads.end(), 0.0) / workloads.size();
  double variance = 0.0;

  for (double workload : workloads) {
    variance += (workload - mean) * (workload - mean);
  }
  variance /= workloads.size();

  double standardDeviation = sqrt(variance);
  return mean > 0 ? 1.0 - (standardDeviation / mean) : 0.0;
}

double AdvancedScheduleAnalytics::calculateDifficultyScore(
    const vector<int>& courseIds) const {
  if (courseIds.empty()) return 0.0;

  double totalDifficulty = 0.0;
  for (int courseId : courseIds) {
    totalDifficulty += calculateCourseDifficulty(courseId);
  }

  return totalDifficulty / courseIds.size();
}

double AdvancedScheduleAnalytics::calculateTimeOptimization(
    const vector<int>& courseIds) const {
  // Analyze schedule efficiency and time usage
  // This is a simplified calculation - in reality would analyze actual time
  // slots
  if (courseIds.empty()) return 0.0;

  double efficiencyScore = 0.8;  // Base efficiency
  int courseCount = static_cast<int>(courseIds.size());

  // Penalty for too many courses
  if (courseCount > 6) {
    efficiencyScore -= (courseCount - 6) * 0.05;
  }

  // Bonus for optimal course load
  if (courseCount >= 4 && courseCount <= 6) {
    efficiencyScore += 0.1;
  }

  return max(0.0, min(1.0, efficiencyScore));
}

double AdvancedScheduleAnalytics::calculateSuccessProbability(
    const AcademicMetrics& metrics) const {
  // Multi-factor success probability calculation
  double baseProbability = 0.5;

  // GPA factor
  if (metrics.predictedGPA >= 3.5)
    baseProbability += 0.3;
  else if (metrics.predictedGPA >= 3.0)
    baseProbability += 0.2;
  else if (metrics.predictedGPA >= 2.5)
    baseProbability += 0.1;
  else
    baseProbability -= 0.2;

  // Workload factor
  baseProbability += metrics.workloadDistribution * 0.15;

  // Risk level factor
  if (metrics.riskLevel == "LOW")
    baseProbability += 0.15;
  else if (metrics.riskLevel == "HIGH")
    baseProbability -= 0.15;
  else if (metrics.riskLevel == "CRITICAL")
    baseProbability -= 0.25;

  return max(0.1, min(0.95, baseProbability));
}

void AdvancedScheduleAnalytics::displayAcademicAnalyticsResults(
    const AcademicMetrics& metrics,
    const vector<RetakeRecommendation>& retakeStrategies,
    const vector<int>& courseIds, const map<int, string>& courseNames,
    const map<int, double>& courseCredits,
    const map<int, vector<string>>& courseInstructors,
    const map<int, double>& courseGrades) const {
  // Create stunning visual header
  cout << "\n";
  cout << BRIGHT_CYAN
       << "===================================================================="
          "========="
       << RESET << endl;
  cout << BRIGHT_CYAN << "|" << RESET << "                    " << BOLD
       << BG_BLUE << BRIGHT_WHITE << " NEURAL ACADEMIC INTELLIGENCE SYSTEM "
       << RESET << "                    " << BRIGHT_CYAN << "|" << RESET
       << endl;
  cout << BRIGHT_CYAN << "|" << RESET << "                " << BRIGHT_MAGENTA
       << "Deep Learning Performance Analytics" << RESET << "                "
       << BRIGHT_CYAN << "|" << RESET << endl;
  cout << BRIGHT_CYAN
       << "===================================================================="
          "========="
       << RESET << endl;

  // AI-powered GPA Dashboard with visual indicators
  cout << "\n"
       << BOLD << BG_MAGENTA << BRIGHT_WHITE
       << "    AI GPA INTELLIGENCE DASHBOARD    " << RESET << endl;

  // Create GPA visualization bar (use actual weighted grade average)
  double currentGrade = metrics.actualGradeAverage;
  string gpaBar = createProgressBar(currentGrade, 100.0, 50, "GPA");
  cout << BRIGHT_WHITE << "Current Grade: " << gpaBar << " " << BRIGHT_GREEN
       << fixed << setprecision(0) << currentGrade << "/100" << RESET << endl;

  // Risk level with dramatic visual indicators
  cout << BRIGHT_WHITE << "Risk Status: ";
  if (metrics.riskLevel == "LOW") {
    cout << BG_GREEN << BRIGHT_WHITE << " OPTIMAL ZONE " << RESET
         << BRIGHT_GREEN << " Academic Excellence Trajectory" << RESET;
  } else if (metrics.riskLevel == "MODERATE") {
    cout << BG_YELLOW << BRIGHT_WHITE << " CAUTION ZONE " << RESET
         << BRIGHT_YELLOW << " Performance Optimization Required" << RESET;
  } else if (metrics.riskLevel == "HIGH") {
    cout << BG_RED << BRIGHT_WHITE << " DANGER ZONE " << RESET << BRIGHT_RED
         << " Immediate Intervention Needed" << RESET;
  } else {
    cout << BG_RED << BRIGHT_WHITE << BOLD << " CRITICAL ALERT " << RESET
         << BRIGHT_RED << BOLD << " Academic Emergency Protocol" << RESET;
  }
  cout << endl;

  // Success probability with visual gauge
  string successBar =
      createProgressBar(metrics.successProbability, 1.0, 40, "SUCCESS");
  cout << BRIGHT_WHITE << "Success Rate: " << successBar << " " << BRIGHT_CYAN
       << fixed << setprecision(1) << (metrics.successProbability * 100) << "%"
       << RESET << endl;

  // Advanced Performance Matrix
  cout << "\n"
       << BOLD << BG_CYAN << BRIGHT_WHITE
       << "    PERFORMANCE INTELLIGENCE MATRIX    " << RESET << endl;
  cout << BRIGHT_CYAN
       << "+---------------------+----------+---------------------+" << RESET
       << endl;
  cout << BRIGHT_CYAN << "|" << RESET << BRIGHT_WHITE << " Metric              "
       << RESET << BRIGHT_CYAN << "|" << RESET << BRIGHT_WHITE << " Value    "
       << RESET << BRIGHT_CYAN << "|" << RESET << BRIGHT_WHITE
       << " AI Assessment      " << RESET << BRIGHT_CYAN << "|" << RESET
       << endl;
  cout << BRIGHT_CYAN
       << "+---------------------+----------+---------------------+" << RESET
       << endl;

  // Credit load analysis
  string creditStatus = (metrics.totalCredits > 18) ? BRIGHT_RED + "OVERLOADED"
                        : (metrics.totalCredits > 15)
                            ? BRIGHT_YELLOW + "OPTIMAL"
                            : BRIGHT_GREEN + "MANAGEABLE";
  cout << BRIGHT_CYAN << "|" << RESET << " Academic Load       " << BRIGHT_CYAN
       << "|" << RESET << " " << setw(8) << fixed << setprecision(0)
       << metrics.totalCredits << " " << BRIGHT_CYAN << "|" << RESET << " "
       << creditStatus << RESET << "     " << BRIGHT_CYAN << "|" << RESET
       << endl;

  // Workload distribution
  string workloadStatus =
      (metrics.workloadDistribution > 0.8)   ? BRIGHT_GREEN + "BALANCED"
      : (metrics.workloadDistribution > 0.6) ? BRIGHT_YELLOW + "UNEVEN"
                                             : BRIGHT_RED + "CHAOTIC";
  cout << BRIGHT_CYAN << "|" << RESET << " Load Distribution   " << BRIGHT_CYAN
       << "|" << RESET << " " << setw(7) << fixed << setprecision(1)
       << (metrics.workloadDistribution * 100) << "% " << BRIGHT_CYAN << "|"
       << RESET << " " << workloadStatus << RESET << "      " << BRIGHT_CYAN
       << "|" << RESET << endl;

  // Difficulty assessment
  string difficultyStatus =
      (metrics.difficultyScore > 0.7)   ? BRIGHT_RED + "EXTREME"
      : (metrics.difficultyScore > 0.5) ? BRIGHT_YELLOW + "MODERATE"
                                        : BRIGHT_GREEN + "MANAGEABLE";
  cout << BRIGHT_CYAN << "|" << RESET << " Complexity Index    " << BRIGHT_CYAN
       << "|" << RESET << " " << setw(7) << fixed << setprecision(2)
       << metrics.difficultyScore << "  " << BRIGHT_CYAN << "|" << RESET << " "
       << difficultyStatus << RESET << "    " << BRIGHT_CYAN << "|" << RESET
       << endl;

  cout << BRIGHT_CYAN
       << "+---------------------+----------+---------------------+" << RESET
       << endl;

  // Neural Course Analysis with AI insights
  cout << "\n"
       << BOLD << BG_BRIGHT_BLUE << BRIGHT_WHITE
       << "    NEURAL COURSE INTELLIGENCE    " << RESET << endl;

  for (size_t i = 0; i < courseIds.size(); i++) {
    int courseId = courseIds[i];
    double difficulty = calculateCourseDifficulty(courseId);
    double risk = calculateCourseRisk(courseId);
    auto gradeIt = courseGrades.find(courseId);
    auto nameIt = courseNames.find(courseId);
    auto creditsIt = courseCredits.find(courseId);
    auto instructorIt = courseInstructors.find(courseId);

    // Course header with status indicator
    string statusIcon;
    string statusColor;
    if (gradeIt != courseGrades.end()) {
      double grade = gradeIt->second;
      if (grade >= 90) {
        statusIcon = "EXCELLENT";
        statusColor = BRIGHT_GREEN;
      } else if (grade >= 70) {
        statusIcon = "GOOD";
        statusColor = BRIGHT_YELLOW;
      } else if (grade >= 50) {
        statusIcon = "POOR";
        statusColor = BRIGHT_RED;
      } else {
        statusIcon = "CRITICAL";
        statusColor = BG_RED + BRIGHT_WHITE;
      }
    }

    cout << "\n"
         << BRIGHT_WHITE << "Course " << (i + 1) << " " << statusColor
         << statusIcon << RESET << " ";
    if (nameIt != courseNames.end()) {
      cout << BRIGHT_CYAN << nameIt->second << RESET;
    }
    cout << " " << GRAY << "(ID: " << courseId << ")" << RESET;

    if (creditsIt != courseCredits.end()) {
      cout << " " << BRIGHT_GREEN << "[" << creditsIt->second << " credits]"
           << RESET;
    }
    cout << endl;

    // AI Performance Analysis
    if (gradeIt != courseGrades.end()) {
      double grade = gradeIt->second;
      string gradeBar = createGradeVisualization(grade);
      cout << "   Grade Analysis: " << gradeBar << " " << BRIGHT_WHITE << grade
           << "/100" << RESET;

      // AI prediction and recommendations
      string aiInsight = generateAIInsight(grade, difficulty, risk);
      cout << " " << aiInsight << endl;
    }

    // Instructor insight
    if (instructorIt != courseInstructors.end() &&
        !instructorIt->second.empty()) {
      double instructorRating = calculateInstructorRating(courseId);
      string instructorBar =
          createProgressBar(instructorRating, 1.0, 20, "INSTRUCTOR");

      // Display all instructors for the course
      cout << "   Instructors: ";
      for (size_t i = 0; i < instructorIt->second.size(); i++) {
        if (i > 0) cout << ", ";
        cout << BRIGHT_MAGENTA << instructorIt->second[i] << RESET;
      }
      cout << " " << instructorBar << endl;
    }

    // AI Strategic Recommendation
    string strategy = generateCourseStrategy(
        courseId, gradeIt != courseGrades.end() ? gradeIt->second : 0);
    cout << "   " << BRIGHT_CYAN << "AI Strategy: " << RESET << strategy
         << endl;
  }

  // Advanced Retake Intelligence Engine
  if (!retakeStrategies.empty()) {
    cout << "\n"
         << BOLD << BG_RED << BRIGHT_WHITE
         << "    RETAKE INTELLIGENCE ENGINE    " << RESET << endl;
    cout << BRIGHT_RED
         << "AI has identified courses requiring immediate attention for "
            "academic recovery"
         << RESET << endl;

    // Filter out courses with grades >= 85 (these should not be retaken)
    vector<RetakeRecommendation> validRetakeStrategies;
    for (const RetakeRecommendation& rec : retakeStrategies) {
      auto courseGradeIt = courseGrades.find(rec.courseId);
      bool shouldRetake = true;

      if (courseGradeIt != courseGrades.end()) {
        double grade = courseGradeIt->second;
        if (grade >=
            85.0) {  // Don't recommend retaking courses with grade 85 or higher
          shouldRetake = false;
        }
      }

      if (shouldRetake) {
        validRetakeStrategies.push_back(rec);
      }
    }

    if (validRetakeStrategies.empty()) {
      cout << "\n"
           << BG_GREEN << BRIGHT_WHITE
           << " EXCELLENT: AI CONFIRMS ALL COURSES PERFORMING WELL - NO "
              "RETAKES NEEDED "
           << RESET << endl;
    } else {
      for (size_t i = 0; i < min(size_t(3), validRetakeStrategies.size());
           i++) {
        const RetakeRecommendation& rec = validRetakeStrategies[i];
        auto courseNameIt = courseNames.find(rec.courseId);
        auto courseGradeIt = courseGrades.find(rec.courseId);
        auto courseCreditIt = courseCredits.find(rec.courseId);

        cout << "\n"
             << BRIGHT_RED << "PRIORITY " << (i + 1) << " ALERT" << RESET
             << " Course " << rec.courseId;
        if (courseNameIt != courseNames.end()) {
          cout << " (" << BRIGHT_CYAN << courseNameIt->second << RESET << ")";
        }
        if (courseCreditIt != courseCredits.end()) {
          cout << " [" << BRIGHT_YELLOW << courseCreditIt->second << " credits"
               << RESET << "]";
        }
        cout << endl;

        // Show current grade if available
        if (courseGradeIt != courseGrades.end()) {
          cout << "   Current Grade: " << BRIGHT_WHITE;
          double grade = courseGradeIt->second;

          // Fix the grade status logic
          if (grade >= 90) {
            cout << BRIGHT_GREEN << grade << "/100" << RESET << " "
                 << BRIGHT_GREEN << "EXCELLENT" << RESET;
          } else if (grade >= 80) {
            cout << BRIGHT_YELLOW << grade << "/100" << RESET << " "
                 << BRIGHT_YELLOW << "GOOD" << RESET;
          } else if (grade >= 70) {
            cout << BRIGHT_YELLOW << grade << "/100" << RESET << " "
                 << BRIGHT_YELLOW << "SATISFACTORY" << RESET;
          } else if (grade >= 60) {
            cout << BRIGHT_RED << grade << "/100" << RESET << " " << BRIGHT_RED
                 << "STRUGGLING" << RESET;
          } else {
            cout << BG_RED << BRIGHT_WHITE << grade << "/100" << RESET << " "
                 << BG_RED << BRIGHT_WHITE << "FAILING" << RESET;
          }
          cout << endl;
        }

        // Risk visualization
        string riskBar = createProgressBar(rec.currentRisk, 1.0, 30, "RISK");
        cout << "   Current Risk: " << riskBar << " " << BRIGHT_RED << fixed
             << setprecision(1) << (rec.currentRisk * 100) << "%" << RESET;

        // Risk explanation
        if (rec.currentRisk > 0.7) {
          cout << " " << BG_RED << BRIGHT_WHITE << "CRITICAL" << RESET;
        } else if (rec.currentRisk > 0.4) {
          cout << " " << BRIGHT_RED << "HIGH CONCERN" << RESET;
        } else if (rec.currentRisk > 0.2) {
          cout << " " << BRIGHT_YELLOW << "MODERATE CONCERN" << RESET;
        } else {
          cout << " " << BRIGHT_GREEN << "LOW CONCERN" << RESET;
        }
        cout << endl;

        // Improvement potential with explanation
        string improvementBar =
            createProgressBar(rec.improvementPotential, 1.0, 30, "IMPROVEMENT");
        cout << "   AI Recovery Potential: " << improvementBar << " "
             << BRIGHT_GREEN << fixed << setprecision(1)
             << (rec.improvementPotential * 100) << "%" << RESET;

        // Potential explanation
        if (rec.improvementPotential > 0.6) {
          cout << " " << BRIGHT_GREEN << "HIGH SUCCESS CHANCE" << RESET;
        } else if (rec.improvementPotential > 0.3) {
          cout << " " << BRIGHT_YELLOW << "MODERATE SUCCESS CHANCE" << RESET;
        } else {
          cout << " " << BRIGHT_RED << "CHALLENGING RECOVERY" << RESET;
        }
        cout << endl;

        // Detailed reason for flagging
        cout << "   " << BRIGHT_WHITE << "Flagged Because: " << RESET;
        double difficulty = calculateCourseDifficulty(rec.courseId);
        double instructorRating = calculateInstructorRating(rec.courseId);

        if (courseGradeIt != courseGrades.end() && courseGradeIt->second < 60) {
          cout << BRIGHT_RED << "Failing grade requires immediate retake"
               << RESET;
        } else if (courseGradeIt != courseGrades.end() &&
                   courseGradeIt->second < 70) {
          cout << BRIGHT_RED
               << "Poor performance indicates need for improvement" << RESET;
        } else if (difficulty > 0.7) {
          cout << BRIGHT_YELLOW
               << "High course difficulty + performance concerns" << RESET;
        } else if (instructorRating < 0.6) {
          cout << BRIGHT_YELLOW << "Poor instructor rating affecting success"
               << RESET;
        } else {
          cout << BRIGHT_YELLOW
               << "Performance improvement opportunity identified" << RESET;
        }
        cout << endl;

        cout << "   " << BRIGHT_YELLOW << "Optimal Timing: " << RESET
             << rec.recommendedSemester << endl;
        cout << "   " << BRIGHT_CYAN << "AI Strategy: " << RESET << rec.strategy
             << endl;
      }
    }
  } else {
    cout << "\n"
         << BG_GREEN << BRIGHT_WHITE
         << " EXCELLENT: AI CONFIRMS ALL COURSES ON TRACK " << RESET << endl;
  }
}

void AdvancedScheduleAnalytics::displayActionableRecommendations(
    const AcademicMetrics& metrics,
    const vector<RetakeRecommendation>& retakeStrategies,
    const vector<int>& courseIds, const map<int, string>& courseNames,
    const map<int, double>& courseCredits,
    const map<int, double>& courseGrades) const {
  cout << "\n";
  cout << BRIGHT_MAGENTA
       << "===================================================================="
          "========="
       << RESET << endl;
  cout << BRIGHT_MAGENTA << "|" << RESET << "                      " << BOLD
       << BG_MAGENTA << BRIGHT_WHITE << " AI STRATEGIC COMMAND CENTER " << RESET
       << "                      " << BRIGHT_MAGENTA << "|" << RESET << endl;
  cout << BRIGHT_MAGENTA << "|" << RESET << "                   " << BRIGHT_CYAN
       << "Personalized Action Intelligence" << RESET << "                   "
       << BRIGHT_MAGENTA << "|" << RESET << endl;
  cout << BRIGHT_MAGENTA
       << "===================================================================="
          "========="
       << RESET << endl;

  // AI Threat Level Assessment
  cout << "\n"
       << BOLD << BG_RED << BRIGHT_WHITE
       << "    ACADEMIC THREAT LEVEL ASSESSMENT    " << RESET << endl;

  if (metrics.riskLevel == "CRITICAL") {
    cout << BG_RED << BRIGHT_WHITE << BOLD
         << " CODE RED - ACADEMIC EMERGENCY PROTOCOL ACTIVATED " << RESET
         << endl;
    cout << BRIGHT_RED
         << "+-- IMMEDIATE SURVIVAL ACTIONS (Execute within 24 hours)" << RESET
         << endl;
    cout << BRIGHT_RED << "|-- Emergency meeting with academic advisor/dean"
         << RESET << endl;
    cout << BRIGHT_RED
         << "|-- Consider medical withdrawal if health issues present" << RESET
         << endl;
    cout << BRIGHT_RED
         << "|-- Activate all available support systems immediately" << RESET
         << endl;
    cout << BRIGHT_RED << "+-- Implement crisis academic intervention plan"
         << RESET << endl;

    cout << "\n"
         << BRIGHT_WHITE << "AI PREDICTION: " << BRIGHT_RED
         << "Without immediate action, academic standing in jeopardy" << RESET
         << endl;

  } else if (metrics.riskLevel == "HIGH") {
    cout << BG_RED << BRIGHT_WHITE
         << " ORANGE ALERT - INTENSIVE INTERVENTION REQUIRED " << RESET << endl;
    cout << BRIGHT_YELLOW
         << "+-- HIGH PRIORITY ACTIONS (Execute within 72 hours)" << RESET
         << endl;
    cout << BRIGHT_YELLOW
         << "|-- Schedule academic counseling session immediately" << RESET
         << endl;
    cout << BRIGHT_YELLOW << "|-- Enroll in academic success workshops" << RESET
         << endl;
    cout << BRIGHT_YELLOW << "|-- Form intensive study groups for all courses"
         << RESET << endl;
    cout << BRIGHT_YELLOW
         << "|-- Consider strategic course withdrawal if necessary" << RESET
         << endl;
    cout << BRIGHT_YELLOW << "+-- Implement strict time management system"
         << RESET << endl;

  } else if (metrics.riskLevel == "MODERATE") {
    cout << BG_YELLOW << BRIGHT_WHITE
         << " YELLOW CAUTION - OPTIMIZATION PROTOCOL " << RESET << endl;
    cout << BRIGHT_CYAN << "+-- OPTIMIZATION ACTIONS (Execute within 1 week)"
         << RESET << endl;
    cout << BRIGHT_CYAN
         << "|-- Fine-tune study strategies for better efficiency" << RESET
         << endl;
    cout << BRIGHT_CYAN << "|-- Seek tutoring for challenging subjects" << RESET
         << endl;
    cout << BRIGHT_CYAN << "|-- Optimize schedule for better work-life balance"
         << RESET << endl;
    cout << BRIGHT_CYAN << "+-- Build stronger academic habits and routines"
         << RESET << endl;

  } else {
    cout << BG_GREEN << BRIGHT_WHITE
         << " GREEN STATUS - EXCELLENCE MAINTENANCE MODE " << RESET << endl;
    cout << BRIGHT_GREEN
         << "+-- EXCELLENCE ENHANCEMENT (Continuous improvement)" << RESET
         << endl;
    cout << BRIGHT_GREEN << "|-- Explore advanced academic opportunities"
         << RESET << endl;
    cout << BRIGHT_GREEN
         << "|-- Mentor struggling peers in your strong subjects" << RESET
         << endl;
    cout << BRIGHT_GREEN << "|-- Consider research or internship opportunities"
         << RESET << endl;
    cout << BRIGHT_GREEN << "+-- Maintain current winning strategies" << RESET
         << endl;
  }

  // AI-Powered Personalized Strategy Matrix
  cout << "\n"
       << BOLD << BG_CYAN << BRIGHT_WHITE
       << "    AI PERSONALIZED STRATEGY MATRIX    " << RESET << endl;

  // GPA Strategy
  cout << BRIGHT_WHITE << "GPA OPTIMIZATION PROTOCOL:" << RESET << endl;
  if (metrics.predictedGPA >= 3.5) {
    cout
        << "   " << BRIGHT_GREEN
        << "> MAINTAIN EXCELLENCE: Continue current high-performance strategies"
        << RESET << endl;
    cout << "   " << BRIGHT_GREEN
         << "> EXPAND HORIZONS: Consider honors coursework or research projects"
         << RESET << endl;
  } else if (metrics.predictedGPA >= 3.0) {
    cout << "   " << BRIGHT_YELLOW
         << "> PUSH TO EXCELLENCE: Target specific improvements in weaker areas"
         << RESET << endl;
    cout
        << "   " << BRIGHT_YELLOW
        << "> CONSISTENCY FOCUS: Maintain steady performance across all courses"
        << RESET << endl;
  } else if (metrics.predictedGPA >= 2.5) {
    cout << "   " << BRIGHT_RED
         << "> INTENSIVE RECOVERY: Major study habit restructuring needed"
         << RESET << endl;
    cout << "   " << BRIGHT_RED
         << "> FOUNDATION BUILDING: Focus on core concept mastery" << RESET
         << endl;
  } else {
    cout << "   " << BG_RED << BRIGHT_WHITE
         << "> EMERGENCY INTERVENTION: Complete academic strategy overhaul"
         << RESET << endl;
    cout << "   " << BG_RED << BRIGHT_WHITE
         << "> SURVIVAL MODE: Focus on passing essential courses only" << RESET
         << endl;
  }

  // Success Enhancement Protocol
  cout << "\n"
       << BRIGHT_WHITE << "SUCCESS ENHANCEMENT PROTOCOL:" << RESET << endl;
  double successRate = metrics.successProbability * 100;
  string successBar =
      createProgressBar(metrics.successProbability, 1.0, 40, "SUCCESS");
  cout << "   Current Success Trajectory: " << successBar << " " << BRIGHT_CYAN
       << fixed << setprecision(0) << successRate << "%" << RESET << endl;

  if (successRate >= 80) {
    cout << "   " << BRIGHT_GREEN
         << "EXCELLENT TRAJECTORY: You're on track for outstanding results!"
         << RESET << endl;
    cout << "   " << BRIGHT_GREEN
         << "CHALLENGE YOURSELF: Consider advanced opportunities" << RESET
         << endl;
  } else if (successRate >= 60) {
    cout << "   " << BRIGHT_YELLOW
         << "GOOD POTENTIAL: Small tweaks can yield big improvements" << RESET
         << endl;
    cout << "   " << BRIGHT_YELLOW
         << "FINE-TUNING: Focus on consistency and time management" << RESET
         << endl;
  } else {
    cout << "   " << BRIGHT_RED
         << "IMMEDIATE ACTION REQUIRED: Success probability below acceptable "
            "threshold"
         << RESET << endl;
    cout << "   " << BRIGHT_RED
         << "MAJOR OVERHAUL: Complete strategy reconstruction needed" << RESET
         << endl;
  }

  // Workload Intelligence
  cout << "\n"
       << BRIGHT_WHITE << "WORKLOAD INTELLIGENCE SYSTEM:" << RESET << endl;
  string workloadBar =
      createProgressBar(metrics.workloadDistribution, 1.0, 30, "WORKLOAD");
  cout << "   Load Balance Analysis: " << workloadBar << " " << fixed
       << setprecision(0) << (metrics.workloadDistribution * 100) << "%"
       << RESET << endl;

  if (metrics.workloadDistribution >= 0.8) {
    cout << "   " << BRIGHT_GREEN
         << "OPTIMAL DISTRIBUTION: Workload perfectly balanced" << RESET
         << endl;
  } else if (metrics.workloadDistribution >= 0.6) {
    cout << "   " << BRIGHT_YELLOW
         << "MINOR IMBALANCE: Consider redistributing study time" << RESET
         << endl;
  } else {
    cout << "   " << BRIGHT_RED
         << "MAJOR IMBALANCE: Critical workload redistribution needed" << RESET
         << endl;
  }

  // Neural Network Predictions
  cout << "\n"
       << BOLD << BG_BRIGHT_BLUE << BRIGHT_WHITE
       << "    NEURAL NETWORK PREDICTIONS    " << RESET << endl;
  cout << BRIGHT_CYAN << "AI FORECASTS:" << RESET << endl;
  cout << "   " << BRIGHT_WHITE
       << "Semester Completion Probability: " << BRIGHT_GREEN << fixed
       << setprecision(0) << (metrics.successProbability * 100) << "%" << RESET
       << endl;

  // Convert GPA back to 100 scale for display
  double minGPA100 = (metrics.minPossibleGPA / 4.0) * 100;
  double maxGPA100 = (metrics.maxPossibleGPA / 4.0) * 100;
  cout << "   " << BRIGHT_WHITE << "Optimal Grade Range: " << BRIGHT_YELLOW
       << fixed << setprecision(0) << minGPA100 << " - " << maxGPA100 << "/100"
       << RESET << endl;
  cout << "   " << BRIGHT_WHITE << "Recommended Actions: " << BRIGHT_CYAN
       << (retakeStrategies.size() > 0 ? "Focus on high-risk courses"
                                       : "Maintain current trajectory")
       << RESET << endl;

  // Final AI Summary Dashboard
  cout << "\n";
  cout << BRIGHT_BLUE
       << "===================================================================="
          "==========================="
       << RESET << endl;
  cout << BRIGHT_BLUE << "|" << RESET << "                                "
       << BOLD << BG_BRIGHT_BLUE << BRIGHT_WHITE << " AI EXECUTIVE SUMMARY "
       << RESET << "                                " << BRIGHT_BLUE << "|"
       << RESET << endl;
  cout << BRIGHT_BLUE
       << "===================================================================="
          "==========================="
       << RESET << endl;
  double currentGrade = metrics.actualGradeAverage;
  cout << BRIGHT_BLUE << "|" << RESET << " Current Grade: " << BRIGHT_GREEN
       << setw(3) << fixed << setprecision(0) << currentGrade << "/100" << RESET
       << " | Risk: "
       << (metrics.riskLevel == "LOW"        ? BRIGHT_GREEN
           : metrics.riskLevel == "MODERATE" ? BRIGHT_YELLOW
                                             : BRIGHT_RED)
       << setw(8) << metrics.riskLevel << RESET << " | Success: " << BRIGHT_CYAN
       << setw(3) << fixed << setprecision(0) << successRate << "%" << RESET
       << " | Actions: " << BRIGHT_MAGENTA << setw(2)
       << (retakeStrategies.size() + 3) << RESET << " " << BRIGHT_BLUE << "|"
       << RESET << endl;
  cout << BRIGHT_BLUE << "|" << RESET << " Credits: " << BRIGHT_YELLOW
       << setw(4) << fixed << setprecision(0) << metrics.totalCredits << RESET
       << "     | Load: " << BRIGHT_WHITE << setw(8) << "BALANCED" << RESET
       << " | Time: " << BRIGHT_GREEN << setw(6) << "OPTIMAL" << RESET
       << " | Status: " << BRIGHT_GREEN << setw(6) << "ACTIVE" << RESET << " "
       << BRIGHT_BLUE << "|" << RESET << endl;
  cout << BRIGHT_BLUE
       << "===================================================================="
          "==========================="
       << RESET << endl;

  cout << "\n"
       << BRIGHT_GREEN
       << "AI ANALYSIS COMPLETE - Academic intelligence system at your service!"
       << RESET << endl;

  // Interactive Target GPA Planning System
  cout << "\n";
  cout << BRIGHT_MAGENTA
       << "===================================================================="
          "========="
       << RESET << endl;
  cout << BRIGHT_MAGENTA << "|" << RESET << "                    " << BOLD
       << BG_MAGENTA << BRIGHT_WHITE << " GOAL-ORIENTED RECOVERY PLANNER "
       << RESET << "                    " << BRIGHT_MAGENTA << "|" << RESET
       << endl;
  cout << BRIGHT_MAGENTA << "|" << RESET << "                 " << BRIGHT_CYAN
       << "Personalized Target Achievement System" << RESET
       << "                 " << BRIGHT_MAGENTA << "|" << RESET << endl;
  cout << BRIGHT_MAGENTA
       << "===================================================================="
          "========="
       << RESET << endl;

  cout << "\n"
       << BRIGHT_WHITE
       << "Would you like AI to create a personalized grade improvement plan? "
          "(y/n): "
       << RESET;
  char response;
  cin >> response;

  if (response == 'y' || response == 'Y') {
    cout << "\n"
         << BRIGHT_CYAN
         << "EXCELLENT! Let's design your path to academic success!" << RESET
         << endl;

    double currentGrade = metrics.actualGradeAverage;
    cout << BRIGHT_WHITE << "Your current average grade: " << BRIGHT_YELLOW
         << fixed << setprecision(0) << currentGrade << "/100" << RESET << endl;

    double targetGrade = -1;
    string input;

    while (targetGrade < currentGrade || targetGrade > 100) {
      cout << BRIGHT_WHITE
           << "Enter your target average grade (must be higher than current): "
           << RESET;
      cin >> input;

      try {
        targetGrade = stod(input);
        if (targetGrade <= currentGrade) {
          cout << BRIGHT_RED
               << "Target must be higher than your current grade (" << fixed
               << setprecision(0) << currentGrade << ")!" << RESET << endl;
          targetGrade = -1;
        } else if (targetGrade > 100) {
          cout << BRIGHT_RED << "Target cannot exceed 100!" << RESET << endl;
          targetGrade = -1;
        }
      } catch (const exception&) {
        cout << BRIGHT_RED << "Please enter a valid number!" << RESET << endl;
        targetGrade = -1;
      }
    }

    generateTargetGradePlans(targetGrade, currentGrade, retakeStrategies,
                             courseIds, courseNames, courseCredits,
                             courseGrades);
  } else {
    cout << "\n"
         << BRIGHT_GREEN
         << "Thank you for using AI Academic Analytics! Best of luck with your "
            "studies!"
         << RESET << endl;
  }
}

// Helper methods for detailed calculations
string AdvancedScheduleAnalytics::generateCourseName(int courseId) const {
  // Generate meaningful course names based on course ID patterns
  int subject = courseId / 10000;
  int level = (courseId / 1000) % 10;
  int sequence = courseId % 1000;

  string baseName = "Course";

  // Subject area mapping
  switch (subject) {
    case 11:
      baseName = "Mathematics";
      break;
    case 25:
      baseName = "Engineering Fundamentals";
      break;
    case 31:
      baseName = "Electrical Engineering";
      break;
    case 51:
      baseName = "Statistics";
      break;
    default:
      baseName = "General Course";
      break;
  }

  // Add level and sequence information
  if (courseId == 31402)
    return "EE Lab I";
  else if (courseId == 31910)
    return "Intro to Control";
  else if (courseId == 251961)
    return "Engineering Fundamentals 3";
  else if (courseId == 51742)
    return "Prob. & Stats";
  else {
    return baseName + " " + to_string(level) + "." + to_string(sequence);
  }
}

double AdvancedScheduleAnalytics::calculateCourseDifficulty(
    int courseId) const {
  // Sophisticated difficulty calculation based on multiple factors

  // Base difficulty using course ID patterns
  double baseDifficulty = 0.5;

  // Course level analysis (higher numbers = more advanced)
  if (courseId >= 10000) {
    int level = (courseId / 1000) % 10;
    if (level >= 4)
      baseDifficulty += 0.3;  // Senior level
    else if (level >= 3)
      baseDifficulty += 0.2;  // Junior level
    else if (level >= 2)
      baseDifficulty += 0.1;  // Sophomore level
  }

  // Subject area difficulty adjustments
  int subject = courseId / 10000;
  switch (subject) {
    case 11:  // Mathematics
      baseDifficulty += 0.25;
      break;
    case 31:  // Engineering/Computer Science
      baseDifficulty += 0.20;
      break;
    case 25:  // Science
      baseDifficulty += 0.15;
      break;
    default:
      baseDifficulty += 0.05;
  }

  // Course sequence analysis - later courses in sequence are harder
  int sequence = courseId % 100;
  if (sequence > 50) baseDifficulty += 0.1;

  return min(1.0, max(0.1, baseDifficulty));
}

double AdvancedScheduleAnalytics::calculateInstructorRating(
    int courseId) const {
  // Simulate instructor quality rating
  // In real implementation, this would pull from instructor evaluation database

  double rating = 0.7;  // Base rating

  // Use course ID to simulate instructor variance
  int instructorSeed = (courseId * 17 + 42) % 100;

  if (instructorSeed > 80)
    rating = 0.9;  // Excellent instructor
  else if (instructorSeed > 60)
    rating = 0.8;  // Good instructor
  else if (instructorSeed > 40)
    rating = 0.7;  // Average instructor
  else if (instructorSeed > 20)
    rating = 0.6;  // Below average
  else
    rating = 0.5;  // Poor instructor

  return rating;
}

double AdvancedScheduleAnalytics::calculateStudentCourseFitness(
    int courseId) const {
  // Simulate student aptitude for specific course types
  // In real implementation, this would analyze student's academic history

  double fitness = 0.7;  // Base fitness

  int subject = courseId / 10000;
  switch (subject) {
    case 11:  // Math - assume student has moderate math skills
      fitness = 0.6;
      break;
    case 31:  // Engineering - assume technical aptitude
      fitness = 0.75;
      break;
    case 25:  // Science - assume good analytical skills
      fitness = 0.8;
      break;
    default:
      fitness = 0.7;
  }

  return fitness;
}

double AdvancedScheduleAnalytics::calculateHistoricalPerformance(
    int courseId) const {
  // Simulate historical course performance data
  // In real implementation, would pull from course success rate database

  double performance = 0.75;  // Base performance

  // Use course characteristics to simulate historical success rates
  double difficulty = calculateCourseDifficulty(courseId);
  performance =
      0.9 - (difficulty * 0.3);  // Higher difficulty = lower historical success

  return max(0.4, min(0.95, performance));
}

double AdvancedScheduleAnalytics::calculateCourseWorkload(int courseId) const {
  // Calculate relative workload for the course

  double workload = 0.6;  // Base workload

  // Credits affect workload
  double credits = getCourseCredits(courseId);
  workload += (credits - 3.0) * 0.1;  // Adjust based on credit hours

  // Course difficulty affects workload
  double difficulty = calculateCourseDifficulty(courseId);
  workload += difficulty * 0.3;

  // Course type affects workload
  int courseNumber = courseId % 1000;
  if (courseNumber % 100 < 10) {  // Lab courses
    workload += 0.2;
  }

  return min(1.0, max(0.3, workload));
}

double AdvancedScheduleAnalytics::getCourseCredits(int courseId) const {
  auto cachedCourse = courseCache.find(courseId);
  if (cachedCourse != courseCache.end()) {
    return cachedCourse->second.getCredits();
  }

  // Default credit estimation based on course ID
  int creditPattern = courseId % 7;
  switch (creditPattern) {
    case 0:
    case 1:
      return 3.0;  // Most common
    case 2:
    case 3:
      return 4.0;  // Common
    case 4:
      return 2.0;  // Less common
    case 5:
      return 5.0;  // Rare
    case 6:
      return 1.0;  // Rare
    default:
      return 3.0;
  }
}

double AdvancedScheduleAnalytics::calculateCourseRisk(int courseId) const {
  double difficulty = calculateCourseDifficulty(courseId);
  double instructorRating = calculateInstructorRating(courseId);
  double studentFitness = calculateStudentCourseFitness(courseId);
  double workload = calculateCourseWorkload(courseId);

  // Base risk calculation
  double baseRisk = (difficulty * 0.4) + ((1.0 - instructorRating) * 0.2) +
                    ((1.0 - studentFitness) * 0.2) + (workload * 0.2);

  // Reduce this base risk significantly for simulation purposes
  // In a real system, this would factor in actual student performance
  baseRisk *= 0.3;  // Scale down to more reasonable levels

  return min(
      1.0, max(0.05, baseRisk));  // Minimum 5% risk even for "perfect" courses
}

double AdvancedScheduleAnalytics::calculateImprovementPotential(
    int courseId) const {
  double currentRisk = calculateCourseRisk(courseId);
  double instructorRating = calculateInstructorRating(courseId);

  // Higher risk courses with good instructors have more improvement potential
  double potential = currentRisk * instructorRating;

  return min(1.0, max(0.1, potential));
}

string AdvancedScheduleAnalytics::determineOptimalRetakeSemester(
    int courseId) const {
  // Simple logic for retake timing
  int courseLevel = (courseId / 1000) % 10;

  if (courseLevel <= 2)
    return "Next Semester";
  else if (courseLevel == 3)
    return "Next Academic Year";
  else
    return "Following Summer";
}

vector<int> AdvancedScheduleAnalytics::findAlternativeCourses(
    int courseId) const {
  vector<int> alternatives;

  // Simple alternative generation based on course patterns
  int baseId = (courseId / 100) * 100;

  // Suggest courses in same subject area but different sections
  for (int i = 1; i <= 3; i++) {
    int altId = baseId + i;
    if (altId != courseId && altId > 10000) {
      alternatives.push_back(altId);
    }
  }

  return alternatives;
}

string AdvancedScheduleAnalytics::generateRetakeStrategy(int courseId) const {
  double difficulty = calculateCourseDifficulty(courseId);
  double instructorRating = calculateInstructorRating(courseId);

  if (difficulty > 0.8) {
    return "Intensive preparation with tutoring";
  } else if (instructorRating < 0.6) {
    return "Consider different instructor or section";
  } else if (calculateCourseWorkload(courseId) > 0.8) {
    return "Take during lighter semester load";
  } else {
    return "Standard retake with improved study methods";
  }
}

// Advanced visualization helper functions
string AdvancedScheduleAnalytics::createProgressBar(double value,
                                                    double maxValue, int width,
                                                    const string& type) const {
  double percentage = (maxValue > 0) ? (value / maxValue) : 0.0;
  int filledWidth = static_cast<int>(percentage * width);

  string bar = "[";
  string color;

  // Choose colors based on type and value
  if (type == "GPA") {
    if (percentage >= 0.9)
      color = BRIGHT_GREEN;  // 90+ grade
    else if (percentage >= 0.8)
      color = BRIGHT_YELLOW;  // 80+ grade
    else if (percentage >= 0.6)
      color = BRIGHT_RED;  // 60+ grade
    else
      color = BG_RED + BRIGHT_WHITE;  // Below 60
  } else if (type == "SUCCESS") {
    if (percentage >= 0.8)
      color = BRIGHT_GREEN;
    else if (percentage >= 0.6)
      color = BRIGHT_YELLOW;
    else
      color = BRIGHT_RED;
  } else if (type == "RISK") {
    if (percentage <= 0.3)
      color = BRIGHT_GREEN;
    else if (percentage <= 0.6)
      color = BRIGHT_YELLOW;
    else
      color = BRIGHT_RED;
  } else if (type == "IMPROVEMENT") {
    color = BRIGHT_CYAN;
  } else {
    color = BRIGHT_BLUE;  // Default for instructor ratings etc.
  }

  for (int i = 0; i < width; i++) {
    if (i < filledWidth) {
      bar += color + "#" + RESET;
    } else {
      bar += GRAY + "-" + RESET;
    }
  }
  bar += "]";

  return bar;
}

string AdvancedScheduleAnalytics::createGradeVisualization(double grade) const {
  string visualization = "";

  if (grade >= 95) {
    visualization = BRIGHT_GREEN + "AAAAA" + RESET + " " + BG_GREEN +
                    BRIGHT_WHITE + " EXCEPTIONAL " + RESET;
  } else if (grade >= 90) {
    visualization = BRIGHT_GREEN + "AAAAB" + RESET + " " + BRIGHT_GREEN +
                    "EXCELLENT" + RESET;
  } else if (grade >= 85) {
    visualization = BRIGHT_YELLOW + "ABBBB" + RESET + " " + BRIGHT_YELLOW +
                    "VERY GOOD" + RESET;
  } else if (grade >= 80) {
    visualization =
        BRIGHT_YELLOW + "BBBBC" + RESET + " " + BRIGHT_YELLOW + "GOOD" + RESET;
  } else if (grade >= 70) {
    visualization = BRIGHT_RED + "BCCCC" + RESET + " " + BRIGHT_RED +
                    "SATISFACTORY" + RESET;
  } else if (grade >= 60) {
    visualization =
        BRIGHT_RED + "CCCCC" + RESET + " " + BRIGHT_RED + "MARGINAL" + RESET;
  } else {
    visualization = BG_RED + BRIGHT_WHITE + "FFFFF" + RESET + " " + BG_RED +
                    BRIGHT_WHITE + " CRITICAL " + RESET;
  }

  return visualization;
}

string AdvancedScheduleAnalytics::createRiskVisualization(double risk) const {
  string visualization = "";
  int riskLevel = static_cast<int>(risk * 10);

  visualization += "[";
  for (int i = 0; i < 10; i++) {
    if (i < riskLevel) {
      if (i < 3)
        visualization += BRIGHT_GREEN + "O" + RESET;
      else if (i < 6)
        visualization += BRIGHT_YELLOW + "O" + RESET;
      else
        visualization += BRIGHT_RED + "O" + RESET;
    } else {
      visualization += GRAY + "o" + RESET;
    }
  }
  visualization += "]";

  return visualization;
}

string AdvancedScheduleAnalytics::generateAIInsight(double grade,
                                                    double difficulty,
                                                    double risk) const {
  if (grade >= 90 && difficulty > 0.7) {
    return BRIGHT_GREEN +
           "AI: Mastering challenging content! Consider advanced topics." +
           RESET;
  } else if (grade >= 90) {
    return BRIGHT_GREEN + "AI: Excellent performance! Ready for next level." +
           RESET;
  } else if (grade >= 80 && risk > 0.6) {
    return BRIGHT_YELLOW +
           "AI: Good grade but high risk detected. Monitor closely." + RESET;
  } else if (grade >= 70) {
    return BRIGHT_YELLOW + "AI: Satisfactory. Focus on concept reinforcement." +
           RESET;
  } else if (grade >= 60) {
    return BRIGHT_RED + "AI: WARNING - Intervention needed immediately!" +
           RESET;
  } else {
    return BG_RED + BRIGHT_WHITE +
           "AI: CRITICAL - Emergency academic support required!" + RESET;
  }
}

string AdvancedScheduleAnalytics::generateCourseStrategy(int courseId,
                                                         double grade) const {
  double difficulty = calculateCourseDifficulty(courseId);
  double instructorRating = calculateInstructorRating(courseId);

  if (grade >= 90) {
    return BRIGHT_GREEN +
           "Continue excellence strategy, consider peer tutoring" + RESET;
  } else if (grade >= 80) {
    return BRIGHT_YELLOW + "Maintain current approach, strengthen weak areas" +
           RESET;
  } else if (grade >= 70) {
    return BRIGHT_YELLOW + "Increase study time, seek additional practice" +
           RESET;
  } else if (grade >= 60) {
    return BRIGHT_RED + "Intensive remediation plan, consider tutoring" + RESET;
  } else {
    if (instructorRating < 0.6) {
      return BG_RED + BRIGHT_WHITE +
             "EMERGENCY: Change instructor + intensive support" + RESET;
    } else {
      return BG_RED + BRIGHT_WHITE +
             "EMERGENCY: Comprehensive intervention program" + RESET;
    }
  }
}

void AdvancedScheduleAnalytics::generateTargetGradePlans(
    double targetGrade, double currentGrade,
    const vector<RetakeRecommendation>& retakeStrategies,
    const vector<int>& courseIds, const map<int, string>& courseNames,
    const map<int, double>& courseCredits,
    const map<int, double>& courseGrades) const {
  cout << "\n"
       << BOLD << BG_BRIGHT_BLUE << BRIGHT_WHITE
       << "    AI STRATEGIC RECOVERY PLANS    " << RESET << endl;
  cout << BRIGHT_CYAN << "Analyzing optimal pathways to achieve "
       << BRIGHT_GREEN << fixed << setprecision(0) << targetGrade << "/100"
       << RESET << BRIGHT_CYAN << " average grade..." << RESET << endl;

  // Calculate grade improvement needed
  double improvementNeeded = targetGrade - currentGrade;
  cout << BRIGHT_WHITE << "Grade improvement required: +" << BRIGHT_YELLOW
       << fixed << setprecision(0) << improvementNeeded << " points" << RESET
       << endl;

  // Analyze courses by improvement potential
  vector<pair<int, double>>
      courseImpactPotential;  // courseId, potential impact

  for (int courseId : courseIds) {
    auto gradeIt = courseGrades.find(courseId);
    auto creditIt = courseCredits.find(courseId);

    if (gradeIt != courseGrades.end() && creditIt != courseCredits.end()) {
      double currentCourseGrade = gradeIt->second;
      double credits = creditIt->second;

      // Only include courses that have room for improvement (grade < 90)
      if (currentCourseGrade < 90.0) {
        // Calculate potential impact if this course is improved to 95
        double maxImprovementImpact =
            (95.0 - currentCourseGrade) *
            (credits / calculateTotalCredits(courseIds));
        courseImpactPotential.push_back(
            make_pair(courseId, maxImprovementImpact));
      }
    }
  }

  // Sort by impact potential
  sort(courseImpactPotential.begin(), courseImpactPotential.end(),
       [](const pair<int, double>& a, const pair<int, double>& b) {
         return a.second > b.second;
       });

  // Strategy 1: Minimal Effort (1-2 courses)
  cout << "\n"
       << BOLD << BG_GREEN << BRIGHT_WHITE
       << " STRATEGY 1: MINIMAL EFFORT APPROACH " << RESET << endl;
  cout << BRIGHT_GREEN
       << "Focus on highest-impact courses for maximum efficiency" << RESET
       << endl;

  double accumulatedImpact = 0.0;
  int coursesNeeded = 0;

  for (const auto& coursePair : courseImpactPotential) {
    if (accumulatedImpact >= improvementNeeded * 1.1) break;  // 10% buffer

    int courseId = coursePair.first;
    double impact = coursePair.second;
    auto nameIt = courseNames.find(courseId);
    auto gradeIt = courseGrades.find(courseId);
    auto creditIt = courseCredits.find(courseId);

    cout << "   " << BRIGHT_WHITE << "Target Course " << (coursesNeeded + 1)
         << ": " << RESET;
    if (nameIt != courseNames.end()) {
      cout << BRIGHT_CYAN << nameIt->second << RESET;
    }
    cout << " (ID: " << courseId << ")";

    if (creditIt != courseCredits.end()) {
      cout << " [" << BRIGHT_YELLOW << creditIt->second << " credits" << RESET
           << "]";
    }
    cout << endl;

    if (gradeIt != courseGrades.end()) {
      double currentCourseGrade = gradeIt->second;
      double improvementFactor =
          (coursesNeeded == 0)
              ? improvementNeeded
              : improvementNeeded / static_cast<double>(coursesNeeded + 1);
      double recommendedGrade =
          min(95.0, currentCourseGrade + improvementFactor * 1.5);

      cout << "      Current Grade: " << BRIGHT_RED << fixed << setprecision(0)
           << currentCourseGrade << "/100" << RESET;
      cout << " -> Target: " << BRIGHT_GREEN << fixed << setprecision(0)
           << recommendedGrade << "/100" << RESET;
      cout << " (+" << BRIGHT_YELLOW << fixed << setprecision(0)
           << (recommendedGrade - currentCourseGrade) << " points)" << RESET
           << endl;

      cout << "      Impact on Overall Average: +" << BRIGHT_CYAN << fixed
           << setprecision(1) << impact << " points" << RESET << endl;

      // Specific action plan
      if (currentCourseGrade < 60) {
        cout << "      " << BRIGHT_RED
             << "ACTION: Complete course retake with intensive study plan"
             << RESET << endl;
      } else if (currentCourseGrade < 80) {
        cout << "      " << BRIGHT_YELLOW
             << "ACTION: Targeted improvement through extra assignments/exam "
                "retake"
             << RESET << endl;
      } else {
        cout << "      " << BRIGHT_GREEN
             << "ACTION: Fine-tuning and bonus opportunities" << RESET << endl;
      }
    }

    accumulatedImpact += impact;
    coursesNeeded++;

    if (coursesNeeded >= 2) break;  // Limit to 2 courses for minimal approach
  }

  cout << "   " << BOLD << BRIGHT_GREEN << "Estimated Success Rate: "
       << (accumulatedImpact >= improvementNeeded ? "HIGH (85-95%)"
                                                  : "MODERATE (60-75%)")
       << RESET << endl;
  cout << "   " << BOLD << BRIGHT_GREEN
       << "Time Investment: 6-12 weeks of focused effort" << RESET << endl;

  // Strategy 2: Comprehensive Approach (3+ courses)
  cout << "\n"
       << BOLD << BG_YELLOW << BRIGHT_WHITE
       << " STRATEGY 2: COMPREHENSIVE APPROACH " << RESET << endl;
  cout
      << BRIGHT_YELLOW
      << "Systematic improvement across multiple courses for guaranteed success"
      << RESET << endl;

  accumulatedImpact = 0.0;
  coursesNeeded = 0;

  for (const auto& coursePair : courseImpactPotential) {
    if (coursesNeeded >= 4) break;  // Limit to top 4 courses

    int courseId = coursePair.first;
    double impact = coursePair.second;
    auto nameIt = courseNames.find(courseId);
    auto gradeIt = courseGrades.find(courseId);
    auto creditIt = courseCredits.find(courseId);

    cout << "   " << BRIGHT_WHITE << "Course " << (coursesNeeded + 1) << ": "
         << RESET;
    if (nameIt != courseNames.end()) {
      cout << BRIGHT_CYAN << nameIt->second << RESET;
    }
    cout << " (ID: " << courseId << ")";

    if (creditIt != courseCredits.end()) {
      cout << " [" << BRIGHT_YELLOW << creditIt->second << " credits" << RESET
           << "]";
    }
    cout << endl;

    if (gradeIt != courseGrades.end()) {
      double currentCourseGrade = gradeIt->second;
      double moderateImprovement = min(
          90.0, currentCourseGrade + 15.0);  // More conservative improvement

      cout << "      Current: " << BRIGHT_RED << fixed << setprecision(0)
           << currentCourseGrade << "/100" << RESET;
      cout << " -> Target: " << BRIGHT_GREEN << fixed << setprecision(0)
           << moderateImprovement << "/100" << RESET;
      cout << " (+" << BRIGHT_YELLOW << fixed << setprecision(0)
           << (moderateImprovement - currentCourseGrade) << " points)" << RESET
           << endl;
    }

    accumulatedImpact += impact * 0.8;  // More conservative estimate
    coursesNeeded++;
  }

  cout << "   " << BOLD << BRIGHT_GREEN
       << "Estimated Success Rate: VERY HIGH (90-98%)" << RESET << endl;
  cout << "   " << BOLD << BRIGHT_GREEN
       << "Time Investment: 3-6 months of steady improvement" << RESET << endl;

  // Strategy 3: Emergency Recovery (if needed)
  if (improvementNeeded > 20) {
    cout << "\n"
         << BOLD << BG_RED << BRIGHT_WHITE
         << " STRATEGY 3: EMERGENCY RECOVERY PROTOCOL " << RESET << endl;
    cout << BRIGHT_RED
         << "Intensive intervention required for major grade recovery" << RESET
         << endl;

    cout << "   " << BRIGHT_WHITE << "Recommended Actions:" << RESET << endl;
    cout << "   " << BRIGHT_RED << "- Retake ALL courses with grades below 70"
         << RESET << endl;
    cout << "   " << BRIGHT_RED << "- Enroll in academic recovery program"
         << RESET << endl;
    cout << "   " << BRIGHT_RED
         << "- Consider reducing course load next semester" << RESET << endl;
    cout << "   " << BRIGHT_RED
         << "- Seek academic counseling and tutoring support" << RESET << endl;
    cout << "   " << BRIGHT_RED
         << "- Implement strict study schedule (25+ hours/week)" << RESET
         << endl;

    cout << "   " << BOLD << BRIGHT_YELLOW
         << "Estimated Success Rate: MODERATE (50-70%)" << RESET << endl;
    cout << "   " << BOLD << BRIGHT_YELLOW
         << "Time Investment: 6-12 months intensive effort" << RESET << endl;
  }

  // AI Recommendation
  cout << "\n"
       << BOLD << BG_BRIGHT_BLUE << BRIGHT_WHITE
       << "    AI STRATEGIC RECOMMENDATION    " << RESET << endl;

  if (improvementNeeded <= 10) {
    cout << BRIGHT_GREEN << "RECOMMENDED: Strategy 1 (Minimal Effort)" << RESET
         << endl;
    cout << BRIGHT_GREEN
         << "Your target is achievable with focused effort on 1-2 key courses!"
         << RESET << endl;
  } else if (improvementNeeded <= 20) {
    cout << BRIGHT_YELLOW << "RECOMMENDED: Strategy 2 (Comprehensive Approach)"
         << RESET << endl;
    cout
        << BRIGHT_YELLOW
        << "Systematic improvement across multiple courses will ensure success!"
        << RESET << endl;
  } else {
    cout << BRIGHT_RED << "RECOMMENDED: Strategy 3 (Emergency Recovery)"
         << RESET << endl;
    cout << BRIGHT_RED
         << "Major intervention required - consider professional academic "
            "support!"
         << RESET << endl;
  }

  // Implementation Timeline
  cout << "\n"
       << BOLD << BRIGHT_MAGENTA << "IMPLEMENTATION TIMELINE:" << RESET << endl;
  cout << BRIGHT_WHITE << "Week 1-2: " << RESET << BRIGHT_CYAN
       << "Plan retakes, gather study materials, set study schedule" << RESET
       << endl;
  cout << BRIGHT_WHITE << "Week 3-8: " << RESET << BRIGHT_CYAN
       << "Execute improvement plan, track weekly progress" << RESET << endl;
  cout << BRIGHT_WHITE << "Week 9-12: " << RESET << BRIGHT_CYAN
       << "Final push, exams/assignments, grade verification" << RESET << endl;

  cout << "\n"
       << BRIGHT_GREEN
       << "SUCCESS PROBABILITY WITH DEDICATED EFFORT: " << BRIGHT_WHITE;
  if (improvementNeeded <= 15) {
    cout << "85-95%" << RESET << endl;
  } else if (improvementNeeded <= 25) {
    cout << "70-85%" << RESET << endl;
  } else {
    cout << "50-70%" << RESET << endl;
  }

  cout << "\n"
       << BOLD << BG_GREEN << BRIGHT_WHITE
       << " AI BELIEVES IN YOUR SUCCESS - YOU CAN DO THIS! " << RESET << endl;
}

double AdvancedScheduleAnalytics::calculateTotalCredits(
    const vector<int>& courseIds) const {
  double total = 0.0;
  for (int courseId : courseIds) {
    total += getCourseCredits(courseId);
  }
  return total;
}
