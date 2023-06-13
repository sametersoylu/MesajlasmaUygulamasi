#include "cutil.hpp"
int readStatus = 2; 

void sighandle(int signal) {
    if(readStatus == 1) {
        sleep(10); 
    }
    if(readStatus == 0) {
        sleep(4); 
        readStatus = 2; 
        return; 
    }
    exit(EXIT_FAILURE); 
}


ClientConnection::ClientConnection(in_port_t addr) {
    server = {
        AF_INET,
        htons(addr),
        INADDR_ANY
    };
    signal(SIGINT, &sighandle); 
}

void ClientConnection::Connect() {
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(connect(sock, (sockaddr*)&server, sizeof(server))) {
        perror("Connect");
    }
}

void ClientConnection::Send(std::string data, int opt) {
    if(!send(sock, data.c_str(), data.length(), opt)) {
        perror("Send");
    }
}

std::string ClientConnection::Recv(size_t size) {
    std::string holder(size, ' '); 
    pid_t ch = fork();
    readStatus = 1; 
    if(ch == 0) {
        Close(); 
        kill(getppid(), SIGINT);
        return std::string(); 
    }
    int recsize = 0;
    recsize = recv(sock, (void*)holder.c_str(), size, 0);
    readStatus = 0; 
    return holder.substr(0, recsize);
}

void ClientConnection::Close() {
    close(sock);
}

sock_t &ClientConnection::getSocket() {
    return sock; 
}

sockaddr_in &ClientConnection::getServerAddr() {
    return server; 
}