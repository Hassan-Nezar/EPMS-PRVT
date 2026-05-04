#include "../include/user.h"

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

    //getters:
    string get_firstname() const {
        return firstName;
    }
    string get_lastname() const {
        return lastName;
    }
    string get_id() const {
        return id;
    }
    string get_password() const {
        return password;
    }
    string get_role() const {
        return role;
    }
    //setters:
    void set_firstname(string fn) {
        firstName = fn;
    }
    void set_lastname(string ln) {
        lastName = ln;
    }
    void set_id(string ident) {
        id = ident;
    }
    void set_password(string passwd) {
        password = passwd;
    }
    void set_role(string rl) {
        role = rl;
    }
}; 

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
    //getters:
    string get_gpa() const {
        return gpa;
    }
    string get_year() const {
        return year;
    }
    string get_major() const {
        return major;
    }
    //setters:
    void set_gpa(string gp) {
        gpa = gp;
    }
    void set_year(string yr) {
        year = yr;
    }
    void set_major(string mjr) {
        major = mjr;
    }
}; 

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
    
    //getters:
        string get_department() const {
        return department;
    }
    string get_salary() const {
        return salary;
    }
    //setters:
    void set_department(string dpt) {
        department = dpt;
    }
    void set_salary(string slry) {
        salary = slry;
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
