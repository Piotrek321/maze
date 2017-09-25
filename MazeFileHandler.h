#ifndef MazeFileHandler_H_
#define MazeFileHandler_H_
#include <vector>
#include <iostream>
#include "maze.h"
#include "MazeScreenManager.h"

using namespace std;
class Maze;
struct Field;

class MazeFileHandler
{

public:
	Maze getMazeFromFile(std::string fileName);

	static void printMazeToFile(const vector<vector<Field>> &maze);

};

#endif
