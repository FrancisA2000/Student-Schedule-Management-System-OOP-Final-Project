#ifndef ADVANCED_SCHEDULE_ANALYTICS_H
#define ADVANCED_SCHEDULE_ANALYTICS_H

#include <chrono>
#include <cstring>
#include <fstream>
#include <functional>
#include <future>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <thread>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

#include "Course.h"
#include "Lab.h"
#include "Lecture.h"
#include "Lesson.h"
#include "Schedule.h"
#include "Tutorial.h"

using namespace std;

// Forward declarations and function types for ScheduleManager integration
typedef function<bool(int, int, const string&)> AddLessonFunction;
typedef function<bool(int, int, const string&)> RemoveLessonFunction;
typedef function<int()>
    CreateScheduleFunction;  // Returns actual created schedule ID
typedef function<void()> ReloadSystemDataFunction;

/**
 * @class AdvancedScheduleAnalytics
 * @brief Enterprise-grade academic schedule analytics and optimization system
 *
 * This class implements three sophisticated non-trivial functions using
 * advanced algorithms, data structures, and software engineering patterns. Each
 * function represents production-quality software with comprehensive error
 * handling, performance optimization, and intelligent decision-making
 * capabilities.
 *
 * Architecture Pattern: Facade + Strategy + Observer
 * Performance: Optimized for O(n log n) operations with memoization
 * Thread Safety: Async operations for I/O-intensive tasks
 */
class AdvancedScheduleAnalytics {
 private:
  // =================== ADVANCED DATA STRUCTURES ===================

  /**
   * @struct ConflictContext
   * @brief Comprehensive conflict information with resolution metadata
   */
  struct ConflictContext {
    struct LessonInfo {
      int courseId;
      string courseName;
      string lessonType;
      string groupId;
      string day;
      string startTime;
      int duration;
      string classroom;
      string building;
      string instructor;
      double creditWeight;
      int priority;

      LessonInfo() = default;
      LessonInfo(int cId, const string& cName, const string& lType,
                 const string& gId, const string& d, const string& st, int dur,
                 const string& cls, const string& bld, const string& inst,
                 double cw = 0.0, int pri = 0)
          : courseId(cId),
            courseName(cName),
            lessonType(lType),
            groupId(gId),
            day(d),
            startTime(st),
            duration(dur),
            classroom(cls),
            building(bld),
            instructor(inst),
            creditWeight(cw),
            priority(pri) {}
    };

    LessonInfo lesson1;
    LessonInfo lesson2;
    int overlapMinutes;
    double conflictSeverity;
    vector<string> resolutionStrategies;
    bool isResolvable;

    ConflictContext(const LessonInfo& l1, const LessonInfo& l2, int overlap,
                    double severity)
        : lesson1(l1),
          lesson2(l2),
          overlapMinutes(overlap),
          conflictSeverity(severity),
          isResolvable(false) {}
  };

  /**
   * @struct OptimizationCandidate
   * @brief Advanced course combination with quality metrics
   */
  struct OptimizationCandidate {
    vector<Course> courses;
    double totalCredits;
    double qualityScore;
    double conflictProbability;
    double workloadBalance;
    double instructorDiversity;
    map<string, vector<shared_ptr<Lesson>>> scheduleMap;
    vector<string> constraints;

    OptimizationCandidate()
        : totalCredits(0),
          qualityScore(0),
          conflictProbability(0),
          workloadBalance(0),
          instructorDiversity(0) {}

    bool operator<(const OptimizationCandidate& other) const {
      return qualityScore > other.qualityScore;  // Max heap
    }
  };

  /**
   * @struct AcademicMetrics
   * @brief Comprehensive academic performance analytics
   */
  struct AcademicMetrics {
    struct CoursePerformance {
      Course course;
      double grade;
      double creditWeight;
      double gpaContribution;
      double retakeImpact;
      string performanceCategory;
      int improvementPriority;

      CoursePerformance(const Course& c, double g)
          : course(c),
            grade(g),
            creditWeight(0),
            gpaContribution(0),
            retakeImpact(0),
            improvementPriority(0) {}
    };

