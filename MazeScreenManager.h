#ifndef MazeScreenManager_H_
#define MazeScreenManager_H_
#include <vector>
#include <iostream>

#include <unistd.h>
#include "maze.h" //Field

#ifdef _WIN32
	#include <windows.h>
#endif
class Maze;
using namespace std;

class MazeScreenManager
{
public:
	static void show(ostream& stream, const vector<vector<Field>> &maze);
	static void clearScreen(int x, int y);
	static void printToScreen(const vector<vector<Field>> &maze);

};

#endif
