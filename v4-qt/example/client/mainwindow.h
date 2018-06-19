#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QTcpSocket"
#include "tool.h"
#include "ui_mainwindow.h"
//#include "pvd.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void handle_connected()
    {
        prt(info,"connected");
        ui->lineEdit_ip->setText(server_ip.append(" ok"));
     //   emit connect_done();
    }


    void handle_server_msg()
    {
        QString ret_ba=tcp_socket->readAll();
        ui->plainTextEdit_rec->setPlainText(ret_ba);

        prt(info,"%d",ret_ba.length());
        string valid_buf;
        valid_buf.clear();
        tmp_msg.append(ret_ba.toUtf8().data());
        while(JsonStr::get_valid_buf(tmp_msg,valid_buf)) {
            //DataPacket pkt(string(valid_buf.data()));
            prt(info,"get %d bytes ",valid_buf.size());

        }
    }

    void connect_to_server(QString ip)
    {
        if(ip.size()){
            //  prt(info,"state %d ",tcp_socket->state());
            if(tcp_socket->state()==QAbstractSocket::ConnectedState)
                tcp_socket->disconnectFromHost();
            //     prt(info,"new ip %s connect (%d)",server_ip.toStdString().data(),tcp_socket->state());
            if(ip==server_ip){
                prt(info,"reconnecting %s",server_ip.toStdString().data());
                tcp_socket->connectToHost(server_ip,12345);
            }else{
                prt(info,"connecting %s",ip.toStdString().data());
                server_ip=ip;
                tcp_socket->connectToHost(server_ip,12345);
            }

        }else{
        }
    }
    void  displayError(QAbstractSocket::SocketError socketError)
    {
        prt(info,"err when connecting to server");
        switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:
            qDebug()<<"1";
            break;
        case QAbstractSocket::ConnectionRefusedError:
            qDebug()<<"1";
            break;
        default:
            qDebug()<<"1";
        }
    }

private slots:
    void on_pushButton_connect_clicked();



    bool send(const QByteArray ba)
    {
        //emit send_done(ba);
        bool ret=false;
        int write_bytes=0;
        int len=ba.length();
        write_bytes=tcp_socket->write(ba.data(),ba.length());
        bool flush_ret=tcp_socket->flush();//TODO,not work for flush
        if(flush_ret){
            prt(info,"flush ok");
        }else{
            prt(info,"flush err");
        }

        if(write_bytes!=len){
            prt(info,"send %d bytes in state %d , %d bytes left",write_bytes,tcp_socket->state(),len-write_bytes);
        }else{
            ret=true;
        }
        return ret;
    }
    void on_pushButton_send_clicked();

    void on_pushButton_clear_clicked();

private:
    QString server_ip;
    Ui::MainWindow *ui;
    QTcpSocket *tcp_socket;
    string tmp_msg;
};

#endif // MAINWINDOW_H
