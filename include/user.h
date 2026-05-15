#ifndef USER_H
#define USER_H

#include <string>

using namespace std;

class User {
protected:
    string firstName, lastName, id, password, role;

public:
    User(string fn, string ln, string ident, string pass, string r);
    virtual ~User() = default;
    virtual void displayInfo();

    // getters:
    string get_firstname() const;
    string get_lastname() const;
    string get_id() const;
    string get_password() const;
    string get_role() const;
    // setters:
    void set_firstname(string fn);
    void set_lastname(string ln);
    void set_id(string ident);
    void set_password(string passwd);
    void set_role(string rl);
};

class Student : public User {
private:
    string gpa, year, major;
public:
    Student(string fn, string ln, string ident, string pass, string g, string y, string m);
    void displayInfo() override;

    // getters:
    string get_gpa() const;
    string get_year() const;
    string get_major() const;
    // setters:
    void set_gpa(string gp);
    void set_year(string yr);
    void set_major(string mjr);
};

class Professor : public User {
private:
    string department, salary;
public:
    Professor(string fn, string ln, string ident, string pass, string dept, string sal);
    void displayInfo() override;

    //getters:
    string get_department() const;
    string get_salary() const;
    //setters:
    void set_department(string dpt); // Fixed: Added parameter
    void set_salary(string slry);    // Fixed: Added parameter
};

class Admin : public User {
private:
    string bio;
public:
    Admin(string fn, string ln, string ident, string pass, string b);
    void displayInfo() override;

    // Added missing getters/setters for Admin
    string get_bio() const;
    void set_bio(string b);
};

#endif