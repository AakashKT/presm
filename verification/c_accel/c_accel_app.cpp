#include "c_accel_app.h"

TestHandler* get_test_handler()
{
    return new CAccelTestHandler();
}

bool CAccelTestHandler::run_test(std::string test_name, std::vector<std::string> args)
{
    if(args.size() == 0 || !(args[0] == "host" || args[0] == "device")) {
        std::cout << "Additional app argument specifying either 'host' or 'device' is required for these tests!" << std::endl;
        return false;
    }

    if(test_name == "sanity")
        run_sanity(args);
    else if(test_name == "conditionals")
        run_conditionals(args);
    else if(test_name == "fuction_call")
        run_function_call(args);
    else
        return false;

    return true;    
}