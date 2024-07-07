#include "NetworkInfo.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <chrono>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/ip_icmp.h>
#include <unistd.h>
#include <netinet/in.h>
#include <ifaddrs.h>



using namespace std;


/*
 * @brief : Calculates the Time taken to resolve the hostname
 *
 * This funtions queries / resolve the hostname using getaddrinfo.
 *
 * @param : no parameters
 * @return : On success returns the the time taken to resolve the hostname in micro seconds.
 * @throws : runtime_error
 * @note : This funtion is written for Linux OS.
 * @warning
 */

long NetworkInfo::timeToResolveHostname()
{
    addrinfo hints, *hostAddr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //for any ip type (ipv4 or v6)
    hints.ai_socktype = SOCK_STREAM;
    
    auto start = chrono::high_resolution_clock::now(); //Start Time before the dns resolve using getaddrinfo
    int err = getaddrinfo(hostname.c_str(), NULL, &hints, &hostAddr);
    auto end = chrono::high_resolution_clock::now(); //End Time

    if (err != 0) 
    {
        throw std::runtime_error("Failed to resolve host name , Error : " + string(gai_strerror(err)));
    }

    freeaddrinfo(hostAddr); 
    
    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}

/*
 * @brief : icmpRoundTripTime calculates the round trip time for ICMP ping 
 *
 * This funtions queries / resolve the hostname using getaddrinfo,
 * And calculates the ICMP ping round trip time by sending ICMP message to resolved addr
 *
 * @param : no parameters
 * @return :  On success returns the the time taken to ICMP ping to the hostname in micro seconds.
 * @throws : runtime_error
 * @note : This funtion is written for Linux OS.
 * @warning
 */

long NetworkInfo::icmpRoundTripTime()
{
    addrinfo hints, *hostAddr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //for any ip type (ipv4 or v6)
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(hostname.c_str(), NULL, &hints, &hostAddr);
    if (err != 0)
    {
        throw std::runtime_error("Failed to resolve host name , Error : " + string(gai_strerror(err)));
    }

    //using the first address in the hostAddr list
    int sock = socket(hostAddr->ai_family, SOCK_DGRAM, IPPROTO_ICMP);
    if (sock < 0) {
        throw std::runtime_error("Failed to create socket " +  string(strerror(errno)));
    }

    char sendbuf[64] = {0};
    icmp* icmp_hdr = (icmp*) sendbuf;
    icmp_hdr->icmp_type = ICMP_ECHO;
    icmp_hdr->icmp_code = 0;
    icmp_hdr->icmp_cksum = 0;
    icmp_hdr->icmp_seq = 0;
    icmp_hdr->icmp_id = getpid();

    //Staring the timer after dns resolve to calculate only the ping round trip time.
    auto start = chrono::high_resolution_clock::now(); 
    ssize_t sent = sendto(sock, sendbuf, sizeof(sendbuf), 0, hostAddr->ai_addr, hostAddr->ai_addrlen);
    if (sent < 0) {
        throw std::runtime_error("Failed to send ICMP echo packet " +  string(strerror(errno)));
    }

    char recvbuf[64] = {0};
    ssize_t received = recv(sock, recvbuf, sizeof(recvbuf), 0);
    auto end = chrono::high_resolution_clock::now();

    close(sock);
    freeaddrinfo(hostAddr);

    if (received < 0) {
        throw std::runtime_error("No reply received. ");
    }

    return chrono::duration_cast<chrono::microseconds>(end - start).count();
}


/*
 * @brief : tcpConnectTime calculates the time taken to create a TCP connection. 
 *
 * This funtions queries / resolve the hostname using getaddrinfo,
 * And calculates the time taken to create a TCP connection to the addr.
 *
 * @param : port number in integer
 * @return :  On success returns the the time taken for TCP connection and the interface name to create the connection in a std::pair
 * @throws : runtime_error
 * @note : This funtion is written for Linux OS.
 * @warning
 */
pair<long, std::string> NetworkInfo::tcpConnectTime(int port)
{
    addrinfo hints, *hostAddr;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //for any ip type (ipv4 or v6)
    hints.ai_socktype = SOCK_STREAM;

    // Pass the port number in c string format
    int err = getaddrinfo(hostname.c_str(), to_string(port).c_str(), &hints, &hostAddr); 
    if (err != 0)
    {
        throw std::runtime_error("Failed to resolve host name , Error : " + string(gai_strerror(err)));
    }

    int sock = socket(hostAddr->ai_family, SOCK_DGRAM, IPPROTO_ICMP);
    if (sock < 0) {
        throw std::runtime_error("Failed to create socket " +  string(strerror(errno)));
    }


    //Staring the timer after dns resolve to calculate only the TCP connection time.
    auto start = chrono::high_resolution_clock::now(); 
    err = connect(sock, hostAddr->ai_addr, hostAddr->ai_addrlen);
    auto end = chrono::high_resolution_clock::now();
   
    if (err < 0) 
    {
        throw std::runtime_error("Failed to connect to the host : " +  string(strerror(errno)));
    } 

    //find the local interface bound to the socket
    string interfaceName = "unknown";
    
    struct sockaddr_storage addr; //using sockaddr_storage struct to handle both Ipv4 and Ipv6
    struct ifaddrs *ifaddr, *ifa;
    socklen_t addr_len = sizeof(addr);

    //Get the addr details that the socket bound to locally
    if (getsockname(sock, (struct sockaddr*)&addr, &addr_len) == -1) 
    {
        throw std::runtime_error("getsockname failed : " +  string(strerror(errno)));
    }

    //Get all the system interfaces addr details
    if (getifaddrs(&ifaddr) == -1) 
    {
        throw std::runtime_error("getifaddrs failed : " +  string(strerror(errno)));
    }

    // Look for the interface containing the sockets's local IP.
    // When found, ifa->ifa_name contains the name of the interface that TCP connection is created.
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) 
    {
        if(ifa->ifa_addr)
        {
            if (AF_INET == ifa->ifa_addr->sa_family) 
            {
                //cast to IPV4 struct
                struct sockaddr_in *inaddr = (struct sockaddr_in *)ifa->ifa_addr;
                struct sockaddr_in *sock_inaddr = (struct sockaddr_in *)&addr;
                if (inaddr->sin_addr.s_addr == sock_inaddr->sin_addr.s_addr) 
                {
                    if (ifa->ifa_name) 
                    {
                        interfaceName = ifa->ifa_name;
                        break;
                    }
                }
            }
            else if(AF_INET6 == ifa->ifa_addr->sa_family) 
            {
                //cast to IPv6 struct
                struct sockaddr_in6 *in6addr = (struct sockaddr_in6 *)ifa->ifa_addr;
                struct sockaddr_in6 *sock_in6addr = (struct sockaddr_in6 *)&addr;

                if (memcmp(&in6addr->sin6_addr, &sock_in6addr->sin6_addr, sizeof(struct in6_addr)) == 0)  
                {
                    if (ifa->ifa_name) 
                    {
                        interfaceName = ifa->ifa_name;
                        break;
                    }
                }
            }
        }
    }

    close(sock);
    freeaddrinfo(hostAddr);
    freeifaddrs(ifaddr);

    return {std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), interfaceName}; //return in a pair
}
