#include "../headers/Client/cutil.hpp"
class Communicator {
    public: 
    void registerUser(ClientConnection con, std::string user) {

    }

    static void sendMessage(ClientConnection con, std::string rcvr, std::string message) {
        con.Connect();
        con.Send(std::string("TRG") + rcvr);
        std::string holder = con.Recv(); 
        if(holder.substr(0,3) == "CNF") {
            std::cerr << "Target user " << holder.substr(3, holder.size() - 3) << " could not found!" << std::endl;
            return; 
        }
        con.Send(std::string("MSG") + message); 
        con.Close(); 

    }
};


auto main(int argc, char **argv) -> int {
    in_port_t port = 8080; 
    ClientConnection con(port); 
    system("clear");
    //Communicator::sendMessage(con, "samet", "mesaj");
    con.Connect(); 
    con.Send(std::string("TRG") + std::string("samet\n"));
    std::cout << std::endl; 
    for(int i = 0; i < 30; i++) {
        sleep(1);
        fputs("\033[A\033[2K",stdout);
        rewind(stdout);
        ftruncate(1,0);
        std::cout << "\t" << i << std::endl;
    }
    // std::string holder = con.Recv(); 
    // std::cout << holder << std::endl; 
    // if(holder.substr(0,3) == "CNF") {
    //     std::cerr << "Target user " << holder.substr(3, holder.size() - 3) << " could not found!" << std::endl;
    //     con.Close(); 
    //     return 0;
    // }
    con.Send(std::string("MSG") + "some random msg"); 
    con.Close();
    return 0; 
}