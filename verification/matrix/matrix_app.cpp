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
    else
        return false;
    
    return true;    
}