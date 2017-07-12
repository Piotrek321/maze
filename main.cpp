
#include "maze.h"

int main()
{
    int a =1;
Maze x;

//getMazeFromFile("test1.txt");
    //srand( time( NULL ) );
    while(a--)
    {
        std::stringstream name ;
        name <<"test" <<x.nrOfMazes<< ".txt";
        x.ofs.open(name.str());

        x.clearStack(x.stck);
        x.nrOfMazes++;
        x.fillMaze();
        bool endGame = false;
        while(!endGame)
        {
            if(x.stuck())
            {
                if(x.availableFields())
                {
                    //if(DEBUG) cout <<"Myx: " << myx << " myy: " << myy  <<"\n";
                    x.goBack();
                    continue;
                }
                else
                {
                  endGame = true;
                }
            }
            if(!endGame)
            {
                x.chooseMove();
            }
        }
        x.maze[x.starty][x.startx].sign = 'S';
        x.maze[x.myy][x.myx].sign = 'X';
        x.printToScreen();
        x.printToFile();
        x.ofs.close();
				int nrOfCrossroads = x.createCrossroads() +2;
				x.show(cout);
		

			
			x.createGraph(nrOfCrossroads);
			getchar();
			
    }

    return 0;
}
