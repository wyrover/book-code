// Ex10_04.cpp
// Arithmetic with time-point objects
#include <iostream>                                      // For standard streams
#include <iomanip>                                       // For stream manipulators
#include <chrono>                                        // For duration, time_point templates
#include <ratio>                                         // For ratio templates
using namespace std::chrono;

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
    using TimePoint = time_point<steady_clock>;
    time_point<steady_clock> tp1 {duration<int>(20)};
    time_point<system_clock> tp2 {3min};
    time_point<high_resolution_clock> tp3 {2h};
    std::cout << "tp1 is ";
    print_timepoint(tp1);
    std::cout << "tp2 is ";
    print_timepoint(tp2);
    std::cout << "tp3 is ";
    print_timepoint(tp3);
    auto tp4 = tp2 + tp3.time_since_epoch();
    std::cout << "tp4 is tp2 with tp3 added: ";
    print_timepoint(tp4);
    std::cout << "tp1 + tp2 is ";
    print_timepoint(tp1 + tp2.time_since_epoch());
    tp2 += duration<time_point<system_clock>::rep, std::milli> {20'000};
            std::cout << "tp2 incremented by 20,000 milliseconds is ";
            print_timepoint(tp2);
        }