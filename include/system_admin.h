#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
//this file is still under works
using namespace std;


class Admin_utilities {
public:
    void promoteUser(string id) { 
        cout << "User " << id << " has been promoted to Senior Status." << endl; 
    }

    #include <vector>

void check_register() {
    ifstream file("program_files/register_queue.txt");
    if (!file.is_open()) { //check if file is open
        cout << "Queue file not found or empty." << endl;
        return;
    }

    // vector library works like dynamic array
    vector<string> fnames, lnames, years, passes, majors;
    string f, l, y, p, m;
    int count = 1;

    cout << "\nRegistrations in queue: " << endl;
    
    // Read the queue
    while (file >> f >> l >> y >> p >> m) {
        cout << "[" << count << "] " << f << " " << l << " | Year: " << y << " | Major: " << m << endl;
        
        fnames.push_back(f); 
        lnames.push_back(l);
        years.push_back(y); 
        passes.push_back(p); 
        majors.push_back(m);
        count++;
    }
    file.close();

    if (fnames.empty()) { //check if vector is empty
        cout << "No registrations in queue right now." << endl;
        return;
    }

    
    int registry;
    cout << "\nEnter the Number of the registry(or 0 to cancel): ";
    cin >> registry;

    if (registry > 0 && registry <= fnames.size()) {
        int index = registry - 1; // vector like array index start at 0
        char decision;
        
        // Accept or Deny
        cout << "Process " << fnames[index] << "? (A)ccept or (D)eny: ";
        cin >> decision;

        if (decision == 'A' || decision == 'a') {
            string last4;
            cout << "Enter last 4 digits of student new ID: ";
            cin >> last4;

            // put data together in formatte string
            string final_id = years[index] + last4;

            // write to student.txt file
            ofstream student_file("program_files/student.txt", ios::app);
            student_file << fnames[index] << " " << lnames[index] << " " << final_id << " " << passes[index] << " 0.0 " << years[index] << " " << majors[index] << endl;
            student_file.close();
            
            cout << "student accepted!" << endl;
        } 
        else if (decision == 'D' || decision == 'd') {
            cout << "student denied!" << endl;
        } else {
            cout << "Invalid choice. Aborting process." << endl;
            return; 
        }

        //ios::truc is used to delete file
        ofstream rewrite_file("program_files/register_queue.txt", ios::trunc); 
        
        // after delete file content, you write everyone back again except the processed registratioen
        for (int i = 0; i < fnames.size(); i++) {
            if (i != index) { 
                rewrite_file << fnames[i] << " " << lnames[i] << " " << years[i] << " " << passes[i] << " " << majors[i] << endl;
            }
        }
        rewrite_file.close();
    }
}
};

//placeholder
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

//placeholder
class System_monitor {
public:
    void checkServerStatus() { 
        cout << "[MONITOR]: All .txt database files are currently reachable." << endl; 
    }
};

//placeholder
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