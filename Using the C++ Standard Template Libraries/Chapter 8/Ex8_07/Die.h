#ifndef DIE_H
#define DIE_H
#include <random>                                        // For discrete_distribution and random number generator
#include <vector>                                        // For vector container
#include <algorithm>                                     // For remove()
#include <iterator>                                      // For iterators and begin() and end()

// Alias for param_type for discrete distribution
using Params = std::discrete_distribution<size_t>::param_type;

std::default_random_engine& rng();

// Class to represent a die with six faces with arbitrary values
// Face values do not need to be unique.
class Die
{
public:
    Die()
    {
        values.push_back(0);
    };

    // Constructor
    Die(std::initializer_list<size_t> init)
    {
        std::vector<size_t> faces {init};                      // Stores die face values
        auto iter = std::begin(faces);
        auto end_iter = std::end(faces);
        std::vector<size_t> wts;                               // Stores weights for face values

        while (iter != end_iter) {
            values.push_back(*iter);
            wts.push_back(std::count(iter, end_iter, *iter));
            end_iter = std::remove(iter, end_iter, *iter++);
        }

        dist.param(Params {std::begin(wts), std::end(wts)});
    }

    size_t throw_it()
    {
        return values[dist(rng())];
    }

private:
    std::discrete_distribution<size_t> dist;                // Probability distributtion for values
    std::vector<size_t> values;                             // Face values
};
#endif