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


using namespace std;


/*
 * @brief : Calculates the Time taken to resolve the hostname
 *
 * This funtions queries / resolve the hostname using getaddrinfo.
 *
 * @param : no parameters
 * @return : On success returns the the time taken to resolve the hostname in micro seconds.
 * @throws :
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
        cerr << "Failed to resolve hostname: " << gai_strerror(err) << endl;
        exit(EXIT_FAILURE);
    }

#if 0    
    //Debug logs
    char ipv4[INET_ADDRSTRLEN];
    struct sockaddr_in *addr4;
    
    /* IPv6 */
    char ipv6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *addr6;
    cout << "Host Name " << hostname << " to below IP addresses :" << endl; 
    for (; hostAddr != NULL; hostAddr = hostAddr->ai_next) 
    { 
        if (hostAddr->ai_addr->sa_family == AF_INET) 
        {
            addr4 = (struct sockaddr_in *) hostAddr->ai_addr;
            inet_ntop(AF_INET, &addr4->sin_addr, ipv4, INET_ADDRSTRLEN);
            cout << "IPv4 address :" << ipv4 << endl;
        }
        else if (hostAddr->ai_addr->sa_family == AF_INET6) 
        {
            addr6 = (struct sockaddr_in6 *) hostAddr->ai_addr;
            inet_ntop(AF_INET6, &addr6->sin6_addr, ipv6, INET6_ADDRSTRLEN);
            cout << "IPv6 address :" << ipv6 << endl;
        }
    }
#endif
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
 * @throws :
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
        cerr << "Failed to resolve hostname: " << gai_strerror(err) << endl;
        exit(EXIT_FAILURE);
    }

    //using the first address in the hostAddr list
    int sock = socket(hostAddr->ai_family, SOCK_DGRAM, IPPROTO_ICMP);
    if (sock < 0) {
        cerr << "Failed to create socket" << endl;
        exit(EXIT_FAILURE);
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
        cerr << "Failed to send ICMP packet" << endl;
        exit(EXIT_FAILURE);
    }

    char recvbuf[64] = {0};
    ssize_t received = recv(sock, recvbuf, sizeof(recvbuf), 0);
    auto end = chrono::high_resolution_clock::now();

    close(sock);
    freeaddrinfo(hostAddr);

    if (received < 0) {
        cerr << "Failed to receive ICMP response" << endl;
        exit(EXIT_FAILURE);
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
 * @throws :
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
        cerr << "Failed to resolve hostname: " << gai_strerror(err) << endl;
        exit(EXIT_FAILURE);
    }

    int sock = socket(hostAddr->ai_family, SOCK_DGRAM, IPPROTO_ICMP);
    if (sock < 0) {
        cerr << "Failed to create socket" << endl;
        exit(EXIT_FAILURE);
    }


    //Staring the timer after dns resolve to calculate only the TCP connection time.
    auto start = chrono::high_resolution_clock::now(); 
    err = connect(sock, hostAddr->ai_addr, hostAddr->ai_addrlen);
    auto end = chrono::high_resolution_clock::now();
   
    if (err < 0) 
    {
        cerr << "Failed to connect to host" << endl;
        exit(EXIT_FAILURE);
    } 

    close(sock);
    freeaddrinfo(hostAddr);

    return {std::chrono::duration_cast<std::chrono::microseconds>(end - start).count(), "dummy"}; //return in a pair
}
