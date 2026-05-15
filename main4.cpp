#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "include/auth_manager.h"
#include "include/course_manager.h"
#include "include/professor_utils.h"
#include "include/search_manager.h"
#include "include/student_utils.h"
#include "include/system_admin.h"
#include "include/user.h"

auth_manager auth;
CourseManager courseMgr;
ProfessorUtils profUtils;
Attendance_manager attendMgr;
Student_utilities studUtils;
Reporting_manager reportMgr;
search_manager sm;

auth_manager::login_result currentUser;

int main()
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(1000, 700, "LMS GUI", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    static char name[64] = "";
    static char id[64] = "";
    static char pass[64] = "";

    static bool loggedIn = false;
    static int role = 0; // 1 admin, 2 prof, 3 student

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ================= LOGIN =================
        if (!loggedIn)
        {
            ImGui::Begin("Login");

            ImGui::InputText("Name", name, 64);
            ImGui::InputText("ID", id, 64);
            ImGui::InputText("Password", pass, 64, ImGuiInputTextFlags_Password);

            if (ImGui::Button("Login"))
            {
                std::string n = name;
                std::string i = id;
                std::string p = pass;

                auto res = auth.login(n, i, p);

                if (res.success)
                {
                    loggedIn = true;
                    currentUser = res;

                    if (res.role == "Admin") role = 1;
                    else if (res.role == "Professor") role = 2;
                    else role = 3;
                }
            }

            if (!loggedIn)
                ImGui::Text("Please login");
            else
                ImGui::Text("Logged in");

            ImGui::End();
        }

        // ================= ADMIN =================
        if (loggedIn && role == 1)
        {
            ImGui::Begin("Admin Panel");

            if (ImGui::Button("Profile"))
            {
                Admin a(currentUser.first_name, currentUser.last_name,
                        currentUser.id, "HIDDEN", currentUser.extra1);
                a.displayInfo();
            }

            if (ImGui::Button("Add Course"))
                courseMgr.addCourse("CS101", "Intro", "3");

            if (ImGui::Button("System Report"))
                reportMgr.generateSystemReport();

            if (ImGui::Button("Search User"))
                sm.search_user_byid("123", currentUser.role);

            if (ImGui::Button("Logout"))
                loggedIn = false;

            ImGui::End();
        }

        // ================= PROFESSOR =================
        if (loggedIn && role == 2)
        {
            ImGui::Begin("Professor Panel");

            if (ImGui::Button("Profile"))
            {
                Professor p(currentUser.first_name, currentUser.last_name,
                            currentUser.id, "HIDDEN",
                            currentUser.extra1, currentUser.extra2);
                p.displayInfo();
            }

            if (ImGui::Button("Assign Grade"))
                profUtils.setStudentGrade("S1", "CS101", "A");

            if (ImGui::Button("Attendance"))
                attendMgr.markAttendance("S1", "CS101", "2026-01-01", "Present");

            if (ImGui::Button("Logout"))
                loggedIn = false;

            ImGui::End();
        }

        // ================= STUDENT =================
        if (loggedIn && role == 3)
        {
            ImGui::Begin("Student Panel");

            if (ImGui::Button("Profile"))
            {
                Student s(currentUser.first_name, currentUser.last_name,
                          currentUser.id, "HIDDEN",
                          currentUser.extra1, currentUser.extra2,
                          currentUser.extra3);
                s.displayInfo();
            }

            if (ImGui::Button("My Grades"))
                studUtils.viewGrades(currentUser.id);

            if (ImGui::Button("Enroll"))
                courseMgr.enrollStudent(currentUser.id, "CS101");

            if (ImGui::Button("Courses"))
                courseMgr.listAllCourses();

            if (ImGui::Button("Logout"))
                loggedIn = false;

            ImGui::End();
        }

        ImGui::Render();

        int w, h;
        glfwGetFramebufferSize(window, &w, &h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}