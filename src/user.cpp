#include "../include/user.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

User::User(string fn, string ln, string ident, string pass, string r) {
    firstName = fn;
    lastName = ln;
    id = ident;
    password = pass;
    role = r;
}


void User::displayInfo() {
    cout << "Name: " << firstName << " " << lastName << " | Role: " << role << endl;
}

//getters:
string User::get_firstname() const {
    return firstName;
}
string User::get_lastname() const {
    return lastName;
}
string User::get_id() const {
    return id;
}
string User::get_password() const {
    return password;
}
string User::get_role() const {
    return role;
}

//setters:
void User::set_firstname(string fn) {
    firstName = fn;
}
void User::set_lastname(string ln) {
    lastName = ln;
}
void User::set_id(string ident) {
    id = ident;
}
void User::set_password(string passwd) {
    password = passwd;
}
void User::set_role(string rl) {
    role = rl;
}

// ---------------- Student ----------------

Student::Student(string fn, string ln, string ident, string pass, string g, string y, string m)
    : User(fn, ln, ident, pass, "Student") {
    gpa = g;
    year = y;
    major = m;
}

void Student::displayInfo() {
    User::displayInfo();
    cout << "GPA: " << gpa << " | Year: " << year << " | Major: " << major << endl;
}

string Student::get_gpa() const {
    return gpa;
}
string Student::get_year() const {
    return year;
}
string Student::get_major() const {
    return major;
}

void Student::set_gpa(string gp) {
    gpa = gp;
}
void Student::set_year(string yr) {
    year = yr;
}
void Student::set_major(string mjr) {
    major = mjr;
}

// professor

Professor::Professor(string fn, string ln, string ident, string pass, string dept, string sal)
    : User(fn, ln, ident, pass, "Professor") {
    department = dept;
    salary = sal;
}

void Professor::displayInfo() {
    User::displayInfo();
    cout << "Dept: " << department << " | Salary: " << salary << endl;
}

string Professor::get_department() const {
    return department;
}
string Professor::get_salary() const {
    return salary;
}

// Fixed: Changed User:: to Professor::
void Professor::set_department(string dpt) {
    department = dpt;
}
void Professor::set_salary(string slry) {
    salary = slry;
}

// admin

Admin::Admin(string fn, string ln, string ident, string pass, string b)
    : User(fn, ln, ident, pass, "Admin") {
    bio = b;
}

void Admin::displayInfo() {
    User::displayInfo();
    cout << "Admin Bio: " << bio << endl;
}

// Added missing Admin implementations
string Admin::get_bio() const {
    return bio;
}

void Admin::set_bio(string b) {
    bio = b;
}
