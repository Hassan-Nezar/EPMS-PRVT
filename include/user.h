#ifndef USER_H
#define USER_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class User {
protected:
    string firstName, lastName, id, password, role;

public:
    User(string fn, string ln, string ident, string pass, string r) {
        firstName = fn;
        lastName = ln;
        id = ident;
        password = pass;
        role = r;
    }
    virtual ~User() {} 
    virtual void displayInfo() {
        cout << "Name: " << firstName << " " << lastName << " | Role: " << role << endl;
    }
}; // <--- MUST HAVE THIS SEMICOLON

class Student : public User {
private:
    string gpa, year, major;
public:
    Student(string fn, string ln, string ident, string pass, string g, string y, string m)
        : User(fn, ln, ident, pass, "Student") {
        gpa = g; year = y; major = m;
    }
    void displayInfo() override {
        User::displayInfo();
        cout << "GPA: " << gpa << " | Year: " << year << " | Major: " << major << endl;
    }
}; // <--- MUST HAVE THIS SEMICOLON

class Professor : public User {
private:
    string department, salary;
public:
    Professor(string fn, string ln, string ident, string pass, string dept, string sal)
        : User(fn, ln, ident, pass, "Professor") {
        department = dept; salary = sal;
    }
    void displayInfo() override {
        User::displayInfo();
        cout << "Dept: " << department << " | Salary: " << salary << endl;
    }
}; 

class Admin : public User {
private:
    string bio;
public:
    Admin(string fn, string ln, string ident, string pass, string b)
        : User(fn, ln, ident, pass, "Admin") {
        bio = b;
    }
    void displayInfo() override {
        User::displayInfo();
        cout << "Admin Bio: " << bio << endl;
    }
}; 

#endif