#ifndef TEST_H
#define TEST_H
#include <Python.h>
#include <iostream>
#include "conversion.h"
using namespace std;
using namespace cv;


template <typename T>
class py_arg_t{
public:
    py_arg_t(T d,string n):data(d),des(n)
    {}
    T data;
    string des;
};
template <typename... T>
void set_item(PyObject* pArgs,int index)
{
}
template <class... T,typename HEAD>
void set_item(PyObject* pArgs,int index,HEAD arg,T...args)
{
    int sz=sizeof...(args);
    PyTuple_SetItem(pArgs, index++, Py_BuildValue(((string)arg.des).data(),arg.data));
    set_item(pArgs,index,args...);
}
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

#include <thread>
#include <mutex>
class GILLock
{
public:
    GILLock() : _state(PyGILState_Ensure()) {}
    ~GILLock()
    {
        PyGILState_Release(_state);
    }
private:
    PyGILState_STATE _state;
};
class Test
{
    string name;
    PyObject *pName,*pModule,*pDict;
    mutex  lk;
    PyThreadState * mainThreadState ;
    NDArrayConverter *p_cvt;
public:
    Test(string n):name(n)
    {
        //fun1();
        //init();
        // Py_Initialize();
        // p_cvt=new NDArrayConverter();
//        thread(bind(&Test::fun1,
//                    this)).detach();

    PyEval_InitThreads();
    PyEval_ReleaseLock();
    PyRun_SimpleString( "import sys");
   PyRun_SimpleString("sys.path.append('./')");
        thread(bind(&Test::fun2,
                    this)).detach();
        thread(bind(&Test::fun2,
                    this)).detach();

//        printf("-----------%x\n",this_thread::get_id());

       //   fun2();
        //        this_thread::sleep_for(chrono::microseconds(3000000));
    }
    ~Test()
    {
        // delete p_cvt;
        // release();
    }
    void fun2()
    {
        int flg=0;
        while(1) {
    //    Py_Initialize();
     //   PyEval_SaveThread();

        GILLock gil;
        pName = PyString_FromString("run");
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
         printf("%d\n",flg++);fflush(NULL);

        Py_DECREF(pDict);


        Py_DECREF(pModule);
        Py_DECREF(pName);
       //    PyGILState_Ensure();
          //   PyEval_ReleaseThread(PyThreadState_Get());
       //      PyThreadState_Get();
     //

//          if ( Py_IsInitialized() )
//        Py_Finalize();
        }
    }

    void fun1()
    {
        //  Py_Initialize();
        int flg=0;
        //        if ( !Py_IsInitialized() ) //检测是否初始化成功
        //        {
        //            Py_Initialize();
        //              PyEval_InitThreads();
        //           }    PyRun_SimpleString( "import sys");
        //              PyRun_SimpleString("sys.path.append('./')");
        //   printf("init srr\n");
        // return NULL;


        while(1){
            lk.lock();
            printf("-------------------testname------------->  %s\n",name.data());
            //            if ( !Py_IsInitialized() ) //检测是否初始化成功
            //            {
            //                Py_Initialize();
            //                PyRun_SimpleString( "import sys");
            // PyRun_SimpleString("sys.path.append('./')");

            //                //   printf("init srr\n");
            //                // return NULL;
            //            }
            //            Py_Initialize();
            //            PyEval_InitThreads();
            //            PyEval_ReleaseLock();
            Py_Initialize();   PyRun_SimpleString( "import sys"); PyRun_SimpleString("sys.path.append('./')");
            GILLock gil;
            pName = PyString_FromString("run");
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

            Py_DECREF(pDict);


            Py_DECREF(pModule);
            Py_DECREF(pName);
            if (! Py_IsInitialized() ) //检测是否初始化成功
            {
                // Py_Finalize();
            }
            //
            //  this_thread::sleep_for(chrono::microseconds(100000));
            //  Py_Finalize();
#if 0
            Py_DECREF(pDict);
            Py_DECREF(pModule);
            Py_DECREF(pName);
            Py_Finalize();
#endif
            flg++;
            lk.unlock();
        }
    }
    void fun()
    {



        int  ttt=5;
        //  PyEval_ReleaseLock();
        // PyEval_AcquireLock();

        //  mainThreadState = PyThreadState_Get();
        while(ttt--){
            lk.lock();

            //            Py_BEGIN_ALLOW_THREADS;
            //            Py_BLOCK_THREADS;
            //   PyEval_AcquireLock();

            printf("get lock ok  %x\n",this_thread::get_id());

            printf("-------------------testname------------->  %s\n",name.data());
            Mat mt;
            mt.create(640,480,CV_8UC3);

            p_cvt->toNDArray(mt);

            //  PyEval_AcquireLock();
            GILLock gil;
            call_py("test_fun",pDict);
            // PyEval_ReleaseLock();
            fflush(NULL);


            this_thread::sleep_for(chrono::microseconds(100000));
            //            Py_UNBLOCK_THREADS;
            //            Py_END_ALLOW_THREADS;




            printf("put lock ok %x\n",this_thread::get_id());
            lk.unlock();
            // this_thread::sleep_for(chrono::microseconds(10000000));

        }
        printf(" -------------------->>>done \n");fflush(NULL);
        //PyEval_ReleaseLock();
        // PyEval_AcquireLock();


        // PyEval_RestoreThread(t);

    }
    void *init_py()
    {

        Py_Initialize(); //初始化Python环境
        if ( !Py_IsInitialized() ) //检测是否初始化成功
        {
            printf("init srr\n");
            return NULL;
        }
        else
        {
            PyEval_InitThreads();     //开启多线程支持
            //    int nInit = PyEval_ThreadsInitialized();  //检测线程支持是否开启成功
            //     if ( nInit )
            {
                //  PyEval_SaveThread();  //因为调用PyEval_InitThreads成功后，当前线程就拥有了GIL，释放当前线程的GIL，
            }
        }

    }

    void init()
    {

        Py_Initialize();
        // init_py();
        //        if ( !Py_IsInitialized() ) {
        //            printf("init err\n");
        //        }else{
        //            printf("init ok\n");
        //        }
        PyEval_InitThreads();   ;

        PyEval_ReleaseLock();

        //  PyEval_SaveThread();
        //        PyRun_SimpleString( "import sys");
        //        PyRun_SimpleString("sys.path.append('./')");

        pName = PyString_FromString("run");


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
        //   PyEval_AcquireLock();
        Py_Finalize();

    }
};

#endif // TEST_H
