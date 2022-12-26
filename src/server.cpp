
#include "network.h"

int main(){
    
    std::mt19937 engine(time(NULL));
    Field b = Field::GenerateField(engine);
    HandleGame game(b);

    StartServer(game);

    return 0;
}