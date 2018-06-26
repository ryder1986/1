#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    tcp_socket=new QTcpSocket();
    connect(tcp_socket,SIGNAL(error(QAbstractSocket::SocketError)),this,SLOT(displayError(QAbstractSocket::SocketError)));
    connect(tcp_socket,SIGNAL(readyRead()),this,SLOT(handle_server_msg()),Qt::DirectConnection);
    connect(tcp_socket,SIGNAL(connected()),this,SLOT(handle_connected()),Qt::DirectConnection);
    ui->lineEdit_ip->setText("192.168.1.216");
    ui->plainTextEdit_send->setPlainText("\{\"cmd\"\:\"get config\"\}");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_connect_clicked()
{
    QString ip=ui->lineEdit_ip->text();
    if(tcp_socket->state()==QAbstractSocket::ConnectedState){
           tcp_socket->disconnectFromHost();
            qDebug()<<"dsiaconect";
            // ui->lineEdit_ip->setText(server_ip);

    }

    if(ip.length()){
        server_ip=ip;
        tcp_socket->connectToHost(server_ip,12345);
    }
}

void MainWindow::on_pushButton_send_clicked()
{
    send(ui->plainTextEdit_send->toPlainText().toUtf8());
}

void MainWindow::on_pushButton_clear_clicked()
{
//ui->plainTextEdit_send->clear();
ui->plainTextEdit_rec->clear();
}
