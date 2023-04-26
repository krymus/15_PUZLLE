#include "headers.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;


int main()
{
    int** gamestate = new int* [4];
    for(int i=0; i<4; i++)
    {
        gamestate[i] = new int[4];
        for (int j=0; j<4; j++)
        {
            gamestate[i][j] = i*4+j +1;
        }
    }

    gamestate[3][3] = 0;
    gamestate[2][2] = 15;
    gamestate[2][3] = 11;
    gamestate[3][2] = 12;

    
    cout<<"Start state: "<<endl;
    printgamestate(gamestate);

    if(solvable(gamestate))
    {
        cout<<"solvable"<<endl<<endl;
        //astar(gamestate);
    }
    else cout<<"not solvable"<<endl;

    delete[] gamestate;
    return 0;
}