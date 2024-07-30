#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<iomanip>

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

    //遍历图
    void dfsTraversal(); //深度优先遍历图
    void dfsTraversalRecursive(int vertexIndex, vector<int>& visitedVertexIndexs);
    int getFirstAdjacentVertexIndex(int vertexIndex);
    int getNextAdjacentVertexIndex(int vertexIndex, int adjacentVertexIndex);
    void bfsTraversal(); //广度优先遍历图

    void dijkstra(int startVertexIndex, vector<int>& prev,
      vector<int>& minDistanceCollection);
    void prim(int vertexIndex);
    void kruskal();

    void printGraph();
    int getVertexIndex(char vertex);
    EdgeData* getAllEdges();
    void sortAllEdges(EdgeData* edges);
    int getVertexEnd(vector<int>& vends, int vertexIndex);
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

void MatrixUDG::dfsTraversal() {
    vector<int> visitedVertexIndexs(m_numberOfVertex, 0);
    cout<<"DFS: ";
    for(int vertexIndex = 0; vertexIndex < m_numberOfVertex; vertexIndex++) {
        if(!visitedVertexIndexs[vertexIndex]) {
            dfsTraversalRecursive(vertexIndex, visitedVertexIndexs);
        }
    }
    cout<<endl;
}

void MatrixUDG::dfsTraversalRecursive(int vertexIndex,
  vector<int>& visitedVertexIndexs) {
    int adjacentVertexIndex;
    visitedVertexIndexs[vertexIndex] = 1;
    cout<<m_vertexCollection[vertexIndex]<<" ";
    adjacentVertexIndex = getFirstAdjacentVertexIndex(vertexIndex);
    while(adjacentVertexIndex >= 0) {
        if(!visitedVertexIndexs[adjacentVertexIndex]) {
            dfsTraversalRecursive(adjacentVertexIndex, visitedVertexIndexs);
        }
        adjacentVertexIndex = getNextAdjacentVertexIndex(vertexIndex, adjacentVertexIndex);
    }
}

int MatrixUDG::getFirstAdjacentVertexIndex(int vertexIndex) {
    if(vertexIndex < 0 || vertexIndex >= m_numberOfVertex) {
        return -1;
    }
    for(int i = 0; i < m_numberOfVertex; i++) {
        if(m_graphMatrix[vertexIndex][i] != 0 &&
           m_graphMatrix[vertexIndex][i] != INF) {
            return i;
        }
    }
    return -1;
}

int MatrixUDG::getNextAdjacentVertexIndex(int vertexIndex,
  int adjacentVertexIndex) {
    if(vertexIndex < 0 || vertexIndex >= m_numberOfVertex ||
       adjacentVertexIndex < 0 || adjacentVertexIndex >= m_numberOfVertex) {
        return -1;
    }
    for(int i = adjacentVertexIndex + 1; i < m_numberOfVertex; i++) {
        if(m_graphMatrix[vertexIndex][i] != 0 &&
           m_graphMatrix[vertexIndex][i] != INF) {
            return i;
        }
    }
    return -1;
}

void MatrixUDG::bfsTraversal() {
    queue<int> vertexIndexsQueue;
    vector<int> visitedVertexIndexs(m_numberOfVertex, 0);
    cout<<"BFS: ";
    vertexIndexsQueue.push(0);
    while(! vertexIndexsQueue.empty()) {
        int size = vertexIndexsQueue.size();
        for(int i = 0; i < size; i++) {
            int vertexIndex = vertexIndexsQueue.front();
            vertexIndexsQueue.pop();
            if(!visitedVertexIndexs[vertexIndex]) {
                visitedVertexIndexs[vertexIndex] = 1;
                cout<<m_vertexCollection[vertexIndex]<<" ";
            }
            int adjacentVertexIndex = getFirstAdjacentVertexIndex(vertexIndex);
            while(adjacentVertexIndex >= 0) {
                if(!visitedVertexIndexs[adjacentVertexIndex]) {
                    visitedVertexIndexs[adjacentVertexIndex] = 1;
                    cout<<m_vertexCollection[adjacentVertexIndex]<<" ";
                    vertexIndexsQueue.push(adjacentVertexIndex);
                }
                adjacentVertexIndex = getNextAdjacentVertexIndex(vertexIndex, adjacentVertexIndex);
            }
        }
    }
    cout<<endl;
}

void MatrixUDG::dijkstra(int startVertexIndex, vector<int>& prev,
  vector<int>& minDistanceCollection) {
    int minDistance;
    int newDistance;
    int minDistanceIndex;
    vector<int> flags(m_numberOfVertex, 0);
    for(int i = 0; i < m_numberOfVertex; i++) {
        minDistanceCollection[i] = m_graphMatrix[startVertexIndex][i];
    }
    flags[startVertexIndex] = 1;
    minDistanceCollection[startVertexIndex] = 0;
    //遍历m_numberOfVertex - 1次，每次找出一个顶点的最短路径
    for(int i = 1; i < m_numberOfVertex; i++) {
        minDistance = INF;
        for(int j = 0; j < m_numberOfVertex; j++) {
            if(flags[j] == 0 && minDistanceCollection[j] < minDistance) {
                minDistance = minDistanceCollection[j];
                minDistanceIndex = j;
            }
        }
        flags[minDistanceIndex] = 1;
        for(int j = 0; j < m_numberOfVertex; j++) {
            newDistance = ((m_graphMatrix[minDistanceIndex][j] == INF) ? INF :
                           (minDistance + m_graphMatrix[minDistanceIndex][j]));
            if(flags[j] == 0 && (newDistance < minDistanceCollection[j])) {
                minDistanceCollection[j] = newDistance;
                prev[j] = minDistanceIndex;
            }
        }
    }
    //打印dijkstra最短路径的结果
    cout<<"dijkstra("<<m_vertexCollection[startVertexIndex]<<"):  "<<endl;
    for(int i = 0; i < m_numberOfVertex; i++) {
        cout<<"  shortest("<<m_vertexCollection[startVertexIndex]
            <<", "<<m_vertexCollection[i]<<")="<<minDistanceCollection[i]<<endl;
    }
}

