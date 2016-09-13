#pragma once

#include <ostream>
#include <string>
#include <queue>

// Sample Error class with just a priority and a string error description.
class Error
{
public:
    Error(int priority, const std::string& errMsg) : mPriority(priority), mError(errMsg) {}
    int getPriority() const
    {
        return mPriority;
    }
    const std::string& getErrorString() const
    {
        return mError;
    }

    friend bool operator<(const Error& lhs, const Error& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Error& err);

private:
    int mPriority;
    std::string mError;
};

// Simple ErrorCorrelator class that returns highest priority errors first.
class ErrorCorrelator
{
public:
    // Add an error to be correlated.
    void addError(const Error& error);

    // Retrieve the next error to be processed.
    Error getError();

private:
    std::priority_queue<Error> mErrors;
};
