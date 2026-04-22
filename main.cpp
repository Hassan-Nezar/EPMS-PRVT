#include <iostream>
#include <string>
#include "include/auth_manager.h"
#include "include/User.h"
#include "include/course_manager.h"
#include "include/professor_utils.h"
#include "include/student_utils.h"
#include "include/system_admin.h"

using namespace std;

void showHeader() {
    cout << "\n================================================" << endl;
    cout << "   ZEWAIL CITY ENTERPRISE MANAGEMENT SYSTEM     " << endl;
    cout << "================================================" << endl;
}

int main() {
    auth_manager auth;
    CourseManager courseMgr;
    ProfessorUtils profUtils;
    Attendance_manager attendMgr; // Class 10
    Student_utilities studUtils;
    Reporting_manager reportMgr;  // Class 11
    Database_logger logger;       // Class 12
    
    while (true) {
        showHeader();
        string name, id, pass;
        cout << "Login Required (or type 'exit' to quit)" << endl;
        cout << "Full Name: "; getline(cin >> ws, name);
        if (name == "exit") break;
        cout << "ID: "; cin >> id;
        cout << "Password: "; cin >> pass;

        auth_manager::login_result res = auth.login(name, id, pass);

        if (res.success) {
            logger.logAction("User Login: " + res.id);
            int choice = 0;

            // --- STUDENT MENU ---
            if (res.role == "Student") {
                Student s(res.first_name, res.last_name, res.id, "HIDDEN", res.extra1, res.extra2, res.extra3);
                while (choice != 5) {
                    cout << "\n1. Profile\n2. Transcript\n3. Enroll\n4. Catalog\n5. Logout\nChoice: ";
                    cin >> choice;
                    if (choice == 1) s.displayInfo();
                    if (choice == 2) studUtils.viewGrades(res.id);
                    if (choice == 3) {
                        string c; cout << "Course Code: "; cin >> c;
                        courseMgr.enrollStudent(res.id, c);
                    }
                    if (choice == 4) courseMgr.listAllCourses();
                }
            } 
            // --- PROFESSOR MENU ---
            else if (res.role == "Professor") {
                Professor p(res.first_name, res.last_name, res.id, "HIDDEN", res.extra1, res.extra2);
                while (choice != 5) {
                    cout << "\n1. Profile\n2. Assign Grade\n3. Mark Attendance\n4. View Class List\n5. Logout\nChoice: ";
                    cin >> choice;
                    if (choice == 1) p.displayInfo();
                    if (choice == 2) {
                        string sid, ccode, grade;
                        cout << "Student ID: "; cin >> sid;
                        cout << "Course Code: "; cin >> ccode;
                        cout << "Grade: "; cin >> grade;
                        profUtils.setStudentGrade(sid, ccode, grade);
                    }
                    if (choice == 3) {
                        string sid, ccode, date, stat;
                        cout << "Student ID: "; cin >> sid;
                        cout << "Course Code: "; cin >> ccode;
                        cout << "Date (YYYY-MM-DD): "; cin >> date;
                        cout << "Status (Present/Absent): "; cin >> stat;
                        attendMgr.markAttendance(sid, ccode, date, stat);
                    }
                    if (choice == 4) {
                        string c; cout << "Course Code: "; cin >> c;
                        profUtils.viewEnrolledStudents(c);
                    }
                }
            }
            // --- ADMIN MENU ---
            else if (res.role == "Admin") {
                Admin a(res.first_name, res.last_name, res.id, "HIDDEN", res.extra1);
                while (choice != 5) {
                    cout << "\n1. Profile\n2. Add Course\n3. Univ. Stats\n4. System Check\n5. Logout\nChoice: ";
                    cin >> choice;
                    if (choice == 1) a.displayInfo();
                    if (choice == 2) {
                        string c, n, cr;
                        cout << "Code: "; cin >> c;
                        cout << "Name: "; cin >> n;
                        cout << "Credits: "; cin >> cr;
                        courseMgr.addCourse(c, n, cr);
                    }
                    if (choice == 3) reportMgr.generateSystemReport();
                    if (choice == 4) {
                        System_monitor monitor;
                        monitor.checkServerStatus();
                    }
                }
            }
        } else {
            cout << "Invalid credentials." << endl;
        }
    }
    return 0;
}