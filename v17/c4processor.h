#ifndef PVDPROCESSOR_H
#define PVDPROCESSOR_H
#include "tool.h"
#include "videoprocessor.h"
#include "c4common.h"
#include "datapacket.h"
#include "track/Ctracker.h"

#include "jsondatadealer.h"
#define KALMAN_LOST_FRAME_THREDHOLD 5
#define KALMAN_TRACE_LEN 100

#define C4FILE "res/combined.txt.model"
#define alg_c4_file2 "res/combined2.txt.model"
#define config_file "res/config.json"

#include "jsondatadealer.h"
#include "datapacket.h"

class PointData:public JsonData{
public:
    int x;
    int y;

    PointData(DataPacket pkt):JsonData(pkt)
    {
    }
    void decode()
    {
        DataPacket *p_pkt=&config;
        GET_INT_VALUE_FROM_PKT_(this,p_pkt,x);
        GET_INT_VALUE_FROM_PKT_(this,p_pkt,y);
    }
    void encode()
    {
        DataPacket *p_pkt=&config;
        SET_INT_VALUE_FROM_PKT_(this,p_pkt,x);
        SET_INT_VALUE_FROM_PKT_(this,p_pkt,y);
    }
};
class PolyData:public JsonDataArray
{
public:
    vector<PointData> points;

    PolyData(vector <DataPacket> pkt):JsonDataArray(pkt)
    {
    }
    void decode()
    {
        for(DataPacket p:config){
            PointData pd(p);
            pd.decode();
            points.push_back(pd);
        }
    }
    void encode()
    {
        vector <DataPacket> *p_pkt=&config;
        p_pkt->clear();
        for(PointData d:points){
            d.encode();
            p_pkt->push_back(d.data());
        }
    }
};
class PvdC4ProcessorData:public JsonData
{
public:
    int scan_step;
    string ratio;
    PvdC4ProcessorData(DataPacket pkt):JsonData(pkt)
    {
    }
    void decode()
    {
        DataPacket *p_pkt=&config;
        GET_INT_VALUE_FROM_PKT_(this,p_pkt,scan_step);
        GET_STRING_VALUE_FROM_PKT_(this,p_pkt,ratio);
    }
    void encode()
    {
        DataPacket *p_pkt=&config;
        SET_INT_VALUE_FROM_PKT_(this,p_pkt,scan_step);
        SET_STRING_VALUE_FROM_PKT_(this,p_pkt,ratio);
    }
};

class PvdProcessorData:public JsonData
{
public:
    PolyData detect_area;
    int channel_id;
    PvdC4ProcessorData pvd_c4;
    PvdProcessorData(DataPacket pkt):JsonData(pkt),
        detect_area(GET_ARRAY_VALUE_FROM_PKT(detect_area,pkt)),
        pvd_c4(GET_VALUE_FROM_PKT(pvd_c4,pkt))
    {
    }
    void decode()
    {
        DataPacket *p_pkt=&config;
        GET_INT_VALUE_FROM_PKT_(this,p_pkt,channel_id);
        detect_area.decode();
        pvd_c4.decode();
    }
    void encode()
    {
        DataPacket *p_pkt=&config;
        SET_INT_VALUE_FROM_PKT_(this,p_pkt,channel_id);
        detect_area.encode();
        pvd_c4.encode();
    }
};

class PvdC4Processor : public VideoProcessor,public VdData<PvdProcessorData>
{
    typedef struct process_result{
        int width;
        int height;
        bool exist;
        int count;
        int front_count;
        int back_count;
        int other_count;
        int duration;
        int flow;
        vector <Rect> rects;
    }m_result;

public:
    float string2f(string str)
    {
        return atof(str.data());
    }
    string f2string(float f)
    {
        stringstream ss;
        ss<<f;
        return ss.str();
    }
    PvdC4Processor(DataPacket pkt):VideoProcessor(),VdData(pkt)
    {
        loaded=false;
        p_scanner=new DetectionScanner(HUMAN_height,HUMAN_width,HUMAN_xdiv,
                                       HUMAN_ydiv,256,string2f(private_data.pvd_c4.ratio));
        tracker=new CTracker(0.2f, 0.1f, 60.0f,
                             KALMAN_LOST_FRAME_THREDHOLD,
                             KALMAN_TRACE_LEN);
    }