void MatrixUDG::prim(int vertexIndex) {
    int minDistance;
    int sum = 0;
    int primsIndex = 0;
    vector<char> prims(m_numberOfVertex + 1, 0);
    vector<int> weights(m_numberOfVertex, 0);

    //构建最小生成树
    prims[primsIndex++] = m_vertexCollection[vertexIndex];
    for(int i = 0; i < m_numberOfVertex; i++) {
        weights[i] = m_graphMatrix[vertexIndex][i];
    }
    weights[vertexIndex] = 0;
    for(int i = 0; i < m_numberOfVertex; i++) {
        if(i == vertexIndex) {
            continue;
        }
        int j = 0;
        int k = 0;
        minDistance = INF;
        while(j < m_numberOfVertex) {
            if(weights[j] != 0 && weights[j] < minDistance) {
                minDistance = weights[j];
                k = j;
            }
            j++;
        }
        prims[primsIndex++] = m_vertexCollection[k];
        weights[k] = 0;
        for(j = 0; j < m_numberOfVertex; j++) {
            if(weights[j] != 0 && m_graphMatrix[k][j] < weights[j]) {
                weights[j] = m_graphMatrix[k][j];
            }
        }
    }

    //计算最小生成树的权值
    for(int i = 1; i < primsIndex; i++) {
        minDistance = INF;
        int endVertexIndex = getVertexIndex(prims[i]);
        for(int j = 0; j < i; j++) {
            int startVertexIndex = getVertexIndex(prims[j]);
            if(m_graphMatrix[startVertexIndex][endVertexIndex] < minDistance) {
                minDistance = m_graphMatrix[startVertexIndex][endVertexIndex];
            }
        }
        sum += minDistance;
    }

    //打印最小生成树
    cout<<"PRIM("<<m_vertexCollection[vertexIndex]<<")="<<sum<<": ";
    for(int i = 0; i < primsIndex; i++) {
        cout<<prims[i]<<" ";
    }
    cout<<endl;
}

void MatrixUDG::kruskal() {
    int startVertexIndex;
    int endVertexIndex;
    vector<EdgeData> minEdgeCollection;
    vector<int> vends(m_numberOfVertex, 0);

    EdgeData* edges;
    edges = getAllEdges();
    sortAllEdges(edges);
    for(int i = 0; i < m_numberOfEdge; i++) {
        startVertexIndex = getVertexIndex(edges[i].start);
        endVertexIndex = getVertexIndex(edges[i].end);
        int m = getVertexEnd(vends, startVertexIndex);
        int n = getVertexEnd(vends, endVertexIndex);
        if(m != n) {
            vends[m] = n;
            minEdgeCollection.push_back(edges[i]);
        }
    }
    delete []edges;
    edges = nullptr;

    int length = 0;
    for(int i = 0; i < minEdgeCollection.size(); i++) {
        length += minEdgeCollection[i].weight;
    }
    cout<<"Kruskal="<<length<<": ";
    for(int i = 0; i < minEdgeCollection.size(); i++) {
        cout<<"("<<minEdgeCollection[i].start<<", "<<
            minEdgeCollection[i].end<<") ";
    }
    cout<<endl;
}

void MatrixUDG::printGraph() {
    cout<<"Matrix Graph: "<<endl;
    for(int i = 0; i < m_numberOfVertex; i++) {
        for(int j = 0; j < m_numberOfVertex; j++) {
            cout<<setw(12)<<m_graphMatrix[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
}

int MatrixUDG::getVertexIndex(char vertex) {
    for(int i = 0; i < m_numberOfVertex; i++) {
        if(vertex == m_vertexCollection[i]) {
            return i;
        }
    }
    return -1;
}

EdgeData* MatrixUDG::getAllEdges() {
    int edgeIndex = 0;
    EdgeData* edges = new EdgeData[m_numberOfEdge];
    for(int i = 0; i < m_numberOfVertex; i++) {
        for(int j = i + 1; j < m_numberOfVertex; j++) {
            if(m_graphMatrix[i][j] != INF) {
                edges[edgeIndex].start = m_vertexCollection[i];
                edges[edgeIndex].end = m_vertexCollection[j];
                edges[edgeIndex].weight = m_graphMatrix[i][j];
                edgeIndex++;
            }
        }
    }
    return edges;
}

void MatrixUDG::sortAllEdges(EdgeData* edges) {
    for(int i = 0; i < m_numberOfEdge; i++) {
        for(int j = i + 1; j < m_numberOfEdge; j++) {
            if(edges[i].weight > edges[j].weight) {
                swap(edges[i], edges[j]);
            }
        }
    }
}

int MatrixUDG::getVertexEnd(vector<int>& vends, int vertexIndex) {
    while(vends[vertexIndex] != 0) {
        vertexIndex = vends[vertexIndex];
    }
    return vertexIndex;
}

//测试dijkstra算法
void testGraph() {
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
    graph->printGraph();
    graph->dfsTraversal();
    graph->bfsTraversal();

    graph->dijkstra(3, prev, minDistanceCollection);
    graph->prim(0);
    graph->kruskal();
}

int main()
{
    testGraph();
}

