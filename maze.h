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
	void makeMove( int y, int x);//, int &myy, int &myx
	//typedef decltype(bind(&Maze::makeMove, *this, int(), int())) makeMoveBind;



	void fillMaze();

	bool stuck();

	vector  <function<void()>> lookAround();

	int availableFields();

	void chooseMove();

	//template <typename T>
	void clearStack(stack<Field> _stck);

	void goBack();

	bool isDeadEnd(int y, int x);

	int createCrossroads();

	vector<Field> getOnePath(int y, int x);

	void findShortestWayOut(int nrOfCrossroads);
	bool isCrossroad(int y,int x);

	bool isReachable(int starty, int startx, int destinationy, int destinationx);

	void createGraph(int nrOfCrossroads);
	int myx,myy, nrOfMazes=0;
	vector<Field*> crossroads;
	stack<Field> stck;


	int height;
	int width ;

	int startRow, endRow;
	FieldInGraph ** graph;
};

#endif

