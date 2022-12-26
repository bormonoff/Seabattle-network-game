#include "network.h"

int main(int argc, char* argv[]){

    if(argc != 2 || argc != 3){
        std::cout<<"Launch program using <port> <seed> for server or <port> <seed> <IP> for client "<<std::endl;
        return 1;
    }

    std::mt19937 engine(std::stoi(argv[2]));
    Field b = Field::GenerateField(engine);
    HandleGame game(b);
    
    if(argc == 1){
        StartServer(game, std::stoi(argv[1]));
    }else{
        StartClient(game, std::stoi(argv[1]), argv[3]);
    }
    return 0;
}
