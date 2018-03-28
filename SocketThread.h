#pragma once

#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <iostream>
#include <utility>

#include "Actions.h"

#include "SQLCommands.h"
#include "ErrorMessages.h"

class SocketThread : public QThread
{
    Q_OBJECT

public:

    explicit SocketThread( qintptr socDesc, QObject *parent = 0 );
    void run();

private:

    qintptr socketDescriptor;
    QTcpSocket *socket;


    // Common
    ActionsEnum ParseAction( QString command );
    std::pair<QString, QJsonObject> ParseCommand ( QString command );
    void ExecActions( QByteArray data );

private slots:

    void ReadClientDataHandler();
    void DisconnectHandler();

};
