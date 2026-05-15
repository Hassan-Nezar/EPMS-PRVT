// main.cpp - University Management System
// Backend: GLFW + OpenGL3 + ImGui

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>

#include "include/auth_manager.h"
#include "include/course_manager.h"
#include "include/professor_utils.h"
#include "include/search_manager.h"
#include "include/student_utils.h"
#include "include/system_admin.h"
#include "include/user.h"
#include "UI.h"

// ── All managers (persist for the entire session) ───────────────────────────
auth_manager       auth;
CourseManager      courseMgr;
ProfessorUtils     profUtils;
Attendance_manager attendMgr;
Student_utilities  studUtils;
Reporting_manager  reportMgr;
search_manager     sm;
UIManager          ui;

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

int main() {
    // ── [1] Init GLFW ────────────────────────────────────────────────────────
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return -1;

    // OpenGL 3.3 Core
    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // ── [2] Create window ────────────────────────────────────────────────────
    GLFWwindow* window = glfwCreateWindow(900, 650,
                                          "Zewail City EMS", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // vsync

    // ── [3] Init ImGui ───────────────────────────────────────────────────────
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    // Style — dark theme, slightly rounded
    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding    = 6.0f;
    style.FrameRounding     = 4.0f;
    style.ItemSpacing       = ImVec2(10, 8);
    style.WindowPadding     = ImVec2(16, 16);
    style.Colors[ImGuiCol_WindowBg]       = ImVec4(0.10f, 0.10f, 0.13f, 1.00f);
    style.Colors[ImGuiCol_Header]         = ImVec4(0.20f, 0.40f, 0.60f, 0.80f);
    style.Colors[ImGuiCol_HeaderHovered]  = ImVec4(0.26f, 0.52f, 0.78f, 0.80f);
    style.Colors[ImGuiCol_Button]         = ImVec4(0.20f, 0.40f, 0.65f, 1.00f);
    style.Colors[ImGuiCol_ButtonHovered]  = ImVec4(0.26f, 0.52f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_FrameBg]        = ImVec4(0.18f, 0.18f, 0.22f, 1.00f);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // ── [4] Render loop ──────────────────────────────────────────────────────
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // ↓ This is the only call you need — it drives the entire UI
        ui.render(auth, courseMgr, profUtils, attendMgr, studUtils, reportMgr, sm);

        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.08f, 0.08f, 0.10f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // ── [5] Cleanup ──────────────────────────────────────────────────────────
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

