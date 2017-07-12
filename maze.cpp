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
#include "maze.h"
using namespace std;
#define DEBUG 0

struct Field;
vector <vector<Field> > maze;
int height =15;
int width =15;
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

void Maze::makeMove(int y, int x);//, int &myy, int &myx

//typedef decltype(bind(makeMove, int(), int())) makeMoveBind;

void Maze::clearScreen(int x, int y)
{
#ifdef _WIN32
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
#else
    cout << string( 50, '\n' );
#endif
}



void Maze::show(ostream& stream)
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

void Maze::getMazeFromFile(std::string fileName)
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

void Maze::printToFile()
{
    show(ofs);
}
void Maze::printToScreen()
{
    cout << "\n\n\n";
    show(cout);
}

void Maze::fillMaze()
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


bool Maze::stuck()
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

void Maze::makeMove(int y, int x)//, int &myy, int &myx
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

/*vector <makeMoveBind> Maze::lookAround()
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
}*/


int Maze::availableFields()
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

void Maze::chooseMove()
{
   /* vector<makeMoveBind> directions = lookAround();
    int dir = rand()%directions.size() ;
    makeMoveBind go = directions[dir];
    go();*/
}

template <typename T>
void Maze::clearStack(stack<T> _stck)
{
	while (!_stck.empty())
  {
	   _stck.pop();
  }
}
void Maze::goBack()
{
  maze[myy][myx].sign = ' ';
  stck.pop();
  Field tmp  = stck.top();
  myx= tmp.x;
  myy = tmp.y;
  maze[myy][myx].sign = '*';
  printToScreen();
}
bool Maze::isDeadEnd(int y, int x)
{
	if(maze[y][x].sign != '#' && maze[y][x].sign != 'S' && maze[y][x].sign != 'X')
	{
		int ctr =0;
		if(maze[y-1][x].sign == '#')ctr++;
		if(maze[y][x-1].sign == '#')ctr++;
		if(maze[y+1][x].sign == '#')ctr++;
		if(maze[y][x+1].sign == '#')ctr++;
		if(ctr>=3) 		return true;
	}
return false;

}
int Maze::createCrossroads()
{    
int numberOfCrossroads =0;
	for(int i =0;i<height;i++)
  {
	  for(int j =0;j<width;j++)
    {
			if(maze[i][j].sign == ' ')
			{			
			 	if(maze[i-1][j].sign != '@' && maze[i-1][j].sign != 'S' && maze[i-1][j].sign != 'X' && maze[i+1][j].sign != '@'&& maze[i+1][j].sign != 'S' && maze[i-1][j].sign != maze[i+1][j].sign )
				{
					maze[i][j].sign = '@';
					numberOfCrossroads++;
				}
				else if(maze[i][j-1].sign != '@' && maze[i][j-1].sign != 'S' && maze[i][j-1].sign != 'X' && maze[i][j+1].sign != '@' && maze[i][j+1].sign != 'S' && maze[i][j-1].sign != maze[i][j+1].sign )
				{
					maze[i][j].sign = '@';
					numberOfCrossroads++;
				}
				else if(isDeadEnd(i,j))
				{
					maze[i][j].sign = '@';
					numberOfCrossroads++;
				}
				
				//getchar();
			
			}
    }

  }	
	show(cout);
	
getchar();
return Maze::numberOfCrossroads;
}
vector<Field> getOnePath(int y, int x)
{
	vector<Field> fd;
  if(maze[y][x-1].sign != '#')
	{
		fd.push_back(maze[y][x-1]);
	}
  if(maze[y][x+1].sign != '#')
	{
		fd.push_back(maze[y][x+1]);
	}
  if(maze[y-1][x].sign != '#')
	{
		fd.push_back(maze[y-1][x]);
	}
  if(maze[y+1][x].sign != '#')
	{
		fd.push_back(maze[y+1][x]);
	}	

	return fd;
}

bool Maze::isCrossroad(int y,int x)
{
	if(maze[y][x].sign == '@' || maze[y][x].sign == 'S' || maze[y][x].sign == 'X')
		return true;
return false;
}

void createGraph(int nrOfCrossroads);