    double weightedGPA;
    double cumulativeGPA;
    double semesterGPA;
    double projectedGPA;
    double totalCredits;  // Changed from int to double for fractional credits
    string academicStanding;
    vector<CoursePerformance> courseAnalysis;
    map<string, double> subjectAreaPerformance;
    vector<string> strategicRecommendations;

    // Additional fields for academic analytics
    double predictedGPA;
    double minPossibleGPA;
    double maxPossibleGPA;
    double actualGradeAverage;  // Weighted average grade on 100 scale
    string riskLevel;
    int coursesCount;
    int retakeOptions;
    double workloadDistribution;
    double difficultyScore;
    double timeOptimizationScore;
    double successProbability;

    // Constructor to initialize all fields
    AcademicMetrics()
        : weightedGPA(0),
          cumulativeGPA(0),
          semesterGPA(0),
          projectedGPA(0),
          totalCredits(0),
          predictedGPA(0),
          minPossibleGPA(0),
          maxPossibleGPA(0),
          actualGradeAverage(0),
          coursesCount(0),
          retakeOptions(0),
          workloadDistribution(0),
          difficultyScore(0),
          timeOptimizationScore(0),
          successProbability(0) {}
  };

  /**
   * @struct RetakeStrategy
   * @brief Smart retake planning for optimal GPA improvement
   */
  struct RetakeStrategy {
    struct RetakeCourse {
      int courseId;
      double targetGrade;
      double difficultyLevel;  // 1-10 scale

      RetakeCourse(int id, double target, double difficulty)
          : courseId(id), targetGrade(target), difficultyLevel(difficulty) {}
    };

    string name;
    string description;
    vector<RetakeCourse> courses;
    double projectedGPA;
    double effortLevel;  // 1-10 scale

    RetakeStrategy() : projectedGPA(0), effortLevel(0) {}
  };

  /**
   * @struct RetakeRecommendation
   * @brief Detailed retake recommendation with strategy
   */
  struct RetakeRecommendation {
    int courseId;
    double currentRisk;
    double improvementPotential;
    string recommendedSemester;
    vector<int> alternativeCourses;
    string strategy;

    RetakeRecommendation()
        : courseId(0), currentRisk(0), improvementPotential(0) {}
  };

  /**
   * @struct TimeSlotMatrix
   * @brief Optimized time slot representation using bit manipulation
   */
  struct TimeSlotMatrix {
    static constexpr int DAYS_PER_WEEK = 7;
    static constexpr int HOURS_PER_DAY = 24;
    static constexpr int MINUTES_PER_HOUR = 60;

    // Bit matrix for O(1) conflict detection
    uint64_t timeMatrix[DAYS_PER_WEEK];  // Each bit = 15-minute slot

    TimeSlotMatrix() { memset(timeMatrix, 0, sizeof(timeMatrix)); }

    void setTimeSlot(int day, int hour, int minute, int duration);
    bool hasConflict(int day, int hour, int minute, int duration) const;
    double getUtilizationRatio() const;
  };

  // =================== DATA ACCESS ===================

  mutable const vector<Course>* courses_ptr;
  mutable const vector<Schedule>* schedules_ptr;
  mutable const map<int, vector<shared_ptr<Lesson>>>* courseLessons_ptr;

  // Function pointers for ScheduleManager operations
  mutable AddLessonFunction addLessonCallback;
  mutable RemoveLessonFunction removeLessonCallback;
  mutable CreateScheduleFunction createScheduleCallback;
  mutable ReloadSystemDataFunction reloadSystemData;

  // =================== PERFORMANCE OPTIMIZATION ===================

  mutable unordered_map<string, vector<shared_ptr<Lesson>>> lessonCache;
  mutable unordered_map<int, Course> courseCache;
  mutable unordered_map<string, ConflictContext> conflictCache;

  // Thread pool for asynchronous operations
  mutable vector<future<void>> asyncTasks;

  // =================== ALGORITHM IMPLEMENTATIONS ===================

  // =================== UTILITY FUNCTIONS ===================

  int timeStringToMinutes(const string& timeStr) const;
  string minutesToTimeString(int minutes) const;

  vector<string> parseCSVLine(const string& line) const;

  vector<shared_ptr<Lesson>> loadLessonsForCourse(
      int courseId, const string& lessonType) const;

