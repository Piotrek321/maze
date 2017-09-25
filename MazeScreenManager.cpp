#include "MazeScreenManager.h"

void MazeScreenManager::show(ostream& stream, const vector<vector<Field>> &maze)
{
  clearScreen(0,0);
  for(int i =0;i<maze.size();i++)
  {
    for(int j =0;j<maze[i].size();j++)
    {
      stream<< maze[i][j].sign;
    }
    stream << "\n";
  }
#ifdef _WIN32
  Sleep(1);
#else
  usleep(50000);
#endif

}

void MazeScreenManager::clearScreen(int x, int y)
{
#ifdef _WIN32
		static const HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    std::cout.flush();
    COORD coord = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hOut, coord);
#else
    cout << string( 50, '\n' );
#endif
}


void MazeScreenManager::printToScreen(const vector<vector<Field>> &maze)
{
    cout << "\n\n\n";
    show(cout, maze);
}
