#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdio>
#include <time.h>
#include <functional>
#include <unistd.h>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <fstream>
#include <sstream>
using namespace std;
#define DEBUG 0

struct Field;
vector <vector<Field> > maze;
int height =35;
int width =35;
int startx, starty;
#ifdef _WIN32
	int sleepTime= 10;
#else
	int sleepTime= 50000;
#endif
int myx,myy, nrOfMazes;
stack<Field> stck;
std::ofstream ofs;
enum Dirs{LEFT=0 , RIGHT, UP, DOWN};

void makeMove(int y, int x);//, int &myy, int &myx

typedef decltype(bind(makeMove, int(), int())) makeMoveBind;

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


void show(ostream& stream)
{
    clearScreen(0,0);
    for(int i =0;i<height;i++)
    {
        for(int j =0;j<width;j++)
        {
          stream<< maze[i][j].sign;
        }
        stream << "\n";
    }
#ifdef _WIN32
    Sleep(sleepTime);
#else
		usleep(sleepTime);
#endif

}

void getMazeFromFile(std::string fileName)
{
	std::ifstream ifs(fileName);
  std::string line;
	int x=0, y=0;
	while(getline(ifs,line))
	{
			vector<Field> ve;
			std::cout << line <<"\n";
			for(auto &_field : line)
			{
				Field field;
				field.x = x;
				field.y = y;
				std::cout <<"field: " << _field << "\n";
				switch(_field)
				{
					case '#':
						field.sign= '#';
						field.wall = true; 
						break;
					case ' ':
						field.sign= ' ';
						field.wall = false;  
						break; 
					case 'S':
						field.sign= 'S';
						field.wall = false; 
						startx = x;
						starty = y; 
						break; 
					case 'X':
						field.sign= 'X';
						field.wall = false; 
						break; 
				}
				ve.push_back(field);
				x++;
			}
 		maze.push_back(ve);
	x=0;
	y++;
	}

show(cout);
}

void printToFile()
{
    show(ofs);
}
void printToScreen()
{
    cout << "\n\n\n";
    show(cout);
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

void makeMove(int y, int x)//, int &myy, int &myx
{
 if(!maze[myy+y][myx+x].wall && !maze[myy+(2*y)][myx+(2*x)].wall && !maze[myy+(2*y)][myx+(2*x)].visited  )
    {
        maze[myy][myx].sign = ' ';
        maze[myy+y][myx+x].visited = true;
        maze[myy+y][myx+x].sign = '*';
        printToScreen();
        maze[myy+y][myx+x].sign = ' ';
        myy += (2*y);
        myx += (2*x);
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
        stck.push( maze[myy][myx]);
    }
}

vector <makeMoveBind> lookAround()
{
    vector <makeMoveBind> vct;
    if(!maze[myy][myx-1].wall  && !maze[myy][myx-2].visited )
        vct.push_back(bind(makeMove, 0, -1) );
    if(!maze[myy][myx+1].wall  && !maze[myy][myx+2].visited )
       vct.push_back(bind(makeMove, 0, 1) );
    if(!maze[myy-1][myx].wall  && !maze[myy-2][myx].visited)
        vct.push_back(bind(makeMove, -1, 0) );
    if(!maze[myy+1][myx].wall && !maze[myy+2][myx].visited )
        vct.push_back(bind(makeMove, 1, 0) );
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

void chooseMove()
{
    vector<makeMoveBind> directions = lookAround();
    int dir = rand()%directions.size() ;
    makeMoveBind go = directions[dir];
    go();
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
  printToScreen();
}
int main()
{
    int a =10;
//getMazeFromFile("test1.txt");
    srand( time( NULL ) );
    while(a--)
    {
        std::stringstream name ;
        name <<"test" <<nrOfMazes<< ".txt";
        ofs.open(name.str());

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
        printToScreen();
        printToFile();
        ofs.close();
//getch();

    }

    return 0;
}
