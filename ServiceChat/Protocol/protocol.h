﻿#ifndef PROTOCOL_H
#define PROTOCOL_H
#include "stdafx.h"
#include <QtNetwork>
class Protocol
{
public:
    Protocol();
protected:
    PROTOCOLTYPE type;
    QTcpSocket socket;
};

#endif // PROTOCOL_H