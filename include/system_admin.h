#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include <string>
#include <vector>

using namespace std;

struct registration_request {
    string fname;
    string lname;
    string year;
    string pass;
    string major;
};

class Admin_utilities {
public:
    void promoteUser(string id);
    void check_register();
    vector<registration_request> get_register_queue();
    void accept_register(int index, string last4);
    void deny_register(int index);
};

class Reporting_manager {
public:
    void generateSystemReport();
    void calculateCourseAverage(string courseCode);
    void calculateUniversityAverage();
};

class System_monitor {
public:
    void checkServerStatus();
};

class Database_logger {
public:
    void logAction(string msg);
};

#endif
