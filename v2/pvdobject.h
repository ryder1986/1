#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "pvd.h"
template<typename TP>
class JsonDataDealer{
public:
    virtual void trans(DataPacket &data,TP arg)=0;
    virtual void trans(TP &arg,DataPacket data)=0;
    virtual DataPacket get_config()=0;
};
#endif // PVDOBJECT_H
