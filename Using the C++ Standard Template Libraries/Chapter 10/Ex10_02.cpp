// Ex10_02.cpp
// Dropping bricks safely from a tall building using valarray objects
#include <numeric>                                       // For iota()
#include <iostream>                                      // For standard streams
#include <iomanip>                                       // For stream manipulators
#include <algorithm>                                     // For for_each()
#include <valarray>                                      // For valarray
const static double g
{
    32.0
};                            // Acceleration due to gravity ft/sec/sec

int main()
{
    double height {};                                      // Building height
    std::cout << "Enter the approximate height of the building in feet: ";
    std::cin >> height;
    // Calculate brick flight time in seconds
    double end_time {std::sqrt(2 * height / g)};
    size_t max_time {1 + static_cast<size_t>(end_time + 0.5)};
    std::valarray<double> times(max_time + 1);               // Array to accommodate times
    std::iota(std::begin(times), std::end(times), 0);        // Initialize: 0 to max_time
    *(std::end(times) - 1) = end_time;                       // Set the last time value
    // Calculate distances each second
    auto distances = times.apply([](double t) {
        return 0.5 * g * t * t;
    });
    // Calculate speed each second
    auto v_fps = sqrt(distances.apply([](double d) {
        return 2 * g * d;
    }));
    // Lambda expression to output results
    auto print = [](double v) {
        std::cout << std::setw(5) << static_cast<int>(std::round(v));
    };
    // Output the times - the last is a special case...
    std::cout << "Time (seconds): ";
    std::for_each(std::begin(times), std::end(times) - 1, print);
    std::cout << std::setw(5) << std::fixed << std::setprecision(2) << *(std::end(times) - 1);
    std::cout << "\nDistances(feet):";
    std::for_each(std::begin(distances), std::end(distances), print);
    std::cout << "\nVelocity(fps):  ";
    std::for_each(std::begin(v_fps), std::end(v_fps), print);
    // Get velocities in mph and output them
    auto v_mph = v_fps.apply([](double v) {
        return v * 60 / 88;
    });
    std::cout << "\nVelocity(mph):  ";
    std::for_each(std::begin(v_mph), std::end(v_mph), print);
    std::cout << std::endl;
}