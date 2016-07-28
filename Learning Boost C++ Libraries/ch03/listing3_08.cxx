#include "Logger.h"  // Listing 3.6a

void doLogging(const std::string& msg, ...)
{
  std::string filename = "/var/MyApp/log/app.log";
  std::unique_ptr<Logger> logger = 
                std::make_unique<Logger>(filename);
  logger->log(msg, "hello", "world");
}

