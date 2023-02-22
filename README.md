# UDPIdent
A simple C++ namespace library to identify a network station via [UDP broadcast](https://en.wikipedia.org/wiki/User_Datagram_Protocol).
Developed for ESP8266 on arduino framework but should run on other machines as well. 

# Use case
You typically use this library to identify a device on a given LAN but you know determine the IP address before compile time. For example the device needs to follow [DCHP](https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol).

There are more [modern ways](https://en.wikipedia.org/wiki/Dynamic_Host_Configuration_Protocol) to accomplish this instead of UDP broadcasting.
However, this way is light weighted and easy to understand.

# How it works
First a device sends a udp broadcast package (magic packet) to the corresponding subnet. For example 192.168.1.255.

The device which likes to pair (=wants to know the IP) parses the UDP packages. Once it was found it reply with another magic acknowledge package. Thats it. Now both devices know they counter IP address etc.

```C++
#include "UDPIdent"
#include <IPAddress.h>

// device 1
void main()
{
    // do some setup
    UDP::begin();
    IPAdress remotePartner;
    while (not remotePartner){
        UDP::broadcast();
        delay(500); // let pass some time.
        UDP::identify(remotePartner);
    }
    printf("My new friend is: %s\n", remotePartner);
    UDP::end();
}

/* ... */

// device 2
void main()
{   
    // do some setup
    IPAddress remotePartner;
    UDP::begin();
    bool paired = false;
    while (not paired){
        // wait for magic package
        paired = UDP::pair(remotePartner);
    }
    printf("Paired with: %s\n", IPAddress.toString());
    UDP::end(); // frees memory from server
}

```