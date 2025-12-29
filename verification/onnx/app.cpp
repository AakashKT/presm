#include "app.h"

int main(int argc, char *argv[])
{
    if(argc == 1) {
        std::cout << "Please provide test name." << std::endl;
        return -1;
    }

    std::string test_name(argv[1]);

    if(test_name == "sanity")
        run_sanity();
    else {
        std::cout << "Test with name '" << test_name << "' does not exist." << std::endl;
        return -1;
    }

    return 0;
}