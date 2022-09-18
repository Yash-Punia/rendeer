#include <iostream>
#include "engine.h"

using namespace rendeer;

int main()
{
    Engine::Create();
    Engine::Run();
    
    std::cout<< "\nPress enter to continue...";
    std::cin.ignore();
    return 0;
}
