#include "Schedule.h"
#include "Lecture.h"
#include "Tutorial.h"
#include "Lab.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <set>

// Constructor
Schedule::Schedule(int id) : scheduleId(id) {
}

// Copy constructor
Schedule::Schedule(const Schedule& other) 
    : scheduleId(other.scheduleId), lessons(other.lessons) {
}

// Assignment operator
Schedule& Schedule::operator=(const Schedule& other) {
    if (this != &other) {
        scheduleId = other.scheduleId;
        lessons = other.lessons;
    }
    return *this;
}

// Destructor
Schedule::~Schedule() {
}

// Getters
int Schedule::getScheduleId() const {
    return scheduleId;
}

const vector<shared_ptr<Lesson>>& Schedule::getLessons() const {
    return lessons;
}

// Setters
void Schedule::setScheduleId(int id) {
    scheduleId = id;
}

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
            return lesson->getCourseId() == courseId && lesson->getGroupId() == groupId;
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

// Print schedule in table format
void Schedule::printSchedule() const {
    cout << "\n===============================================================================" << endl;
    cout << "                              SCHEDULE " << scheduleId << "                               " << endl;
    cout << "===============================================================================" << endl;
    
    if (lessons.empty()) {
        cout << "\n                        No lessons in this schedule.\n" << endl;
        return;
    }
    
    // Define time slots (8:00 to 20:00 in 1-hour intervals)
    vector<string> timeSlots;
    for (int hour = 8; hour <= 20; hour++) {
        timeSlots.push_back((hour < 10 ? "0" : "") + to_string(hour) + ":00");
    }
    
    // Define days of the week
    vector<string> days = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    
    // Create a map to store lessons by day and time
    map<pair<string, string>, vector<string>> scheduleGrid;
    
    // Populate the grid with lessons
    for (const auto& lesson : lessons) {
        string day = lesson->getDay();
        string startTime = lesson->getStartTime();
        int duration = lesson->getDuration();
        
        // Find course name from the courses list
        // For now, we'll use a simplified approach with predefined course names
        string courseName = "";
        int courseId = lesson->getCourseId();
        
        // Add some common course names for better display
        if (courseId == 11004) courseName = "Calculus I";
        else if (courseId == 11006) courseName = "Calculus II";
        else if (courseId == 11121) courseName = "ODE";
        else if (courseId == 31910) courseName = "Control";
        else if (courseId == 31999) courseName = "Energy Conv";
        else if (courseId == 41090) courseName = "Chemistry";
        else if (courseId == 51742) courseName = "Prob&Stats";
        else if (courseId == 251961) courseName = "Eng Fund 3";
        else if (courseId == 31402) courseName = "EE Lab I";
        else if (courseId == 11057) courseName = "Adv English";
        else courseName = "Course" + to_string(courseId);
        
        // Create visual symbols and colors based on lesson type (with ANSI colors)
        string typeSymbol = "";
        string colorCode = "";
        string resetColor = "\033[0m"; // Reset color
        
        if (lesson->getLessonType() == "Lecture") {
            typeSymbol = "[L]";
            colorCode = "\033[1;34m"; // Bold Blue for lectures
        } else if (lesson->getLessonType() == "Tutorial") {
            typeSymbol = "[T]";
            colorCode = "\033[1;32m"; // Bold Green for tutorials
        } else if (lesson->getLessonType() == "Lab") {
            typeSymbol = "[B]";
            colorCode = "\033[1;31m"; // Bold Red for labs
        }
        
        // Convert lesson info to display string with detailed formatting
        string courseIdStr = to_string(lesson->getCourseId());
        string classroom = lesson->getClassroom();
        string building = lesson->getBuilding();
        
        // Create detailed lesson info with color (3 lines)
        string lessonInfo = colorCode + typeSymbol + courseIdStr + resetColor + "\n" +
                           courseName + "\n" +
                           classroom + " " + building;
        
        // Handle multi-hour lessons - add to multiple time slots
        int startHour = stoi(startTime.substr(0, 2));
        for (int h = 0; h < duration; h++) {
            int currentHour = startHour + h;
            if (currentHour <= 20) {
                string timeSlot = (currentHour < 10 ? "0" : "") + to_string(currentHour) + ":00";
                // Create a unique lesson identifier to avoid false conflicts
                string uniqueLessonInfo = lessonInfo + "|" + lesson->getGroupId();
                scheduleGrid[{day, timeSlot}].push_back(uniqueLessonInfo);
            }
        }
    }
    
    // Print the enhanced table header
    cout << "\n+--------+------------------+------------------+------------------+------------------+------------------+------------------+" << endl;
    cout << "|  Time  |     Sunday       |     Monday       |     Tuesday      |    Wednesday     |    Thursday      |     Friday       |";
    cout << "\n+--------+------------------+------------------+------------------+------------------+------------------+------------------+" << endl;
    
    // Print each time slot row with enhanced formatting
    for (const string& timeSlot : timeSlots) {
        // First, determine the maximum number of lines needed for this row
        int maxLines = 1;
        for (const string& day : days) {
            auto it = scheduleGrid.find({day, timeSlot});
            if (it != scheduleGrid.end() && !it->second.empty()) {
                // Count newlines in the lesson info
                for (const string& lessonInfo : it->second) {
                    int lineCount = 1 + count(lessonInfo.begin(), lessonInfo.end(), '\n');
                    maxLines = max(maxLines, lineCount);
                }
            }
        }
        
        // Print each line of the row
        for (int line = 0; line < maxLines; line++) {
            if (line == 0) {
                cout << "| " << left << setw(6) << timeSlot << " ";
            } else {
                cout << "|        ";
            }
            
            for (const string& day : days) {
                cout << "|";
                
                auto it = scheduleGrid.find({day, timeSlot});
                if (it != scheduleGrid.end() && !it->second.empty()) {
                    // Check for real conflicts (different group IDs at same time)
                    set<string> uniqueGroups;
                    for (const string& lessonInfo : it->second) {
                        // Extract group ID from the lesson info (after the last |)
                        size_t lastPipe = lessonInfo.find_last_of("|");
                        if (lastPipe != string::npos) {
                            string groupId = lessonInfo.substr(lastPipe + 1);
                            uniqueGroups.insert(groupId);
                        }
                    }
                    
                    string cellContent = "";
                    
                    if (uniqueGroups.size() > 1) {
                        // True conflict - different lessons at same time
                        if (line == 0) {
                            cellContent = "** CONFLICT **";
                        } else if (line == 1) {
                            cellContent = "Multiple lessons";
                        } else if (line == 2) {
                            cellContent = "at same time";
                        }
                    } else {
                        // Single lesson - normal display
                        string lessonInfo = it->second[0];
                        // Remove the unique identifier (everything after last |)
                        size_t lastPipe = lessonInfo.find_last_of("|");
                        if (lastPipe != string::npos) {
                            lessonInfo = lessonInfo.substr(0, lastPipe);
                        }
                        
                        // Extract the specific line
                        vector<string> lines;
                        stringstream ss(lessonInfo);
                        string singleLine;
                        while (getline(ss, singleLine)) {
                            lines.push_back(singleLine);
                        }
                        
                        if (line < (int)lines.size()) {
                            cellContent = lines[line];
                        }
                    }
                    
                    // Calculate visible length (excluding ANSI color codes)
                    string visibleContent = cellContent;
                    size_t pos = 0;
                    while ((pos = visibleContent.find("\033[", pos)) != string::npos) {
                        size_t endPos = visibleContent.find("m", pos);
                        if (endPos != string::npos) {
                            visibleContent.erase(pos, endPos - pos + 1);
                        } else {
                            break;
                        }
                    }
                    
                    // Truncate if too long (based on visible length)
                    if (visibleContent.length() > 16) {
                        // Need to truncate the original string carefully to preserve color codes
                        int visibleChars = 0;
                        string truncated = "";
                        for (size_t i = 0; i < cellContent.length() && visibleChars < 16; i++) {
                            if (cellContent.substr(i, 2) == "\033[") {
                                // Find the end of the color code
                                size_t endPos = cellContent.find("m", i);
                                if (endPos != string::npos) {
                                    truncated += cellContent.substr(i, endPos - i + 1);
                                    i = endPos;
                                    continue;
                                }
                            }
                            truncated += cellContent[i];
                            visibleChars++;
                        }
                        cellContent = truncated;
                        visibleContent = visibleContent.substr(0, 16);
                    }
                    
                    // Pad to exactly 16 visible characters
                    int paddingNeeded = static_cast<int>(16 - visibleContent.length());
                    if (paddingNeeded < 0) paddingNeeded = 0; // Safety check
                    string padding(paddingNeeded, ' ');
                    
                    // Ensure no newlines in cellContent that could break table structure
                    for (char& c : cellContent) {
                        if (c == '\n' || c == '\r') {
                            c = ' '; // Replace line breaks with spaces in output
                        }
                    }
                    
                    cout << " " << cellContent << padding << " ";
                } else {
                    cout << "                  ";
                }
            }
            cout << "|" << endl;
        }
        
        // Add separator lines every few hours for better readability
        if (timeSlot == "12:00" || timeSlot == "16:00") {
            cout << "+--------+------------------+------------------+------------------+------------------+------------------+------------------+" << endl;
        }
    }
    
    cout << "+--------+------------------+------------------+------------------+------------------+------------------+------------------+" << endl;
    
    // Enhanced legend with symbols and formatting
    cout << "\n+--- LEGEND ----------------------------------------------------------------------+" << endl;
    cout << "| \033[1;34m[L]\033[0m Lecture (Blue)  \033[1;32m[T]\033[0m Tutorial (Green)  \033[1;31m[B]\033[0m Lab (Red)                        |" << endl;
    cout << "| Format: [Type][CourseID]                                                        |" << endl;
    cout << "|         Course Name                                                             |" << endl;
    cout << "|         [Classroom Building]                                                    |" << endl;
    cout << "| ** CONFLICT ** = Multiple lessons at same time                                  |" << endl;
    cout << "+---------------------------------------------------------------------------------+" << endl;
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
void Schedule::clearSchedule() {
    lessons.clear();
}

// Get number of lessons
int Schedule::getLessonCount() const {
    return lessons.size();
}

// Save schedule to CSV
void Schedule::saveToCSV(const string& filename) const {
    ofstream file(filename);
    if (file.is_open()) {
        file << "ScheduleID,LessonType,CourseID,Day,StartTime,Duration,Classroom,Building,Teacher,GroupID" << endl;
        for (const auto& lesson : lessons) {
            file << scheduleId << ","
                 << lesson->getLessonType() << ","
                 << lesson->getCourseId() << ","
                 << lesson->getDay() << ","
                 << lesson->getStartTime() << ","
                 << lesson->getDuration() << ","
                 << lesson->getClassroom() << ","
                 << lesson->getBuilding() << ","
                 << lesson->getTeacher() << ","
                 << lesson->getGroupId() << endl;
        }
        file.close();
    }
}

// Load schedule from CSV
void Schedule::loadFromCSV(const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        getline(file, line); // Skip header
        
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
                    // Parse with correct field order: ScheduleID,LessonType,CourseID,Day,StartTime,Duration,Classroom,Building,Teacher,GroupID
                    int schedId = stoi(tokens[0]);          // ScheduleID
                    string lessonType = tokens[1];          // LessonType  
                    int courseId = stoi(tokens[2]);         // CourseID
                    string day = tokens[3];                 // Day
                    string startTime = tokens[4];           // StartTime
                    int duration = stoi(tokens[5]);         // Duration
                    string classroom = tokens[6];           // Classroom
                    string building = tokens[7];            // Building
                    string teacher = tokens[8];             // Teacher
                    string groupId = tokens[9];             // GroupID
                    
                    // Create appropriate lesson objects based on type
                    shared_ptr<Lesson> lesson;
                    if (lessonType == "Lecture") {
                        lesson = make_shared<Lecture>(courseId, day, startTime, duration, classroom, building, teacher, groupId);
                    } else if (lessonType == "Tutorial") {
                        lesson = make_shared<Tutorial>(courseId, day, startTime, duration, classroom, building, teacher, groupId);
                    } else if (lessonType == "Lab") {
                        lesson = make_shared<Lab>(courseId, day, startTime, duration, classroom, building, teacher, groupId);
                    }
                    
                    if (lesson && schedId == scheduleId) {
                        lessons.push_back(lesson);
                    }
                } catch (const exception& e) {
                    // Skip malformed lines
                    continue;
                }
            }
        }
        file.close();
    }
}
