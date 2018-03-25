#pragma once

#include <QString>
#include <QJsonObject>
#include <QTcpSocket>
#include <utility>

#include "ErrorMessages.h"

class Utilities
{
public:

     static QString randString(int len);
     static std::pair<bool, QString> CheckSeqId ( QTcpSocket *socket, QJsonObject params );
     static void SendOk ( QTcpSocket *socket, QString seqId );

};
