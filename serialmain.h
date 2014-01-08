#ifndef SERIALMAIN_H
#define SERIALMAIN_H

#include "qextserialport.h"
#include <QString>
#include <QObject>
#include <QtCore>


class SerialMain : public QObject
{
    Q_OBJECT

public:
    SerialMain(QObject *parent = 0);
    ~SerialMain();
    void transmitMsg(const QByteArray &message);

private:
    QextSerialPort *port;

    //QString message;


    void receiveMsg();
    void appendCR();
    void appendLF();
    void closePort();
    void openPort();


signals:
    void dataReceived(const QByteArray&);

private slots:

    void onReadyRead();
};

#endif // SERIALMAIN_H
