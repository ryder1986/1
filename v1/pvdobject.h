#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "pvd.h"

//class PvdObject
//{
//public:
//    PvdObject();
//    virtual void decode_data()=0;
//    DataPacket get_data()
//    {
//        return DataPacket();
//    }

//private:
//    void encode_data()
//    {

//    }
//};
//template<typename TP>
//class BaseConfig{
//public:
//    BaseConfig(TP d)
//    {
//        data=d;
//    }

//    void decode_data()
//    {

//    }

//    void encode_data()
//    {

//    }
//    DataPacket get_data()
//    {
//        return DataPacket();
//    }

//private:
//    TP data;
//};
template<typename TP>
class JsonDataDealer{
    //    DataPaket data;
   // TP arg;
public:
    JsonDataDealer()
    {
     //   arg=arg1;
    }
    virtual void trans(DataPacket &data,TP arg)=0;
    virtual void trans(TP &arg,DataPacket data)=0;
    virtual DataPacket get_config()=0;
//    {
//        DataPacket ret;
//        trans(ret,arg);
//        return ret;
//    }
};
#endif // PVDOBJECT_H
