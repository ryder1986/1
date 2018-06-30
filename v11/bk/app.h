#ifndef APP_H
#define APP_H

#include "server.h"
#include "tool.h"

#include "pvd.h"
#include  "camera.h"
#include "pvdobject.h"
typedef struct app_arg{
    vector <DataPacket> cams;
    int server_port;
}app_arg_t;

class App:public JsonDataDealer<app_arg_t>
{
public:
    App();
    void start()
    {
        cout << "app start" << endl;
    }
    void decode(DataPacket pkt)
    {
        private_data.server_port=pkt.get_int("server_port");
        private_data.cams=pkt.get_array_packet("cameras");
    }
    void encode(DataPacket &pkt)
    {
        pkt.set_int("server_port",private_data.server_port);
        pkt.set_array_packet("cameras",private_data.cams);
    }


private:

    void process_client_data(Session *clt,char *data,int len)
    {
        str_stream.append(string(data,len));
        prt(info,"string is %s",str_stream.data());
        get_config();
#if 1
        string valid_buf;
        valid_buf.clear();

        while(JsonStr::get_valid_buf(str_stream,valid_buf)) {//Get valid json object, TODO:we only check {} matches, we should check json grammar
            prt(info,"process string   %s",valid_buf.data());
            prt(info,"left string   %s",str_stream.data());
            DataPacket data(valid_buf);
            string cmd=data.get_string("cmd");
            if(cmd=="set config"){
                cm.set_config(data.get_string("data"));
                restart_all();
            }

            if(cmd=="get config"){
             //   clt->send(cm.get_config().data().data(),cm.get_config().data().length());
              DataPacket cfg=  cm.get_config();
              DataPacket cfg_root;
              cfg_root.set_pkt("config",cfg);
              cfg_root.set_string("cmd",cmd);
               clt->send(cfg_root.data().data(),cfg_root.data().length());
            }
            if(cmd=="reset alg"){
             //   clt->send(cm.get_config().data().data(),cm.get_config().data().length());
              //       cm.set_config(data.get_string("data"));
//                int idx= data.get_int("cam_index");
//                if(idx&&idx<=cms.size()){
//                    DataPacket cns= data.get_pkt("channels");
//                    Camera *p=cms[idx-1];
//                    p->run_process();
//                }
             }



            if(cmd=="delete camera"){
                 int index=data.get_int("index");
                 del_camera(index);
            }
            if(cmd=="add camera"){
                int index=data.get_int("index");
                DataPacket cam_data=data.get_pkt("data");
                add_camera(index,cam_data);
            }
            //clt->send("hi,i get json",10);
        }
#endif
    }
    void process_camera_data(Camera *clt,const char *data,int len)
    {
        int fd=Socket::UdpCreateSocket(5000);
        Socket::UdpSendData(fd,"192.168.1.216",12346,data,len);
        close(fd);

    }

    void restart_all()
    {
        stop_cams();
        start_cams();
    }

    //TODO,we dont need clients message, so result can be send by udp one day.
    void process_data_from_output(Camera *clt,char *data,int len)
    {
        //ignore request now, we just send all clts with results
    }

    void start_cams()
    {
        for(DataPacket p:private_data.cams){
            cms.push_back(new Camera(p,bind(&App::process_camera_data,
                                            this,placeholders::_1,
                                            placeholders::_2,
                                            placeholders::_3)));
        }
    }
    void stop_cams()
    {
        for(Camera *c:cms){
            delete c;
        }
        cms.clear();
    }
    void add_camera(int index,DataPacket data)//after who ?  0~size
    {
        if(0<=index&&index<=cms.size()){
            Camera *c=new Camera(data,bind(&App::process_camera_data,
                                           this,placeholders::_1,
                                           placeholders::_2,
                                           placeholders::_3));
            vector<Camera*>::iterator it=cms.begin();
            cms.insert(it+index,c);
          //  cms.insert(cms::iterator+index);
        }
    }
    void del_camera(int index)//delete who ? 1~size
    {
        if(1<=index&&index<=cms.size()){
            delete cms[index-1];
            vector<Camera*>::iterator it=cms.begin();
            cms.erase(it+index-1);
        }
    }

private:
    vector <Session*> *stream_cmd;//clients who connected to our server
    vector <Session*> *stream_output;//client who request our video alg result
    string str_stream;//
    ConfigManager cm;
    vector <Camera*> cms;
    LocationService lservice;
};

#endif // APP_H
