#include "logging.h"

Logger::Logger()
{
}

Logger::~Logger()
{
#if LOG_ENABLED==1
    this->error_file.close();
#endif
}

void Logger::init(std::string name, bool is_csv)
{
#if LOG_ENABLED==1
    if(is_csv)
        this->error_file.open(name + "_log.csv", std::ofstream::out | std::ofstream::app);
    else
        this->error_file.open(name + "_log.txt", std::ofstream::out | std::ofstream::app);
#endif
}

void Logger::log_error(std::string op)
{
#if LOG_ENABLED==1
    std::unique_lock lock(this->mtx);
    this->error_file << "ERROR:" << op << std::endl;
#endif
}

void Logger::log_error_and_exit(std::string op)
{
#if LOG_ENABLED==1
    this->log_error(op);
    this->error_file << "ERROR EXIT" << std::endl;
    exit(1);
#endif
}

void Logger::log_info(std::string op)
{
#if LOG_ENABLED==1
    std::unique_lock lock(this->mtx);
    this->error_file << "INFO:\n" << op << std::endl;
#endif
}

void Logger::log_plain(std::string op)
{
#if LOG_ENABLED==1
    std::unique_lock lock(this->mtx);
    this->error_file << op << std::endl;
#endif
}
