// Logger.h
#ifndef LOGGER_H
#define LOGGER_H
#include <memory>
#include <string>

class Logger
{
public:
    Logger(const std::string& filename) {}
    ~Logger() {}
    void log(const std::string& message, ...) {}
    // other methods
};

std::unique_ptr<Logger> make_logger(
    const std::string& filename)
{
    std::unique_ptr<Logger> logger(new Logger(filename));
    return logger;
}

#endif /* LOGGER_H */
