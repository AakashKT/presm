#include "matrix.h"
#include "matrix_app.h"

void run_sanity(std::vector<std::string> args)
{
    mInit();

    // MIntDeviceMemory a(60);
    // MIntDeviceMemory b(20);
    // MIntDeviceMemory c(10);

    // mAdd(a, b, c);

    // std::cout << a.getValue() << " + " << b.getValue() << " = " << c.getValue() << std::endl;

    // std::string temp;
    // std::cin >> temp;

    mFree();
}