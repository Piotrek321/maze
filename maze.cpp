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

struct Field;
vector <vector<Field> > maze;
int height =15;
int width =15;
int startx, starty;
#ifdef _WIN32
	int sleepTime= 10;
#else
	int sleepTime= 50000;
#endif
int myx,myy, nrOfMazes;
stack<Field> stck;
std::ofstream ofs;
enum Dirs{LEFT=0 , RIGHT, UP, DOWN};

void makeMove(int y, int x);//, int &myy, int &myx

typedef decltype(bind(makeMove, int(), int())) makeMoveBind;

void clearScreen(int x, int y)
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

struct Field
{
    int x,y;
    char sign;
    bool visited = false;
    bool wall = false;

};


void show(ostream& stream)
{
    clearScreen(0,0);
    for(int i =0;i<height;i++)
    {
        for(int j =0;j<width;j++)
        {
          stream<< maze[i][j].sign;
        }
        stream << "\n";
    }
#ifdef _WIN32
    Sleep(sleepTime);
#else
		usleep(sleepTime);
#endif

}

void getMazeFromFile(std::string fileName)
{
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
						startx = x;
						starty = y; 
						break; 
					case 'X':
						field.sign= 'X';
						field.wall = false; 
						break; 
				}
				ve.push_back(field);
				x++;
			}
 		maze.push_back(ve);
	x=0;
	y++;
	}

show(cout);
}

void printToFile()
{
    show(ofs);
}
void printToScreen()
{
    cout << "\n\n\n";
    show(cout);
}

void fillMaze()
{
    maze.clear();
    for(int i =0;i<height;i++)
    {
        vector<Field> ve;
        for(int j =0;j<width;j++)
        {
            Field field;
            field.sign= '#';
            field.x = j;
            field.y = i;
            if(i == 0 || j == 0 || i == height-1 || j == width-1)
            {
                field.wall = true;
            }
            ve.push_back(field);
        }
        maze.push_back(ve);
    }

    myx = (rand()%(width-1)) +1 ;
    myy = (rand()%(height-1)) +1;
    startx = (myx %2) ? myx : --myx;
    starty = (myy %2) ? myy : --myy;

    myx = startx;
    myy = starty;
    maze[myy][myx].sign = 'S';
    maze[myy][myx].visited= true;
    stck.push(maze[myy][myx]);

}


bool stuck()
{
    if(myx-2 <= 0  || maze[myy][myx-2].visited )
    {
        if(myx+2 >= width  || maze[myy][myx+2].visited )
        {
            if(myy-2 <= 0  || maze[myy-2][myx].visited)
            {
               if(myy+2 >= height || maze[myy+2][myx].visited )
               {
                 return 1;
               }
            }
        }
    }
    return 0;
}

void makeMove(int y, int x)//, int &myy, int &myx
{
 if(!maze[myy+y][myx+x].wall && !maze[myy+(2*y)][myx+(2*x)].wall && !maze[myy+(2*y)][myx+(2*x)].visited  )
    {
        maze[myy][myx].sign = ' ';
        maze[myy+y][myx+x].visited = true;
        maze[myy+y][myx+x].sign = '*';
        printToScreen();
        maze[myy+y][myx+x].sign = ' ';
        myy += (2*y);
        myx += (2*x);
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
        stck.push( maze[myy][myx]);
    }
}

vector <makeMoveBind> lookAround()
{
    vector <makeMoveBind> vct;
    if(!maze[myy][myx-1].wall  && !maze[myy][myx-2].visited )
        vct.push_back(bind(makeMove, 0, -1) );
    if(!maze[myy][myx+1].wall  && !maze[myy][myx+2].visited )
       vct.push_back(bind(makeMove, 0, 1) );
    if(!maze[myy-1][myx].wall  && !maze[myy-2][myx].visited)
        vct.push_back(bind(makeMove, -1, 0) );
    if(!maze[myy+1][myx].wall && !maze[myy+2][myx].visited )
        vct.push_back(bind(makeMove, 1, 0) );
    return vct;
}


int availableFields()
{
    int ctr=0;
     for(int i =0;i<height;i++)
    {
        for(int j =0;j<width;j++)
        {
            if(i == 0 || j == 0 || i == height-1 || j == width-1){}
            else
            {
                if(i%2 != 0 && j%2 != 0 && maze[i][j].sign == '#')
                {
                    ctr ++;
                }
            }
        }
    }
    return ctr;
}

void chooseMove()
{
    vector<makeMoveBind> directions = lookAround();
    int dir = rand()%directions.size() ;
    makeMoveBind go = directions[dir];
    go();
}

