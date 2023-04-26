#include <iostream>
#include <cmath>
#include <vector>
#include <set>
#include <queue>
#include <random>
using namespace std;

set<vector<vector<int> > > gamestates;
int sumOfVisited = 0;
int sumOfMoves = 0;

struct node
{
    vector<vector<int> > gamestate;
    vector<char> path;
    int grade;
    int depth;
    int x0;
    int y0;
};

void printgamestate(vector<vector<int> > gamestate)
{
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            cout<<gamestate[i][j];
            cout<<" ";
            if(gamestate[i][j]<10) cout<<" ";
        }
        cout<<endl;
    }
}

int heuristic(vector<vector<int> > gamestate)
{
    int grade = 0;

    for(int i = 0; i<4; i++)
    {
        for(int j=0; j<4; j++)
        {
            if(gamestate[i][j]==0) continue; 
            grade = grade + 3*(abs(i-(gamestate[i][j]-1)/4)) + (abs(j-(gamestate[i][j]-1)%4));
        }
    }

    return grade;
}


class  compare{
    public:
       bool operator()(node a, node b){
           if(a.grade > b.grade){
               return true;
           }
           return false;
      }
};

bool visited(vector<vector<int> > gamestate) {

    if (gamestates.find(gamestate) == gamestates.end()) return false;
    else return true;
}

void astar(vector<vector<int> > gamestate)
{
    gamestates.clear();
    node start;
    start.gamestate = gamestate;
    start.depth = 0;
    start.grade = heuristic(gamestate);
    priority_queue<node, vector<node>, compare> q;
    start.x0 = 3;
    start.y0 = 3;
    q.push(start);
    gamestates.insert(gamestate);

    while(!q.empty())
    {
        node current = q.top();
        q.pop();
        //printgamestate(current.gamestate);
        /*
        cout<<"Gamestate: "<<endl;
        
        cout<<"Path: ";
        for(int i=0; i<current.path.size(); i++) cout<<current.path[i]<<" ";
        cout<<endl;
        cout<<"Position of zero: "<<current.y0<<" "<<current.x0<<endl;
        cout<<"Deph: "<<current.depth<<endl;
        cout<<"Gamestate heuristic: "<<heuristic(current.gamestate)<<endl;
        cout<<"Grade: "<<current.grade<<endl<<endl; */
        bool found = true;
        bool found0 = false;

        for(int i=0; i<4; i++)
        {
            for(int j=0; j<4; j++)
            {
                if(i==3 && j==3) break;
                if(current.gamestate[i][j] == 0)
                {
                    found0 = true;
                    found = false;
                    break;
                }
                if(current.gamestate[i][j] != i*4 + j+1) found = false;
            }
            if(found0) break;
        }

        if(found)
        {
            cout<<current.path.size()<<" moves: ";
            for(int i=0; i<current.path.size(); i++)
            {
                cout<<current.path[i]<<" ";
            }
            sumOfMoves += current.path.size();
            sumOfVisited += gamestates.size();

            cout<<endl<<gamestates.size()<<" visited states"<<endl;
            cout<<endl;
            return;
        }



        for(int i=0; i<4; i++) // up down left right
        {
            node *a = new node;
            a->path = current.path;
            bool valid = true;
            switch(i)
            {
                case 0: // up
                if(current.y0 == 0)
                {
                    valid = false;
                    break;  
                } 
                a->gamestate = current.gamestate;
                a->gamestate[current.y0][current.x0] = a->gamestate[current.y0-1][current.x0];
                a->gamestate[current.y0-1][current.x0] = 0;
                a->path.push_back('U');
                a->y0 = current.y0 - 1;
                a->x0 = current.x0;
                break;

                case 1: // down
                if(current.y0 == 3) 
                {
                    valid = false;
                    break;  
                } 
                a->gamestate = current.gamestate;
                a->gamestate[current.y0][current.x0] = a->gamestate[current.y0+1][current.x0];
                a->gamestate[current.y0+1][current.x0] = 0;
                a->path.push_back('D');
                a->y0 = current.y0 + 1;
                a->x0 = current.x0;
                break;

                case 2: // left
                if(current.x0 == 0)
                {
                    valid = false;
                    break;  
                } 
                a->gamestate = current.gamestate;
                a->gamestate[current.y0][current.x0] = a->gamestate[current.y0][current.x0-1];
                a->gamestate[current.y0][current.x0-1] = 0;
                a->path.push_back('L');
                a->x0 = current.x0 - 1;
                a->y0 = current.y0;
                break;

                case 3: // right
                if(current.x0 == 3) 
                {
                    valid = false;
                    break;  
                } 
                a->gamestate = current.gamestate;
                a->gamestate[current.y0][current.x0] = a->gamestate[current.y0][current.x0+1];
                a->gamestate[current.y0][current.x0+1] = 0;
                a->path.push_back('R');
                a->x0 = current.x0 +1;
                a->y0 = current.y0;
                break;
            }
            if(!valid || visited(a->gamestate)) continue;  

            a->depth = current.depth + 1;
            a->grade = a->depth + heuristic(a->gamestate);
            q.push(*a);
            gamestates.insert(a->gamestate);
        }
    }
}