  double calculateQualityScore(const OptimizationCandidate& candidate) const;
  double calculateConflictProbability(const vector<Course>& courses) const;
  double calculateWorkloadBalance(const vector<Course>& courses) const;
  double calculateInstructorDiversity(const vector<Course>& courses) const;

  void optimizeScheduleLayout(OptimizationCandidate& candidate) const;

  string determineAcademicStanding(double gpa) const;
  vector<string> generateStrategicRecommendations(
      const AcademicMetrics& metrics) const;

  // Smart course scheduling with conflict-free validation
  bool tryAddCompleteCourse(int scheduleId, int courseId,
                            bool quietMode = false) const;

  double calculateRetakeImpact(const Course& course, double currentGrade,
                               double totalCredits) const;

  void displayProgressAnimation(const string& operation, int steps) const;
  void displayConflictReport(const vector<ConflictContext>& conflicts) const;
  void displayOptimizationResults(const OptimizationCandidate& candidate) const;
  void displayAcademicReport(const AcademicMetrics& metrics,
                             double targetGrade) const;

  // Smart retake strategy functions
  void generateSmartRetakeStrategy(const AcademicMetrics& metrics,
                                   double targetGrade) const;
  RetakeStrategy findMinimalRetakeStrategy(const AcademicMetrics& metrics,
                                           double pointsToGain) const;
  RetakeStrategy findBalancedRetakeStrategy(const AcademicMetrics& metrics,
                                            double pointsToGain) const;
  double calculateDifficulty(double currentGrade, double targetGrade) const;

  // Academic Analytics helper methods
  double calculateAdvancedGPAPrediction(const vector<int>& courseIds) const;
  double calculateMinimumGPA(const vector<int>& courseIds) const;
  double calculateMaximumGPA(const vector<int>& courseIds) const;
  string assessAcademicRisk(const AcademicMetrics& metrics) const;
  vector<RetakeRecommendation> generateRetakeStrategies(
      const vector<int>& courseIds) const;
  double calculateWorkloadDistribution(const vector<int>& courseIds) const;
  double calculateDifficultyScore(const vector<int>& courseIds) const;
  double calculateTimeOptimization(const vector<int>& courseIds) const;
  double calculateSuccessProbability(const AcademicMetrics& metrics) const;
  void displayAcademicAnalyticsResults(
      const AcademicMetrics& metrics,
      const vector<RetakeRecommendation>& retakeStrategies,
      const vector<int>& courseIds, const map<int, string>& courseNames,
      const map<int, double>& courseCredits,
      const map<int, vector<string>>& courseInstructors,
      const map<int, double>& courseGrades) const;
  void displayActionableRecommendations(
      const AcademicMetrics& metrics,
      const vector<RetakeRecommendation>& retakeStrategies,
      const vector<int>& courseIds, const map<int, string>& courseNames,
      const map<int, double>& courseCredits,
      const map<int, double>& courseGrades) const;

  // Course analysis helper methods
  double calculateCourseDifficulty(int courseId) const;
  double calculateInstructorRating(int courseId) const;
  double calculateStudentCourseFitness(int courseId) const;
  double calculateHistoricalPerformance(int courseId) const;
  double calculateCourseWorkload(int courseId) const;
  double getCourseCredits(int courseId) const;
  double calculateCourseRisk(int courseId) const;
  double calculateImprovementPotential(int courseId) const;
  string determineOptimalRetakeSemester(int courseId) const;
  vector<int> findAlternativeCourses(int courseId) const;
  string generateRetakeStrategy(int courseId) const;

  // Advanced visualization and AI helper methods
  string createProgressBar(double value, double maxValue, int width,
                           const string& type) const;
  string createGradeVisualization(double grade) const;
  string createRiskVisualization(double risk) const;
  string generateAIInsight(double grade, double difficulty, double risk) const;
  string generateCourseStrategy(int courseId, double grade) const;
  string generateCourseName(int courseId) const;

  // Target grade planning system
  void generateTargetGradePlans(
      double targetGrade, double currentGrade,
      const vector<RetakeRecommendation>& retakeStrategies,
      const vector<int>& courseIds, const map<int, string>& courseNames,
      const map<int, double>& courseCredits,
      const map<int, double>& courseGrades) const;
  double calculateTotalCredits(const vector<int>& courseIds) const;

