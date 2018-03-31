#include "Utilities.h"

QString Utilities::randString(int len)
{

    srand( time( 0 ) );

    QString str;
    str.resize(len);

    for (int s = 0; s < len ; ++s)
        str[s] = QChar('A' + char( rand() % ('z' - 'A')));

    return str;
}

std::pair<bool, QString> Utilities::CheckSeqId ( QTcpSocket *socket, QJsonObject params )
{
    QString seqId = params["seqId"].toString();

    if ( seqId.size() == 0 )
    {
        socket->write( SEQ_ID_IS_NULL );
        return std::make_pair( true, "NULL" );
    }

    return std::make_pair( false, seqId );
}

void Utilities::SendOk( QTcpSocket *socket, QString seqId )
{
    socket->write( QString("{ \"seqId\" : \"" + seqId + "\", \"msg\": \"OK\" }").toUtf8() );
}
