#include <iostream>
#include "SystemInfo.h"
#include "NetworkInfo.h"

using namespace std;
int main()
{
    try 
    {
        cout << "System Info:" << endl;
        cout << "\tFree Memory: " << SystemInfo::getFreeMemory() <<" KB"<< endl;
        cout << "\tUsed Memory: " << SystemInfo::getUsedMemory() <<" KB"<< endl;
        cout << "\tNumber of running processes: " << SystemInfo::getRunningProcesses() <<endl;
        cout << endl;

        NetworkInfo google("google.com");
        cout << "Network performance info of " << google.getHostName() << " : "<< endl;
        cout << "\tTime to resolve "<< google.getHostName() << " : " << google.timeToResolveHostname() <<" µs"<< endl;
        cout << "\tICMP Round trip time to  "<< google.getHostName() << " : " << google.icmpRoundTripTime() <<" µs"<< endl;
        auto tcpResult = google.tcpConnectTime(443);
        cout << "\tTime to establish a TCP connection to  "<< google.getHostName() << " : " << tcpResult.first <<" µs, using interface: "<< tcpResult.second  <<endl;

    }
    catch (const std::exception &ex) 
    {
        cerr << "\nError: " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
