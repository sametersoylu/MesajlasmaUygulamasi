#include "sutil.hpp"

void ServerConnection::init(in_port_t addr) {
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket" << std::endl; 
        exit(EXIT_FAILURE);
    }
    serv.sin_family = AF_INET; 
    serv.sin_addr.s_addr = INADDR_ANY; 
    serv.sin_port = htons(addr);
    address = addr; 
    new_sock = 0;
    len = 0;  
}

void ServerConnection::init(in_addr_t ip_addr, in_port_t addr) {
    if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket" << std::endl; 
        exit(EXIT_FAILURE);
    }
    serv.sin_family = AF_INET; 
    serv.sin_addr.s_addr = ip_addr;
    serv.sin_port = htons(addr);
    address = addr; 
    new_sock = 0;
    len = 0;  
}

ServerConnection::ServerConnection(in_addr_t addr) {
    init(addr);
}

ServerConnection::ServerConnection(in_port_t ip_addr, in_addr_t port) {
    init(ip_addr, port); 
}

std::string ServerConnection::Recv(size_t size) {
    std::string holder(size, ' '); 
    int recsize = recv(new_sock, (void*)holder.c_str(), size, 0);
    if(recsize < 0) {
        std::cerr << "Recv failed: ";
        perror(""); 
    }
    return holder.substr(0, recsize); 
}

sock_t &ServerConnection::getSocket() {
    return sock; 
}

sock_t &ServerConnection::getRemoteSocket() {
    return new_sock;
}

sockaddr_in &ServerConnection::getServerAddr() {
    return serv; 
}

sockaddr_in &ServerConnection::getRemoteAddr() {
    return remote; 
}

void ServerConnection::Bind() {
    if(address == 0) {
        printf("No address to bind!");
        return; 
    }   
    if(bind(sock, (sockaddr*)&serv, sizeof(serv))) {
        std::cerr << "Bind" << std::endl; 
    }
}

void ServerConnection::Accept(void (util)(ServerConnection*)) {
    if(address == 0) {
        printf("No address to accept from!");
        return; 
    }  
    while((new_sock = accept(sock, (sockaddr*)&remote, &len)) >= 0) {
        if(!fork()) {
            close(sock); 
            printf("A client connected to a child!\n"); 
            util(this); 
            close(new_sock); 
            printf("A client terminated connection successfully!\n"); 
            exit(EXIT_SUCCESS);
        }
    }
}

void ServerConnection::Start(in_port_t addr, void(util)(ServerConnection*)) {
    init(addr); 
    Bind(); 
    listen(sock, 100);
    std::clog << "Server started! Ready to accept connections!" << std::endl; 
    Accept(util);
}

void ServerConnection::Start(in_port_t addr) {
    init(addr); 
    Bind();
    listen(sock, 100);
    std::clog << "Server started! Ready to accept connections!" << std::endl; 
    Accept([](ServerConnection*) {return; });
}

void ServerConnection::Start(void (util)(ServerConnection*)) {
    if(address == 0) {
        std::cerr << "No address set!"; 
        return; 
    }
    init(address); 
    Bind(); 
    listen(sock, 100);
    std::clog << "Server started! Ready to accept connections!" << std::endl; 
    Accept(util);
}

void ServerConnection::Start() {
    if(address == 0) {
        std::cerr << "No address set!"; 
        return; 
    }
    init(address); 
    Bind(); 
    listen(sock, 100);
    std::clog << "Server started! Ready to accept connections!" << std::endl; 
    Accept([](ServerConnection*) {return; });
}

void ServerConnection::Send(std::string data, int opt) {
    int sent = 0; 
    while (sent < data.length()) { 
        sent += send(new_sock, data.c_str() + sent, data.length() - sent, opt); 
    }

}

void ServerConnection::Send(std::string data) {
    send(new_sock, data.c_str(), data.length(), 0); 
}