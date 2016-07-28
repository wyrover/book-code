// Ex8_04.cpp
// Finding pi by throwing a stick
#include <iostream>                                             // For standard streams
#include <random>                                               // For distributions, random number gen
#include <cmath>                                                // For sin() function

int main()
{
    const double pi = 3.1415962;
    double stick_length {};                                       // Stick length
    double board_width {};                                        // Board width
    std::cout << "Enter the width of a floorboard: ";
    std::cin >> board_width;
    std::cout << "Enter the length of the stick (must be less than " << board_width << "): ";
    std::cin >> stick_length;

    if (board_width < stick_length) stick_length = 0.9 * board_width;

    std::uniform_real_distribution<> angle {0.0, pi};             // Distribution for angle of stick
    std::uniform_real_distribution<> position {0.0, board_width}; // Distribution for stick center position, relative to board edge
    std::random_device rd;                                        // Non-deterministic seed source
    std::default_random_engine rng {rd()};                        // Create random number generator
    const size_t throws {
        5'000'000
    };                              // Number of random throws
    size_t hits {};                                               // Count of stick intersecting the board

    // Throw the stick down throws times
    for (size_t i {}; i < throws; ++i) {
        double y {position(rng)};
        double theta {angle(rng)};

        // Check if the stick crosses the edge of a board
        if (((y + stick_length * sin(theta) / 2) >= board_width) || ((y - stick_length * sin(theta) / 2) <= 0))
            ++hits;                                                   // It does, so increment count
    }

    std::cout << "Probability of the stick crossing the edge of a board is: "
              << (static_cast<double>(hits) / throws) << std::endl;
    std::cout << "Pi is: " << (2 * stick_length * throws) / (board_width * hits) << std::endl;
}
