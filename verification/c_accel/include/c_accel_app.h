#ifndef C_ACCEL_APP_H
#define C_ACCEL_APP_H

#include "app.h"

class CAccelTestHandler : public TestHandler {
public:
    bool run_test(std::string test_name, std::vector<std::string> args) override;
};

void run_sanity(std::vector<std::string> args);

#endif