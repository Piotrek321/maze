#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
#include <list>
#include <queue>
#include <limits>
#define EDGE_VALUE 6

using namespace std;

class CompareDist
{
public:
    bool operator()(pair<int,int> n1,pair<int,int> n2) {
        return n1.second>n2.second;
    }
};

class Graph
{
  public:
    Graph(int size): m_size(size), m_edgeValue(EDGE_VALUE){};//było size+1
    void init();
    void printGraph();
    void addEdge(int vertex1, int vertex2,int distance, bool uniDirected=1);
    void printDistance(int startIndex);
    void Dijkstra(int startIndex);
    void addEdgeToMatrix(int vertex1, int vertex2, int distance, bool uniDirected=1);
    void tranformAdjacencyMatrixIntoAdjacencyList();

  private:
    int m_size;
    int m_edgeValue;
    std::priority_queue<pair<int,int>, vector<pair<int,int>>,CompareDist> distQueue;
    vector<list<pair<int,int>>> adjacency_list;
    vector<vector<int>> adjacency_matrix;
    vector<int> distancesVector;

};

void Graph::init()
{
 distancesVector = vector<int> (m_size, std::numeric_limits<int>::max());

 adjacency_list.resize(m_size);
 adjacency_matrix = std::vector<std::vector<int>> (m_size, std::vector<int>(m_size, -1));
}

void Graph::printGraph()
{

  for(int index=1;index < adjacency_list.size(); index++)
  {
    cout <<"Adjacency_list\n";
    cout <<"index: " << index << "\n";
    for(auto x: adjacency_list[index])
    {
      cout << x.first << " ";
    }
    cout << "\n";
    for(auto x: adjacency_list[index])
    {
      cout << x.second << " ";
    }
    cout <<"\n";
  }
  cout <<"Adjacency_matrix\n";
  for(int index=1;index < adjacency_matrix.size(); index++)
  {
    for(int x=1;x < adjacency_matrix[index].size(); x++)
    {
      cout << adjacency_matrix[index][x] << " ";
    }
    cout <<"\n";
  }
}

void Graph::addEdgeToMatrix(int vertex1, int vertex2, int distance, bool uniDirected)
{
  if(adjacency_matrix[vertex1][vertex2] > distance || adjacency_matrix[vertex1][vertex2]==-1)
  {
    adjacency_matrix[vertex1][vertex2] = distance;
    if(uniDirected) adjacency_matrix[vertex2][vertex1] = distance;
  }
}

void Graph::Dijkstra(int startIndex)
{
  distancesVector[startIndex] = 0;
  distancesVector[0] = 0;
  distQueue.push(make_pair(startIndex, 0));
  while(!distQueue.empty())
  {
    pair<int,int> currentNode = distQueue.top();
    distQueue.pop();

    for(auto ind : adjacency_list[currentNode.first])
    {
      int dist= ind.second + distancesVector[currentNode.first];
      if(distancesVector[ind.first] >dist )
      {
        distQueue.push(ind);
        distancesVector[ind.first] = dist;
      }
    }
   }
}

void Graph::tranformAdjacencyMatrixIntoAdjacencyList()
{
  for(int index=1;index < adjacency_matrix.size(); index++)
  {
    for(int x=index+1;x < adjacency_matrix[index].size(); x++)
    {
      if(adjacency_matrix[index][x] != -1)
      {
        adjacency_list[index].push_back(make_pair(x,adjacency_matrix[index][x]));
        adjacency_list[x].push_back(make_pair(index,adjacency_matrix[index][x]));
      }
    }
  }
}
void Graph::printDistance(int startIndex)
{
  for(int s =1; s< distancesVector.size(); s++)
  {
    if(s == startIndex) continue;
    if(distancesVector[s] == 0 || distancesVector[s] == std::numeric_limits<int>::max())
      cout << "-1 ";
    else
    {
      cout << distancesVector[s]  << " ";
    }
  }
  cout << "\n";
}

int main()
{
  ios::sync_with_stdio(false);
  int testCases, nodes, edges, node1, node2, distance, index;
  cin >>testCases;
  while(testCases--)
  {
    cin >> nodes >>edges;
    Graph g(nodes);
    g.init();
    for(int i =0; i<edges; i++)
    {
      cin >>node1 >>node2 >>distance;
      g.addEdgeToMatrix(node1, node2, distance);
    }
    g.tranformAdjacencyMatrixIntoAdjacencyList();
    cin >>index;
    g.Dijkstra(index);
    g.printDistance(index);
  }
  return 0;
}
