#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

class systemInfo{
    public:
        unsigned long getFreeMemory();
        unsigned long getUsedMemory();
        int getRunningProcesses();
};

#endif
