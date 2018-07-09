#include <iostream>
#include "videosource.h"
#include "tool.h"
#include "app.h"
using namespace std;
LogFile Tool1::log_file1;
int main()
{
    prt(info,"app start");
    ConfigManager cm;
    App app(&cm);
    app.start();
    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}

