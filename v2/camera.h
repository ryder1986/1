#ifndef CAMERA_H
#define CAMERA_H

#include "tool.h"
#include "test.h"
#include "videosource.h"
#include "c4processor.h"
#include "videoprocessor.h"
#include "hogprocessor.h"

class Camera:public Test
{
public:
    //   TestProcess():src("rtsp://192.168.1.95:554/av0_1")
    // TestProcess():src("rtsp://192.168.1.216:8554/test1")
    //  TestProcess():src("rtsp://192.168.1.95:554/av0_1")
    // TestProcess():src("/media/sf_E_DRIVE/test-videos/27s-640x480-gop.mp4")
    Camera():src("rtsp://192.168.1.95:554/av0_1"),quit(false)
    {
        DataPacket pkt;

        pkt.set_int("step",2);
        pkt.set_string("ratio","0.7");
        pro=new PvdC4Processor(pkt);
        //      pro=new PvdHogProcessor(pkt);
    }


    ~Camera()
    {
        delete pro;
        prt(info," quit test process ");
    }
    void fun()
    {
        prt(info,"test %s ",typeid(Camera).name());
    }
    void test_config()
    {
        DataPacket pkt= pro->get_config();
        int step=  pkt.get_int("step");
    }

    void run_process()
    {
        Mat frame;
        while(!quit){
            this_thread::sleep_for(chrono::milliseconds(10));
            if(src.get_frame(frame)&&frame.cols>0&&frame.rows>0){
                //       prt(info,"get a frame ");
                vector <Rect> rcts;
                Rect area(0,0,640,480);

                pro->process(frame,rcts,area);
                //        prt(info,"result %d ",rcts.size());

                if(rcts.size()>0){
                    cv::Rect rc=rcts.front();
                    //    prt(info,"%d %d %d %d  ",rc.x,rc.y,rc.width,rc.height);
                    rectangle(frame,rc, cv::Scalar(0,255,255), 1);
                }
                imshow("123",frame);
                waitKey(1);
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
    VideoSource src;
    VideoProcessor *pro;
    bool quit;
};

#endif // CAMERA_H
