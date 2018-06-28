#include <iostream>
#include "videosource.h"
#include "testtimer.h"
#include "testprocess.h"
#include "testsource.h"
#include "tool.h"
#include "app.h"
using namespace std;
LogFile Tool1::log_file1;
#include <Python.h>
void *init_py()
{

        Py_Initialize(); //初始化Python环境
        if ( !Py_IsInitialized() ) //检测是否初始化成功
        {
            return NULL;
        }
        else
        {
            PyEval_InitThreads();     //开启多线程支持
            int nInit = PyEval_ThreadsInitialized();  //检测线程支持是否开启成功
            if ( nInit )
            {
                PyEval_SaveThread();  //因为调用PyEval_InitThreads成功后，当前线程就拥有了GIL，释放当前线程的GIL，
            }
        }

}

int main()
{
    //    Py_Initialize();
  //  init_py();
    prt(info,"app start");
    App app;
    app.start();
    while(1)
    {
        this_thread::sleep_for(chrono::milliseconds(1000));
    }
    return 0;
}

