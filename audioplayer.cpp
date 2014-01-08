#include "audioplayer.h"
#include <iostream>

AudioPlayer::AudioPlayer(QObject *parent) :
    QObject(parent)
{
    serial= new SerialMain;
    connect(serial,SIGNAL(dataReceived(QByteArray)),this,SLOT(slot_receiveData(QByteArray)));


    file= new QFile("8k8bitpcm-2.raw");
    //file= new QFile("Sine440.raw");
    if(!file->open(QIODevice::ReadOnly)) {
        //QMessageBox::information(0, "error", file.errorString());
        std::cerr<<"Error reading file"<<std::endl;
    }
   // in= new QTextStream(&file);
}



void AudioPlayer::slot_receiveData(const QByteArray &a){

 //   m_ReceivedData.append(a);//Add the new data at the end;

    if(a.size()>0){
        std::cout<<"SendNewData"<<std::endl;
        sendData();
    }


    QString In = a;
    std::cout<<"String; "<<In.toStdString()<<std::endl;

    if(In=="OK"){
        std::cout<<"New Data needed"<<std::endl;
    }


    std::cout<<"here"<<std::endl;

}

void AudioPlayer::sendData(){


    qint64 bufSize = 64;
    char *buf = new char[bufSize];
    qint64 dataSize;

    if(!file->atEnd()){
        dataSize = file->read(buf, bufSize);
        std::cout<<"ToSend: "<<dataSize<<std::endl;
        std::cout<<buf<<std::endl;

        QByteArray test(buf,bufSize);
        std::cout<<"ByteArray: "<<test.size()<<std::endl;

        for(int i=0;i<test.size();i++){
            test[i]=(char)test[i]+128;
        }

        for(int i=0;i<test.size();i++){
            std::cout<<(int)test[i]<<std::endl;
        }

        serial->transmitMsg(test);
    }


   // file.close();


    //serial->transmitMsg();

}
