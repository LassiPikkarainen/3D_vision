#include "window.hpp"

int main() 
{
    Window* w = new Window();
    w->Init(1200,800);
    w->Run();

}