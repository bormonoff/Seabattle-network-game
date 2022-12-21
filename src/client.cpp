#include "battlefield.h"
#include <random>
#include <ctime>

void PrintFields(Field& player, Field& enemy){
char sign{'A'};
std::string left_tab = "  ";

player.PrintHeadLine(std::cout);
std::cout<<std::endl;
for (int i = 0; i < player.m_fieldSize; i++)
{   
    std::cout<<left_tab;
    std::cout<<i<<" ";
    player.PrintFieldLine(std::cout, i);
    std::cout<<" "<<i;
    std::cout<<left_tab<<std::endl;
}
player.PrintHeadLine(std::cout);
std::cout<<std::endl;

}

int main(){

std::mt19937 engine(time(NULL));
Field b = Field::GenerateField(engine);
Field a;
PrintFields(b, a);

 return 0;
}