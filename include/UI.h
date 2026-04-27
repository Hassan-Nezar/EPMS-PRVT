#ifndef UI_H
#define UI_H

#include "imgui.h"
#include "auth_manager.h"
#include "include/auth_manager.h"
#include "include/User.h"
#include "include/course_manager.h"
#include "include/professor_utils.h"
#include "include/student_utils.h"
#include "include/system_admin.h"
#include "include/search_manager.h"
#include <iostream>
#include <vector>
#include <string>

class UIManager {
public:
    //fsm
    enum AppState { MAIN_MENU, LOGIN, REGISTER, ADMIN_DASH, STUDENT_DASH, PROF_DASH };
    AppState currentState = MAIN_MENU;

    //imgui doesnth ave string
    char inputUser[64] = "";
    char inputID[64] = "";
    char inputPass[64] = "";
    char inputMajor[64] = "";
    char inputYear[64] = "";

    
    void render(auth_manager& auth) {
        switch (currentState) {
            case MAIN_MENU:    showMainMenu(); break;
            case LOGIN:        showLoginPage(auth); break;
            case REGISTER:     showRegisterPage(auth); break;
            case ADMIN_DASH:   showAdminMenu(auth); break;
            // case STUDENT_DASH: showStudentMenu(); break;
            // case PROF_DASH:    showProfessorMenu(); break;
        }
    }

private:
    // --- [1] MAIN MENU ---
    void showMainMenu() {
        ImGui::Begin("Zewail City EMS - Home");
        ImGui::Text("Welcome, please select an option:");
        ImGui::Separator();

        if (ImGui::Button("Login", ImVec2(200, 40))) currentState = LOGIN;
        if (ImGui::Button("Register", ImVec2(200, 40))) currentState = REGISTER;
        
        ImGui::Spacing();
        if (ImGui::Button("Exit System", ImVec2(200, 40))) exit(0);
        ImGui::End();
    }

    // --- [2] LOGIN PAGE ---
    void showLoginPage(auth_manager& auth) {
        ImGui::Begin("Login Portal");

        ImGui::InputText("Name", inputUser, 64);
        ImGui::InputText("ID", inputID, 64);
        ImGui::InputText("Password", inputPass, 64, ImGuiInputTextFlags_Password);

        if (ImGui::Button("Submit")) {
            // Handoff: C++ automatically converts these char arrays to strings for auth.login
            auto res = auth.login(inputUser, inputID, inputPass);
            
            if (res.success) {
                if (res.role == "Admin") currentState = ADMIN_DASH;
                else if (res.role == "Student") currentState = STUDENT_DASH;
                else if (res.role == "Professor") currentState = PROF_DASH;
            } else {
                // You can add a "Login Failed" text here later
            }
        }

        if (ImGui::Button("Back")) currentState = MAIN_MENU;
        ImGui::End();
    }

    // --- [3] REGISTER PAGE ---
    void showRegisterPage(auth_manager& auth) {
        ImGui::Begin("Student Registration");
        
        ImGui::InputText("Full Name", inputUser, 64);
        ImGui::InputText("Enrollment Year (e.g. 2026)", inputYear, 64);
        ImGui::InputText("Desired Password", inputPass, 64);
        
        // Major Selection (Simplified for now)
        ImGui::Text("Enter Major (SWE, CS, AI, etc.):");
        ImGui::InputText("Major", inputMajor, 64);

        if (ImGui::Button("Submit Application")) {
            // Call your auth_manager::register_user logic here
            // Then clear the inputs
            currentState = MAIN_MENU; 
        }

        if (ImGui::Button("Cancel")) currentState = MAIN_MENU;
        ImGui::End();
    }

    // --- [4] ADMIN MENU ---
    void showAdminMenu(auth_manager& auth) {
        ImGui::Begin("Admin Dashboard");
        ImGui::Text("Logged in as System Administrator");
        ImGui::Separator();

        if (ImGui::Button("Check Registration Queue")) {
            // This would call your check_register() function
        }

        if (ImGui::Button("Manage Courses")) {
            // Open Course Manager window
        }

        if (ImGui::Button("Logout")) {
            currentState = MAIN_MENU;
        }
        ImGui::End();
    }
};

#endif