#pragma once

#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTcpServer>
#include "SocketThread.h"

class Application : public QTcpServer
{
    Q_OBJECT

public:

    explicit Application( QObject *parent = 0 );
    void run ();

private:
    QSqlDatabase db;

    void SetDB();

protected:
     void incomingConnection( qintptr socketDescriptor );

};
