#ifndef SEARCHMANAGER_H
#define SEARCHMANAGER_H
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

class search_manager { 
public:
    void search_user_byid(string targetID, string userRole) {
        int slength = targetID.length();
        
        if (userRole == "Admin" || userRole == "Professor") {
            if (slength == 8) {
                string first4 = targetID.substr(0, 4);

                
                if (first4 == "0000") {
                    if (userRole == "Admin") {
                        ifstream file("program_files/admin.txt");
                        string firstn, lastn, fileID, pass, bio;
                        while (file >> firstn >> lastn >> fileID >> pass >> bio) {
                            if (targetID == fileID) {
                                cout << "Name: " << firstn << " " << lastn << "\nID: " << fileID << "\nRole: Admin\nBio: " << bio << endl;
                            }
                        }
                    } else {
                        cout << "Access Denied: Professors cannot search for admins!" << endl;
                    }
                }
                
                else if (first4 == "9999") {
                    ifstream file("program_files/professor.txt");
                    string firstn, lastn, fileID, pass, faculty, salary;
                    while (file >> firstn >> lastn >> fileID >> pass >> faculty >> salary) {
                        if (targetID == fileID) {
                            cout << "Name: " << firstn << " " << lastn << "\nID: " << fileID << "\nRole: Professor\nFaculty: " << faculty << endl;
                        }
                    }
                }
                
                else {
                    ifstream file("program_files/student.txt");
                    string firstn, lastn, fileID, pass, gpa, year, major;
                    while (file >> firstn >> lastn >> fileID >> pass >> gpa >> year >> major) {
                        if (targetID == fileID) {
                            cout << "Name: " << firstn << " " << lastn << "\nID: " << fileID << "\nGPA: " << gpa << "\nYear: " << year << "\nMajor: " << major << endl;
                        }
                    }
                }
            } else {
                cout << "Invalid ID. IDs must be 8 characters long." << endl;
            }
        } else {
            cout << "Access Denied: Students cannot use the search feature." << endl;
        }
    }
};

#endif