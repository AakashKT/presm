#include "logging.h"

Logger::Logger()
{
}

Logger::~Logger()
{
    this->log_info("Closing log.");
    this->error_file.close();
}

void Logger::init(std::string name)
{
    this->error_file.open(name + "_log.txt");
}

void Logger::log_error(std::string op)
{
    this->error_file << "ERROR: " << op << std::endl;
}

void Logger::log_error_and_exit(std::string op)
{
    this->log_error(op);
    this->error_file << "EXITED." << std::endl;
    exit(1);
}

void Logger::log_info(std::string op)
{
    this->error_file << "INFO: " << op << std::endl;
}
