#include<iostream>
#include<vector>
#include<string>

using namespace std;

#define INF (~(0x1<<31))  //用来代表无穷大

//边的结构体
struct EdgeData {
    char start;  //边的起点
    char end;   //边的终点
    int weight; //边的权重
    EdgeData() { }
    EdgeData(char s, char e, int w)
        :start(s), end(e), weight(w) { }
};

//管理图的类
class MatrixUDG {
public:
    MatrixUDG() { }
    MatrixUDG(const vector<char>& vertexCollection,
      const vector<vector<int>>& graphMatrix);

    void dijkstra(int vertex, vector<int>& prev,
      vector<int>& minDistanceCollection);

private:
    vector<char> m_vertexCollection;
    int m_numberOfVertex;
    int m_numberOfEdge = 0;
    vector<vector<int>> m_graphMatrix;
};

MatrixUDG::MatrixUDG(const vector<char>& vertexCollection,
  const vector<vector<int>>& graphMatrix) {
    m_numberOfVertex = vertexCollection.size();
    m_vertexCollection = vertexCollection;
    m_graphMatrix = graphMatrix;
    for(int i = 0; i < m_numberOfVertex; i++) {
        for(int j = 0; j < m_numberOfVertex; j++) {
            if(i != j && m_graphMatrix[i][j] != INF) {
                m_numberOfEdge++;
            }
        }
    }
    m_numberOfEdge /= 2;
}

void MatrixUDG::dijkstra(int vertex, vector<int>& prev,
  vector<int>& minDistanceCollection) {
    int minDistance;
    int newDistance;
    int k;
    //flags[i]=1表示“顶点vertex"到"顶点i"的最短路径已经成功获取
    vector<int> flags(m_numberOfVertex, 0);
    for(int i = 0; i < m_numberOfVertex; i++) {
        minDistanceCollection[i] = m_graphMatrix[vertex][i];
    }
    flags[vertex] = 1;
    minDistanceCollection[vertex] = 0;
    //遍历m_numberOfVertex - 1次，每次找出一个顶点的最短路径
    for(int i = 1; i < m_numberOfVertex; i++) {
        minDistance = INF;
        for(int j = 0; j < m_numberOfVertex; j++) {
            if(flags[j] == 0 && minDistanceCollection[j] < minDistance) {
                minDistance = minDistanceCollection[j];
                k = j;
            }
        }
        flags[k] = 1;
        for(int j = 0; j < m_numberOfVertex; j++) {
            newDistance = (m_graphMatrix[k][j] == INF ? INF : (minDistance + m_graphMatrix[k][j]));
            if(flags[j] == 0 && (newDistance < minDistanceCollection[j])) {
                minDistanceCollection[j] = newDistance;
                prev[j] = k;
            }
        }
    }
    //打印dijkstra最短路径的结果
    cout<<"dijkstra("<<m_vertexCollection[vertex]<<"):  "<<endl;
    for(int i = 0; i < m_numberOfVertex; i++) {
        cout<<"  shortest("<<m_vertexCollection[vertex]
            <<", "<<m_vertexCollection[i]<<")="<<minDistanceCollection[i]<<endl;
    }
}

//测试dijkstra算法
void test_dijkstra() {
    //图的顶点
    vector<char> vertexCollection = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G'
    };
    //图的邻接矩阵
    vector<vector<int>> graphMatrix = {
             /*A*//*B*//*C*//*D*//*E*//*F*//*G*/
      /*A*/ {   0,  12, INF, INF, INF,  16,  14},
      /*B*/ {  12,   0,  10, INF, INF,   7, INF},
      /*C*/ { INF,  10,   0,   3,   5,   6, INF},
      /*D*/ { INF, INF,   3,   0,   4, INF, INF},
      /*E*/ { INF, INF,   5,   4,   0,   2,   8},
      /*F*/ {  16,   7,   6, INF,   2,   0,   9},
      /*G*/ {  14, INF, INF, INF,   8,   9,   0}
    };
    int numberOfVertex = vertexCollection.size();
    vector<int> prev(numberOfVertex, 0);
    vector<int> minDistanceCollection(numberOfVertex, 0);
    MatrixUDG* graph = new MatrixUDG(vertexCollection, graphMatrix);
    graph->dijkstra(3, prev, minDistanceCollection);
}

int main()
{
    test_dijkstra();
}

