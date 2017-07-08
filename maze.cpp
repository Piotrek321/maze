#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdio>
#include <time.h>
#include <functional>
#include <unistd.h>

#include <fstream>
#include <sstream>

#ifdef _WIN32
	#include <windows.h>
#endif
using namespace std;
#define DEBUG 0
#define printToScreen 1

#ifdef _WIN32
	int sleepTime= 10;
#else
	int sleepTime= 50000;
#endif
struct Field;
vector <vector<Field> > maze;
int height =35;
int width =35;
int startx, starty;

int myx,myy, nrOfMazes;
stack<Field> stck;

enum Dirs{LEFT=0 , RIGHT, UP, DOWN};

void clearScreen(int x, int y)
{
#ifdef _WIN32
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
#else
    cout << string( 100, '\n' );
#endif
}

struct Field
{
    int x,y;
    char sign;
    bool visited = false;
    bool wall = false;

};


void show(bool toScreen, bool toFile = false)
{
    std::stringstream name ;
    name <<"test" <<nrOfMazes<< ".txt";
    std::ofstream ofs (name.str(), std::ofstream::out);
    clearScreen(0,0);
    if(toScreen) cout << "\n\n\n";

    for(int i =0;i<height;i++)
    {
        for(int j =0;j<width;j++)
        {
            if(toFile) ofs<< maze[i][j].sign;
            if(toScreen) cout << maze[i][j].sign;
        }
        if(toFile) ofs << "\n";
        if(toScreen) cout << "\n";
    }

    usleep(sleepTime);
    ofs.close();
}
void fillMaze()
{
    maze.clear();
    for(int i =0;i<height;i++)
    {
        vector<Field> ve;
        for(int j =0;j<width;j++)
        {
            Field field;
            field.sign= '#';
            field.x = j;
            field.y = i;
            if(i == 0 || j == 0 || i == height-1 || j == width-1)
            {
                field.wall = true;
            }
            ve.push_back(field);
        }
        maze.push_back(ve);
    }

    myx = (rand()%(width-1)) +1 ;
    myy = (rand()%(height-1)) +1;
    startx = (myx %2) ? myx : --myx;
    starty = (myy %2) ? myy : --myy;

    myx = startx;
    myy = starty;
    maze[myy][myx].sign = 'S';
    maze[myy][myx].visited= true;
    stck.push(maze[myy][myx]);

}


bool stuck()
{
    if(myx-2 <= 0  || maze[myy][myx-2].visited )
    {
        if(myx+2 >= width  || maze[myy][myx+2].visited )
        {
            if(myy-2 <= 0  || maze[myy-2][myx].visited)
            {
               if(myy+2 >= height || maze[myy+2][myx].visited )
               {
                 return 1;
               }
            }
        }
    }
    return 0;
}


vector <int> lookAround()
{
    vector <int> vct;
    if(!maze[myy][myx-1].wall  && !maze[myy][myx-2].visited )
        vct.push_back(LEFT);
    if(!maze[myy][myx+1].wall  && !maze[myy][myx+2].visited )
        vct.push_back(RIGHT);
    if(!maze[myy-1][myx].wall  && !maze[myy-2][myx].visited)
        vct.push_back(UP);
    if(!maze[myy+1][myx].wall && !maze[myy+2][myx].visited )
        vct.push_back(DOWN);
    return vct;
}

int availableFields()
{
    int ctr=0;
     for(int i =0;i<height;i++)
    {
        for(int j =0;j<width;j++)
        {
            if(i == 0 || j == 0 || i == height-1 || j == width-1){}
            else
            {
                if(i%2 != 0 && j%2 != 0 && maze[i][j].sign == '#')
                {
                    ctr ++;
                }
            }
        }
    }
    return ctr;
}

void makeMove(int y, int x)//, int &myy, int &myx
{
 if(!maze[myy+y][myx+x].wall && !maze[myy+(2*y)][myx+(2*x)].wall && !maze[myy+(2*y)][myx+(2*x)].visited  )
    {
        maze[myy][myx].sign = ' ';
        maze[myy+y][myx+x].visited = true;
        maze[myy+y][myx+x].sign = '*';
        show(printToScreen);
        maze[myy+y][myx+x].sign = ' ';
        myy += (2*y);
        myx += (2*x);
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
        stck.push( maze[myy][myx]);
    }
}

void chooseMove()
{
    vector<int> directions = lookAround();
    int dir = rand()%directions.size() ;
    dir = directions[dir];

    switch(dir)
   {
                case LEFT:
                    makeMove(0,-1);
                    break;
                case RIGHT:
                    makeMove(0,1);
                    break;
                case UP:
                    makeMove(-1,0);
                    break;
                case DOWN:
                    makeMove(1,0);
                    break;
                default:
                    if(DEBUG)cout <<"ERROR\n";
                }
}

template <typename T>
void clearStack(stack<T> _stck)
{
    while (!_stck.empty())
    {
        _stck.pop();
    }
}
void goBack()
{
  maze[myy][myx].sign = ' ';
  stck.pop();
  Field tmp  = stck.top();
  myx= tmp.x;
  myy = tmp.y;
  maze[myy][myx].sign = '*';
  show(printToScreen);
}
int main()
{
    int a =5;
    srand( time( NULL ) );
    while(a--)
    {
        clearStack(stck);
        nrOfMazes++;
        fillMaze();
        bool endGame = false;
        while(!endGame)
        {
            if(stuck())
            {
                if(availableFields())
                {
                    if(DEBUG) cout <<"Myx: " << myx << " myy: " << myy  <<"\n";
                    goBack();
                    continue;
                }
                else
                {
                  endGame = true;
                }
            }
            if(!endGame)
            {
                chooseMove();
            }
        }
        maze[starty][startx].sign = 'S';
        maze[myy][myx].sign = 'X';
        show(printToScreen, 1);

//getch();

    }
    return 0;
}

