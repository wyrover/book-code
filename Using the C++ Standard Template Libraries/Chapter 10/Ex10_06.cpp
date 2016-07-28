// Ex10_06.cpp
// Using complex object to generate a fractal image of a Julia set
#include <iostream>                                      // For standard streams
#include <iomanip>                                       // For stream manipulators
#include <complex>                                       // For complex types
#include <chrono>                                        // For clocks, duration, and time_point
using std::complex;
using namespace std::chrono;
using namespace std::literals;

// Outputs the exact interval in seconds for a time_point<>
template<typename TimePoint>
void print_timepoint(const TimePoint& tp, size_t places = 0)
{
    auto elapsed = tp.time_since_epoch();          // duration object for the interval
    auto seconds = duration_cast<duration<double>>(elapsed).count();
    std::cout << std::fixed << std::setprecision(places) << seconds << " seconds\n";
}

int main()
{
    const int width {100}, height {100};                     // Image width and height
    size_t count {100};                                      // Iterate count for recursion
    char image[width][height];
    auto start_time = std::chrono::steady_clock::now();      // time_point object for start
    complex<double> c { -0.7, 0.27015};                      // Constant in z = z*z + c

    for (int i {}; i < width; ++i) {                         // Iterate over pixels in the width
        for (int j {}; j < height; ++j) {                      // Iterate over pixels in the height
            // Scale real and imaginary parts to be between -1 and +1
            auto re = 1.5 * (i - width / 2) / (0.5 * width);
            auto im = (j - height / 2) / (0.5 * height);
            complex<double> z {re, im};                          // Point in the complex plane
            image[i][j] = ' ';                                   // Point not in the Julia set

            // Iterate z=z*z+c count times
            for (size_t k {}; k < count; ++k) {
                z = z * z + c;
            }

            if (std::abs(z) < 2.0)                               // If point not escaping...
                image[i][j] = '*';                                 // ...it's in the Julia set
        }
    }

    auto end_time = std::chrono::steady_clock::now();        // time_point object for end
    auto elapsed = end_time - start_time.time_since_epoch();
    std::cout << "Time to generate a Julia set with " << width << "x" << height << " pixels is ";
    print_timepoint(elapsed, 9);
    std::cout << "The Julia set looks like this:\n";

    for (size_t i {}; i < width; ++i) {
        for (size_t j {}; j < height; ++j)
            std::cout << image[i][j];

        std::cout << '\n';
    }
}