    ~PvdC4Processor()
    {
        delete p_scanner;
        delete tracker;
    }
    string get_rst()
    {
        return alg_rst;
    }

    virtual bool process(Mat img_src,vector<Rect> &rects,Rect detect_area)
    {
        bool ret=false;
        m_result r;

        r.width=img_src.cols;
        r.height=img_src.rows;

        //  Mat img=img_src(detect_area);
        //  private_data.area=detect_area;
        //    Mat img=img_src(private_data.area);
        Mat img=img_src;
        if(real_process(img,r)){
            ret=true;
        }else
            ret=false;
        rects.clear();
        rects=r.rects;
        return  ret;
    }
private:

    const int HUMAN_height = 108;
    const int HUMAN_width = 36;
    const int HUMAN_xdiv = 9;
    const int HUMAN_ydiv = 4;


    // The detector

    // ---------------------------------------------------------------------
    // Helper functions


    // compute the Sobel image "ct" from "original"

    // A simple post-process (NMS, non-maximal suppression)
    // "result" -- rectangles before merging
    //          -- after this function it contains rectangles after NMS
    // "combine_min" -- threshold of how many detection are needed to survive
    void PostProcess(std::vector<CRect>& result,const int combine_min)
    {
        std::vector<CRect> res1;
        std::vector<CRect> resmax;
        std::vector<int> res2;
        bool yet;
        CRect rectInter;

        for(unsigned int i=0,size_i=result.size(); i<size_i; i++)
        {
            yet = false;
            CRect& result_i = result[i];
            for(unsigned int j=0,size_r=res1.size(); j<size_r; j++)
            {
                CRect& resmax_j = resmax[j];
                if(result_i.Intersect(rectInter,resmax_j))
                {
                    if(  rectInter.Size()>0.6*result_i.Size()
                         && rectInter.Size()>0.6*resmax_j.Size()
                         )
                    {
                        CRect& res1_j = res1[j];
                        resmax_j.Union(resmax_j,result_i);
                        res1_j.bottom += result_i.bottom;
                        res1_j.top += result_i.top;
                        res1_j.left += result_i.left;
                        res1_j.right += result_i.right;
                        res2[j]++;
                        yet = true;
                        break;
                    }
                }
            }
            if(yet==false){
                res1.push_back(result_i);
                resmax.push_back(result_i);
                res2.push_back(1);
            }
        }

        for(unsigned int i=0,size=res1.size(); i<size; i++)
        {
            const int count = res2[i];
            CRect& res1_i = res1[i];
            res1_i.top /= count;
            res1_i.bottom /= count;
            res1_i.left /= count;
            res1_i.right /= count;
        }

        result.clear();
        for(unsigned int i=0,size=res1.size(); i<size; i++)
            if(res2[i]>combine_min)
                result.push_back(res1[i]);
    }

    // If one detection (after NMS) is inside another, remove the inside one
    void RemoveCoveredRectangles(std::vector<CRect>& result)
    {
        std::vector<bool> covered;
        covered.resize(result.size());
        std::fill(covered.begin(),covered.end(),false);
        CRect inter;
        for(unsigned int i=0; i<result.size(); i++)
        {
            for(unsigned int j=i+1; j<result.size(); j++)
            {
                result[i].Intersect(inter,result[j]);
                double isize = inter.Size();
                if(isize>result[i].Size()*0.65)
                    covered[i] = true;
                if(isize>result[j].Size()*0.65)
                    covered[j] = true;
            }
        }
        std::vector<CRect> newresult;
        for(unsigned int i=0; i<result.size(); i++)
            if(covered[i]==false)
                newresult.push_back(result[i]);
        result.clear();
        result.insert(result.begin(),newresult.begin(),newresult.end());
        newresult.clear();
    }

