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
  ImGuiIO& io = ImGui::GetIO();
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
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
  RenderPreview();
}

void Raytracer::RenderUISettings() {
  {
    ImGui::Begin("Settings");

    const bool is_rendering = renderer_.State() == Renderer::RenderState::Running;
    ImGui::BeginDisabled(is_rendering);

    ImGui::ListBox("Scene", &renderer_settings_.scene_type, kSceneNames, IM_ARRAYSIZE(kSceneNames), 3);

    static bool use_preview_window_resolution = true;
    ImGui::Checkbox("Use Preview Window Resolution", &use_preview_window_resolution);
    if (!use_preview_window_resolution) {
      ImGui::InputInt("Width", &renderer_settings_.width, 100, 1000);
      ImGui::InputInt("Height", &renderer_settings_.height, 100, 1000);
    }

    ImGui::InputInt("Samples per Pixel", &renderer_settings_.samples_per_pixel, 10, 100);

    ImGui::RadioButton("Chunk by Chunk", &renderer_settings_.mode, RenderMode::ChunkByChunk);
    ImGui::SameLine();
    ImGui::RadioButton("Row by Row", &renderer_settings_.mode, RenderMode::RowByRow);

    ImGui::BeginDisabled(renderer_settings_.mode != RenderMode::ChunkByChunk);
    ImGui::InputInt("Chunk Size", &renderer_settings_.chunk_size, 1, 10);
    ImGui::EndDisabled();

    if (ImGui::Button("Render")) {
      if (use_preview_window_resolution) {
        renderer_settings_.width = viewport_width_;
        renderer_settings_.height = viewport_height_;
      }
      OnRender();
    }
    ImGui::EndDisabled();

    ImGui::Separator();  // --------------------------------------------------

    const RendererStatistics statistics = renderer_.Statistics();
    ImGui::Text("Resolution: %d x %d", statistics.width, statistics.height);
    if (statistics.render_time_ms != std::chrono::milliseconds::zero()) {
      using namespace std::chrono;
      auto ms = statistics.render_time_ms;
      auto s = duration_cast<seconds>(ms);
      ms -= duration_cast<milliseconds>(s);
      auto m = duration_cast<minutes>(s);
      s -= duration_cast<seconds>(m);
      ImGui::Text("Rendering Time: %d min %lld s %lld ms", m.count(), s.count(), ms.count());
    }

    ImGui::End();
  }
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
  renderer_.OnResize(renderer_settings_.width, renderer_settings_.height);
  renderer_.Render(renderer_settings_);
}
}  // namespace rt
