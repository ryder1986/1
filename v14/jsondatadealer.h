#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "datapacket.h"
template<typename TP>
class JsonDataDealer{
protected:
    TP private_data;
#define GET_INT_VALUE_FROM_PKT(mem,container) {private_data.mem=container.get_int(#mem);}

#define GET_INT_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_int(#mem);}
#define GET_STRING_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_string(#mem);}
#define GET_ARRAY_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_array_packet(#mem);}

#define SET_INT_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_int(#mem,container->mem);}
#define SET_STRING_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_string(#mem,container->mem);}
#define SET_ARRAY_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_array_packet(#mem,container->mem);}

    public:
//    virtual void encode(DataPacket &data)=0;
//    virtual void decode(DataPacket data)=0;
//    virtual DataPacket get_config()=0;
//    virtual void set_config(DataPacket pkt)=0;
    void set_config(DataPacket pkt)
    {
        private_data.decode(&pkt);
    }
    DataPacket get_config()
    {
        private_data.encode();
    }
    JsonDataDealer()
    {

    }
    JsonDataDealer(DataPacket pkt)
    {
        prt(info,"decode");
        private_data.decode(&pkt);
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
