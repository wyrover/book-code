// Ex4_06.cpp
// Analyzing how and when the number of buckets in an unordered_map container increases
#include <iostream>                              // For standard streams
#include <iomanip>                               // For stream manipulators
#include <string>                                // For string class
#include <unordered_map>                         // For unordered_map container
#include <vector>                                // For vector container
#include <algorithm>                             // For max_element() algorithm

using std::string;
using std::unordered_map;

// Outputs number of elements in each bucket
void list_bucket_counts(const std::vector<size_t>& counts)
{
    for (size_t i {}; i < counts.size(); ++i) {
        std::cout << "bucket[" << std::setw(2) << i << "] = " << counts[i] << "  ";

        if ((i + 1) % 6 == 0) std::cout << '\n';
    }

    std::cout << std::endl;
}

int main()
{
    unordered_map<string, size_t> people;
    float mlf {people.max_load_factor()};                         // Current maximum load factor
    size_t n_buckets {people.bucket_count()};                     // Number of buckets in container
    std::vector<size_t> bucket_counts(people.bucket_count());     // Records number of elements per bucket
    string name {"Name"};                                         // Key - with value appended
    size_t value {1};                                             // Element value
    size_t max_count {8192};                                      // Maximum number of elements to insert
    auto lf = people.load_factor();                               // Current load factor
    bool rehash {false};                                          // Records when rehash occurs

    while (mlf <= 1.5f) {                                         // Loop until max load factor is 1.5
        std::cout << "\n\n***************New Container***************"
                  << "\nNumber of buckets: " << n_buckets << "  Maximum load factor: " << mlf << std::endl;

        // Insert max elements in container
        for (size_t n_elements {}; n_elements < max_count; ++n_elements) {
            lf = people.load_factor();                               // Record load factor
            people.emplace("name" + std::to_string(++value), value);
            auto new_count = people.bucket_count();                  // Current bucket count

            if (new_count > n_buckets) {                             // If bucket count increases...
                // Output info
                std::cout << "\nBucket count increased to " << new_count
                          << ". Load factor was " << lf << " and is now " << people.load_factor()
                          << "\nMaximum elements in a bucket was "
                          << *std::max_element(std::begin(bucket_counts), std::end(bucket_counts)) << std::endl;

                if (n_buckets <= 64) {
                    std::cout << "Bucket counts before increase were: " << std::endl;
                    list_bucket_counts(bucket_counts);
                }

                n_buckets = new_count;                                  // Update bucket count
                bucket_counts = std::vector < size_t > (n_buckets);     // New vector for counts
                rehash = true;                                          // Record rehash occurred
            }

            // Record current bucket counts
            for (size_t i {}; i < n_buckets; ++i)
                bucket_counts[i] = people.bucket_size(i);

            if (rehash) {                                             // If the container was rehashed...
                // ...output info
                rehash = false;                                         // Reset rehash indicator
                std::cout << "\nRehashed container. Bucket count is " << n_buckets << ". Element count is " << people.size()
                          << "\nMaximum element count in a bucket is now "
                          << *std::max_element(std::begin(bucket_counts), std::end(bucket_counts)) << std::endl;

                if (n_buckets <= 64) {                                  // If no more than 64 buckets...
                    std::cout << "\nBucket counts after rehash are:\n";
                    list_bucket_counts(bucket_counts);
                }
            }
        }

        std::cout << "Final state for this container is:\n"
                  << "Bucket count: " << people.bucket_count() << "  Element count: " << people.size()
                  << "  Maximum element count in a bucket: "
                  << *std::max_element(std::begin(bucket_counts), std::end(bucket_counts)) << std::endl;
        value = 1;                                                  // Reset key suffix
        people = unordered_map<string, size_t>();                   // New empty container
        n_buckets = people.bucket_count();
        bucket_counts = std::vector < size_t >(n_buckets);          // New vector for bucket counts
        mlf += 0.25f;                                               // Increase max load factor...
        people.max_load_factor(mlf);                                // ...and set for container
    }
}