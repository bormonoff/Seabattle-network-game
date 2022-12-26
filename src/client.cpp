#include "network.h"

int main(){
    
    std::mt19937 engine(time(NULL));
    Field b = Field::GenerateField(engine);
    HandleGame game(b);
   
    StartClient(game);

 return 0;
}