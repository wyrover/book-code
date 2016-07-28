#include "Logger.h"

void doLogging(const std::string& msg, ...)
{
    std::string logfile = "/var/MyApp/log/app.log";
    std::unique_ptr<Logger> logger = make_logger(logfile);
    logger->log(msg, "Hello", "world");
}

int main()
{}
