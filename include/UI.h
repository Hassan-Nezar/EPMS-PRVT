#ifndef UI_H
#define UI_H

#include "imgui.h"
#include "include/auth_manager.h"
#include "include/user.h"
#include "include/course_manager.h"
#include "include/professor_utils.h"
#include "include/student_utils.h"
#include "include/system_admin.h"
#include "include/search_manager.h"

#include <string>
#include <vector>

// ─────────────────────────────────────────────
//  Small helper: draw a coloured banner text
// ─────────────────────────────────────────────
static void BannerText(const char* text, ImVec4 col = ImVec4(0.4f, 0.8f, 1.0f, 1.0f)) {
    ImGui::PushStyleColor(ImGuiCol_Text, col);
    ImGui::Text("%s", text);
    ImGui::PopStyleColor();
}

class UIManager {
public:
    // ── FSM states ──────────────────────────────
    enum AppState {
        MAIN_MENU,
        LOGIN,
        REGISTER,
        ADMIN_DASH,
        PROFESSOR_DASH,
        STUDENT_DASH
    };
    AppState currentState = MAIN_MENU;

    // ── Shared login result (kept alive between frames) ──
    auth_manager::login_result loggedInUser;

    // ── Feedback messages ───────────────────────
    std::string feedbackMsg = "";
    ImVec4      feedbackCol = ImVec4(1, 1, 1, 1);

