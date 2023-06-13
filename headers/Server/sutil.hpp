#ifndef SUTIL_HPP

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

class ServerConnection {
    private:
    sock_t sock, new_sock;
    socklen_t len; 
    in_addr_t address; 
    sockaddr_in serv;
    sockaddr_in remote;
    std::string lastreceived; 
    void init(in_port_t);
    void init(in_addr_t, in_port_t);

    public:
    ServerConnection() = delete; 
    ServerConnection(in_addr_t);
    ServerConnection(in_port_t, in_addr_t); 
    sock_t &getSocket();
    sock_t &getRemoteSocket();
    sockaddr_in &getServerAddr();
    sockaddr_in &getRemoteAddr();
    void Bind();
    void Accept(void (util)(ServerConnection*));
    void Start(in_port_t, void (util)(ServerConnection*));
    void Start(in_port_t);
    void Start(void (util)(ServerConnection*));
    void Start();
    void Send(std::string, int);
    void Send(std::string);
    std::string Recv(size_t = 1024); 

};
#endif