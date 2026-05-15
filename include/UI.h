#ifndef UI_H
#define UI_H

#include "imgui.h"
#include "auth_manager.h"
#include "user.h"
#include "course_manager.h"
#include "professor_utils.h"
#include "student_utils.h"
#include "system_admin.h"
#include "search_manager.h"

#include <string>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <sstream>

// ─────────────────────────────────────────────
//  Small helper: draw a coloured banner text
// ─────────────────────────────────────────────
static void BannerText(const char* text, ImVec4 col = ImVec4(0.4f, 0.8f, 1.0f, 1.0f)) {
    ImGui::PushStyleColor(ImGuiCol_Text, col);
    ImGui::Text("%s", text);
    ImGui::PopStyleColor();
}

template <typename Func>
static std::string CaptureCout(Func func) {
    std::ostringstream output;
    std::streambuf* oldCout = std::cout.rdbuf(output.rdbuf());
    func();
    std::cout.rdbuf(oldCout);
    return output.str();
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
    std::string outputMsg = "";

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
    void showOutputBox() {
        if (!outputMsg.empty()) {
            ImGui::Spacing();
            ImGui::Separator();
            ImGui::TextDisabled("Output:");
            ImGui::BeginChild("##output", ImVec2(0, 110), true);
            ImGui::TextWrapped("%s", outputMsg.c_str());
            ImGui::EndChild();
        }
    }

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
            outputMsg = "";
            currentState = LOGIN;
        }
        ImGui::Spacing();
        if (ImGui::Button("Register as Student", ImVec2(380, 45))) {
            feedbackMsg = "";
            outputMsg = "";
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
            auth_manager::login_result res;
            outputMsg = CaptureCout([&]() {
                res = auth.login(std::string(lName), std::string(lID), std::string(lPass));
            });
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
        if (ImGui::Button("Back", ImVec2(100, 40))) {
            outputMsg = "";
            currentState = MAIN_MENU;
        }

        if (!feedbackMsg.empty()) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
        }
        showOutputBox();
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
                outputMsg = CaptureCout([&]() {
                    auth.register_user(rFName, rLName, rYear, rPass, majors[rMajorIdx]);
                });

                feedbackMsg = "[OK] Application submitted! Awaiting Admin approval.";
                feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);

                // clear fields
                memset(rFName, 0, sizeof(rFName));
                memset(rLName, 0, sizeof(rLName));
                memset(rYear,  0, sizeof(rYear));
                memset(rPass,  0, sizeof(rPass));
                rMajorIdx = 0;
            } else {
                outputMsg = "";
                feedbackMsg = "[!] Please fill in all fields.";
                feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
            }
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(100, 40))) {
            feedbackMsg = "";
            outputMsg = "";
            currentState = MAIN_MENU;
        }

        if (!feedbackMsg.empty()) {
            ImGui::Spacing();
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
        }
        showOutputBox();
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
    vector<registration_request> registerQueue;
    int selectedRegister = -1;
    char registerLast4[8] = "";

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
            if (ImGui::Button("Show Profile##admin", ImVec2(150, 30))) {
                Admin a(loggedInUser.first_name, loggedInUser.last_name,
                        loggedInUser.id, "HIDDEN", loggedInUser.extra1);
                outputMsg = CaptureCout([&]() {
                    a.displayInfo();
                });
            }
        }

        // ── [2] Add Course ───────────────────────
        if (ImGui::CollapsingHeader("Add Course")) {
            ImGui::InputText("Course Code",    acCode, sizeof(acCode));
            ImGui::InputText("Course Name",    acName, sizeof(acName));
            ImGui::InputText("Credits",        acCred, sizeof(acCred));
            if (ImGui::Button("Add##course", ImVec2(120, 30))) {
                if (acCode[0] && acName[0] && acCred[0]) {
                    outputMsg = CaptureCout([&]() {
                        courseMgr.addCourse(acCode, acName, acCred);
                    });
                    feedbackMsg = "[OK] Course added.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(acCode, 0, sizeof(acCode));
                    memset(acName, 0, sizeof(acName));
                    memset(acCred, 0, sizeof(acCred));
                } else {
                    outputMsg = "";
                    feedbackMsg = "[!] Fill all course fields.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [3] University Stats ─────────────────
        if (ImGui::CollapsingHeader("University Stats")) {
            if (ImGui::Button("Generate Report##admin", ImVec2(200, 30))) {
                outputMsg = CaptureCout([&]() {
                    reportMgr.generateSystemReport();
                });
            }
        }

        // ── [4] System Check ─────────────────────
        if (ImGui::CollapsingHeader("System Check")) {
            if (ImGui::Button("Check Server Status", ImVec2(200, 30))) {
                System_monitor monitor;
                outputMsg = CaptureCout([&]() {
                    monitor.checkServerStatus();
                });
            }
        }

        // ── [5] Search Directory ─────────────────
        if (ImGui::CollapsingHeader("Search Directory")) {
            ImGui::InputText("User ID##asearch", adminSearchID, sizeof(adminSearchID));
            if (ImGui::Button("Search##admin", ImVec2(120, 30))) {
                if (adminSearchID[0]) {
                    outputMsg = CaptureCout([&]() {
                        sm.search_user_byid(std::string(adminSearchID), loggedInUser.role);
                    });
                }
            }
        }

        // ── [6] Review Registrations ─────────────
        if (ImGui::CollapsingHeader("Review Registrations")) {
            Admin_utilities adminUtils;

            if (ImGui::Button("Refresh Queue", ImVec2(150, 30))) {
                registerQueue = adminUtils.get_register_queue();
                if (selectedRegister >= registerQueue.size()) {
                    selectedRegister = -1;
                }
                std::ostringstream msg;
                if (registerQueue.empty()) {
                    msg << "No registrations in queue right now.";
                } else {
                    msg << "Registrations in queue:\n";
                    for (int i = 0; i < registerQueue.size(); i++) {
                        msg << "[" << (i + 1) << "] "
                            << registerQueue[i].fname << " " << registerQueue[i].lname
                            << " | Year: " << registerQueue[i].year
                            << " | Major: " << registerQueue[i].major << "\n";
                    }
                }
                outputMsg = msg.str();
            }

            if (!registerQueue.empty()) {
                ImGui::Spacing();
                ImGui::Text("Pending Applications:");
                ImGui::BeginChild("##registerQueue", ImVec2(0, 130), true);
                for (int i = 0; i < registerQueue.size(); i++) {
                    std::string label = registerQueue[i].fname + " " + registerQueue[i].lname +
                                        " | Year: " + registerQueue[i].year +
                                        " | Major: " + registerQueue[i].major;
                    if (ImGui::Selectable(label.c_str(), selectedRegister == i)) {
                        selectedRegister = i;
                    }
                }
                ImGui::EndChild();

                if (selectedRegister >= 0 && selectedRegister < registerQueue.size()) {
                    ImGui::Text("Selected: %s %s",
                                registerQueue[selectedRegister].fname.c_str(),
                                registerQueue[selectedRegister].lname.c_str());
                } else {
                    ImGui::TextDisabled("Select an application from the list.");
                }
            } else {
                ImGui::TextDisabled("Click Refresh Queue to load pending registrations.");
            }

            ImGui::InputText("Last 4 ID Digits", registerLast4, sizeof(registerLast4));

            if (ImGui::Button("Accept##register", ImVec2(120, 30))) {
                if (selectedRegister < 0 || selectedRegister >= registerQueue.size()) {
                    outputMsg = "";
                    feedbackMsg = "[!] Select an application first.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                } else if (strlen(registerLast4) != 4) {
                    outputMsg = "";
                    feedbackMsg = "[!] Enter exactly 4 ID digits.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                } else {
                    outputMsg = CaptureCout([&]() {
                        adminUtils.accept_register(selectedRegister + 1, registerLast4);
                    });
                    registerQueue = adminUtils.get_register_queue();
                    selectedRegister = -1;
                    memset(registerLast4, 0, sizeof(registerLast4));
                    feedbackMsg = "[OK] Registration processed.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Deny##register", ImVec2(120, 30))) {
                if (selectedRegister < 0 || selectedRegister >= registerQueue.size()) {
                    outputMsg = "";
                    feedbackMsg = "[!] Select an application first.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                } else {
                    outputMsg = CaptureCout([&]() {
                        adminUtils.deny_register(selectedRegister + 1);
                    });
                    registerQueue = adminUtils.get_register_queue();
                    selectedRegister = -1;
                    feedbackMsg = "[OK] Registration processed.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                }
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (!feedbackMsg.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }
        showOutputBox();

        if (ImGui::Button("Logout", ImVec2(120, 35))) {
            feedbackMsg  = "";
            outputMsg = "";
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
            if (ImGui::Button("Show Profile##prof", ImVec2(150, 30))) {
                Professor p(loggedInUser.first_name, loggedInUser.last_name,
                            loggedInUser.id, "HIDDEN",
                            loggedInUser.extra1, loggedInUser.extra2);
                outputMsg = CaptureCout([&]() {
                    p.displayInfo();
                });
            }
        }

        // ── [2] Assign Grade ─────────────────────
        if (ImGui::CollapsingHeader("Assign Grade")) {
            ImGui::InputText("Student ID##pgrade",  pGradeSID,    sizeof(pGradeSID));
            ImGui::InputText("Course Code##pgrade", pGradeCourse, sizeof(pGradeCourse));
            ImGui::InputText("Grade##pgrade",       pGradeVal,    sizeof(pGradeVal));
            if (ImGui::Button("Assign##grade", ImVec2(120, 30))) {
                if (pGradeSID[0] && pGradeCourse[0] && pGradeVal[0]) {
                    outputMsg = CaptureCout([&]() {
                        profUtils.setStudentGrade(pGradeSID, pGradeCourse, pGradeVal);
                    });
                    feedbackMsg = "[OK] Grade assigned.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(pGradeSID, 0, sizeof(pGradeSID));
                    memset(pGradeCourse, 0, sizeof(pGradeCourse));
                    memset(pGradeVal, 0, sizeof(pGradeVal));
                } else {
                    outputMsg = "";
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
                    outputMsg = CaptureCout([&]() {
                        attendMgr.markAttendance(pAttSID, pAttCourse, pAttDate, pAttStatus);
                    });
                    feedbackMsg = "[OK] Attendance recorded.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(pAttSID,    0, sizeof(pAttSID));
                    memset(pAttCourse, 0, sizeof(pAttCourse));
                    memset(pAttDate,   0, sizeof(pAttDate));
                    memset(pAttStatus, 0, sizeof(pAttStatus));
                } else {
                    outputMsg = "";
                    feedbackMsg = "[!] Fill all attendance fields.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [4] View Class List ──────────────────
        if (ImGui::CollapsingHeader("View Class List")) {
            ImGui::InputText("Course Code##pclass", pClassCourse, sizeof(pClassCourse));
            if (ImGui::Button("View##class", ImVec2(120, 30))) {
                if (pClassCourse[0]) {
                    outputMsg = CaptureCout([&]() {
                        profUtils.viewEnrolledStudents(pClassCourse);
                    });
                }
            }
        }

        // ── [5] Search Directory ─────────────────
        if (ImGui::CollapsingHeader("Search Directory")) {
            ImGui::InputText("User ID##psearch", profSearchID, sizeof(profSearchID));
            if (ImGui::Button("Search##prof", ImVec2(120, 30))) {
                if (profSearchID[0]) {
                    outputMsg = CaptureCout([&]() {
                        sm.search_user_byid(std::string(profSearchID), loggedInUser.role);
                    });
                }
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (!feedbackMsg.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }
        showOutputBox();

        if (ImGui::Button("Logout", ImVec2(120, 35))) {
            feedbackMsg  = "";
            outputMsg = "";
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
            if (ImGui::Button("Show Profile##student", ImVec2(150, 30))) {
                Student s(loggedInUser.first_name, loggedInUser.last_name,
                          loggedInUser.id, "HIDDEN",
                          loggedInUser.extra1, loggedInUser.extra2, loggedInUser.extra3);
                outputMsg = CaptureCout([&]() {
                    s.displayInfo();
                });
            }
        }

        // ── [2] My Transcript ────────────────────
        if (ImGui::CollapsingHeader("My Transcript")) {
            if (ImGui::Button("View Grades", ImVec2(160, 30))) {
                outputMsg = CaptureCout([&]() {
                    studUtils.viewGrades(loggedInUser.id);
                });
            }
        }

        // ── [3] Enroll in Course ─────────────────
        if (ImGui::CollapsingHeader("Enroll in Course")) {
            ImGui::InputText("Course Code##senroll", sEnrollCode, sizeof(sEnrollCode));
            if (ImGui::Button("Enroll##s", ImVec2(120, 30))) {
                if (sEnrollCode[0]) {
                    outputMsg = CaptureCout([&]() {
                        courseMgr.enrollStudent(loggedInUser.id, sEnrollCode);
                    });
                    feedbackMsg = "[OK] Enrollment request sent.";
                    feedbackCol = ImVec4(0.4f, 1.0f, 0.5f, 1.0f);
                    memset(sEnrollCode, 0, sizeof(sEnrollCode));
                } else {
                    outputMsg = "";
                    feedbackMsg = "[!] Enter a course code.";
                    feedbackCol = ImVec4(1.0f, 0.6f, 0.2f, 1.0f);
                }
            }
        }

        // ── [4] Course Catalog ───────────────────
        if (ImGui::CollapsingHeader("Course Catalog")) {
            if (ImGui::Button("List All Courses", ImVec2(180, 30))) {
                outputMsg = CaptureCout([&]() {
                    courseMgr.listAllCourses();
                });
            }
        }

        ImGui::Spacing();
        ImGui::Separator();

        if (!feedbackMsg.empty()) {
            ImGui::PushStyleColor(ImGuiCol_Text, feedbackCol);
            ImGui::TextWrapped("%s", feedbackMsg.c_str());
            ImGui::PopStyleColor();
            ImGui::Spacing();
        }
        showOutputBox();

        if (ImGui::Button("Logout", ImVec2(120, 35))) {
            feedbackMsg  = "";
            outputMsg = "";
            currentState = MAIN_MENU;
        }
        ImGui::End();
    }
};

#endif // UI_H
