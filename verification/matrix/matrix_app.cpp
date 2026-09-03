#include "matrix_app.h"

TestHandler* get_test_handler()
{
    return new MatrixTestHandler();
}

bool MatrixTestHandler::run_test(std::string test_name, std::vector<std::string> args)
{
    if(test_name == "sanity")
        run_sanity(args);
    else if(test_name == "addition")
        run_addition(args);
    else if(test_name == "multiply_power_2")
        run_mulp2(args);
    else if(test_name == "divide_power_2")
        run_divp2(args);
    else if(test_name == "multiple_commands")
        run_multiple(args);
    else
        return false;
    
    return true;    
}