#ifndef NETWORKINFO_H
#define NETWORKINFO_H

#include <string>
#include <utility>

using namespace std;

class NetworkInfo 
{
    string hostname; //Host name to be tested 
public:
    //If hostname is not passed set  it to default value.
    NetworkInfo()
    {
        hostname = "thousandeyes.com";
    }
    
    NetworkInfo(string hostName):hostname(move(hostName))
    {
    }

    ~NetworkInfo()
    {
    }

    const string& getHostName() const  { return hostname; }

    long timeToResolveHostname();
    long icmpRoundTripTime();
    pair<long, string> tcpConnectTime(int port); //Returning the time taken and interface name in a pair
};

#endif
