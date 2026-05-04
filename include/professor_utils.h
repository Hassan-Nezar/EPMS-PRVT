#ifndef PROFESSORUTILS_H
#define PROFESSORUTILS_H

#include <string>

using namespace std;

class ProfessorUtils {
public:
    void setStudentGrade(string studentID, string courseCode, string grade);
    void viewEnrolledStudents(string courseCode);
};

class Attendance_manager {
public:
    void markAttendance(string studentID, string courseCode, string date, string status);
};

#endif