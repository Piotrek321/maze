# maze#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <stack>
#include <cstdio>
#include <conio.h>
using namespace std;

struct Field
{
    int x,y;
    char sign;
    bool visited = false;
    bool wall = false;

};
vector <vector<Field>> maze;
int height =10;
int width =12;

void show()
{
    for(int i =0;i<height;i++)
    {
        for(int j =0;j<width;j++)
        {
            cout << maze[i][j].sign;

        }
        cout << "\n";
    }
}
int myx,myy;

bool stuck()
{cout <<"STC\n";
    if(maze[myy][myx-1].wall || maze[myy][myx-1].visited )
    { cout << "1 if myx " <<  myx << " myy " << myy << " wall: " << maze[myy][myx-1].wall << " visited: " << maze[myy][myx-1].visited << "\n";
        if(maze[myy][myx+1].wall || maze[myy][myx+1].visited )
    { cout << "2 if myx " <<  myx << " myy " << myy << " wall: " << maze[myy][myx+1].wall << " visited: " << maze[myy][myx+1].visited << "\n";
            if(maze[myy-1][myx].wall || maze[myy-1][myx].visited )
    { cout << "3 if myx " <<  myx << " myy " << myy << " wall: " << maze[myy-1][myx].wall << " visited: " << maze[myy-1][myx].visited << "\n";
                if(maze[myy+1][myx].wall || maze[myy+1][myx].visited )
    { cout << "4 if myx " <<  myx << " myy " << myy << " wall: " << maze[myy+1][myx].wall << " visited: " << maze[myy+1][myx].visited << "\n";
                    return 1;
                }
            }

        }
    }
    return 0;
}

int main()
{
for(int i =0;i<height;i++)
{vector<Field> ve;

    for(int j =0;j<width;j++)
    {
        Field f;
        f.sign= '#';
        f.x = j;
        f.y = i;
        if(i == 0 || j == 0 || i == height-1 || j == width-1)
        {
            f.wall = true;
        }
        ve.push_back(f);
        //maze[j][i].sign

    }
    std::cout <<"size: " << ve.size() <<"\n";
    maze.push_back(ve);
}
    std::cout <<"maze: " << maze.size() <<"\n";

//show();
stack<Field> stck;
maze[1][1].sign = '*';
stck.push(maze[1][1]);
enum Dirs{LEFT , RIGHT, UP, DOWN};
myx = 1;
myy = 1;

while(1)
{bool continu = false;
    int dir = rand()%4;
    if(stuck())
    {getch();
        cout <<"STUCK!\n";
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = ' ';
        stck.pop();
        Field tmp  = stck.top();
        myx= tmp.x;
        myy = tmp.y;
        maze[myy][myx].sign = '*';
        continu  = true;
            show();

       //continue;


    }

    cout <<"AFTER STUCK\n";

 if(continu)
    {
        continue;
    }
    switch(dir)
    {
    case LEFT:
    if(!maze[myy][myx-1].wall && !maze[myy][myx-1].visited )
    {   maze[myy][myx].visited = true;
        maze[myy][myx].sign = ' ';
        myx --;
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
        stck.push( maze[myy][myx]);
        cout <<"LEFT, not wall\n";
    }else
    {
        cout <<"LEFT,wall\n";
    }
        break;
    case RIGHT:
    if(!maze[myy][myx+1].wall && !maze[myy][myx+1].visited)
    {maze[myy][myx].visited = true;
        maze[myy][myx].sign = ' ';
        myx ++;
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
                stck.push( maze[myy][myx]);

        cout <<"RIGHT, not wall\n";
    }else
    {
        cout <<"RIGHT,wall\n";
    }
        break;

    case UP:
    if(!maze[myy-1][myx].wall && !maze[myy-1][myx].visited)
    {maze[myy][myx].visited = true;
        maze[myy][myx].sign = ' ';
        myy--;
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
                stck.push( maze[myy][myx]);

        cout <<"UP, not wall\n";
    }else
    {
        cout <<"UP,wall\n";
    }
        break;

    case DOWN:
    if(!maze[myy+1][myx].wall && !maze[myy+1][myx].visited)
    {maze[myy][myx].visited = true;
        maze[myy][myx].sign = ' ';
        myy ++;
        maze[myy][myx].visited = true;
        maze[myy][myx].sign = '*';
            stck.push( maze[myy][myx]);

        cout <<"DOWN, not wall\n";
    }else
    {
        cout <<"DOWN,wall\n";
    }
        break;

    default:
        cout <<"ERROR\n";
    }

    show();
    getch();

}



    return 0;
}
