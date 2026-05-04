#ifndef COURSEMANAGER_H
#define COURSEMANAGER_H
 
#include <string>
 
using namespace std;
 
class CourseManager {
public:
    void addCourse(string code, string name, string credits);
    void enrollStudent(string studentID, string courseCode);
    void listAllCourses();
};
 
#endif