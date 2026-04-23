#ifndef PROFESSORUTILS_H
#define PROFESSORUTILS_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class ProfessorUtils {
public:
    
    void setStudentGrade(string studentID, string courseCode, string grade) {
    ifstream enrollFile("program_files/enrollments.txt");
    string sID, cCode;
    bool isEnrolled = false;

    
    while (enrollFile >> sID >> cCode) {
        if (sID == studentID && cCode == courseCode) {
            isEnrolled = true;
            break;
        }
    }
    enrollFile.close();

    if (isEnrolled) {
        ofstream gradeFile("program_files/grades.txt", ios::app);
        gradeFile << studentID << " " << courseCode << " " << grade << endl;
        gradeFile.close();
        cout << "Success: Grade recorded for Student " << studentID << endl;
    } else {
        cout << "Error: This student is not enrolled in " << courseCode << ". Grade rejected." << endl;
    }
}

    
    void viewEnrolledStudents(string courseCode) {
        ifstream file("program_files/enrollments.txt");
        string sID, cCode;
        cout << "\n--- Students Enrolled in " << courseCode << " ---" << endl;
        while (file >> sID >> cCode) {
            if (cCode == courseCode) {
                cout << "Student ID: " << sID << endl;
            }
        }
        file.close();
    }
};

class Attendance_manager {
public:
    void markAttendance(string studentID, string courseCode, string date, string status) {
        ofstream file("program_files/attendance.txt", ios::app);
        if (file.is_open()) {
            file << studentID << " " << courseCode << " " << date << " " << status << endl;
            file.close();
            cout << "Attendance recorded: Student " << studentID << " was " << status << "." << endl;
        }
    }
};


#endif