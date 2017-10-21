#include "MazeAlgorithm.h"

MazeAlgorithm::MazeAlgorithm(Maze &_maze)
{
  maze = _maze.maze;
	//mazeHandler = _maze;
}

bool MazeAlgorithm::isDeadEnd(int y, int x)
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

bool MazeAlgorithm::isCrossroad(int y,int x)
{
	if(maze[y][x].sign == '@' || maze[y][x].sign == 'S' || maze[y][x].sign == 'X')
		return true;
return false;
}

void MazeAlgorithm::createCrossroads()
{
  m_nrOfCrossroads =0;
	for(int i =0;i<maze.size();i++)
  {
	  for(int j =0;j<maze[i].size();j++)
    {
			if(maze[i][j].sign == ' ')
			{
			 	if(maze[i-1][j].sign != '@' && maze[i-1][j].sign != 'S' && maze[i-1][j].sign != 'X' && maze[i+1][j].sign != '@'&& maze[i+1][j].sign != 'S' && maze[i-1][j].sign != maze[i+1][j].sign )
				{
					maze[i][j].sign = '@';
					m_nrOfCrossroads++;
				}
				else if(maze[i][j-1].sign != '@' && maze[i][j-1].sign != 'S' && maze[i][j-1].sign != 'X' && maze[i][j+1].sign != '@' && maze[i][j+1].sign != 'X' && maze[i][j+1].sign != 'S' && maze[i][j-1].sign != maze[i][j+1].sign )
				{
					maze[i][j].sign = '@';
					m_nrOfCrossroads++;
				}
				else if(isDeadEnd(i,j))
				{
					maze[i][j].sign = '@';
					m_nrOfCrossroads++;
				}
			}
    }

  }
  m_nrOfCrossroads+=2;
	MazeScreenManager::show(cout, maze);
}

bool MazeAlgorithm::isReachable(int starty, int startx, int destinationy, int destinationx)
{
	if(DEBUG)cout << "isReachable starty: " << starty << " startx: " << startx << " destinationy: " <<destinationy << " destinationx: " << destinationx <<"\n";
	if(starty - destinationy == 0) //sameRow
	{
		for(int i=startx+1; i <destinationx+1; i++)
		{
			if(DEBUG)cout <<"isReachable the same row\n";
			if(isCrossroad(starty, i) && i == destinationx) return true;
			if(isCrossroad(starty, i) && i != destinationx) return false;
			if(maze[starty][i].sign =='#') return false;
		}
	}

	if(startx - destinationx == 0) //sameColumn
	{
		for(int i=starty+1; i <destinationy+1; i++)
		{
			if(DEBUG)cout <<"isReachable the same column\n";
			if(DEBUG)cout <<"maze[i][startx]: " << maze[i][startx].sign << "\n";
			if(isCrossroad(i, startx) && i == destinationy ) return true;
			if(isCrossroad(i, startx) && i != destinationy ) return false;
			if(maze[i][startx].sign =='#') return false;
		}
	}

	return false;
}

void MazeAlgorithm::prepareEmptyGraph()
{
  FieldInGraph field;
  field.value = 0;
	for(int j = 0; j <m_nrOfCrossroads; ++j)
	{
    vector<FieldInGraph> vect;
		for(int i = 0; i <m_nrOfCrossroads; ++i)
		{
      field.x = i;
      field.y = j;
      vect.push_back(field);
		}
    graph.push_back(vect);
	}
}

void MazeAlgorithm::printGraph()
{
	for(int j = 0; j <m_nrOfCrossroads; j++)
	{
		for(int i = 0; i <m_nrOfCrossroads; i++)
		{
			cout << graph[j][i].value << " ";
		}cout <<"\n";
	}
	cout <<"EndRow: " << endRow << " startRow: " << startRow << "\n";
}

void MazeAlgorithm::collectCrossroads()
{
	int iter = 0;
  for(int i =0;i<maze.size();i++)
  {
    for(int j =0;j<maze[i].size();j++)
		{
			if(isCrossroad(i,j))
			{

				if(maze[i][j].sign == 'S') startRow = iter;
				if(maze[i][j].sign == 'X') endRow = iter;
				iter ++;
				crossroads.push_back(&maze[i][j]);
			}
		}
  }
	if(m_nrOfCrossroads != crossroads.size()) cout <<"ERROR!!!\n";
}

