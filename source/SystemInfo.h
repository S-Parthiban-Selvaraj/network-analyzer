#ifndef SYSTEMINFO_H
#define SYSTEMINFO_H

class SystemInfo{
    public:
        // Creating below functions as static 
        static unsigned long getFreeMemory();
        static unsigned long getUsedMemory();
        static int getRunningProcesses();
};

#endif
