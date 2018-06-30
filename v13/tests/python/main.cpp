//#include <QCoreApplication>
#include <Python.h>
#include <iostream>
using namespace std;
#include "test.h"
int main(int argc, char *argv[])
{
    if (! Py_IsInitialized() )
        Py_Initialize();
    //    Py_Initialize();
    //    Py_Initialize();
    //     // init_py();
    //    //        if ( !Py_IsInitialized() ) {
    //    //            printf("init err\n");
    //    //        }else{
    //    //            printf("init ok\n");
    //    //        }
    //    PyEval_InitThreads();   ;
    //    PyEval_InitThreads();   ;
    //    PyEval_InitThreads();   ;
    //    PyEval_ReleaseLock();
    //    PyEval_ReleaseLock();
    //    PyEval_ReleaseLock();
    //    PyEval_ReleaseLock();
    //    PyEval_ReleaseLock();
    //    PyEval_ReleaseLock();
    //    PyRun_SimpleString( "import sys");
    //    PyRun_SimpleString("sys.path.append('./')");
    //    QCoreApplication a(argc, argv);
    //   // LocationService s;

    //    return a.exec();
    //  init();
    //call_py("test_fun",pDict);
    //release();
    Test *t1=new Test("test1");
    this_thread::sleep_for(chrono::microseconds(3000000));
    //  delete t1;

//    Test *t2=new Test("test2");
//    this_thread::sleep_for(chrono::microseconds(3000000));
    //  delete t2;
    //    this_thread::sleep_for(chrono::microseconds(3000000));
    //    Test *t2=new Test("test2");
    //    this_thread::sleep_for(chrono::microseconds(3000000));
    //    Test *t3=new Test("test3");
    //    this_thread::sleep_for(chrono::microseconds(3000000));
    //    Test *t4=new Test("test4");
    //    this_thread::sleep_for(chrono::microseconds(3000000));
    while(1){
        this_thread::sleep_for(chrono::microseconds(3000000));
    }

    return 0;
}

