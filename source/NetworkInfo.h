#ifndef NETWORKINFO_H
#define NETWORKINFO_H

#include <string>
#include <utility>

using namespace std;

class NetworkInfo 
{
    string hostname; //Host name to be tested 
public:
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
    long timeToResolveHostname();

};

#endif
