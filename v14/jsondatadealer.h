#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "datapacket.h"
template<typename TP>
class JsonDataDealer{
protected:
    TP private_data;
    #define GET_INT_VALUE_FROM_PKT(mem,container) {private_data.mem=container.get_int(#mem);}
public:
//    virtual void encode(DataPacket &data)=0;
//    virtual void decode(DataPacket data)=0;
    virtual DataPacket get_config()=0;
    virtual void set_config(DataPacket pkt)=0;

    JsonDataDealer()
    {

    }
    //    JsonDataDealer(DataPacket pkt)
    //    {
    //         get_config();
    //          decode(pkt);
    //    }
    //    JsonDataDealer(DataPacket pkt)
    //    {
    //        decode(pkt);
    //    }
};
#endif // PVDOBJECT_H
