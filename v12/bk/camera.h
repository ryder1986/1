#ifndef CAMERA_H
#define CAMERA_H

#include "tool.h"
#include "test.h"
#include "videosource.h"
#include "c4processor.h"
#include "videoprocessor.h"
#include "hogprocessor.h"
#include "pvd.h"
#include "pvdobject.h"
typedef struct CameraArg{
    string url;
    vector <DataPacket >channels;
}CameraArg_t;
class Camera:JsonDataDealer<CameraArg_t>
{
    //  CameraArg_t arg;
    function <void(Camera *,const char *,int)>callback_result;
public:

    //   TestProcess():src("rtsp://192.168.1.95:554/av0_1")
    // TestProcess():src("rtsp://192.168.1.216:8554/test1")
    //  TestProcess():src("rtsp://192.168.1.95:554/av0_1")
    // TestProcess():src("/media/sf_E_DRIVE/test-videos/27s-640x480-gop.mp4")
    //    Camera():src("rtsp://192.168.1.95:554/av0_1"),quit(false)
    //    {
    //        DataPacket pkt;

    //        pkt.set_int("step",2);
    //        pkt.set_string("ratio","0.7");
    //        pro=new PvdC4Processor(pkt);
    //        //      pro=new PvdHogProcessor(pkt);
    //    }
#if 1
    Camera(DataPacket cfg,function <void(Camera *,const char *,int)>fc):JsonDataDealer(),quit(false),callback_result(fc)
    {
        DataPacket pkt;    pkt.set_int("step",2);  pkt.set_string("ratio","0.7");
        decode(cfg);
        for(DataPacket p:private_data.channels){
//            if(p.get_string("selected_alg")=="pvd_c4")
         pros.push_back(new PvdC4Processor(p.get_pkt("pvd_c4")));
          //         pros.push_back(new PvdMvncProcessor(p.get_pkt("pvd_c4")));
                 // pros.push_back(new PvdHogProcessor(p.get_pkt("pvd_c4")));
        }
        src=new VideoSource(private_data.url);
        start();
    }
#else
    Camera(DataPacket cfg):JsonDataDealer(cfg),quit(false)
    {

        src=new VideoSource(private_data.url);
    }
#endif
    void encode(DataPacket &data)
    {
        data.set_string("url",private_data.url);
        data.set_array_packet("channels",private_data.channels);
    }

    void decode(DataPacket data)
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
    //    void test_config()
    //    {
    //        DataPacket pkt= pro->get_config();
    //        int step=  pkt.get_int("step");
    //    }
//    void reset_alg(DataPacket cns_pkt)
//    {
////        for(DataPacket p:private_data.channels){
//////            if(p.get_string("selected_alg")=="pvd_c4")
////         pros.push_back(new PvdC4Processor(p.get_pkt("pvd_c4")));
////          //         pros.push_back(new PvdMvncProcessor(p.get_pkt("pvd_c4")));
////                 // pros.push_back(new PvdHogProcessor(p.get_pkt("pvd_c4")));
////        }
//    }

    void run_process()
    {
        Mat frame;
        while(!quit){
            this_thread::sleep_for(chrono::milliseconds(10));
            if(src->get_frame(frame)&&frame.cols>0&&frame.rows>0){
                //       prt(info,"get a frame ");
                vector <Rect> rcts;
                Rect area(0,0,640,480);
                for(VideoProcessor *pro:pros)
                    pro->process(frame,rcts,area);
                //        prt(info,"result %d ",rcts.size());

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
                    //rectangle(frame,rc, cv::Scalar(0,255,255), 1);
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
