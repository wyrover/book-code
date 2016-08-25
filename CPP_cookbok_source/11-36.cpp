#include <stdexcept>
#include <bitset>

bool fullAdder(bool b1, bool b2, bool& carry)
{
    bool sum = (b1 ^ b2) ^ carry;
    carry = (b1 && b2) || (b1 && carry) || (b2 && carry);
    return sum;
}

bool fullSubtractor(bool b1, bool b2, bool& borrow)
{
    bool diff;

    if (borrow) {
        diff = !(b1 ^ b2);
        borrow = !b1 || (b1 && b2);
    } else {
        diff = b1 ^ b2;
        borrow = !b1 && b2;
    }

    return diff;
}

template<unsigned int N>
bool bitsetLtEq(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N - 1; i >= 0; i--) {
        if (x[i] && !y[i]) return false;

        if (!x[i] && y[i]) return true;
    }

    return true;
}

template<unsigned int N>
bool bitsetLt(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N - 1; i >= 0; i--) {
        if (x[i] && !y[i]) return false;

        if (!x[i] && y[i]) return true;
    }

    return false;
}

template<unsigned int N>
bool bitsetGtEq(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N - 1; i >= 0; i--) {
        if (x[i] && !y[i]) return true;

        if (!x[i] && y[i]) return false;
    }

    return true;
}

template<unsigned int N>
bool bitsetGt(const std::bitset<N>& x, const std::bitset<N>& y)
{
    for (int i = N - 1; i >= 0; i--) {
        if (x[i] && !y[i]) return true;

        if (!x[i] && y[i]) return false;
    }

    return false;
}

template<unsigned int N>
void bitsetAdd(std::bitset<N>& x, const std::bitset<N>& y)
{
    bool carry = false;

    for (int i = 0; i < N; i++) {
        x[i] = fullAdder(x[i], y[i], carry);
    }
}

template<unsigned int N>
void bitsetSubtract(std::bitset<N>& x, const std::bitset<N>& y)
{
    bool borrow = false;

    for (int i = 0; i < N; i++) {
        if (borrow) {
            if (x[i]) {
                x[i] = y[i];
                borrow = y[i];
            } else {
                x[i] = !y[i];
                borrow = true;
            }
        } else {
            if (x[i]) {
                x[i] = !y[i];
                borrow = false;
            } else {
                x[i] = y[i];
                borrow = y[i];
            }
        }
    }
}

template<unsigned int N>
void bitsetMultiply(std::bitset<N>& x, const std::bitset<N>& y)
{
    std::bitset<N> tmp = x;
    x.reset();

    // we want to minimize the number of times we shift and add
    if (tmp.count() < y.count()) {
        for (int i = 0; i < N; i++)
            if (tmp[i]) bitsetAdd(x, y << i);
    } else {
        for (int i = 0; i < N; i++)
            if (y[i]) bitsetAdd(x, tmp << i);
    }
}

template<unsigned int N>
void bitsetDivide(std::bitset<N> x, std::bitset<N> y,
                  std::bitset<N>& q, std::bitset<N>& r)
{
    if (y.none()) {
        throw std::domain_error("division by zero undefined");
    }

    q.reset();
    r.reset();

    if (x.none()) {
        return;
    }

    if (x == y) {
        q[0] = 1;
        return;
    }

    r = x;

    if (bitsetLt(x, y)) {
        return;
    }

    // count significant digits in divisor and dividend
    unsigned int sig_x;

    for (int i = N - 1; i >= 0; i--) {
        sig_x = i;

        if (x[i]) break;
    }

    unsigned int sig_y;

    for (int i = N - 1; i >= 0; i--) {
        sig_y = i;

        if (y[i]) break;
    }

    // align the divisor with the dividend
    unsigned int n = (sig_x - sig_y);
    y <<= n;
    // make sure the loop executes the right number of times
    n += 1;

    // long division algorithm, shift, and subtract
    while (n--) {
        // shift the quotient to the left
        if (bitsetLtEq(y, r)) {
            // add a new digit to quotient
            q[n] = true;
            bitsetSubtract(r, y);
        }

        // shift the divisor to the right
        y >>= 1;
    }
}