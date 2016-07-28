// Supermarket checkout - using smart pointers to customers in a queue
#ifndef CHECKOUT_H
#define CHECKOUT_H
#include <queue>                                 // For queue container
#include <memory>                                // For smart pointers
#include "Customer.h"
using PCustomer = std::unique_ptr<Customer>;

class Checkout
{
private:
    std::queue<PCustomer> customers;                // The queue waiting to checkout

public:
    void add(PCustomer&& customer)
    {
        customers.push(std::move(customer));
    }
    size_t qlength() const
    {
        return customers.size();
    }

    // Increment the time by one minute
    void time_increment()
    {
        if (customers.front()->time_decrement().done())  // If the customer is done...
            customers.pop();                               // ...remove from the queue
    };

    bool operator<(const Checkout& other) const
    {
        return qlength() < other.qlength();
    }
    bool operator>(const Checkout& other) const
    {
        return qlength() < other.qlength();
    }
};
#endif