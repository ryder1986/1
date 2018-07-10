#ifndef PVDOBJECT_H
#define PVDOBJECT_H

#include "tool.h"
#include "datapacket.h"

#define GET_INT_VALUE_FROM_PKT(mem,container) {private_data.mem=container.get_int(#mem);}

#define GET_STRING_VALUE_FROM_PKT(mem,container) (container.get_string(#mem))
#define GET_ARRAY_VALUE_FROM_PKT(mem,container) (container.get_array_packet(#mem))
#define GET_VALUE_FROM_PKT(mem,container) (container.get_pkt(#mem))






#define GET_INT_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_int(#mem);}
#define GET_STRING_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_string(#mem);}
#define GET_ARRAY_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_array_packet(#mem);}
#define GET_VALUE_FROM_PKT_(container,container_json,mem) {container->mem=container_json->get_pkt(#mem);}

#define SET_INT_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_int(#mem,container->mem);}
#define SET_STRING_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_string(#mem,container->mem);}
#define SET_ARRAY_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_array_packet(#mem,container->mem);}
#define SET_VALUE_FROM_PKT_(container,container_json,mem) {container_json->set_pkt(#mem,container->mem);}

template<typename TP>
class VdData{
protected:
    TP private_data;
public:
    VdData(TP d):private_data(d)
    {
        private_data.decode();
    }
};

class JsonData{
protected:
    DataPacket config;
public:
    JsonData(DataPacket pkt)
    {
        config=pkt;
    }
    virtual void decode()=0;
    virtual void encode()=0;
    DataPacket data()
    {
        return config;
    }

};
class JsonDataArray{
protected:
    vector <DataPacket> config;
public:
    JsonDataArray(vector <DataPacket> pkt)
    {
        config=pkt;
    }
    virtual void decode()=0;
    virtual void encode()=0;
    vector <DataPacket>  data()
    {
        return config;
    }

};
#endif // PVDOBJECT_H
