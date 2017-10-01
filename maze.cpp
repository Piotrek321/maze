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
#include "maze.h"
using namespace std;

struct Field;

int Maze::nrOfMazes = 0;

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

void Maze::makeMove( int y, int x)
{
 if(!maze[myy+y][myx+x].wall && !maze[myy+(2*y)][myx+(2*x)].wall && !maze[myy+(2*y)][myx+(2*x)].visited  )
    {
        maze[myy][myx].sign = ' ';
        maze[myy+y][myx+x].visited = true;
        maze[myy+y][myx+x].sign = '*';
        MazeScreenManager::printToScreen(maze);
        maze[myy+y][myx+x].sign = ' ';
        myy += (2*y);
        myx += (2*x);
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
        stck.push( maze[myy][myx]);
    }
}

vector <function<void()>> Maze::lookAround()
{
    vector <function<void()>> vct;
    if(!maze[myy][myx-1].wall  && !maze[myy][myx-2].visited )
        vct.push_back(bind( &Maze::makeMove, this, 0, -1) );
    if(!maze[myy][myx+1].wall  && !maze[myy][myx+2].visited )
        vct.push_back(bind( &Maze::makeMove, this, 0, 1) );
    if(!maze[myy-1][myx].wall  && !maze[myy-2][myx].visited)
        vct.push_back(bind( &Maze::makeMove, this, -1, 0) );
    if(!maze[myy+1][myx].wall && !maze[myy+2][myx].visited )
        vct.push_back(bind( &Maze::makeMove, this, 1, 0) );

    return vct;
}


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
    vector<function<void()>> directions = lookAround();
    int dir = rand()%directions.size() ;
    function<void()> go = directions[dir];
    go();
}

//template <typename T>
void Maze::clearStack(stack<Field> _stck)
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
  MazeScreenManager::printToScreen(maze);
}

void Maze::createMaze()
{
  nrOfMazes ++;
	clearStack(stck);
  fillMaze();
  bool endGame = false;
  while(!endGame)
  {
  	if(stuck())
    {
  	  if(availableFields())
      {
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
	//endx = myx;
	//endy = myy;
  MazeScreenManager::printToScreen(maze);
	MazeFileHandler::printMazeToFile(maze, nrOfMazes);
	MazeScreenManager::show(cout, maze);

	//int nrOfCrossroads = createCrossroads() +2;

	//createGraph(nrOfCrossroads);
	//findShortestWayOut(nrOfCrossroads);
	

	getchar();
}

