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

        NetworkInfo thousandEyes;
        cout << "Network performance info of " << thousandEyes.getHostName() << " : "<< endl;
        cout << "\tTime to resolve "<< thousandEyes.getHostName() << " : " << thousandEyes.timeToResolveHostname() <<" µs"<< endl;
        cout << "\tICMP Round trip time to  "<< thousandEyes.getHostName() << " : " << thousandEyes.icmpRoundTripTime() <<" µs"<< endl;
        auto tcpResult = thousandEyes.tcpConnectTime(443);
        cout << "\tTCP connection time to  "<< thousandEyes.getHostName() << " : " << tcpResult.first <<" µs, from interface: "<< tcpResult.second  <<endl;

        cout << endl;

        NetworkInfo google("somerandom.gg");
        cout << "Network performance info of " << google.getHostName() << " : "<< endl;
        cout << "\tTime to resolve "<< google.getHostName() << " : " << google.timeToResolveHostname() <<" µs"<< endl;
        cout << "\tICMP Round trip time to  "<< google.getHostName() << " : " << google.icmpRoundTripTime() <<" µs"<< endl;
        tcpResult = google.tcpConnectTime(443);
        cout << "\tTCP connection time to  "<< google.getHostName() << " : " << tcpResult.first <<" µs, from interface: "<< tcpResult.second  <<endl;

    }
    catch (const std::exception &ex) 
    {
        cerr << "\nError: " << ex.what() << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
