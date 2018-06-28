//#include <QCoreApplication>
#include <Python.h>
#include <iostream>
using namespace std;
PyObject *pName,*pModule,*pDict;

template <class... T>
PyObject * call_py(string fun_name,PyObject *pDict,T...args )
{
    PyObject*pFunc,*pArgs;
    // printf("sz %d\n",sizeof...(args));
    pFunc = PyDict_GetItemString(pDict, fun_name.data());
    if ( !pFunc || !PyCallable_Check(pFunc) ) {
        printf("can't find function [%s]",fun_name.data());
        // getchar();
        //return -1;
    }
    int sz=sizeof...(args);
    pArgs = PyTuple_New(sz);
    set_item(pArgs,0,args...);
    //    for(int i=0;i<sz;i++)
    //        PyTuple_SetItem(pArgs, i, Py_BuildValue(args[i].des,args[i].data));
    PyObject*ret=PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    return ret;
}



void init()
{

    Py_Initialize();

    if ( !Py_IsInitialized() ) {
        printf("init err\n");
    }else{
        printf("init ok\n");
    }


    //          PyRun_SimpleString( "import sys");
    //          PyRun_SimpleString("sys.path.append('./')");

    pName = PyString_FromString("sss");


    if(!pName){
        printf("finding err \n");fflush(NULL);
    }else{
        printf("finding ok \n");fflush(NULL);
    }

    pModule = PyImport_Import(pName);
    if ( !pModule ) {
        printf("import fail");fflush(NULL);
    }else{
        printf("import ok\n");fflush(NULL);
    }
    pDict = PyModule_GetDict(pModule);
    if ( !pDict ) {
        printf("can't find dict");fflush(NULL);
    }else{
        printf("dict found\n");fflush(NULL);
    }

}
void release()
{
    Py_DECREF(pName);
    Py_DECREF(pModule);
    Py_DECREF(pDict);
    Py_Finalize();

}
int main(int argc, char *argv[])
{
    //    QCoreApplication a(argc, argv);
    //   // LocationService s;

    //    return a.exec();
    init();
    release();

    return 0;
}

