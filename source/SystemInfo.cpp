#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include "SystemInfo.h"

#define MEM_INFO_FILE "/proc/meminfo"
#define STAT_INFO_FILE "/proc/stat"
using namespace std;

/*
 * @brief : getFreeMem returns the free memeory from the system.
 *
 * This funtions reads the /proc/meminfo file for the memory info and 
 * parses the free memory details. 
 *
 * @param : no parameters
 * @return : On success returns the free memory in Kilo Bytes.
 * @throws : 
 * @note : This funtion is written for Linux OS.
 * @warning 
 */
unsigned long SystemInfo::getFreeMemory()
{
    unsigned long FreeMemory = 0;
    int fd = open(MEM_INFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        cerr << MEM_INFO_FILE << " file open failed" << endl;
        exit(EXIT_FAILURE);
    }

    /* Reading /proc/meminfo for the memory details.
     * And parsing "MemFree" feild. 
     */
    char buffer[1024] = {0};
    ssize_t bytesRead =0;
    while ((bytesRead= read(fd, buffer, sizeof(buffer)-1)) > 0)
    {
        //NULL terminate the buffer.
        buffer[bytesRead] =  '\0';

        // Parse "MemFree:" string to get the free emeory information
        char* memFreeLine = strstr(buffer, "MemFree:");
        if (memFreeLine) 
        {
            sscanf(memFreeLine, "MemFree: %lu kB", &FreeMemory);
            break;
        } 
    }

    close(fd);
    if (!FreeMemory) 
    {
        cerr << MEM_INFO_FILE << " file read failed" << endl;
        exit(EXIT_FAILURE);
    }
    return FreeMemory;
}

/*
 * @brief : getUsedMemory returns the used memeory from the system.
 *
 * This funtions reads the /proc/meminfo file for the memory info and 
 * calculates the used memory from Total and Free meory feilds. 
 *
 * @param : no parameters
 * @return : On success returns the memory usage Kilo Bytes.
 * @throws : 
 * @note : This funtion is written for Linux OS.
 * @warning 
 */
unsigned long SystemInfo::getUsedMemory()
{
    unsigned long FreeMemory = 0;
    unsigned long TotalMemory = 0;
    int fd = open(MEM_INFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        cerr << MEM_INFO_FILE << " file open failed" << endl;
        exit(EXIT_FAILURE);
    }

    /* Reading /proc/meminfo for the memory details.
     * And parsing "MemFree" and "MemTotal": feilds. 
     */
    char buffer[1024] = {0};
    ssize_t bytesRead =0;
    while ((bytesRead= read(fd, buffer, sizeof(buffer)-1)) > 0)
    {

        //NULL terminate the buffer.
        buffer[bytesRead] =  '\0';

        // Parse "MemFree:" string to get the free emeory information
        char* memFreeLine = strstr(buffer, "MemFree:");
        if (memFreeLine && FreeMemory == 0)  
        {
            sscanf(memFreeLine, "MemFree: %lu kB", &FreeMemory);
        }

        // Parse "MemTotal:" string to get the free emeory information
        char* memTotalLine = strstr(buffer, "MemTotal:");
        if (memTotalLine && TotalMemory == 0)
        {
            sscanf(memTotalLine, "MemTotal: %lu kB", &TotalMemory);
        } 

        //Found both free and total meory deatails , break the loop
        if(TotalMemory && FreeMemory)
        {
            break;
        }
    }
    close(fd);

    if (!TotalMemory || !FreeMemory ) 
    {
        cerr << MEM_INFO_FILE << " file read failed" << endl;
        exit(EXIT_FAILURE);
    }
    return (TotalMemory - FreeMemory);
}

/*
 * @brief : getRunningProcesses returns the total number of running processes from the system.
 *
 * This funtions reads the /proc/stat file for the running processes info.
 *
 * @param : no parameters
 * @return : On success returns the total number of running processes.
 * @throws : 
 * @note : This funtion is written for Linux OS.
 * @warning 
 */
int SystemInfo::getRunningProcesses()
{
    int TotalProcesses = -1;
    int fd = open(STAT_INFO_FILE, O_RDONLY);
    if (fd == -1) 
    {
        cerr << STAT_INFO_FILE << " file open failed" << endl;
        exit(EXIT_FAILURE);
    }

    /* Assuming maximum buffer required to read /proc/stat file to get the system statistics info*/
    char buffer[1024] = {0};
    ssize_t bytesRead =0;
    while ((bytesRead= read(fd, buffer, sizeof(buffer)-1)) > 0)
    {

        //NULL terminate the buffer.
        buffer[bytesRead] =  '\0';
        // Parse "processes " string to get the running processes information
        char* totalProcessLine = strstr(buffer, "processes ");
        if (totalProcessLine) 
        {
            sscanf(totalProcessLine, "processes %d", &TotalProcesses);
            break;
        } 
    }

    close(fd);

    if (TotalProcesses == -1) 
    {
        cerr << STAT_INFO_FILE << " file read failed" << endl;
        exit(EXIT_FAILURE);
    }

    return TotalProcesses;
}

