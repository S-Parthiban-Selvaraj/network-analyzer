#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "SystemInfo.h"

#define MEM_INFO_FILE "/proc/meminfo"
using namespace std;
/*
 * @brief : getFreeMem returns the free memeory from the system.
 *
 * This funtions reads the /proc/meminfo file for the memory info and 
 * parses the free memory details. 
 *
 * @param : no parameters
 * @return : On success retuns the memory usage Kilo Bytes.
 * @throws : File Open.
 * @note : This funtion is written for Linux OS.
 * @warning 
 */
unsigned long systemInfo::getFreeMem()
{
    unsigned long FreeMemory = 0;
    int fd = open(MEM_INFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        cerr << MEM_INFO_FILE << " file open failed" << endl;
        exit(EXIT_FAILURE);
    }
    
    //Assuming maximum buffer required to read /proc/meminfo will be 1024 bytes
    char buffer[1024];
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
    if (bytesRead == -1) 
    {
        close(fd);
        cerr << MEM_INFO_FILE << " file read failed" << endl;
        exit(EXIT_FAILURE);
    }

      // Parse "MemFree:" string to get the free emeory information
    char* memFreeLine = strstr(buffer, "MemFree:");
    if (memFreeLine) 
    {
        sscanf(memFreeLine, "MemFree: %lu kB", &FreeMemory);
    } else 
    {
        cerr << " Couldn't read MemFree info. " << endl;
    }

    close(fd);
    return FreeMemory;
}

int main()
{
    systemInfo sysInfo;
    cout << "mem free info " << sysInfo.getFreeMem() << endl;

    return 0;
}
