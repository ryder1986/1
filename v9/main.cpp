#include <iostream>
#include "videosource.h"
#include "testtimer.h"
#include "testprocess.h"
#include "testsource.h"
#include "tool.h"
#include "app.h"
using namespace std;
LogFile Tool1::log_file1;
int main()
{
    prt(info,"app start");
    App app;
    app.start();
    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}

