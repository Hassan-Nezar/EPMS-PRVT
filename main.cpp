#include <iostream>
#include <string>
#include <cstdlib>
#include "include/auth_manager.h"
#include "include/User.h"
#include "include/course_manager.h"
#include "include/professor_utils.h"
#include "include/student_utils.h"
#include "include/system_admin.h"
#include "include/search_manager.h" 
using namespace std;

void showHeader() {
    
    cout << "\n================================================" << endl;
    cout << "           UNIVERSITY MANAGEMENT SYSTEM     " << endl;
    cout << "================================================" << endl;
}

int main() {
    auth_manager auth;
    CourseManager courseMgr;
    ProfessorUtils profUtils;
    Attendance_manager attendMgr;
    Student_utilities studUtils;
    Reporting_manager reportMgr;
    search_manager sm;

    while (true) {
        showHeader();
        string name, id, pass;
        cout << "\n [ LOGIN REQUIRED ] (or type 'exit' to quit)" << endl;
        cout << " Full Name: "; getline(cin >> ws, name);
        if (name == "exit") break;
        cout << " ID: "; cin >> id;
        cout << " Password: "; cin >> pass;

        auth_manager::login_result res = auth.login(name, id, pass);

        if (res.success) {
            int choice = 0;
            cout << "\nWelcome back, " << res.first_name << "!" << endl;

            //admin
            if (res.role == "Admin") {
                while (choice != 6) {
                    cout << "\n--- Admin Dashboard ---\n1. Profile\n2. Add Course\n3. Univ. Stats\n4. System Check\n5. SEARCH DIRECTORY\n6. Logout\nChoice: ";
                    cin >> choice;
                    if (choice == 1) { Admin a(res.first_name, res.last_name, res.id, "HIDDEN", res.extra1); a.displayInfo(); }
                    if (choice == 2) { string c, n, cr; cout << "Code: "; cin >> c; cout << "Name: "; cin >> n; cout << "Credits: "; cin >> cr; courseMgr.addCourse(c, n, cr); }
                    if (choice == 3) reportMgr.generateSystemReport();
                    if (choice == 4) { System_monitor monitor; monitor.checkServerStatus(); }
                    if (choice == 5) { string target; cout << "Enter ID: "; cin >> target; sm.search_user_byid(target, res.role); }
                }
            } 
            //professor
            else if (res.role == "Professor") {
                while (choice != 6) {
                    cout << "\n--- Professor Dashboard ---\n1. Profile\n2. Assign Grade\n3. Mark Attendance\n4. View Class List\n5. SEARCH DIRECTORY\n6. Logout\nChoice: ";
                    cin >> choice;
                    if (choice == 1) { Professor p(res.first_name, res.last_name, res.id, "HIDDEN", res.extra1, res.extra2); p.displayInfo(); }
                    if (choice == 2) { string sid, ccode, grade; cout << "Student ID: "; cin >> sid; cout << "Course: "; cin >> ccode; cout << "Grade: "; cin >> grade; profUtils.setStudentGrade(sid, ccode, grade); }
                    if (choice == 3) { string sid, ccode, date, stat; cout << "ID: "; cin >> sid; cout << "Course: "; cin >> ccode; cout << "Date: "; cin >> date; cout << "Status: "; cin >> stat; attendMgr.markAttendance(sid, ccode, date, stat); }
                    if (choice == 4) { string c; cout << "Course: "; cin >> c; profUtils.viewEnrolledStudents(c); }
                    if (choice == 5) { string target; cout << "Enter ID: "; cin >> target; sm.search_user_byid(target, res.role); }
                }
            }
            //student
            else if (res.role == "Student") {
                while (choice != 5) {
                    cout << "\n--- Student Portal ---\n1. Profile\n2. My Transcript\n3. Enroll in Course\n4. Course Catalog\n5. Logout\nChoice: ";
                    cin >> choice;
                    if (choice == 1) { Student s(res.first_name, res.last_name, res.id, "HIDDEN", res.extra1, res.extra2, res.extra3); s.displayInfo(); }
                    if (choice == 2) studUtils.viewGrades(res.id);
                    if (choice == 3) { string c; cout << "Course Code: "; cin >> c; courseMgr.enrollStudent(res.id, c); }
                    if (choice == 4) courseMgr.listAllCourses();
                }
            }
        } else {
            cout << "\n[!] Access Denied. Check your credentials." << endl;
        }
    }
    return 0;
}