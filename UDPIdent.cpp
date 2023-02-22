#include <Arduino.h>
#include <WiFiUdp.h>
#include <IPAddress.h>
#include <ESP8266WiFi.h>

#include "UDPIdent.h"

namespace UDPIdent{

    const char magicPacket[] {"SSS_REQ"};
    const uint8_t lenMagicPacket = sizeof(magicPacket)-1;
    
    const char magicReply[] {"SSS_ACK"};
    const uint8_t lenReplyPacket = sizeof(magicPacket)-1;
    
    // global UDP server
    WiFiUDP *UDP = nullptr;

    /* Start UDP Server. 
    May interfere with other network activies.
    */
    void begin(){
        if (!UDP){
            UDP = new WiFiUDP();
        }
        UDP->begin(UDPPORT);
    }

    /* Stop all packets and destroy UDP server*/
    void end(){
        if (UDP){
            UDP->stopAll();
            delete UDP;
        }
    }

    /* Pair checks UDP stream for magic message. Received will send a acknowledge
    to the broadcaster.
    */
    bool pair(IPAddress &with){
        if (discovered()){
            with = UDP->remoteIP();
            acknowledge();
            Serial1.print("Paired with: " + with.toString() + "\n");
            return true;
        }
        return false;
    }

    /*Discovers magic message from UDP Stream*/
    bool discovered(){
        uint16_t packetSize = UDP->parsePacket();
        if (packetSize == lenMagicPacket){
            Serial1.printf("Found %d byte long packet:", packetSize );
            char pBuffer[lenMagicPacket];
            UDP->read(pBuffer, lenMagicPacket);
            Serial1.printf("%s\n", pBuffer);
            if (strcmp(magicPacket, pBuffer)) {
                return true;
            }
            return false;

        } else {
            return false;
        }
    }

    /*Send magic acknowledge to broadcaster*/
    void acknowledge(){
        UDP->beginPacket(UDP->remoteIP(), UDP->remotePort());
        UDP->write(magicReply);
        UDP->endPacket();
    }

    /*Sends UDP broadcast to WiFi.broadcastIP()
    i.e. 192.169.1.255
    */
    void broadcast(){
        UDP->beginPacket(WiFi.broadcastIP(), UDPPORT);
        UDP->write(magicPacket);
        UDP->endPacket();
    }

    /* Identifies network station by filtering the UDP Stream for the magic 
    acknowledge message. 
    */
    bool identify(IPAddress &identifiedIP){
        uint16_t packetSize = UDP->parsePacket();
        if (packetSize == lenReplyPacket){
            char pBuffer[lenReplyPacket];
            UDP->read(pBuffer, lenReplyPacket);
            if (strcmp(magicReply, pBuffer)){
                identifiedIP = UDP->remoteIP();
                return true;
            }
            
        }
        return false;
    }

}; // namespace