 public:
  /**
   * @brief Constructor - Initialize analytics engine with data access
   */
  AdvancedScheduleAnalytics(
      const vector<Course>& courses, const vector<Schedule>& schedules,
      const map<int, vector<shared_ptr<Lesson>>>& courseLessons);

  /**
   * @brief Default constructor for when data is set later
   */
  AdvancedScheduleAnalytics();

  /**
   * @brief Set data references for analytics operations
   */
  void setDataReferences(
      const vector<Course>& courses, const vector<Schedule>& schedules,
      const map<int, vector<shared_ptr<Lesson>>>& courseLessons,
      AddLessonFunction addCallback = nullptr,
      RemoveLessonFunction removeCallback = nullptr,
      CreateScheduleFunction createCallback = nullptr,
      ReloadSystemDataFunction reloadCallback = nullptr);

  /**
   * @brief Destructor - Clean up async tasks and caches
   */
  ~AdvancedScheduleAnalytics();

  // =================== MAIN PUBLIC FUNCTIONS ===================

  /**
   * @brief Function 1: Intelligent Conflict Detection and Resolution System
   *
   * Advanced conflict detection using graph theory algorithms to identify
   * time overlaps, spatial conflicts, and instructor conflicts. Provides
   * AI-powered resolution strategies with minimal schedule disruption.
   *
   * Features:
   * - Multi-dimensional conflict analysis (time, space, resources)
   * - Graph-based dependency resolution
   * - Automated fix suggestions with impact analysis
   * - Real-time optimization recommendations
   *
   * @param scheduleId Schedule to analyze
   * @return true if analysis completed successfully, false on error
   *
   * @complexity O(n log n) where n is number of lessons
   * @thread_safety Safe for concurrent read operations
   */
  bool intelligentConflictResolution(int scheduleId);

  /**
   * @brief Function 2: AI-Powered Optimal Schedule Generation
   *
   * Uses genetic algorithms and constraint satisfaction to generate
   * optimal course schedules. Considers multiple optimization criteria
   * including workload balance, instructor diversity, and time preferences.
   *
   * Features:
   * - Genetic algorithm optimization
   * - Multi-criteria decision analysis
   * - Constraint satisfaction solving
   * - Predictive conflict avoidance
   * - Quality scoring with weighted preferences
   *
   * @param targetCredits Target credit hours for the schedule
   * @param tolerance Acceptable deviation from target credits
   * @return true if optimal schedule generated, false otherwise
   *
   * @complexity O(g * p * n) where g=generations, p=population, n=courses
   * @thread_safety Uses async operations for performance
   */
  bool aiOptimalScheduleGeneration(double targetCredits,
                                   double tolerance = 1.0);

  /**
   * @brief Function 3: Advanced Academic Performance Analytics
   *
   * Comprehensive grade analysis with predictive modeling and strategic
   * optimization recommendations. Uses machine learning concepts for
   * performance prediction and improvement suggestions.
   *
   * Features:
   * - Weighted GPA calculations with trend analysis
   * - Performance prediction modeling
   * - Strategic retake impact analysis
   * - Subject area performance breakdown
   * - Personalized improvement recommendations
   *
   * @param scheduleId Schedule to analyze for performance metrics
   * @return true if analysis completed, false on error
   *
   * @complexity O(n log n) with memoization optimization
   * @thread_safety Safe for concurrent operations
   */
  bool advancedAcademicAnalytics(int scheduleId);

  /**
   * @brief Bulletproof course addition with real conflict detection
   * @param scheduleId The schedule to add lessons to
   * @param courseId The course to attempt to add
   * @param quietMode Whether to suppress verbose output
   * @return true if course added without conflicts, false otherwise
   */
  bool tryAddCompleteCourseBulletproof(int scheduleId, int courseId,
                                       bool quietMode) const;

  /**
   * @brief Direct lesson addition as backup when callbacks fail
   * @param scheduleId The schedule to add lessons to
   * @param courseId The course to attempt to add
   * @return true if at least one lesson was added, false otherwise
   */
  bool tryDirectLessonAddition(int scheduleId, int courseId) const;
};

#endif  // ADVANCED_SCHEDULE_ANALYTICS_H
