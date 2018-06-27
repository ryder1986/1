#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "pvd.h"
template<typename TP>
class JsonDataDealer{
protected:
    TP private_data;
public:
    virtual void encode(DataPacket &data)=0;
    virtual void decode(DataPacket data)=0;
    DataPacket get_config()
    {
        DataPacket pkt;
        encode(pkt);
        return pkt;
    }

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