bool solvable(vector<vector<int> > gamestate)
{
    int permutation[15];
    for (int i=0; i<15; i++)
    {
        permutation[i] = gamestate[i/4][i%4];
    }

    int inversions = 0;

    for(int i=0; i<15; i++)
    {
        for(int j = i+1; j<15; j++)
        {
            if(permutation[i]>permutation[j]) inversions++;
        }
    }

    if(inversions%2==0) return true;
    else return false;
}


vector<vector<int> > givegamestate(vector<int> tab)
{
    vector<vector<int> > b;
    for(int i=0; i<4; i++)
    {
        vector<int> a;
        for(int j=0; j<4; j++)
        {
            a.push_back(tab[i*4+j]);

        }
        b.push_back(a);
    }
    return b;
}

vector<int> random_permutation()
{
    srand(time(NULL));
    vector<int> array;
    for(int i=1; i<16; i++) array.push_back(i);

    for(int i = 15; i > 1; i--)
    {
        int swapindex = rand()%i;
        int temp = array[i-1];
        array[i-1] = array[swapindex];
        array[swapindex] = temp;
    }

    return array;
}

vector<vector<int> > randomgamestate()
{
    vector<vector<int> > gamestate;

    for(int i=0; i<4; i++)
    {
        vector<int> a;
        for (int j=0; j<4; j++)
        {
            a.push_back(i*4 + j + 1);
        }
        gamestate.push_back(a);
    }

    gamestate[3][3] = 0;

    int x0 = 3;
    int y0 = 3;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(0, 3);


    for(int i=0; i<100; i++)
    {
        int r = dist(gen);
        bool valid = true;
        switch(r)
        {
            case 0: // up
            if(y0 == 0)
            {
                valid = false;
                break;  
            } 
            gamestate[y0][x0] = gamestate[y0-1][x0];
            gamestate[y0-1][x0] = 0;
            y0--;
            break;

            case 1: // down
            if(y0 == 3) 
            {
                valid = false;  
                break;  
            } 
            gamestate[y0][x0] = gamestate[y0+1][x0];
            gamestate[y0+1][x0] = 0;
            y0++;
            break;

            case 2: // left
            if(x0 == 0)
            {
                valid = false;
                break;  
            } 
            gamestate[y0][x0] = gamestate[y0][x0-1];
            gamestate[y0][x0-1] = 0;
            x0--;
            break;

            case 3: // right
            if(x0 == 3) 
            {
                valid = false;
                break;  
            } 
            gamestate[y0][x0] = gamestate[y0][x0+1];
            gamestate[y0][x0+1] = 0;  
            x0++;  
            break;
        }
        
        if(!valid)
        {
            i--;
           continue; 
        }    
    }

    while(x0<3)
    {
        gamestate[y0][x0] = gamestate[y0][x0+1];
        gamestate[y0][x0+1] = 0;  
        x0++;  
    }

    while(y0<3)
    {
        gamestate[y0][x0] = gamestate[y0+1][x0];
        gamestate[y0+1][x0] = 0; 
        y0++;  
    }

    return gamestate;
}
    



int main()
{
    
    vector<int> g1;
    g1.push_back(4);
    g1.push_back(10);
    g1.push_back(11);
    g1.push_back(9);
    g1.push_back(12);
    g1.push_back(1);
    g1.push_back(13);
    g1.push_back(14);
    g1.push_back(7);
    g1.push_back(3);
    g1.push_back(2);
    g1.push_back(6);
    g1.push_back(5);
    g1.push_back(15);
    g1.push_back(8);
    g1.push_back(0);
    vector<vector<int> > g;
    g = givegamestate(g1);
printgamestate(g);
        astar(g);

/*

    vector<vector<int> > g;
    //g = givegamestate(g1);
    g=randomgamestate();
    


    {10, 7 , 15, 4,
     14, 1 ,  3, 5, 
      6, 12,  5, 7,
     11, 14,  2, 0};

    vector<vector<int> > g;
    



    for(int i=0; i<30; i++)
    {
        while(true)
        {
            g=randomgamestate();
            if(solvable(g)) break;
        }
        
        
    }*/

    //cout<<endl<<"Average moves: "<< sumOfMoves/30<<endl;
    //cout<<"Average visited states: "<< sumOfVisited/30<<endl;

    return 0;
}