template <typename T>
void clearStack(stack<T> _stck)
{
	while (!_stck.empty())
  {
	   _stck.pop();
  }
}
void goBack()
{
  maze[myy][myx].sign = ' ';
  stck.pop();
  Field tmp  = stck.top();
  myx= tmp.x;
  myy = tmp.y;
  maze[myy][myx].sign = '*';
  printToScreen();
}
bool isDeadEnd(int y, int x)
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
int createCrossroads()
{    
int numberOfCrossroads =0;
	for(int i =0;i<height;i++)
  {
	  for(int j =0;j<width;j++)
    {
			if(maze[i][j].sign == ' ')
			{			
			 	if(maze[i-1][j].sign != '@' && maze[i-1][j].sign != 'S' && maze[i-1][j].sign != 'X' && maze[i+1][j].sign != '@'&& maze[i+1][j].sign != 'S' && maze[i-1][j].sign != maze[i+1][j].sign )
				{
					maze[i][j].sign = '@';
					numberOfCrossroads++;
				}
				else if(maze[i][j-1].sign != '@' && maze[i][j-1].sign != 'S' && maze[i][j-1].sign != 'X' && maze[i][j+1].sign != '@' && maze[i][j+1].sign != 'S' && maze[i][j-1].sign != maze[i][j+1].sign )
				{
					maze[i][j].sign = '@';
					numberOfCrossroads++;
				}
				else if(isDeadEnd(i,j))
				{
					maze[i][j].sign = '@';
					numberOfCrossroads++;
				}
				
				//getchar();
			
			}
    }

  }	
	show(cout);
	
getchar();
return numberOfCrossroads;
}
vector<Field> getOnePath(int y, int x)
{
	vector<Field> fd;
  if(maze[y][x-1].sign != '#')
	{
		fd.push_back(maze[y][x-1]);
	}
  if(maze[y][x+1].sign != '#')
	{
		fd.push_back(maze[y][x+1]);
	}
  if(maze[y-1][x].sign != '#')
	{
		fd.push_back(maze[y-1][x]);
	}
  if(maze[y+1][x].sign != '#')
	{
		fd.push_back(maze[y+1][x]);
	}	

	return fd;
}

bool isCrossroad(int y,int x)
{
	if(maze[y][x].sign == '@' || maze[y][x].sign == 'S' || maze[y][x].sign == 'X')
		return true;
return false;
}

void createGraph(int nrOfCrossroads);


int main()
{
    int a =1;
//getMazeFromFile("test1.txt");
    //srand( time( NULL ) );
    while(a--)
    {
        std::stringstream name ;
        name <<"test" <<nrOfMazes<< ".txt";
        ofs.open(name.str());

        clearStack(stck);
        nrOfMazes++;
        fillMaze();
        bool endGame = false;
        while(!endGame)
        {
            if(stuck())
            {
                if(availableFields())
                {
                    //if(DEBUG) cout <<"Myx: " << myx << " myy: " << myy  <<"\n";
                    goBack();
                    continue;
                }
                else
                {
                  endGame = true;
                }
            }
            if(!endGame)
            {
                chooseMove();
            }
        }
        maze[starty][startx].sign = 'S';
        maze[myy][myx].sign = 'X';
        printToScreen();
        printToFile();
        ofs.close();
				int nrOfCrossroads = createCrossroads() +2;
				show(cout);
		

			
			createGraph(nrOfCrossroads);
			getchar();
			
    }

    return 0;
}

bool isReachable(int starty, int startx, int destinationy, int destinationx)
{
	if(DEBUG)cout << "isReachable starty: " << starty << " startx: " << startx << " destinationy: " <<destinationy << " destinationx: " << destinationx <<"\n";
	if(starty - destinationy == 0) //sameRow
	{
		for(int i=startx+1; i <destinationx+1; i++)
		{
			if(DEBUG)cout <<"isReachable the same row\n";
			if(isCrossroad(starty, i)&& i == destinationx) return true;
			if(maze[starty][i].sign =='#') return false;
		} 
	}

	if(startx - destinationx == 0) //sameColumn
	{
		for(int i=starty+1; i <destinationy+1; i++)
		{
			if(DEBUG)cout <<"isReachable the same column\n";
			if(isCrossroad(i, startx) && i == destinationy ) return true;
			if(maze[i][startx].sign =='#') return false;
		} 
	}


return false;
}

