#include "raytracer.h"

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
  RenderUISettings();
  RenderUIStatistics();
  RenderPreview();
}

void Raytracer::RenderUISettings() {
  {
    ImGui::Begin("Settings");

    const bool is_rendering = renderer_.State() == Renderer::RenderState::Running;
    ImGui::BeginDisabled(is_rendering);

    ImGui::ListBox("Scene", &renderer_settings_.scene_type, kSceneNames, IM_ARRAYSIZE(kSceneNames), 3);

    ImGui::InputInt("Samples per Pixel", &renderer_settings_.samples_per_pixel, 10, 100);
    ImGui::InputInt("Child Rays", &renderer_settings_.child_rays, 1, 10);

    ImGui::RadioButton("Chunk by Chunk", &renderer_settings_.mode, RenderMode::ChunkByChunk);
    ImGui::SameLine();
    ImGui::RadioButton("Row by Row", &renderer_settings_.mode, RenderMode::RowByRow);

    ImGui::BeginDisabled(renderer_settings_.mode != RenderMode::ChunkByChunk);
    ImGui::InputInt("Chunk Size", &renderer_settings_.chunk_size, 1, 10);
    ImGui::EndDisabled();

    if (ImGui::Button("Render")) {
      OnRender();
    }
    ImGui::EndDisabled();

    ImGui::End();
  }
}

void Raytracer::RenderUIStatistics() {
  const RendererStatistics statistics = renderer_.Statistics();
  ImGui::Begin("Statistics");
  ImGui::Text("Resolution: %d x %d", statistics.width, statistics.height);
  if (statistics.render_time_ms != 0.0f) {
    ImGui::Text("Rendering Time: %.1f ms", statistics.render_time_ms);
  }
  ImGui::End();
}

void Raytracer::RenderPreview() {
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, {0.0f, 0.0f});
  ImGui::Begin("Result");
  viewport_width_ = static_cast<int32_t>(ImGui::GetContentRegionAvail().x);
  viewport_height_ = static_cast<int32_t>(ImGui::GetContentRegionAvail().y);
  auto render_result = renderer_.Result();
  if (render_result) {
    render_result->Update();
    ImGui::Image(reinterpret_cast<ImTextureID>(render_result->Texture()),
                 {static_cast<float>(render_result->Width()), static_cast<float>(render_result->Height())},
                 {0, 1}, {1, 0});
  }
  ImGui::End();
  ImGui::PopStyleVar();
}

void Raytracer::OnRender() {
  renderer_.OnResize(viewport_width_, viewport_height_);
  renderer_.Render(renderer_settings_);
}
}  // namespace rt
