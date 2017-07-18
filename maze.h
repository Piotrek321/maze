#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdio>
#include <time.h>
#include <functional>
#include <unistd.h>
#include <algorithm>
#ifdef _WIN32
	#include <windows.h>
#endif
#include <fstream>
#include <sstream>
#include <list>
using namespace std;
#define DEBUG 0
struct Field
{
  int x,y;
  char sign;
  bool visited = false;
  bool wall = false;
};

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
private: 
	void makeMove( int y, int x);//, int &myy, int &myx
	//typedef decltype(bind(&Maze::makeMove, *this, int(), int())) makeMoveBind;

	void clearScreen(int x, int y);

	void show(ostream& stream);

	void getMazeFromFile(std::string fileName);


	void printToFile();

	void printToScreen();

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
	std::ofstream ofs;
	vector <vector<Field> > maze;
	int height;
	int width ;
	int startx, starty, endx, endy;
	int startRow, endRow;
	FieldInGraph ** graph;
};



