#include <iostream>
#include "SystemInfo.h"

using namespace std;
int main()
{
    cout << "System Free Memory: " << SystemInfo::getFreeMemory() <<" KB"<< endl;
    cout << "System Used Memory: " << SystemInfo::getUsedMemory() <<" KB"<< endl;
    cout << "System Number of running processes: " << SystemInfo::getRunningProcesses() <<endl;

    return 0;
}
