#pragma once

#include <algorithm>
#include <deque>
#include <mutex>
#include <optional>
#include <thread>
#include <vector>

#include "ray.h"
#include "scene.h"
#include "vector3d.h"

template<typename T>
class TaskQueue {
 public:
  void Push(const T &value) {
    std::lock_guard<std::mutex> lock(mutex_);
    deque_.push_back(value);
  }

  std::optional<T> GetTask() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (deque_.empty()) {
      return std::nullopt;
    }
    T task = deque_.front();
    deque_.pop_front();
    return task;
  }

  void RandomShuffle() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::random_shuffle(deque_.begin(), deque_.end());
  }

 private:
  std::mutex mutex_;
  std::deque<T> deque_;
};

class Renderer {
 public:
  explicit Renderer(const Scene &scene, int nr_thread = static_cast<int>(std::thread::hardware_concurrency()));

  std::vector<std::vector<Color>> Render();

 private:
  std::vector<std::vector<Color>> image_;
  std::mutex mutex_;
  const Scene &scene_;
  TaskQueue<int> task_queue_;
  int nr_threads_;
  int rendered_rows_ = 0;

  void RenderRow(int row_index);

  void RunThread();

  static bool CollideSphere(const Point3D &center, double radius, const Ray &ray);

  static Color ComputeColor(const Ray &ray);
};
