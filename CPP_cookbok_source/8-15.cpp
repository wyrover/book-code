#include <iostream>

using namespace std;

class Balance
{
    // These have to see private data
    friend const Balance operator+(const Balance& lhs, const Balance& rhs);
    friend const Balance operator+(double lhs, const Balance& rhs);
    friend const Balance operator+(const Balance& lhs, double rhs);

public:
    Balance() : val_(0.0) {}
    Balance(double val) : val_(val) {}
    ~Balance() {}

    // Unary operators
    Balance& operator+=(const Balance& other)
    {
        val_ += other.val_;
        return (*this);
    }
    Balance& operator+=(double other)
    {
        val_ += other;
        return (*this);
    }

    double getVal() const
    {
        return (val_);
    }

private:
    double val_;
};

// Binary operators
const Balance operator+(const Balance& lhs, const Balance& rhs)
{
    Balance tmp(lhs.val_ + rhs.val_);
    return (tmp);
}

const Balance operator+(double lhs, const Balance& rhs)
{
    Balance tmp(lhs + rhs.val_);
    return (tmp);
}

const Balance operator+(const Balance& lhs, double rhs)
{
    Balance tmp(lhs.val_ + rhs);
    return (tmp);
}

int main()
{
    Balance checking(500.00), savings(23.91);
    checking += 50;
    Balance total = checking + savings;
    cout << "Checking balance: " << checking.getVal() << '\n';
    cout << "Total balance: "    << total.getVal() << '\n';
}