# Raytracing in C++

> Custom C++ implementation of an image raytracer introduced in the [Ray Tracing](https://raytracing.github.io/) -book series.

In order to learn about the essential math and things related to raytracing, I decided to follow Peter Shirley's great
books about raytracing and implement my version of the raytracer. I tried to follow the general logic and structure of
the books, but took some artistic freedom in certain areas. The key differences are that my version includes a
multi-threaded C++ renderer, various structural changes and some performance improvements.

## C++ Renderer

While following the book series, it was quickly clear to me that multi-threading would be essential in order to avoid
waiting ages for the lengthier renders to finish. Multi-threading (on AMD Ryzen 2700X) was an immense help in following
the book series in a relatively short time span.

### Implementation Details

In a nutshell, the renderer divides the picture into rows and places them into a task queue. The rows are then shuffled
randomly in order to avoid threads rendering similar rows from the get-go, and thus waiting for each other. Each thread
gets a new row from the task queue once the previous one is finished. This approach guarantees that each thread has
tasks for the whole duration of the render. A small price is paid when the threads wait each other when fetching new
tasks from the queue.

Another approach would be to divide the rows equally to all threads at the start. This has one serious downside, that
it's almost impossible to quantify row complexities prior hand.

A more optimal way to divide the tasks could be a combination of these two approaches. At the start giving each thread
some *N* amount of rows and leaving some *M* amount of rows in the queue. The challenge would be to find a good balance.
Since the threads spend way more to time rendering a row than waiting for the task queue to be free, I didn't bother
experimenting with this and deemed the initial approach as sufficient.

## Results

| Processor       | Threads | Image                                                         | Samples | Render Time       |
|-----------------|---------|---------------------------------------------------------------|---------|-------------------|
| AMD Ryzen 2700X | 16      | [In One Weekend (1-13)](/renders/part1/part1-13.png)          | 500     | 15 minutes        |
| AMD Ryzen 2700X | 16      | [The Next Week (2-10)](/renders/part2/part2-10.png)           | 10000   | 1 hour 14 minutes |
| AMD Ryzen 2700X | 16      | [The Rest of Your Life (3-12)](/renders/part3/part3-12-4.png) | 10000   | 24 minutes        |

![In One Weekend - Final Render](/renders/part1/part1-13.png)

![The Next Week - Final Render](/renders/part2/part2-10.png)

![The Rest of Your Life - Final Render](/renders/part3/part3-12-4.png)
