// Defines a customer by their time to checkout
#ifndef CUSTOMER_H
#define CUSTOMER_H

class Customer
{
protected:
    size_t service_t {};                           // Time to checkout
public:
    explicit Customer(size_t st = 10) : service_t {st} {}

    // Decrement time remaining to checkout
    Customer& time_decrement()
    {
        if (service_t > 0)
            --service_t;

        return *this;
    }

    bool done() const
    {
        return service_t == 0;
    }
};
#endif