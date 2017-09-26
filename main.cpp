
#include "maze.h"

int main()
{
    int a =10;

//Maze y(15,15);
//y.getMazeFromFile("test1.txt");

    srand( time( NULL ) );
Maze x(15,15);
    while(a--)
    {
				
        x.createMaze();
		//	getchar();
			
    }

    return 0;
}
