#include "raytracer.h"

#include <chrono>

#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"

namespace rt {

void Raytracer::Run() {
  constexpr int32_t kGlfwVersionMajor = 4;
  constexpr int32_t kGlfwVersionMinor = 6;
  constexpr int32_t kSwapInterval = 1;  // VSYNC
  constexpr char kGlslVersion[] = "#version 130";

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, kGlfwVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, kGlfwVersionMinor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window = glfwCreateWindow(1280, 720, "Raytracer", nullptr, nullptr);
  if (window == nullptr) {
    glfwTerminate();
    return;
  }
  glfwSwapInterval(kSwapInterval);
  glfwMakeContextCurrent(window);
  if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
    return;
  }

  ImGui::CreateContext();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(kGlslVersion);
  ImGui::StyleColorsDark();

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    RenderUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    glfwSwapBuffers(window);
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void Raytracer::RenderUI() {
  {
    static bool real_time = false;
    ImGui::Begin("Settings");
    ImGui::Text("Rendering Time: %.1f ms", render_time_ms_);
    ImGui::Checkbox("Real-Time", &real_time);
    if (real_time) {
      Render();
    }
    ImGui::BeginDisabled(real_time);
    if (ImGui::Button("Render")) {
      Render();
    }
    ImGui::EndDisabled();
    ImGui::End();
  }
  {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
    ImGui::Begin("Result");
    viewport_width_ = static_cast<int32_t>(ImGui::GetContentRegionAvail().x);
    viewport_height_ = static_cast<int32_t>(ImGui::GetContentRegionAvail().y);
    auto render_result = renderer_.GetResult();
    if (render_result) {
      ImGui::Image(reinterpret_cast<ImTextureID>(render_result->Texture()),
                   {static_cast<float>(render_result->Width()), static_cast<float>(render_result->Height())},
                   {0, 1}, {1, 0});
    }
    ImGui::End();
    ImGui::PopStyleVar();
  }
}

void Raytracer::Render() {
  renderer_.OnResize(viewport_width_, viewport_height_);
  render_time_ms_ = renderer_.Render();
}
}  // namespace rt
