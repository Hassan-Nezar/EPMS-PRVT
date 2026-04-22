#ifndef STUDENTUTILS_H
#define STUDENTUTILS_H
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class Student_utilities {
public:
    void viewGrades(string sID) {
        ifstream file("program_files/grades.txt");
        string id, course, grade;
        cout << "\n--- Your Grades ---" << endl;
        while (file >> id >> course >> grade) {
            if (id == sID) { 
                cout << "Course: " << course << " | Grade: " << grade << endl;
            }
        }
        file.close();
    }
};

class View_search {
public:
    void globalSearch(string term) {
        cout << "Searching for: " << term << "... (Results displayed if found)" << endl;
        // Logic to search courses.txt
    }
};
#endif