    // ─────────────────────────────────────────────
    //  Master render — call this every frame
    // ─────────────────────────────────────────────
    void render(auth_manager&      auth,
                CourseManager&     courseMgr,
                ProfessorUtils&    profUtils,
                Attendance_manager& attendMgr,
                Student_utilities& studUtils,
                Reporting_manager& reportMgr,
                search_manager&    sm)
    {
        switch (currentState) {
            case MAIN_MENU:      showMainMenu();                                          break;
            case LOGIN:          showLogin(auth);                                         break;
            case REGISTER:       showRegister(auth);                                      break;
            case ADMIN_DASH:     showAdminDash(auth, courseMgr, reportMgr, sm);          break;
            case PROFESSOR_DASH: showProfessorDash(profUtils, attendMgr, sm);            break;
            case STUDENT_DASH:   showStudentDash(courseMgr, studUtils);                  break;
        }
    }

private:
    // ══════════════════════════════════════════════
    //  [1]  MAIN MENU
    // ══════════════════════════════════════════════
    void showMainMenu() {
        ImGui::SetNextWindowSize(ImVec2(420, 260), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("##main", nullptr,
                     ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                     ImGuiWindowFlags_NoMove);

        BannerText("UNIVERSITY MANAGEMENT SYSTEM", ImVec4(0.4f, 0.85f, 1.0f, 1.0f));
        ImGui::Separator();
        ImGui::Spacing();
        ImGui::Text("Please choose an option to continue:");
        ImGui::Spacing();

        if (ImGui::Button("Login", ImVec2(380, 45))) {
            feedbackMsg = "";
            currentState = LOGIN;
        }
        ImGui::Spacing();
        if (ImGui::Button("Register as Student", ImVec2(380, 45))) {
            feedbackMsg = "";
            currentState = REGISTER;
        }
        ImGui::Spacing();
        if (ImGui::Button("Exit", ImVec2(380, 35)))
            exit(0);

        ImGui::End();
    }

    // ══════════════════════════════════════════════
    //  [2]  LOGIN
    // ══════════════════════════════════════════════
    char lName[128] = "";
    char lID[32]    = "";
    char lPass[64]  = "";

    void showLogin(auth_manager& auth) {
        ImGui::SetNextWindowSize(ImVec2(440, 280), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Login Portal", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::InputText("Full Name", lName, sizeof(lName));
        ImGui::InputText("ID",        lID,   sizeof(lID));
        ImGui::InputText("Password",  lPass, sizeof(lPass),
                         ImGuiInputTextFlags_Password);
        ImGui::Spacing();

        if (ImGui::Button("Login", ImVec2(200, 40))) {
            auto res = auth.login(std::string(lName), std::string(lID), std::string(lPass));
            if (res.success) {
                loggedInUser = res;
                feedbackMsg  = "";
                // clear inputs for next session
                memset(lName, 0, sizeof(lName));
                memset(lID,   0, sizeof(lID));
                memset(lPass, 0, sizeof(lPass));

                if (res.role == "Admin")     currentState = ADMIN_DASH;
                else if (res.role == "Professor") currentState = PROFESSOR_DASH;
                else                          currentState = STUDENT_DASH;
            } else {
                feedbackMsg = "[!] Access Denied. Check your credentials.";
                feedbackCol = ImVec4(1.0f, 0.35f, 0.35f, 1.0f);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Back", ImVec2(100, 40)))
            currentState = MAIN_MENU;

        if (!feedbackMsg.empty()) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::End();
    }

    // ══════════════════════════════════════════════
    //  [3]  REGISTER
    // ══════════════════════════════════════════════
    char rFName[64]  = "";
    char rLName[64]  = "";
    char rYear[8]    = "";
    char rPass[64]   = "";
    int  rMajorIdx   = 0;

    const char* majors[10] = {
        "Cyber_Security", "AI_Robotics",   "Game_Design",
        "Astrophysics",   "Bio_Informatics","Quantum_Computing",
        "Civil_Engineering","Marine_Biology","Software_Systems","Data_Science"
    };

    void showRegister(auth_manager& auth) {
        ImGui::SetNextWindowSize(ImVec2(460, 380), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Student Registration", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        ImGui::InputText("First Name", rFName, sizeof(rFName));
        ImGui::InputText("Last Name",  rLName, sizeof(rLName));
        ImGui::InputText("Year (e.g. 1 for Freshman)", rYear, sizeof(rYear));
        ImGui::InputText("Password",   rPass,  sizeof(rPass),
                         ImGuiInputTextFlags_Password);
        ImGui::Spacing();
        ImGui::Text("Major:");
        ImGui::Combo("##major", &rMajorIdx, majors, IM_ARRAYSIZE(majors));
        ImGui::Spacing();

        if (ImGui::Button("Submit Application", ImVec2(220, 40))) {
            bool valid = (rFName[0] != '\0' && rLName[0] != '\0' &&
                          rYear[0]  != '\0' && rPass[0]  != '\0');
            if (valid) {
                // Write directly to register_queue.txt (mirrors auth_manager::register_user)
                std::ofstream file("program_files/register_queue.txt", std::ios::app);
                file << rFName << " " << rLName << " "
                     << rYear  << " " << rPass  << " "
                     << majors[rMajorIdx] << "\n";
                file.close();

                feedbackMsg = "[OK] Application submitted! Awaiting Admin approval.";
                feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);

                // clear fields
                memset(rFName, 0, sizeof(rFName));
                memset(rLName, 0, sizeof(rLName));
                memset(rYear,  0, sizeof(rYear));
                memset(rPass,  0, sizeof(rPass));
                rMajorIdx = 0;
            } else {
                feedbackMsg = "[!] Please fill in all fields.";
                feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 40))) {
            feedbackMsg = "";
            currentState = MAIN_MENU;
        }

        if (!feedbackMsg.empty()) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
        }
        ImGui::End();
    }

    // ══════════════════════════════════════════════
    //  [4]  ADMIN DASHBOARD
    // ══════════════════════════════════════════════
    // sub-window toggles
    bool showAddCourse   = false;
    bool showSearch      = false;

    // add-course inputs
    char acCode[32]  = "";
    char acName[64]  = "";
    char acCred[8]   = "";

    // search inputs
    char adminSearchID[32] = "";

    void showAdminDash(auth_manager&      auth,
                       CourseManager&     courseMgr,
                       Reporting_manager& reportMgr,
                       search_manager&    sm)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 480), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Admin Dashboard", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        BannerText("Admin Dashboard", ImVec4(1.0f, 0.75f, 0.2f, 1.0f));
        ImGui::Text("Logged in as: %s %s  [ID: %s]",
                    loggedInUser.first_name.c_str(),
                    loggedInUser.last_name.c_str(),
                    loggedInUser.id.c_str());
        ImGui::Separator();
        ImGui::Spacing();

        // ── [1] Profile ──────────────────────────
        if (ImGui::CollapsingHeader("Profile")) {
            Admin a(loggedInUser.first_name, loggedInUser.last_name,
                    loggedInUser.id, "HIDDEN", loggedInUser.extra1);
            a.displayInfo();   // prints to stdout — same as terminal
        }

        // ── [2] Add Course ───────────────────────
        if (ImGui::CollapsingHeader("Add Course")) {
            ImGui::InputText("Course Code",    acCode, sizeof(acCode));
            ImGui::InputText("Course Name",    acName, sizeof(acName));
            ImGui::InputText("Credits",        acCred, sizeof(acCred));
            if (ImGui::Button("Add##course", ImVec2(120, 30))) {
                if (acCode[0] && acName[0] && acCred[0]) {
                    courseMgr.addCourse(acCode, acName, acCred);
                    feedbackMsg = "[OK] Course added.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(acCode, 0, sizeof(acCode));
                    memset(acName, 0, sizeof(acName));
                    memset(acCred, 0, sizeof(acCred));
                } else {
                    feedbackMsg = "[!] Fill all course fields.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [3] University Stats ─────────────────
        if (ImGui::CollapsingHeader("University Stats")) {
            if (ImGui::Button("Generate Report##admin", ImVec2(200, 30)))
                reportMgr.generateSystemReport();  // output goes to stdout/file as normal
            ImGui::TextDisabled("(output printed to console / report file)");
        }

        // ── [4] System Check ─────────────────────
        if (ImGui::CollapsingHeader("System Check")) {
            if (ImGui::Button("Check Server Status", ImVec2(200, 30))) {
                System_monitor monitor;
                monitor.checkServerStatus();
            }
            ImGui::TextDisabled("(output printed to console)");
        }

        // ── [5] Search Directory ─────────────────
        if (ImGui::CollapsingHeader("Search Directory")) {
            ImGui::InputText("User ID##asearch", adminSearchID, sizeof(adminSearchID));
            if (ImGui::Button("Search##admin", ImVec2(120, 30))) {
                if (adminSearchID[0])
                    sm.search_user_byid(std::string(adminSearchID), loggedInUser.role);
            }
            ImGui::TextDisabled("(results printed to console)");
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (!feedbackMsg.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        if (ImGui::Button("Logout", ImVec2(120, 35))) {
            feedbackMsg  = "";
            currentState = MAIN_MENU;
        }
        ImGui::End();
    }

    // ══════════════════════════════════════════════
    //  [5]  PROFESSOR DASHBOARD
    // ══════════════════════════════════════════════
    char pGradeSID[32]   = "";
    char pGradeCourse[32]= "";
    char pGradeVal[8]    = "";

    char pAttSID[32]     = "";
    char pAttCourse[32]  = "";
    char pAttDate[16]    = "";
    char pAttStatus[16]  = "";

    char pClassCourse[32]= "";

    char profSearchID[32]= "";

    void showProfessorDash(ProfessorUtils&     profUtils,
                           Attendance_manager& attendMgr,
                           search_manager&     sm)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 520), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Professor Dashboard", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        BannerText("Professor Dashboard", ImVec4(0.5f, 1.0f, 0.7f, 1.0f));
        ImGui::Text("Logged in as: Prof. %s %s  [ID: %s]  Dept: %s",
                    loggedInUser.first_name.c_str(),
                    loggedInUser.last_name.c_str(),
                    loggedInUser.id.c_str(),
                    loggedInUser.extra1.c_str());
        ImGui::Separator();
        ImGui::Spacing();

        // ── [1] Profile ──────────────────────────
        if (ImGui::CollapsingHeader("Profile")) {
            Professor p(loggedInUser.first_name, loggedInUser.last_name,
                        loggedInUser.id, "HIDDEN",
                        loggedInUser.extra1, loggedInUser.extra2);
            p.displayInfo();
        }

        // ── [2] Assign Grade ─────────────────────
        if (ImGui::CollapsingHeader("Assign Grade")) {
            ImGui::InputText("Student ID##pgrade",  pGradeSID,    sizeof(pGradeSID));
            ImGui::InputText("Course Code##pgrade", pGradeCourse, sizeof(pGradeCourse));
            ImGui::InputText("Grade##pgrade",       pGradeVal,    sizeof(pGradeVal));
            if (ImGui::Button("Assign##grade", ImVec2(120, 30))) {
                if (pGradeSID[0] && pGradeCourse[0] && pGradeVal[0]) {
                    profUtils.setStudentGrade(pGradeSID, pGradeCourse, pGradeVal);
                    feedbackMsg = "[OK] Grade assigned.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(pGradeSID, 0, sizeof(pGradeSID));
                    memset(pGradeCourse, 0, sizeof(pGradeCourse));
                    memset(pGradeVal, 0, sizeof(pGradeVal));
                } else {
                    feedbackMsg = "[!] Fill all grade fields.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [3] Mark Attendance ──────────────────
        if (ImGui::CollapsingHeader("Mark Attendance")) {
            ImGui::InputText("Student ID##patt",  pAttSID,    sizeof(pAttSID));
            ImGui::InputText("Course##patt",       pAttCourse, sizeof(pAttCourse));
            ImGui::InputText("Date (YYYY-MM-DD)",  pAttDate,   sizeof(pAttDate));
            ImGui::InputText("Status (P/A/L)",     pAttStatus, sizeof(pAttStatus));
            if (ImGui::Button("Mark##att", ImVec2(120, 30))) {
                if (pAttSID[0] && pAttCourse[0] && pAttDate[0] && pAttStatus[0]) {
                    attendMgr.markAttendance(pAttSID, pAttCourse, pAttDate, pAttStatus);
                    feedbackMsg = "[OK] Attendance recorded.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(pAttSID,    0, sizeof(pAttSID));
                    memset(pAttCourse, 0, sizeof(pAttCourse));
                    memset(pAttDate,   0, sizeof(pAttDate));
                    memset(pAttStatus, 0, sizeof(pAttStatus));
                } else {
                    feedbackMsg = "[!] Fill all attendance fields.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [4] View Class List ──────────────────
        if (ImGui::CollapsingHeader("View Class List")) {
            ImGui::InputText("Course Code##pclass", pClassCourse, sizeof(pClassCourse));
            if (ImGui::Button("View##class", ImVec2(120, 30))) {
                if (pClassCourse[0])
                    profUtils.viewEnrolledStudents(pClassCourse);
            }
            ImGui::TextDisabled("(output printed to console)");
        }

        // ── [5] Search Directory ─────────────────
        if (ImGui::CollapsingHeader("Search Directory")) {
            ImGui::InputText("User ID##psearch", profSearchID, sizeof(profSearchID));
            if (ImGui::Button("Search##prof", ImVec2(120, 30))) {
                if (profSearchID[0])
                    sm.search_user_byid(std::string(profSearchID), loggedInUser.role);
            }
            ImGui::TextDisabled("(results printed to console)");
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (!feedbackMsg.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        if (ImGui::Button("Logout", ImVec2(120, 35))) {
            feedbackMsg  = "";
            currentState = MAIN_MENU;
        }
        ImGui::End();
    }

    // ══════════════════════════════════════════════
    //  [6]  STUDENT PORTAL
    // ══════════════════════════════════════════════
    char sEnrollCode[32] = "";

    void showStudentDash(CourseManager&     courseMgr,
                         Student_utilities& studUtils)
    {
        ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_Always);
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f,
                                       ImGui::GetIO().DisplaySize.y * 0.5f),
                                ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::Begin("Student Portal", nullptr,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        BannerText("Student Portal", ImVec4(0.6f, 0.6f, 1.0f, 1.0f));
        ImGui::Text("Logged in as: %s %s  [ID: %s]  GPA: %s  Year: %s  Major: %s",
                    loggedInUser.first_name.c_str(),
                    loggedInUser.last_name.c_str(),
                    loggedInUser.id.c_str(),
                    loggedInUser.extra1.c_str(),   // gpa
                    loggedInUser.extra2.c_str(),   // year
                    loggedInUser.extra3.c_str());  // major
        ImGui::Separator();
        ImGui::Spacing();

        // ── [1] Profile ──────────────────────────
        if (ImGui::CollapsingHeader("Profile")) {
            Student s(loggedInUser.first_name, loggedInUser.last_name,
                      loggedInUser.id, "HIDDEN",
                      loggedInUser.extra1, loggedInUser.extra2, loggedInUser.extra3);
            s.displayInfo();
        }

        // ── [2] My Transcript ────────────────────
        if (ImGui::CollapsingHeader("My Transcript")) {
            if (ImGui::Button("View Grades", ImVec2(160, 30)))
                studUtils.viewGrades(loggedInUser.id);
            ImGui::TextDisabled("(output printed to console)");
        }

        // ── [3] Enroll in Course ─────────────────
        if (ImGui::CollapsingHeader("Enroll in Course")) {
            ImGui::InputText("Course Code##senroll", sEnrollCode, sizeof(sEnrollCode));
            if (ImGui::Button("Enroll##s", ImVec2(120, 30))) {
                if (sEnrollCode[0]) {
                    courseMgr.enrollStudent(loggedInUser.id, sEnrollCode);
                    feedbackMsg = "[OK] Enrollment request sent.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(sEnrollCode, 0, sizeof(sEnrollCode));
                } else {
                    feedbackMsg = "[!] Enter a course code.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [4] Course Catalog ───────────────────
        if (ImGui::CollapsingHeader("Course Catalog")) {
            if (ImGui::Button("List All Courses", ImVec2(180, 30)))
                courseMgr.listAllCourses();
            ImGui::TextDisabled("(output printed to console)");
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (!feedbackMsg.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }

        if (ImGui::Button("Logout", ImVec2(120, 35))) {
            feedbackMsg  = "";
            currentState = MAIN_MENU;
        }
        ImGui::End();
    }
};

#endif // UI_H