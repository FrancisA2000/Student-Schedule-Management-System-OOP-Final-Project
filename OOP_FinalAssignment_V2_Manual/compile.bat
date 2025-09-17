@echo off
echo Compiling Schedule Management System...

REM Create obj directory if it doesn't exist
if not exist obj mkdir obj

REM Compile all cpp files
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/Course.cpp -o obj/Course.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/Lesson.cpp -o obj/Lesson.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/Lecture.cpp -o obj/Lecture.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/Tutorial.cpp -o obj/Tutorial.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/Lab.cpp -o obj/Lab.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/Schedule.cpp -o obj/Schedule.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/AdvancedScheduleAnalytics.cpp -o obj/AdvancedScheduleAnalytics.o
g++ -std=c++14 -Wall -Wextra -Iinclude -c src/main.cpp -o obj/main.o

REM Link all object files
g++ obj/Course.o obj/Lesson.o obj/Lecture.o obj/Tutorial.o obj/Lab.o obj/Schedule.o obj/AdvancedScheduleAnalytics.o obj/main.o -o schedule_manager.exe

if exist schedule_manager.exe (
    echo Compilation successful! 
    echo Run the program with: schedule_manager.exe
) else (
    echo Compilation failed!
)

pause
