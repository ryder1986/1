#ifndef CAMERA_H
#define CAMERA_H

#include "tool.h"
#include "test.h"
#include "videosource.h"
#include "c4processor.h"
#include "videoprocessor.h"
#include "datapacket.h"
#include "jsondatadealer.h"
template <typename TP>
class TT
{

};
#define CLS(AA,...) \
class AA{\
    __VA_ARGS__  DataPacket config; \
AA(DataPacket pkt){   config=pkt;}\
  void decode(DataPacket *p_pkt){}  \
}

CLS(CameraArg_t1);
class CameraArg_t
{
public:
    string url;
    vector <DataPacket >channels;
    DataPacket config;
    CameraArg_t(DataPacket pkt)
    {
         config=pkt;
    }

    void decode(DataPacket *p_pkt)
    {
        GET_STRING_VALUE_FROM_PKT_(this,p_pkt,url);
        GET_ARRAY_VALUE_FROM_PKT_(this,p_pkt,channels);
    }
    DataPacket* encode()
    {
        DataPacket *p_pkt=&config;
        SET_STRING_VALUE_FROM_PKT_(this,p_pkt,url);
        SET_ARRAY_VALUE_FROM_PKT_(this,p_pkt,channels);
        return p_pkt;
    }
};
class Camera:JsonData<CameraArg_t>
{
    //  CameraArg_t arg;
    function <void(Camera *,const char *,int)>callback_result;
public:
    Camera(DataPacket cfg,function <void(Camera *,const char *,int)>fc):JsonData(cfg),quit(false),callback_result(fc)
    {

    //    set_config(cfg);
        for(DataPacket p:private_data.channels){
            if(GET_STRING_VALUE_FROM_PKT(selected_alg,p)=="pvd_c4")
                pros.push_back(new PvdC4Processor(p.get_pkt("pvd_c4")));
            //        pros.push_back(new PvdMvncProcessor(p.get_pkt("pvd_c4")));
            // pros.push_back(new PvdHogProcessor(p.get_pkt("pvd_c4")));
        }
        src=new VideoSource(private_data.url);
        start();
    }
    DataPacket get_config()
    {
        DataPacket data;
        data.set_string("url",private_data.url);
        data.set_array_packet("channels",private_data.channels);
        return data;
    }

    void set_config(DataPacket data)
    {
        private_data.url=data.get_string("url");
        private_data.channels=data.get_array_packet("channels");
    }

    ~Camera()
    {
        for(VideoProcessor *pro:pros)
            delete pro;
        pros.clear();
        prt(info," quit test process ");
    }
    void fun()
    {
        prt(info,"test %s ",typeid(Camera).name());
    }

    void run_process()
    {
        Mat frame;
        vector <Rect> rcts;
        while(!quit){
            this_thread::sleep_for(chrono::milliseconds(10));
            if(src->get_frame(frame)&&frame.cols>0&&frame.rows>0){
                //       prt(info,"get a frame ");
                rcts.clear();
                Rect area(0,0,640,480);
                for(VideoProcessor *pro:pros)
                    pro->process(frame,rcts,area);
                if(rcts.size()>0){
                    vector< DataPacket> pkts;
                    for(Rect r:rcts){
                        DataPacket p;
                        p.set_int("x",r.x);
                        p.set_int("y",r.y);
                        p.set_int("w",r.width);
                        p.set_int("h",r.height);
                        pkts.push_back(p);
                    }
                    DataPacket pkt(pkts);
                    string d=pkt.data();
                    callback_result(this,d.data(),d.length());
                    cv::Rect rc=rcts.front();
                    prt(info,"camera get results--> %d %d %d %d  ",rc.x,rc.y,rc.width,rc.height);
                }
                //                imshow("123",frame);
                //                waitKey(1);
            }
        }
    }
    void start()
    {
        //   _start(bind(&TestProcess::run_process,this));
        //  _start(bind(&TestProcess::run_process,this,placeholders::_1),99);
        _start_async(bind(&Camera::run_process,this));
        prt(info,"start done ~~~~~~~~~~~~~~~");
    }
    void stop()
    {
        quit=true;
    }

private:
    VideoSource *src;
    vector <VideoProcessor *>pros;
    bool quit;
};

#endif // CAMERA_H
