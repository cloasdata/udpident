#ifndef UDPIdent_h
#define UDPIdent_h
#include <Arduino.h>
#include <WiFiUdp.h>
#include <IPAddress.h>

#define UDPPORT 4210

namespace UDPIdent
{
    void begin();

    void end();

    bool pair(IPAddress &with);

    bool discovered();

    void acknowledge();

    void broadcast();

    bool identify(IPAddress &identifiedIP);

};

#endif