void createGraph(int nrOfCrossroads)
{
//Prepare graph
	int ** graph;
	graph = new int * [nrOfCrossroads];
	for(int i = 0; i <nrOfCrossroads; ++i)
	{
		graph[i] = new int[nrOfCrossroads];
	}

	for(int j = 0; j <nrOfCrossroads; ++j)
	{				
		for(int i = 0; i <nrOfCrossroads; ++i)
		{
			graph[j][i] = 0;
		}
	}

//Collect all crossroads
	vector<Field*> crossroads;
	for(int i =1;i<height;i++)
	{
		for(int j =1;j<width;j++)
		{
			if(isCrossroad(i,j))
			{
				crossroads.push_back(&maze[i][j]);
			}
		}
	}
	if(nrOfCrossroads != crossroads.size()) cout <<"ERROR!!!\n";
	//Find two nearest crossroads in the same row 
	for(int j =0; j <nrOfCrossroads-1; j++)
	{
		for(int i =j+1; i <nrOfCrossroads; i++)
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
					graph[i][j] = crossroads[i]->x - crossroads[j]->x;
					graph[j][i] = crossroads[i]->x - crossroads[j]->x;
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
					graph[i][j] = crossroads[i]->y - crossroads[j]->y;
					graph[j][i] = crossroads[i]->y - crossroads[j]->y;
				}
			}
		}
	}

	for(int j = 0; j <nrOfCrossroads; j++)
	{			
		for(int i = 0; i <nrOfCrossroads; i++)
		{
			cout << graph[j][i] << " ";
		}cout <<"\n";
	}
}
/*
void createGraph(int nrOfCrossroads)
{
cout << "nrOfCrossroads: " << nrOfCrossroads << "\n";
	int ** graph;
	graph = new int * [nrOfCrossroads];
	for(int i = 0; i <nrOfCrossroads; ++i)
	{
		graph[i] = new int[nrOfCrossroads];
	}
				

	for(int j = 0; j <nrOfCrossroads; ++j)
	{				
		for(int i = 0; i <nrOfCrossroads; ++i)
		{
			graph[j][i] = 0;
		}
	}


	Field * tab;
	tab = new Field [nrOfCrossroads];
	int crs=0;
	int ctr =0;

	int distance=-1;
	bool found =false;

	for(int i =0;i<height;i++)//TODO 1
	{
		for(int j =0;j<width;j++)//TODO 1
		{
			int z =1;
				bool petla =true;
				int distance2=0;
			if(maze[i][j].sign == '@' || maze[i][j].sign == 'X' || maze[i][j].sign == 'S')
			{ctr++;
				cout <<"First if.maze[i][j].sign: " << maze[i][j].sign << " i: "<<i << " j: " << j<< " crs: " << crs <<"\n";
				//CHeckDOwn
				  tab[crs] = maze[i][j];
					while(petla)
					{cout <<"Petla\n";
						distance2++;
						if(maze[i-z][j].sign == '#') 
						{							cout << "i-z: " << i-z << " j: " << j << " sign: " << "#" <<"\n";
							//cout <<"while if break\n";
							petla = false;
						}else if(maze[i-z][j].sign == '@' || maze[i-z][j].sign == 'X' || maze[i-z][j].sign == 'S')
						{cout << "while else if crs: " << crs << " distance2: " << distance2 << "\n";
							//Find crs for this corssroad
							cout << "i-z: " << i-z << " j: " << j << " \n";
							int iter = 0;
							for(int k =0; k <nrOfCrossroads; k++)
							{
								if(tab[k].x == j && tab[k].y == (i-z))
								{iter =k;
									cout << "For if i-z: " << i-z << " j: " << j << " iter: "<< iter << " \n";
								}
							}
							cout <<"ITER: " <<iter << " crs: " << crs << "\n";
							graph[crs][iter] = distance2;
							distance2 = 0;
								for(int j = 0; j <nrOfCrossroads; j++)
								{	//cout <<"J:" << j << " ";			
									for(int i = 0; i <nrOfCrossroads; i++)
									{
										cout << graph[j][i] << " ";
									}cout <<"\n";
								}
								getchar();
							petla = false;
						}else if(maze[i-z][j].sign == ' ')
						{
							cout << "i-z: " << i-z << " j: " << j << " sign: " << " " <<"\n";
					
						}
						z++;
					}
				
				cout <<"Po petli\n\n";
				if(found)	
				{cout <<"Add to graph. Crs: " << crs << " distance: " << distance << "\n";
					graph[crs-1][crs] = distance;
					distance = 0;
					found =false;
					
				}else
				{//cout <<"found =true\n";
					found =true;
				}
				
				crs++;
				
			}
			else if(maze[i][j].sign == '#')
			{cout <<"'#' found =false;\n";
				found =false;
				distance = 0;
			}else if(maze[i][j].sign == ' ')
					distance++;
		}
	}
	for(int j = 0; j <nrOfCrossroads; j++)
	{	//cout <<"J:" << j << " ";			
		for(int i = 0; i <nrOfCrossroads; i++)
		{
			cout << graph[j][i] << " ";
		}cout <<"\n";
	}
					
	for(int i = 0; i <nrOfCrossroads; ++i)
	{
		delete graph[i];
	}
	delete graph;
}*/

