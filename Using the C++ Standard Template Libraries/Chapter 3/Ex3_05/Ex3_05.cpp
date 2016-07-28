// Ex3_05.cpp
// Using smart pointer to simulate supermarket checkouts
#include <iostream>                              // For standard streams
#include <iomanip>                               // For stream manipulators
#include <vector>                                // For vector container
#include <string>                                // For string class
#include <numeric>                               // For accumulate()
#include <algorithm>                             // For min_element & max_element
#include <random>                                // For random number generation
#include <memory>                                // For smart pointers
#include "Checkout.h"
#include "Customer.h"

using std::string;
using distribution = std::uniform_int_distribution<>;
using PCheckout = std::unique_ptr<Checkout>;

// Output histogram of service times
void histogram(const std::vector<int>& v, int min)
{
    string bar(60, '*');                           // Row of asterisks for bar

    for (size_t i {}; i < v.size(); ++i) {
        std::cout << std::setw(3) << i + min << " "  // Service time is index + min
                  << std::setw(4) << v[i] << " "             // Output no. of occurrences
                  << bar.substr(0, v[i])                     // ...and that no. of asterisks
                  << (v[i] > static_cast<int>(bar.size()) ? "..." : "")
                  << std::endl;
    }
}

int main()
{
    std::random_device random_n;
    // Setup minimum & maximum checkout periods - times in minutes
    int service_t_min {2}, service_t_max {15};
    std::uniform_int_distribution<> service_t_d {service_t_min, service_t_max};
    // Setup minimum & maximum number of customers at store opening
    int min_customers {15}, max_customers {20};
    distribution n_1st_customers_d {min_customers, max_customers};
    // Setup minimum & maximum intervals between customer arrivals
    int min_arr_interval {1}, max_arr_interval {5};
    distribution arrival_interval_d {min_arr_interval, max_arr_interval};
    size_t n_checkouts {};
    std::cout << "Enter the number of checkouts in the supermarket: ";
    std::cin >> n_checkouts;

    if (!n_checkouts) {
        std::cout << "Number of checkouts must be greater than 0. Setting to 1." << std::endl;
        n_checkouts = 1;
    }

    std::vector<PCheckout> checkouts;
    checkouts.reserve(n_checkouts);                // Reserve memory for pointers

    // Create the checkouts
    for (size_t i {}; i < n_checkouts; ++i)
        checkouts.push_back(std::make_unique<Checkout>());

    std::vector<int> service_times(service_t_max - service_t_min + 1);

    // Add customers waiting when store opens
    int count {n_1st_customers_d(random_n)};

    std::cout << "Customers waiting at store opening: " << count << std::endl;

    int added {};

    int service_t {};

    // Define comparison lambda for pointers to checkouts
    auto comp = [](const PCheckout & pc1, const PCheckout & pc2) {
        return *pc1 < *pc2;
    };

    while (added++ < count) {
        service_t = service_t_d(random_n);
        auto iter = std::min_element(std::begin(checkouts), std::end(checkouts), comp);
        (*iter)->add(std::make_unique<Customer>(service_t));
        ++service_times[service_t - service_t_min];
    }

    size_t time {};                                // Stores time elapsed
    const size_t total_time {
        600
    };                 // Duration of simulation - minutes
    size_t longest_q {};                           // Stores longest checkout queue length
    // Period until next customer arrives
    int new_cust_interval {arrival_interval_d(random_n)};

    // Run store simulation for period of total_time minutes
    while (time < total_time) {                    // Simulation loops over time
        ++time;                                      // Increment by 1 minute

        // New customer arrives when arrival interval is zero
        if (--new_cust_interval == 0) {
            service_t = service_t_d(random_n);         // Random customer service time
            (*std::min_element(std::begin(checkouts), std::end(checkouts), comp))->add(std::make_unique<Customer>(service_t));
            ++service_times[service_t - service_t_min];  // Record service time

            // Update record of the longest queue length
            for (auto& pcheckout : checkouts)
                longest_q = std::max(longest_q, pcheckout->qlength());

            new_cust_interval = arrival_interval_d(random_n);
        }

        // Update the time in the checkouts - serving the 1st customer in each queue
        for (auto& pcheckout : checkouts)
            pcheckout->time_increment();
    }

    std::cout << "Maximum queue length = " << longest_q << std::endl;
    std::cout << "\nHistogram of service times:\n";
    histogram(service_times, service_t_min);
    std::cout << "\nTotal number of customers today: "
              << std::accumulate(std::begin(service_times), std::end(service_times), 0)
              << std::endl;
}


