#include "renderer.h"

#include <chrono>
#include <ctime>
#include <iostream>

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
  // Camera
  const double kAspectRatio = static_cast<double>(scene_.image_width) / static_cast<double>(scene_.image_height);
  constexpr double kViewportHeight = 2.0;
  const double kViewportWidth = kAspectRatio * kViewportHeight;
  constexpr double kFocalLength = 1.0;

  const Point3D kOrigin{0, 0, 0};
  const Vector3D kHorizontal{kViewportWidth, 0, 0};
  const Vector3D kVertical{0, kViewportHeight, 0};
  const Point3D kLowerLeftCorner = kOrigin - kHorizontal / 2 - kVertical / 2 - Vector3D(0, 0, kFocalLength);

  std::vector<Color> row;
  for (int column = 0; column < scene_.image_width; ++column) {
    double u = static_cast<double>(column) / (scene_.image_width - 1);
    double v = static_cast<double>(row_index) / (scene_.image_height - 1);
    Ray ray(kOrigin, kLowerLeftCorner + u * kHorizontal + v * kVertical - kOrigin);
    Color pixel_color = ComputeColor(ray, scene_.world);
    row.emplace_back(pixel_color);
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

Color Renderer::ComputeColor(const Ray &ray, const Collidables &world) {
  Collision collision;
  if (world.Collide(ray, 0, utils::kInfinity, collision)) {
    return 0.5 * (collision.normal + Color(1, 1, 1));
  }
  Vector3D unit_direction = ray.Direction().UnitVector();
  double t = 0.5 * (unit_direction.Y() + 1.0);
  return (1.0 - t) * Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
}
