//#include <QCoreApplication>
#include <Python.h>
#include <iostream>
using namespace std;
#include "test.h"
int main(int argc, char *argv[])
{
    //    QCoreApplication a(argc, argv);
    //   // LocationService s;

    //    return a.exec();
  //  init();
    //call_py("test_fun",pDict);
    //release();
    Test t;

    while(1){
           this_thread::sleep_for(chrono::microseconds(3000000));
    }

    return 0;
}

