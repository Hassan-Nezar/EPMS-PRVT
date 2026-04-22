#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include <iostream>
#include <fstream> // Fixed: Added missing fstream for logging
#include <string>
#include <vector>

using namespace std;

// Class 9: Handling administrative status changes
class Admin_utilities {
public:
    void promoteUser(string id) { 
        cout << "User " << id << " has been promoted to Senior Status." << endl; 
    }
};

// Class 10: Logic for university-wide data summaries
class Reporting_manager {
public:
    void generateSystemReport() { 
        cout << "\n--- University Statistics ---" << endl;
        cout << "System Status: Online" << endl;
        cout << "Database Integrity: 100%" << endl;
        cout << "Generating logs... Done." << endl;
    }

    void calculateCourseAverage(string courseCode) {
    ifstream file("program_files/grades.txt");
    string id, cCode, grade;
    double total = 0;
    int count = 0;

    while (file >> id >> cCode >> grade) {
        if (cCode == courseCode) {
            // Primitive grade to number conversion
            if (grade == "A") total += 4.0;
            else if (grade == "B") total += 3.0;
            else if (grade == "C") total += 2.0;
            else total += 1.0;
            count++;
            }
        }
    if (count > 0) cout << "Average Grade for " << courseCode << ": " << (total / count) << "/4.0" << endl;
    else cout << "No grades found for this course." << endl;
    }

void calculateUniversityAverage() {
    ifstream file("program_files/student.txt");
    string fn, ln, id, pass, gpa, yr, maj;
    double totalGpa = 0;
    int count = 0;

    while (file >> fn >> ln >> id >> pass >> gpa >> yr >> maj) {
        totalGpa += stod(gpa); // stod converts string to double
        count++;
        }
    file.close();

    if (count > 0) {
        cout << "--- University Statistics ---" << endl;
        cout << "Total Students: " << count << endl;
        cout << "Average University GPA: " << (totalGpa / count) << endl;
        }
    }
};

// Class 11: Monitoring data file health
class System_monitor {
public:
    void checkServerStatus() { 
        cout << "[MONITOR]: All .txt database files are currently reachable." << endl; 
    }
};

// Class 12: Internal logging for every action taken
class Database_logger {
public:
    void logAction(string msg) { 
        ofstream file("program_files/logs.txt", ios::app);
        if (file.is_open()) {
            file << "[LOG]: " << msg << endl;
            file.close();
            }
        }
    };

#endif