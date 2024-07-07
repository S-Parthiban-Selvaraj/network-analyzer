#include <iostream>
#include "SystemInfo.h"
#include "NetworkInfo.h"

using namespace std;
int main()
{
    cout << "System Info:" << endl;
    cout << "\tFree Memory: " << SystemInfo::getFreeMemory() <<" KB"<< endl;
    cout << "\tUsed Memory: " << SystemInfo::getUsedMemory() <<" KB"<< endl;
    cout << "\tNumber of running processes: " << SystemInfo::getRunningProcesses() <<endl;

    cout << "\n\n" << endl;

    NetworkInfo thousandEyes;
    cout << "Network performance info of " << thousandEyes.getHostName() << " : "<< endl;
    cout << "\tTime to resolve "<< thousandEyes.getHostName() << " : " << thousandEyes.timeToResolveHostname() <<" µs"<< endl;
    cout << "\tICMP Round trip time to  "<< thousandEyes.getHostName() << " : " << thousandEyes.icmpRoundTripTime() <<" µs"<< endl;
    
    NetworkInfo google("google.com");
    cout << "Network performance info of " << google.getHostName() << " : "<< endl;
    cout << "\tTime to resolve "<< google.getHostName() << " : " << google.timeToResolveHostname() <<" µs"<< endl;
    cout << "\tICMP Round trip time to  "<< google.getHostName() << " : " << google.icmpRoundTripTime() <<" µs"<< endl;
    return 0;
}