    void LoadCascade(DetectionScanner& ds)
    {
        std::vector<NodeDetector::NodeType> types;
        std::vector<int> upper_bounds;
        std::vector<std::string> filenames;

        types.push_back(NodeDetector::CD_LIN); // first node
        //upper_bounds.push_back(256);	//100
#if 1

        upper_bounds.push_back(100);	//100
        // filenames.push_back("combined.txt.model");
        filenames.push_back(C4FILE);
        types.push_back(NodeDetector::CD_HIK); // second node
        //upper_bounds.push_back(117);	//353

        upper_bounds.push_back(20);	//20   small model ,like 3M
        //      upper_bounds.push_back(50);	//353
        //         // upper_bounds.push_back(353);	//353
        // filenames.push_back("combined2.txt.model");
        filenames.push_back(alg_c4_file2);
#else
        upper_bounds.push_back(100);	//100
        filenames.push_back("combined.txt.model");
        types.push_back(NodeDetector::CD_HIK); // second node
        //upper_bounds.push_back(117);	//353

        upper_bounds.push_back(256);	//353  big model,like 30M
        //      upper_bounds.push_back(50);	//353
        //         // upper_bounds.push_back(353);	//353
        filenames.push_back("combined2.txt.model");
#endif
        ds.LoadDetector(types,upper_bounds,filenames);
        // You can adjust these parameters for different speed, accuracy etc
        //   ds.cascade->nodes[0]->thresh += 0.8;
        ds.cascade->nodes[0]->thresh += string2f(private_data.pvd_c4.ratio);
        ds.cascade->nodes[1]->thresh -= 0.095;
    }

    bool real_process( Mat &src_image,m_result &rst)
    {
        std::vector<cv::Rect>  result_rects;
        std::vector<Point_t>  centers;
        bool ret=false;
        if(!loaded){
            LoadCascade(*p_scanner);
            std::cout<<"Detectors loaded."<<std::endl;
            loaded=true;
        }

        int step_size = 9;
        //int step_size = 2;
        //   float rate = 0.5;
        bool rect_organization = true;
        IntImage<double> original;

        if(result_rects.size() != 0)
        {
            result_rects.clear();
        }

        double start_time;
        start_time = cv::getTickCount();

        if( src_image.empty() )
        {
            return ret;
        }
        cv::Mat detect_region = src_image;

        original.Load( detect_region );
        std::vector<CRect> results;
        p_scanner->FastScan(original, results, step_size);

        if(rect_organization)
        {
            PostProcess(results,2);
            PostProcess(results,0);
            RemoveCoveredRectangles(results);
        }
#if 0
        for(size_t i = 0; i < results.size(); i++)
        {
            cv::Rect real_position;
            real_position.x = results[i].left/2;
            real_position.y = results[i].top/2;
            real_position.width = (results[i].right - results[i].left)/2;
            real_position.height = (results[i].bottom - results[i].top)/2;

            //   cv::rectangle(detect_region, real_position, cv::Scalar(0,255,0), 2);
            result_rects.push_back(real_position);
        }
#else

        for(size_t i = 0; i < results.size(); i++)
        {
            cv::Rect real_position;
            real_position.x = results[i].left;
            real_position.y = results[i].top;
            real_position.width = (results[i].right - results[i].left);
            real_position.height = (results[i].bottom - results[i].top);
            //   cv::rectangle(detect_region, real_position, cv::Scalar(0,255,0), 2);
            result_rects.push_back(real_position);

            centers.push_back(Point_t(real_position.x + ((float)real_position.width/2),
                                      real_position.y + ((float)real_position.height/2)));

        }

        tracker->Update(centers, result_rects, CTracker::RectsDist);
#endif
        double end_time = cv::getTickCount();
        double spend_time;
        spend_time = 1000 * (fabs(end_time - start_time) / cv::getTickFrequency());
        if(result_rects.size()>0)
        {
            ret=true;
        }
        rst.rects=result_rects;
#if 1
#endif
        return ret;
    }

private:
    bool loaded;
    DetectionScanner *p_scanner;
    CTracker *tracker;
};
#endif // PVDPROCESSOR_H