bool Maze::isReachable(int starty, int startx, int destinationy, int destinationx)
{
	if(DEBUG)cout << "isReachable starty: " << starty << " startx: " << startx << " destinationy: " <<destinationy << " destinationx: " << destinationx <<"\n";
	if(starty - destinationy == 0) //sameRow
	{
		for(int i=startx+1; i <destinationx+1; i++)
		{
			if(DEBUG)cout <<"isReachable the same row\n";
			if(isCrossroad(starty, i)&& i == destinationx) return true;
			if(maze[starty][i].sign =='#') return false;
		} 
	}

	if(startx - destinationx == 0) //sameColumn
	{
		for(int i=starty+1; i <destinationy+1; i++)
		{
			if(DEBUG)cout <<"isReachable the same column\n";
			if(isCrossroad(i, startx) && i == destinationy ) return true;
			if(maze[i][startx].sign =='#') return false;
		} 
	}


return false;
}

void Maze::createGraph(int nrOfCrossroads)
{
//Prepare graph
	int ** graph;
	graph = new int * [nrOfCrossroads];
	for(int i = 0; i <nrOfCrossroads; ++i)
	{
		graph[i] = new int[nrOfCrossroads];
	}

	for(int j = 0; j <nrOfCrossroads; ++j)
	{				
		for(int i = 0; i <nrOfCrossroads; ++i)
		{
			graph[j][i] = 0;
		}
	}

//Collect all crossroads
	vector<Field*> crossroads;
	for(int i =1;i<height;i++)
	{
		for(int j =1;j<width;j++)
		{
			if(isCrossroad(i,j))
			{
				crossroads.push_back(&maze[i][j]);
			}
		}
	}
	if(nrOfCrossroads != crossroads.size()) cout <<"ERROR!!!\n";
	//Find two nearest crossroads in the same row 
	for(int j =0; j <nrOfCrossroads-1; j++)
	{
		for(int i =j+1; i <nrOfCrossroads; i++)
		{
			if(crossroads[j]->y == crossroads[i]->y)
			{
				//isReachable
				if(DEBUG)cout <<"the same row\n";	
				if(DEBUG)cout <<"crossroads[j].y: " << crossroads[j]->y << 	" crossroads[j].x: "<< crossroads[j]->x << "\n";
				if(DEBUG)cout <<"crossroads[i].y: " << crossroads[i]->y << 	" crossroads[i].x: "<< crossroads[i]->x << "\n";
				if(DEBUG)cout << "isReachable: " << isReachable(crossroads[j]->y, crossroads[j]->x , crossroads[i]->y, crossroads[i]->x) <<"\n";
				if(isReachable(crossroads[j]->y, crossroads[j]->x , crossroads[i]->y, crossroads[i]->x))
				{
					graph[i][j] = crossroads[i]->x - crossroads[j]->x;
					graph[j][i] = crossroads[i]->x - crossroads[j]->x;
				}
			}
			if(crossroads[j]->x == crossroads[i]->x)
			{
				//isReachable
				if(DEBUG)cout <<"the same column\n";	
				if(DEBUG)cout <<"crossroads[j].y: " << crossroads[j]->y << 	" crossroads[j].x: "<< crossroads[j]->x << "\n";
				if(DEBUG)cout <<"crossroads[i].y: " << crossroads[i]->y << 	" crossroads[i].x: "<< crossroads[i]->x << "\n";
				if(DEBUG)cout << "isReachable: " << isReachable(crossroads[j]->y, crossroads[j]->x , crossroads[i]->y, crossroads[i]->x) <<"\n";
				if(isReachable(crossroads[j]->y, crossroads[j]->x , crossroads[i]->y, crossroads[i]->x))
				{
					graph[i][j] = crossroads[i]->y - crossroads[j]->y;
					graph[j][i] = crossroads[i]->y - crossroads[j]->y;
				}
			}
		}
	}

	for(int j = 0; j <nrOfCrossroads; j++)
	{			
		for(int i = 0; i <nrOfCrossroads; i++)
		{
			cout << graph[j][i] << " ";
		}cout <<"\n";
	}
}


