#include <iostream>
#include "Application.hpp"

int main(int argc, char const *argv[])
{
    Application app;
    while (app.Update())
    {
        app.Render();
    }
    
    return 0;
}
