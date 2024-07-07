#include <iostream>
#include "SystemInfo.h"
#include "NetworkInfo.h"

using namespace std;
int main()
{
    cout << "System Free Memory: " << SystemInfo::getFreeMemory() <<" KB"<< endl;
    cout << "System Used Memory: " << SystemInfo::getUsedMemory() <<" KB"<< endl;
    cout << "System Number of running processes: " << SystemInfo::getRunningProcesses() <<endl;


    NetworkInfo thousandEyes;
    cout << "System Used Memory: " << thousandEyes.timeToResolveHostname() <<" µs"<< endl;
    
    return 0;
}
