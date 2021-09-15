#include "renderer.h"

#include <chrono>
#include <ctime>
#include <iostream>

#include "material.h"
#include "random.h"
#include "utils.h"

Renderer::Renderer(const Scene &scene, int nr_threads) : scene_(scene) {
  const int kHardwareThreads = static_cast<int>(std::thread::hardware_concurrency());
  nr_threads_ = std::clamp(nr_threads, 1, std::min(scene.image_height, kHardwareThreads));
  image_.resize(scene_.image_height);
}

std::vector<std::vector<Color>> Renderer::Render() {
  std::cout << "Rendering a " <<
            scene_.image_width << " X " << scene_.image_height << " scene"
            << " with " << nr_threads_ << " threads." << std::endl;

  for (int row = 0; row < scene_.image_height; ++row) {
    task_queue_.Push(row);
  }

  // Randomly shuffle tasks in order to separate rows with similar complexities.
  // This might help to avoid rows finishing at the same time and thus avoid threads waiting for each other.
  task_queue_.RandomShuffle();

  auto start_time = std::chrono::system_clock::now();
  {
    auto time = std::chrono::system_clock::to_time_t(start_time);
    std::cout << "Rendering started at: " << std::ctime(&time) << std::endl;
  }

  std::vector<std::thread> threads;
  for (int t = 0; t < nr_threads_; ++t) {
    threads.emplace_back(&Renderer::RunThread, this);
  }

  std::for_each(threads.begin(), threads.end(), [](auto &t) { t.join(); });

  auto end_time = std::chrono::system_clock::now();
  {
    auto time = std::chrono::system_clock::to_time_t(end_time);
    std::cout << "Rendering finished at: " << std::ctime(&time) << std::endl;
  }

  std::chrono::duration<double> elapsed_seconds = end_time - start_time;
  std::cout << "Elapsed time: ";
  if (elapsed_seconds.count() < 60.0) {
    std::cout << elapsed_seconds.count() << " s" << std::endl;
  } else {
    std::cout << elapsed_seconds.count() / 60.0 << " min" << std::endl;
  }

  return image_;
}

void Renderer::RenderRow(int row_index) {
  std::vector<Color> row;
  for (int column = 0; column < scene_.image_width; ++column) {
    Color pixel{0, 0, 0};
    for (int sample = 1; sample <= scene_.samples_per_pixel; ++sample) {
      double u = (column + RandomDouble()) / (scene_.image_width - 1);
      double v = (row_index + RandomDouble()) / (scene_.image_height - 1);
      Ray ray = scene_.camera->GetRay(u, v);
      pixel += ComputeColor(ray, scene_.world, scene_.background_color);
    }
    row.emplace_back(pixel);
  }

  image_[row_index] = row;
}

void Renderer::RunThread() {
  while (true) {
    std::optional<int> task = task_queue_.GetTask();
    if (!task.has_value()) {
      return;  // No more tasks.
    }
    RenderRow(task.value());
    mutex_.lock();
    ++rendered_rows_;
    std::cout << "Rows rendered: " << rendered_rows_ << " / " << scene_.image_height << '\n';
    mutex_.unlock();
  }
}

Color Renderer::ComputeColor(const Ray &ray, const Collidables &world, const Color &background_color) {
  int child_rays = Scene::kMaxChildRays;

  Color computed_color(0, 0, 0);  // Output.

  // "Recursion" variables.
  Color current_attenuation(1, 1, 1);
  Ray current_ray = ray;

  while (child_rays--) {
    Collision collision;
    const bool collided = world.Collide(current_ray, 0.001, utils::kInfinity, collision);
    if (!collided) {
      return background_color;
    }

    // Update output.
    Color emitted_color = collision.material->Emit(collision.u, collision.v, collision.point);
    computed_color += emitted_color * current_attenuation;

    Color attenuation;
    Ray scattered_ray;
    double pdf;

    const bool scattered = collision.material->Scatter(current_ray, collision, attenuation, scattered_ray, pdf);
    if (!scattered) {
      return computed_color;
    }

    current_attenuation *= attenuation * collision.material->ScatteringPDF(current_ray, collision, scattered_ray) / pdf;
    current_ray = scattered_ray;
  }
  return {0, 0, 0};  // All child rays have been consumed.
}
