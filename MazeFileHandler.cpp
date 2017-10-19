#include "MazeFileHandler.h"

void MazeFileHandler::printMazeToFile(const vector<vector<Field>> &maze, int nrOfMazes)
{
	std::ofstream ofs;
	std::stringstream name ;
  name <<"test" << nrOfMazes << ".txt";
  ofs.open(name.str());
  MazeScreenManager::show(ofs, maze);
  ofs.close();
}


Maze MazeFileHandler::getMazeFromFile(std::string fileName)
{
  Maze maze;
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
						maze.startx = x;
						maze.starty = y;
						break;
					case 'X':
						field.sign= 'X';
						field.wall = false;
						break;
				}
				ve.push_back(field);
				x++;
			}
 		maze.maze.push_back(ve);
		x=0;
		y++;
	}

	MazeScreenManager::printToScreen(maze.maze);
  return maze;
}
