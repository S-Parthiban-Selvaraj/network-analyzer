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
 * @return : On success retuns the free memory in Kilo Bytes.
 * @throws : 
 * @note : This funtion is written for Linux OS.
 * @warning 
 */
unsigned long systemInfo::getFreeMemory()
{
    unsigned long FreeMemory = 0;
    int fd = open(MEM_INFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        cerr << MEM_INFO_FILE << " file open failed" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "File Read Successful" << endl;

    /* Assuming maximum buffer required to read /proc/meminfo will be 1024 bytes.
     * And MemFree: feild will be in the biginning of the file. 
     */
    char buffer[1024] = {0};
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer)-1);
    if (bytesRead == -1) 
    {
        close(fd);
        cerr << MEM_INFO_FILE << " file read failed" << endl;
        exit(EXIT_FAILURE);
    }
    
    //NULL terminate the buffer.
    buffer[bytesRead] =  '\0';
    cout << "bytesRead : " << bytesRead <<"\n  Buffer Read : \n" << buffer << endl;
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

/*
 * @brief : getUsedMemory returns the used memeory from the system.
 *
 * This funtions reads the /proc/meminfo file for the memory info and 
 * calculates the used memory from Total and Free meory feilds. 
 *
 * @param : no parameters
 * @return : On success retuns the memory usage Kilo Bytes.
 * @throws : 
 * @note : This funtion is written for Linux OS.
 * @warning 
 */
unsigned long systemInfo::getUsedMemory()
{
    unsigned long FreeMemory = 0;
    unsigned long TotalMemory = 0;
    int fd = open(MEM_INFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        cerr << MEM_INFO_FILE << " file open failed" << endl;
        exit(EXIT_FAILURE);
    }

    cout << "File Read Successful" << endl;

    /* Assuming maximum buffer required to read /proc/meminfo will be 1024 bytes.
     * And "MemFree" and "MemTotal": feilds will be in the biginning of the file. 
     */
    char buffer[1024] = {0};
    ssize_t bytesRead = read(fd, buffer, sizeof(buffer)-1);
    if (bytesRead == -1) 
    {
        close(fd);
        cerr << MEM_INFO_FILE << " file read failed" << endl;
        exit(EXIT_FAILURE);
    }
    
    //NULL terminate the buffer.
    buffer[bytesRead] =  '\0';
    cout << "bytesRead : " << bytesRead <<"\n  Buffer Read : \n" << buffer << endl;
    // Parse "MemFree:" string to get the free emeory information
    char* memFreeLine = strstr(buffer, "MemFree:");
    if (memFreeLine) 
    {
        sscanf(memFreeLine, "MemFree: %lu kB", &FreeMemory);
    } else 
    {
        cerr << " Couldn't read MemFree info. " << endl;
    }

    // Parse "MemTotal:" string to get the free emeory information
    char* memTotalLine = strstr(buffer, "MemTotal:");
    if (memFreeLine)
    {
        sscanf(memTotalLine, "MemTotal: %lu kB", &TotalMemory);
    } else
    {
        cerr << " Couldn't read MemFree info. " << endl;
    }


    close(fd);
    return (TotalMemory - FreeMemory);
}

int main()
{
    systemInfo sysInfo;
    cout << "*********** mem free info " << sysInfo.getFreeMemory() << endl;
    cout << " ************ Used memeory info " << sysInfo.getUsedMemory() << endl;

    return 0;
}
