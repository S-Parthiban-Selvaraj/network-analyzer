#include "NetworkInfo.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <chrono>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>


using namespace std;

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
        cerr << "Failed to resolve hostname: " << gai_strerror(err) << std::endl;
        exit(EXIT_FAILURE);
    }
    
    //Debug logs
    char ipv4[INET_ADDRSTRLEN];
    struct sockaddr_in *addr4;
    
    /* IPv6 */
    char ipv6[INET6_ADDRSTRLEN];
    struct sockaddr_in6 *addr6;

    for (; hostAddr != NULL; hostAddr = hostAddr->ai_next) {
        if (hostAddr->ai_addr->sa_family == AF_INET) {
            addr4 = (struct sockaddr_in *) hostAddr->ai_addr;
            inet_ntop(AF_INET, &addr4->sin_addr, ipv4, INET_ADDRSTRLEN);
            cout << "IPv6 address :" << ipv4 << endl;
        }
        else if (hostAddr->ai_addr->sa_family == AF_INET6) {
            addr6 = (struct sockaddr_in6 *) hostAddr->ai_addr;
            inet_ntop(AF_INET6, &addr6->sin6_addr, ipv6, INET6_ADDRSTRLEN);
            cout << "IPv6 address :" << ipv6 << endl;
        }
    }
    
    freeaddrinfo(hostAddr); 
    
    return chrono::duration_cast<std::chrono::microseconds>(end - start).count();
}
