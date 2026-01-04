#ifndef VERIFICATION_APP_H
#define VERIFICATION_APP_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstdlib>

class TestHandler {
public:
    virtual bool run_test(std::string test_name, std::vector<std::string> args) = 0;
};

TestHandler* get_test_handler();

#endif