 /****************************************************************************
 **
 ** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (qt-info@nokia.com)
 **
 ** This file is part of the examples of the Qt Toolkit.
 **
 ** $QT_BEGIN_LICENSE:BSD$
 ** You may use this file under the terms of the BSD license as follows:
 **
 ** "Redistribution and use in source and binary forms, with or without
 ** modification, are permitted provided that the following conditions are
 ** met:
 **   * Redistributions of source code must retain the above copyright
 **     notice, this list of conditions and the following disclaimer.
 **   * Redistributions in binary form must reproduce the above copyright
 **     notice, this list of conditions and the following disclaimer in
 **     the documentation and/or other materials provided with the
 **     distribution.
 **   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
 **     the names of its contributors may be used to endorse or promote
 **     products derived from this software without specific prior written
 **     permission.
 **
 ** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 ** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 ** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 ** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 ** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 ** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 ** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 ** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 ** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 ** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 ** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 ** $QT_END_LICENSE$
 **
 ****************************************************************************/

 #include "fortunethread.h"

 #include <QtNetwork>
 #include <QDebug>

 FortuneThread::FortuneThread(int socketDescriptor, const QString &fortune, QObject *parent)
     : QThread(parent), socketDescriptor(socketDescriptor), text(fortune)
 {
 }

 void FortuneThread::run()
 {
     qDebug()<< "socket initialized"<<endl;
     QTcpSocket tcpSocket;

     if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
         emit error(tcpSocket.error());
         return;
     }

     qDebug()<< "socket setup"<<endl;
     qDebug()<<tcpSocket.state()<<endl;

     if (!tcpSocket.waitForReadyRead(30))
     {
         qDebug()<<"Not ready for data"<<endl;
     }
     QByteArray buff = tcpSocket.readAll();
     QString s = buff;
     qDebug()<<s<<endl;
     /*QDataStream in(&tcpSocket);
     qint64 streamSize;
     in.setVersion(QDataStream::Qt_4_0);
     quint16 blockSize = 0;
     if (blockSize == 0) {
         qDebug()<<"enter blocksize 0"<<endl;
         int size = (int)sizeof(quint16);
         qDebug()<< size <<endl;
         streamSize = tcpSocket.bytesAvailable();
         qDebug()<<streamSize<<endl;


         if (streamSize < size)
         {
             qDebug()<<"blockSize less than size"<<endl;

         }

         in >> blockSize;
     }

     qDebug()<<blockSize<<endl;
     streamSize = tcpSocket.bytesAvailable();
    qDebug()<<streamSize<<endl;

    if (tcpSocket.bytesAvailable() < blockSize)
         return;

     QString nextFortune;
     in >> nextFortune;
     qDebug() << nextFortune << endl;
     tcpSocket.flush();
     qDebug()<< "in message received"<<endl;
*/

     QByteArray block;
     QDataStream out(&block, QIODevice::WriteOnly);
     out.setVersion(QDataStream::Qt_4_0);
     out << (quint16)0;
     out << text;
     out.device()->seek(0);
     out << (quint16)(block.size() - sizeof(quint16));

     tcpSocket.write(block);

     qDebug()<< "out message send"<<endl;
     tcpSocket.disconnectFromHost();
     tcpSocket.waitForDisconnected();
     qDebug()<< "socket closed"<<endl;
 }
 void FortuneThread::onNewData()
 {


 }
