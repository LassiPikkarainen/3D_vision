#include "window.hpp"

int main() 
{
    Window* w = new Window();
    w->Init(1920,1080);
    w->Run();

}