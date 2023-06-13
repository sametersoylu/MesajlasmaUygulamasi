#include "../headers/Server/sutil.hpp"
#include <vector>
#include <cstring>
#include <stdint.h>

int * ptr; 
std::string execName; 
std::vector<std::string> users; 

void sigchld(int signal) {
    wait(NULL);
    std::clog << "Child exited!!" << std::endl; 
}

void sigint(int signal) {
    if(close(*ptr)) {
        exit(EXIT_FAILURE);
    }
    std::clog << "SERVER TERMINATED!" << std::endl; 
    exit(EXIT_SUCCESS);
}

void setsignals() {
    signal(SIGCHLD, &sigchld);
    signal(SIGINT, &sigint);
}


auto todo(ServerConnection * con) -> void {
    std::string holder; 
    
    holder = con->Recv();  
    if(holder.substr(0, 3) == "REG") {
        users.insert(users.end(), holder.substr(3, holder.size() - 3));
        return; 
    }
    if(holder.substr(0, 3) == "EXT") {
        for(std::vector<std::string>::iterator it = users.begin(); it <= users.end(); it++) {
            if(*it == holder.substr(3, holder.size() - 3)) {
                std::cout << "User " << *it << " exited!"; 
                users.erase(it);
                break; 
            }
        }
        return; 
    }
    if(holder.substr(0, 3) == "TRG") {
        bool valid = false;
        std::cout << "First rcv: " << holder << std::endl; 
        for(std::vector<std::string>::iterator it = users.begin(); it <= users.end(); it++) {
            if(*it == holder.substr(3, holder.size() - 3)) {
                valid = true; 
            }
        }
        std::cout << "valid? " << (valid ? "Yes" : "No") << std::endl; 
        if(!valid) {
            holder = con->Recv();
            std::cout << " Second recv: "<< holder << std::endl; 
            std::cout << "Attempted to reach out a user that doesn't exist!" << std::endl;
            //con->Send("CNF" + holder.substr(3, holder.size() - 3));
        }
        if(valid) {
            //con->Send("VAL"); 
            holder = con->Recv();
            std::cout << " Second recv: "<< holder << std::endl; 
            if(holder.substr(0, 3) == "MSG") {
                std::cout << holder.substr(3, holder.size() - 3) << std::endl; 
            }
        }
        
    } 
}

void printHelp(std::string executeable_name) {
    std::cout << "Please use -i  or --ip to set ip address, and -p or --port to set port. Default will accept any address and also will attach to port 8080." << std::endl;
    std::cout << "Example usage: " << executeable_name << " -ip 127.0.0.1 -p 8080"  << std::endl;  
}

class Args {
    private: 
    std::string text; 
    public: 
    Args(std::string text) : text(text) {

    }
    void help() {
        if(!strcmp("-h", text.c_str()) || !strcmp("--help", text.c_str())) {
            printHelp(execName);
        }
    }

    bool ip() {
        
        return false; 
    }
};

void nothing() {

}

auto main(int argc, char **argv) -> int {
    execName = argv[0];
    setsignals();
    in_port_t port = 8080; 
    in_addr_t ip_addr = INADDR_ANY;
    std::string ip_holder; 
    if(argc >= 2) {
        for(int i = 1; i < argc; i++) {
            Args(argv[i]).help(); 
            if(!strcmp("-i", argv[i]) || !strcmp("--ip", argv[i])) {
                if(i < argc) {
                    ip_addr = inet_addr(argv[i + 1]);
                    ip_holder = argv[i + 1]; 
                    if(ip_addr == UINT32_MAX) {
                        std::cerr << "Incorrect IP address type! Starting with default setings!" << std::endl; 
                        ip_addr = INADDR_ANY;  
                    }
                } 
            }
            if(!strcmp("-p", argv[i]) || !strcmp("--port", argv[i])) {
                bool valid = true; 
                if(i < argc) {
                    for(auto x : std::string(argv[i + 1])) {
                        if(!(x >= '0' && x <= '9')) {
                            valid = false; 
                        }
                    }
                    if(valid) {
                        port = atoi(argv[i + 1]); 
                    }
                    if(!valid) {
                        std::cout << "Incorrect port typing! Starting with default settings!" << std::endl; 
                    }
                }
            }
        }
    }
    std::cout << "IP Set to: " << (ip_addr == INADDR_ANY ? "INADDR_ANY" : ip_holder) << " | Port set to: " << port << std::endl; 
    sleep(1); 
    ServerConnection con(ip_addr,port);
    ptr = &con.getSocket(); 
    con.Start(todo);   
    
    
    return 0; 
}