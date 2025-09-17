#ifndef COURSE_H
#define COURSE_H

#include <string>
using namespace std;

class Course {
private:
    int courseId;
    string name;
    int credits;
    string examDateA;
    string examDateB;
    string lecturer;

public:
    // Default constructor
    Course();
    
    // Constructor
    Course(int id, const string& courseName, int courseCredits, 
           const string& dateA, const string& dateB, const string& courseLecturer);
    
    // Copy constructor
    Course(const Course& other);
    
    // Assignment operator
    Course& operator=(const Course& other);
    
    // Destructor
    ~Course();
    
    // Getters
    int getCourseId() const;
    string getName() const;
    int getCredits() const;
    string getExamDateA() const;
    string getExamDateB() const;
    string getLecturer() const;
    
    // Setters
    void setCourseId(int id);
    void setName(const string& courseName);
    void setCredits(int courseCredits);
    void setExamDateA(const string& dateA);
    void setExamDateB(const string& dateB);
    void setLecturer(const string& courseLecturer);
    
    // Print course details
    void printCourse() const;
};

#endif
