#ifndef MATRIX_APP_H
#define MATRIX_APP_H

#include "app.h"

class MatrixTestHandler : public TestHandler {
public:
    bool run_test(std::string test_name, std::vector<std::string> args) override;
};

void run_sanity(std::vector<std::string> args);
void run_addition(std::vector<std::string> args);
void run_multiplication(std::vector<std::string> args);
void run_divp2(std::vector<std::string> args);
void run_multiple(std::vector<std::string> args);

#endif