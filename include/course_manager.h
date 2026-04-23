#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class CourseManager {
public:
    //for admin
    void addCourse(string code, string name, string credits) {
        ofstream file("program_files/courses.txt", ios::app);
        if (file.is_open()) {
            // Endl first to avoid the "gluing" issue we saw earlier
            file << endl << code << " " << name << " " << credits;
            file.close();
            cout << "Course " << code << " added successfully." << endl;
        }
    }

    //for student
    void enrollStudent(string studentID, string courseCode) {
        ifstream courseFile("program_files/courses.txt");
        string c, n, cr;
        bool exists = false;

        //check if course exists
        while (courseFile >> c >> n >> cr) {
            if (c == courseCode) {
                exists = true;
                break;
            }
        }
        courseFile.close();

        if (exists) {
            ofstream file("program_files/enrollments.txt", ios::app);
            file << studentID << " " << courseCode << endl;
            file.close();
            cout << "Successfully enrolled in " << courseCode << "!" << endl;
        } else {
            cout << "Error: Course " << courseCode << " does not exist in the catalog." << endl;
        }
    }

    //lists courses
    void listAllCourses() {
        ifstream file("program_files/courses.txt");
        string code, name, credits;
        cout << "\n--- University Course Catalog ---" << endl;
        while (file >> code >> name >> credits) {
            if(code != "") { // Skip empty lines
                cout << "ID: " << code << " | Name: " << name << " (" << credits << " Credits)" << endl;
            }
        }
        file.close();
    }
};

#endif