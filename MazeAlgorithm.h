#ifndef MazeAlgorithm_H_
#define MazeAlgorithm_H_
#include "maze.h"

struct FieldInGraph
{
  int value, x, y;
  bool visited = false;
};


class MazeAlgorithm
{
public:
  MazeAlgorithm(Maze &_maze);
	void findShortestWayOut(int nrOfCrossroads);
	void createGraph();//int nrOfCrossroads);
	bool isReachable(int starty, int startx, int destinationy, int destinationx);
	int createCrossroads();
	bool isDeadEnd(int y, int x);
	bool isCrossroad(int y,int x);
  ~MazeAlgorithm()
	{
		for(int i = 0; i <m_nrOfCrossroads; ++i)
		{
			delete graph[i];
		}
		delete graph;
		crossroads.clear();
	}
  int m_nrOfCrossroads;
  Maze mazeHandler;
	stack<Field> stck;
	FieldInGraph ** graph;
	vector<Field*> crossroads;
	vector <vector<Field>> maze;
  int startRow,endRow;
};
#endif
