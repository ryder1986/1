#ifndef SERVER_H
#define SERVER_H
#include "tool.h"
#include "socket.h"
class Tcpserver{
    function <void(Session *,char *,int)>fct;
public:
    Tcpserver(int p):port(p),quit(false)
    {
        fd= Socket::StartTcpServerSock(port,1000,1000);
        auto func=bind(&Tcpserver::listen,this);
        trd=new thread([func](){func();});
    }
    Tcpserver(int p,function <void(Session *,char *,int)>fc):port(p),quit(false),fct(fc)
    {
        fd= Socket::StartTcpServerSock(port,1000,1000);
        auto func=bind(&Tcpserver::listen,this);
        trd=new thread([func](){func();});
    }
    Tcpserver(vector<Session*> *clts,int p,function <void(Session *,char *,int)>fc):port(p),quit(false),fct(fc)
    {
        fd= Socket::StartTcpServerSock(port,1000,1000);
        auto func=bind(&Tcpserver::listen,this);
        trd=new thread([func](){func();});
    }
    ~Tcpserver()
    {
        quit=true;
        trd->join();
        int sz=clients.size();
        for(int i=sz-1;i>=0;i--)
        {
            delete clients[i];
            clients.pop_back();
        }
        delete trd;
    }

    void listen()
    {
        while(!quit){

            memset(peer_ip,0,IP_STR_LEN);
            int cl_fd=Socket::WaitTcpConnect(fd,1,peer_ip,&peer_port);
            if(cl_fd>0){
                cout<<"get connect from "<<peer_ip<<":"<<peer_port<<endl;
                Session *s=new Session(cl_fd,peer_ip,peer_port);
                s->end_this=bind(&Tcpserver::quit_session,this,placeholders::_1);
                s->process_data=fct;
                clients.push_back(s);
            }
        }
    }
    void send_test()
    {
        if(clients.size())
        {
            clients[0]->send("123456",6);
        }
    }
    //    void set_ret_func(function <void(char *,int )> func)
    //    {
    //        for(int i=0;i<clients.size();i++){
    //            clients[i]->process_data=func;
    //        }
    //    }

    void size()
    {
        cout<<clients.size();
    }
private:
    void quit_session(Session *s)
    {
        vector <Session *>::iterator e=find(clients.begin(),clients.end(),s);
        clients.erase(e);
        thread([s](){
            delete s;
        }).detach();
    }

    bool quit;
    int port;
    std::thread *trd;
    unsigned short peer_port;
    int fd;
    char peer_ip[IP_STR_LEN];
public:
    vector <Session *> clients;

};
// check port , once "pedestrian" recived , send IP to peer
class LocationService{
    int fd;
public:
    LocationService()
    {
        fd=Socket::CreatBroadcast(12348);
        prt(info,"start location");
        thread([this](){
            while(1){
                this_thread::sleep_for(chrono::microseconds(100));
                string peer_ip;
                int peer_port;
                if(check_port(peer_ip,peer_port)){
                    string ip=get_ip();
                    udp_send(peer_ip,peer_port,(char *)ip.data(),ip.length());
                }
            }

        }).detach();
    }
private:
    // client will send  pedestrian to  12348
    bool check_port(string &peer_ip,int &peer_port)
    {
        //   int fd=Socket::CreatBroadcast(12348);
        unsigned char buf[100];
        memset(buf,0,100);
        int rev_len=Socket::RecvFromBroadcast(buf,strlen("pedestrian"),fd,peer_ip,peer_port);
        // close(fd);
        string str((const char*)buf);
        if(rev_len>0&&"pedestrian"==str)
            return true;
        else
            return false;
    }

    string get_ip()
    {
        char ip[16];
        char mac[16];
        char mask[16];
        Socket::get_ipaddr(ip,mac,mask);
        return string(ip);
    }
    string get_gateway()
    {
        // Socket::get_gateway()
    }
    void udp_send(string ip,int port,char *data,int len )
    {
        // Socket::SendDataByClient((char *)data,len,ip.data(),port);
        int fd=Socket::UdpCreateSocket(0);
        Socket::UdpSendData(fd,ip.data(),port,data,len);
        close(fd);
    }
};

#endif // SERVER_H
