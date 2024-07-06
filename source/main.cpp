#include <iostream>
#include "SystemInfo.h"

using namespace std;
int main()
{
    systemInfo sysInfo;
    cout << "*********** mem free info " << sysInfo.getFreeMemory() << endl;
    cout << " ************ Used memeory info " << sysInfo.getUsedMemory() << endl;
    cout << " ************ Total Process info " << sysInfo.getRunningProcesses() << endl;

    return 0;
}
