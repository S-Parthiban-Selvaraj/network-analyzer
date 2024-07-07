# Network Probe

## Description
`network-probe` is a utility that displays various system and network information when executed without arguments.

## Functionality
- Amount of free and used memory.
- Total number of running processes.
- Time to resolve `thousandeyes.com` to an IP (in microseconds).
- ICMP round trip time to `thousandeyes.com` (in microseconds).
- Time to establish a TCP connection to `thousandeyes.com`, port 443 (in microseconds) and name of the ethernet interface where the TCP connection was established.

## usage
```
./network-probe
```
## Sample Output
```
./network-probe
System Info:
       Free Memory: 35235348 KB
        Used Memory: 492759052 KB
        Number of running processes: 562353898

Network performance info of thousandeyes.com :
        Time to resolve thousandeyes.com : 76105 µs
        ICMP Round trip time to  thousandeyes.com : 8969 µs
        Time to establish a TCP connection to  thousandeyes.com : 2 µs, using interface: eno1
```     

## Build Instructions
### CMake Configuration
The project is configured using CMake. Below is the CMakeLists.txt file used for this project:
```
cmake_minimum_required(VERSION 3.10)
project(network-probe)

set(CMAKE_CXX_STANDARD 11)

add_executable(network-probe source/main.cpp source/SystemInfo.cpp source/NetworkInfo.cpp)

```
### Prerequisites
- CMake 3.10 or later
- A C++11 compatible compiler
- Operating System  : Linux

### Build Steps
```sh
mkdir build
cd build
cmake ..
make
```

# Classes Overview
This repository contains two classes, NetworkInfo and SystemInfo, designed to gather network and system-related information. The NetworkInfo class provides methods to measure various network metrics for a given hostname, while the SystemInfo class provides static methods to retrieve system information.

## NetworkInfo
The NetworkInfo class is designed to measure network-related metrics for a specified hostname. If no hostname is provided, it defaults to "thousandeyes.com".

### Methods
### Constructors:
- `NetworkInfo():` Initializes the NetworkInfo object with the default hostname (`thousandeyes.com`).
- `NetworkInfo(string hostName):` Initializes the NetworkInfo object with the specified hostname.
### Destructor:
- `~NetworkInfo():` Destructor for the NetworkInfo object.
### Accessors:
- `const string& getHostName() const:` Returns the current hostname.
### Network Metrics:
+ `long timeToResolveHostname():` Measures the time taken to resolve the hostname to an IP address.
+ `long icmpRoundTripTime():` Measures the round-trip time for ICMP (ping) packets to the hostname.
+ `pair<long, string> tcpConnectTime(int port):` Measures the time taken to establish a TCP connection to the specified port on the hostname and returns a pair containing the time taken and the interface name.
## SystemInfo
The SystemInfo class provides static methods to retrieve various system-related metrics.

### Static Methods
- `static unsigned long getFreeMemory():` Returns the amount of free memory available on the system from ***/proc/meminfo*** file.
- `static unsigned long getUsedMemory():` Returns the amount of used memory on the system from ***/proc/meminfo*** file.
- `static int getRunningProcesses():` Returns the number of currently running processes on the system from ***/proc/stat*** file.



# Future Improvements:
## General Improvements
- **Cross-Platform Compatibility:** Enhance compatibility with different operating systems, including Windows, macOS, and various Linux distributions.
- **Unit Testing:** Add a comprehensive set of unit tests to ensure the correctness and reliability of the codebase.

## NetworkInfo Class
- **Support for More Protocols:** Extend the NetworkInfo class to support additional network protocols.
- **DNS Query Details:** Provide detailed DNS query information, including query types (A, AAAA, CNAME, etc.) and response records.
- **Traceroute Functionality:** Implement traceroute functionality to map the route packets take to the destination hostname.
- **Network Interface Metrics:** Provide detailed information about network interfaces, including bandwidth usage, packet counts, and errors.

## SystemInfo Class
- **Additional System Details :**  Add methods to retrieve CPU usage metrics, Disk Usage Metrics, Uptime, etc.
