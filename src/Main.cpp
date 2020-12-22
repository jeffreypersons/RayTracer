#include "RayTracer.h"
#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <iomanip>


int main()
{
    const auto console = ::GetConsoleWindow();
    const auto context = ::GetDC(console);
    constexpr auto red = RGB(255, 0, 0);
    constexpr auto yellow = RGB(255, 255, 0);

    for (int i = 100; i < 200; ++i) for (int j = 100; j < 200; ++j)
        ::SetPixel(context, i, j, std::abs(i - j) > 25 ? red : yellow);

    // close window only after prompted
    std::cout << "Press ENTER to continue...";
    std::cin.get();
}
