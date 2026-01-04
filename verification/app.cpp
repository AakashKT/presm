#include "app.h"

int main(int argc, char *argv[])
{
    if(argc == 1) {
        std::cout << "Please provide test name." << std::endl;
        return -1;
    }

    std::string test_name(argv[1]);

    std::vector<std::string> args;
    for(int i=2; i<argc; i++)
        args.push_back(std::string(argv[i]));

    TestHandler* handler = get_test_handler();
    if(!handler->run_test(test_name, args)){
        std::cout << "Test with name '" << test_name << "' does not exist." << std::endl;
        return -1;
    }
    
    return 0;
}