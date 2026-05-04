#ifndef SYSTEMADMIN_H
#define SYSTEMADMIN_H

#include <string>

using namespace std;

class Admin_utilities {
public:
    void promoteUser(string id);
    void check_register();
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