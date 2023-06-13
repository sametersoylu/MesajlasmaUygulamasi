#ifndef CUTIL_HPP
#include <arpa/inet.h>
#include <iostream>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include <wait.h>
#include "../defs.hpp"
typedef int sock_t; 

class ClientConnection {
    private: 

    sock_t sock; 
    sockaddr_in server; 

    public: 
    
    ClientConnection() = delete; 
    ClientConnection(in_port_t);
    void Connect();
    void Send(std::string, int = 0);
    void Close(); 
    std::string Recv(size_t = 1024); 
    sock_t &getSocket();
    sockaddr_in &getServerAddr();
};

#endif