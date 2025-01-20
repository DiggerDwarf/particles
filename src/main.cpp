#include <iostream>
#include "Application.hpp"

int main(int argc, char const *argv[])
{
    Application app;

    do {
        app.Render();
    } while (app.Update());

    return 0;
}
