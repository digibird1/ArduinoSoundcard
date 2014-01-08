#ifndef AUDIOPLAYER_H
#define AUDIOPLAYER_H

#include <QObject>
#include "serialmain.h"

class AudioPlayer : public QObject
{
    Q_OBJECT
public:
    explicit AudioPlayer(QObject *parent = 0);



    void sendData();
    
signals:
    
public slots:
    void slot_receiveData(const QByteArray &a);
private:
    SerialMain *serial;
    QFile *file;
    //QTextStream *in;
    
};

#endif // AUDIOPLAYER_H