void MazeAlgorithm::createGraph()
{
//Prepare graph
	prepareEmptyGraph();
	collectCrossroads();
//Collect all crossroads


	//Find two nearest crossroads in the same row
	for(int j =0; j <m_nrOfCrossroads-1; j++)
	{
		for(int i =j+1; i <m_nrOfCrossroads; i++)
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
					graph[i][j].value = crossroads[i]->x - crossroads[j]->x;
					graph[j][i].value = crossroads[i]->x - crossroads[j]->x;
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
					graph[i][j].value = crossroads[i]->y - crossroads[j]->y;
					graph[j][i].value = crossroads[i]->y - crossroads[j]->y;
				}
			}
		}
	}

	printGraph();
}

void MazeAlgorithm::findShortestWayOut(int m_nrOfCrossroads)
{
  int dist=0;
	list<int> way;
	stack<FieldInGraph> moves;
	int row = startRow;
	way.push_back(startRow);
	bool deadend = true;
	int prev= startRow;

	while(deadend)
	{

		for(int i =0; i<m_nrOfCrossroads;i++)
		{
			if(DEBUG)cout <<"Row: " << row << " i: " << i << "graph[row][i].value: " << graph[row][i].value <<"\n";
			if(row == endRow)
			{
				if(DEBUG)cout <<"row == endRow\n";
				moves.push(graph[row][i]);
				deadend = false;
				break;
			}
			if(graph[row][i].value != 0 && !graph[row][i].visited)
			{
				if(i == prev)
				{
					graph[row][i].visited = true;
					if(DEBUG)cout <<"row == prev\n";

				}else
				{

					dist += graph[row][i].value;
					prev = row;
					graph[row][i].visited =true;
					graph[i][row].visited =true;
					moves.push(graph[row][i]);
					row = i;
					i =-1;
				}
			}
		}
		if(DEBUG)cout <<"Moves.size(): " << moves.size() << "\n";

		if(deadend)
		{
			if(DEBUG)cout <<"Pop\n";
			FieldInGraph move = moves.top();
			moves.pop();

			if(DEBUG)cout <<"move.y: " << move.y << " move.x: " << move.x <<"\n";
			prev = row;
			row = move.y;

			bool ok =true;
			while(ok)
			{
				for(int i =0; i<m_nrOfCrossroads;i++)
				{
					if(DEBUG)cout <<"Second Row: " << row << " i: " << i << "graph[row][i].value: " << graph[row][i].value <<  " graph[row][i].visited: " << graph[row][i].visited << "\n";
					if(graph[row][i].value != 0 && !graph[row][i].visited) ok = false;
				}
				if(ok)
				{
					move = moves.top();
					moves.pop();
					if(DEBUG)cout <<"move.y: " << move.y << " move.x: " << move.x <<"\n";
					row = move.y;

				}

			}
		}
	}
getchar();
   while (!moves.empty()) {
        std::cout<<"moves: " << moves.top().value;
				std::cout<<" y: " << moves.top().y;
				std::cout<<" x: " << moves.top().x << " \n";
        moves.pop();
    }
	cout <<"\n";
}

std::vector<std::vector<int>> MazeAlgorithm::transformCrossroadsIntoGraphOfDistances()
{
  int size = graph[0].size();
  std::vector<std::vector<int>> graphOfDistances = std::vector<std::vector<int>> (size, std::vector<int>(size, 0));

  for(int j = 0; j <size; j++)
  {
    for(int i = 0; i <size; i++)
    {
      graphOfDistances[j][i] = graph[j][i].value;
    }
  }
  return graphOfDistances;
}

/*
for(int i=0; i<size; ++i)
{
  std::transform(graph[i].begin(), graph[i].end(), graphOfDistances[0].begin(),
   [](FieldInGraph a)->int{return a.value ;});
}*/
