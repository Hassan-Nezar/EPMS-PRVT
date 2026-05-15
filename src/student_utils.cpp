#include "../include/student_utils.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

void Student_utilities::viewGrades(string sID) {
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
