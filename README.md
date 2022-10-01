# Raytracing in C++

> C++ implementation of an image raytracer introduced in
> the [Ray Tracing](https://raytracing.github.io/) -book series.

#### Motivation

In order to learn about the essential math and things related to raytracing, I
decided to follow the great [Ray Tracing](https://raytracing.github.io/) -book
series and implement my own version of the raytracer. I followed the general
logic and structure of the books, but took some liberty in certain areas. I
might some day transform the code to use CUDA for accelerated computing.
Keeping that in mind, some architectural changes (namely aiming for no runtime
polymorphism) have been made already in the CPU version.

## Features & Architectural Differences

- **Graphical User Interface**
    - Allows easy adjustment of the renderer's settings and show a live preview
      of the rendering process.
    - Using [ImGui](https://github.com/ocornut/imgui) to as the GUI library
      and [OpenGL](https://www.opengl.org/) as the backend.

- **Multi-Threaded C++ Renderer**
    - Accelerated rendering using multiple CPU cores via
      a [thread pool](https://github.com/bshoshany/thread-pool).

- **No runtime (dynamic) polymorphism for collidable objects**
    - Runtime polymorphism is largely present in the book, and understandably
      so. It does make the code simpler to understand and easier to implement.
    - But it comes with cost of virtual function overhead at runtime.
    - Instead, I've opted to use [*Curiously Recurring Template
      Pattern*](https://en.wikipedia.org/wiki/Curiously_recurring_template_pattern)
      (CRTP) for compile-time polymorphism (CTP).
    - As of now, materials (e.g. Lambertian) and textures (e.g. Checker) still
      use runtime polymorphism. I will need to change that in the future for CUDA compatibility.

## Results

Without any SIMD or other low-level optimizations the results are as follows:

| Processor       | Threads | Image                          | Resolution | Samples | Render Time   |
|-----------------|---------|--------------------------------|------------|---------|---------------|
| AMD Ryzen 2700X | 16      | In One Weekend (1-13)          | 1200x800   | 500     | 16 min 22 sec |
| AMD Ryzen 2700X | 16      | In One Weekend (1-13) with BVH | 1200x800   | 500     | 1 min 51 sec  |

The introduction of BVH was a major improvement. There are probably a lot of
smaller optimizations and low-hanging fruits waiting to be collected. I have
opted not to do any tricky optimizations before I've completed the book. Later
on, I plan to take a look at the lower-level inefficiencies and opportunities
for optimization.
