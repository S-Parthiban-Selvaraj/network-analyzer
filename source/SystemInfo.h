#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

class SystemInfo{
    public:
        // Creating functions as static since it is generic to the system
        static unsigned long getFreeMemory();
        static unsigned long getUsedMemory();
        static int getRunningProcesses();
};

#endif
