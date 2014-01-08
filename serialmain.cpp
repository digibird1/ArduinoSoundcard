#include "serialmain.h"
#include <iostream>

SerialMain::SerialMain(QObject *parent ) : QObject(parent)
{
    port = new QextSerialPort("/dev/ttyACM0", QextSerialPort::EventDriven); //Serial Port
    port->setBaudRate(BAUD115200);
    port->setFlowControl(FLOW_OFF);
    port->setParity(PAR_NONE);
    port->setDataBits(DATA_8);
    port->setStopBits(STOP_1);
    //set timeouts to 500 ms
    port->setTimeout(500);

    connect(port, SIGNAL(readyRead()), SLOT(onReadyRead()));


    openPort();
}


SerialMain::~SerialMain(){
    delete port;
    port = NULL;
}

void SerialMain::transmitMsg(const QByteArray &message)
{
   // int i = port->write(message.toLatin1());
    int i = port->write(message);
    qDebug("trasmitted : %d", i);
}

void SerialMain::receiveMsg()
{
    char buff[1024];
    int numBytes;

    numBytes = port->bytesAvailable();
    if(numBytes > 1024)
        numBytes = 1024;

    int i = port->read(buff, numBytes);
    if (i != -1)
        buff[i] = '\0';
    else
        buff[0] = '\0';
    QString msg = QLatin1String(buff);

    //received_msg->append(msg);
   // received_msg->ensureCursorVisible();
    qDebug("bytes available: %d", numBytes);
    qDebug("received: %d", i);
}

void SerialMain::appendCR()
{
    //message.insert(QLatin1String("\x0D"));
}

void SerialMain::appendLF()
{
   // message.insert(QLatin1String("\x0A"));
}

void SerialMain::closePort()
{
    port->close();
    qDebug("is open: %d", port->isOpen());
}

void SerialMain::openPort()
{
    port->open(QIODevice::ReadWrite | QIODevice::Unbuffered);
    qDebug("is open: %d", port->isOpen());

    if(port->isOpen())std::cout<<"Port Open"<<std::endl;
    else std::cout<<"Port Close"<<std::endl;

}

void SerialMain::onReadyRead()
{
    if (port->bytesAvailable()) {

        //QByteArray test=port->readAll();

        emit dataReceived(port->readAll());

        //QByteArray testHex=test.toHex();


        //std::cout<<(QString::fromLatin1(testHex)).toStdString();
    }
}
