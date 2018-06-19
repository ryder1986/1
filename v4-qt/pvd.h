#ifndef PVD_H
#define PVD_H
#include <cstring>
#include <json/json.h>
#include <json/value.h>
#include "configmanager.h"
#include <QJsonValue>
using namespace std;
using  namespace Json ;
#define JV
#ifdef JV
typedef  Value JsonValue;
#else
typedef QJsonValue JsonValue;
#endif




//class DataArr{
//public:
//    DataArr(vector<DataPacket> data)
//    {

//    }
//};

class DataPacket{
    JsonValue val;
public:
    DataPacket()
    {
    }
#ifdef JV
    DataPacket(string data)
    {
        JsonValue v;
        Reader r;
        bool rst=r.parse(data,v);
        val=v;
    }
    string data()
    {
        FastWriter  w;
        return  w.write(val);
    }

    string get_string(string name)
    {
       return val[name].asString();
    }
    int get_int(string name)
    {
        return val[name].asInt();
    }
    bool get_bool(string name)
    {
        return val[name].asBool();
    }
    DataPacket get_pkt(string name)
    {
        DataPacket rst(val[name]);
        return rst;
    }
    vector<DataPacket>get_array(string name)
    {
        vector<DataPacket> rst;
        return rst;
    }

    void set_string(string name, string v)
    {

    }
    void set_int(string name,int v)
    {

    }
    void set_bool(string name,bool v)
    {

    }
    void set_pkt(string name,DataPacket pkt)
    {

    }
    void set_array(string name,vector<DataPacket> array)
    {

    }

#else
    DataPacket(string data)
    {

    }
    string data()
    {
        string rst;
        return rst;
    }

    string get_string(string name)
    {
        string rst;
        return rst;
    }
    int get_int(string name)
    {
        int rst;
        return rst;
    }
    bool get_bool(string name)
    {
        bool rst;
        return rst;
    }
    DataPacket get_pkt(string name)
    {
        DataPacket rst;
        return rst;
    }
    vector<DataPacket>get_array(string name)
    {
        vector<DataPacket> rst;
        return rst;
    }

    void set_string(string name, string v)
    {

    }
    void set_int(string name,int v)
    {

    }
    void set_bool(string name,bool v)
    {

    }
    void set_pkt(string name,DataPacket pkt)
    {

    }
    void set_array(string name,vector<DataPacket> array)
    {

    }
#endif
};

#endif // PD_H
