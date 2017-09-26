#ifndef Maze_H_
#define Maze_H_
struct Field
{
  int x,y;
  char sign;
  bool visited = false;
  bool wall = false;
};

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdio>

#include <functional>
#include <unistd.h>
#include <algorithm>

#include <fstream>
#include <sstream>
#include <list>
#include "MazeScreenManager.h"
#include "MazeFileHandler.h"
using namespace std;
#define DEBUG 0

struct FieldInGraph
{
  int value, x, y;
  bool visited = false;
};

enum Dirs{LEFT=0 , RIGHT, UP, DOWN};
class Maze
{
public:
	void createMaze();
	Maze(int height_, int width_) : height(height_), width(width_){};
  Maze(){};
	int startx, starty, endx, endy;
	vector <vector<Field>> maze;
private: 
	void makeMove( int y, int x);

	void fillMaze();

	bool stuck();

	vector  <function<void()>> lookAround();

	int availableFields();

	void chooseMove();

	void clearStack(stack<Field> _stck);

	void goBack();

	bool isDeadEnd(int y, int x);











	int myx,myy;



	stack<Field> stck;
	int height;
	int width ;

	int startRow, endRow;

};

#endif
/*
	vector<Field> getOnePath(int y, int x);
vector<Field> Maze::getOnePath(int y, int x)
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
}*/
