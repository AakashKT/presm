#ifndef MLPIN_APP_H
#define MLPIN_APP_H

#include "app.h"

class MLPInTestHandler : public TestHandler {
public:
    bool run_test(std::string test_name, std::vector<std::string> args) override;
};

void run_sanity(std::vector<std::string> args);

#endif