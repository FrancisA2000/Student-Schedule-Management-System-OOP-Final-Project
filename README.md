# Student Schedule Management System (OOP Final Project – 31695, 2025)

## 📌 Short Description
A student schedule management system developed as the final project for the OOP (31695) course. Implemented in three ways: manual C++ OOP with STL, AI-assisted coding, and Base44 web app. Features include course management, schedule creation, conflict detection, and CSV-based persistence.

---

## 📝 Overview
This repository contains the **final project** for the course  
**Object-Oriented Programming (תכנות מכוון אובייקטים, 31695, Semester 2025-2)**.  

The project demonstrates three different implementations of a **Student Schedule Management System**:

1. **Manual Implementation** – Fully coded in **C++** using **OOP principles** and **STL containers**.  
2. **GenAI Implementation** – Developed using **Generative AI tools** (e.g., ChatGPT, Claude, Cursor, Copilot).  
3. **Vibe Coding Implementation** – Created on the **Base44 platform** using the free-tier web environment.

---

## 🚀 Features
- Create, view, and delete schedules (`AddSchedule`, `RmSchedule`).  
- Add or remove specific course groups (`Add <sched> <course> <group>` / `Rm <sched> <course> <group>`).  
- Print detailed course information (`PrintCourse <course_id>`).  
- Print all schedules or a specific schedule in a **tabular timetable format**.  
- Conflict detection and advanced analytics (AI-assisted version).  
- Persistent storage in CSV files (courses, lectures, tutorials, labs, schedules).  
- Extra original features beyond requirements.  

---

## 🛠 Running the Manual Implementation
### Prerequisites
- **C++17 or newer**
- A terminal that supports **ANSI color codes** (for enhanced CLI output)
- CSV files prepared in the `data/` folder:
  - `courses.csv`
  - `<course_id>_lectures.csv`
  - `<course_id>_tutorials.csv`
  - `<course_id>_labs.csv`
  - `schedules.csv`

---

## 🎮 Example Commands

```bash
PrintCourses
More
PrintCourse 31511
AddSchedule
Add 1 31511 L2
PrintSchedule 1
RmSchedule 1
```

---

## 🌐 Vibe Coding App

* Published via **Base44**: [Open App](https://app--academia-flow-0f12e827.base44.app)

---

## 📊 Comparison Criteria

The three implementations are compared on:

* Code quality & efficiency
* Development effort & time investment
* Required skills & tools
* Data structure choices
* Understanding & control over the system
* Issues & limitations
* Contribution to learning
* Satisfaction vs frustration

---

## 👥 Authors

* Francis Aboud
* Bshara Habib

---

## 📚 Sources & References

* Course labs & lectures (OOP 31695)
* C++ STL documentation: [cppreference.com](https://en.cppreference.com)
* Stack Overflow
* ChatGPT & GenAI tools
* Base44 Documentation ([docs.base44.com](https://docs.base44.com))
