/**
 * @file main.cpp
 * @brief Entry point for the Student Schedule Management System
 * 
 * This is the main application entry point for a comprehensive academic
 * schedule management system. The system provides course management,
 * schedule creation, conflict detection, and advanced analytics.
 * 
 * @author OOP Final Assignment
 * @date 2025
 * @version 2.0 - AI Enhanced
 */

#include <iostream>
#include "ScheduleManager.h"

using namespace std;

/**
 * @brief Main application entry point
 * 
 * Initializes the ScheduleManager and starts the main application loop.
 * The ScheduleManager handles all system functionality including:
 * - Course and lesson data management
 * - Schedule creation and manipulation
 * - Advanced conflict detection and resolution
 * - Professional user interface with modern styling
 * - Comprehensive testing suites
 * 
 * @return 0 on successful execution
 */
int main() {
    try {
        // Initialize the Schedule Management System
        ScheduleManager manager;
        
        // Start the main application loop
        manager.run();
        
        return 0;
    }
    catch (const exception& e) {
        cerr << "❌ Fatal Error: " << e.what() << endl;
        return 1;
    }
    catch (...) {
        cerr << "❌ Unknown error occurred during execution." << endl;
        return 1;
    }
}
