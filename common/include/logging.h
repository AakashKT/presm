#ifndef LOGGING_H
#define LOGGING_H

#include "common.h"

class Logger {
public:
    Logger();
    ~Logger();

    void init(std::string name, bool is_csv=false);

    void log_error(std::string op);
    void log_error_and_exit(std::string op);

    void log_info(std::string op);

    void log_plain(std::string op);

private:
    std::ofstream error_file;
    std::mutex mtx;
};

#endif