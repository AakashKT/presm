#include "mlpin_app.h"

TestHandler* get_test_handler()
{
    return new MLPInTestHandler();
}

bool MLPInTestHandler::run_test(std::string test_name, std::vector<std::string> args)
{
    return false;    
}