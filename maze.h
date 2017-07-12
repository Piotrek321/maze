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
struct Field
{
    int x,y;
    char sign;
    bool visited = false;
    bool wall = false;

};

enum Dirs{LEFT=0 , RIGHT, UP, DOWN};
class Maze
{

	void makeMove(int y, int x);//, int &myy, int &myx
	//typedef decltype(bind(&Maze::makeMove, *this, int(), int())) makeMoveBind;

	void clearScreen(int x, int y);

	void show(ostream& stream);

	void getMazeFromFile(std::string fileName);


	void printToFile();

	void printToScreen();

	void fillMaze();

	bool stuck();

	//vector <function<void(int,int)> lookAround();

	int availableFields();

	void chooseMove();

	template <typename T>
	void clearStack(stack<T> _stck);

	void goBack();

	bool isDeadEnd(int y, int x);

	int createCrossroads();

	vector<Field> getOnePath(int y, int x);


	bool isCrossroad(int y,int x);

	bool isReachable(int starty, int startx, int destinationy, int destinationx);

	void createGraph(int nrOfCrossroads);


};



