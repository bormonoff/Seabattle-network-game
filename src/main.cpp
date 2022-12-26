#include "network.h"

int main(int argc, char* argv[]){

    if(argc != 3 && argc != 4){
        std::cout<<"Launch program using <port> <seed> for the server or <port> <seed> <IP> for the client "<<std::endl;
        return 1;
    }

    std::mt19937 engine(std::stoi(argv[2]));
    Field b = Field::GenerateField(engine);
    HandleGame game(b);
    
    if(argc == 3){
        StartServer(game, std::stoi(argv[1]));
    }else if(argc == 4){
        StartClient(game, std::stoi(argv[1]), argv[3]);
    }
    return 0;
}
