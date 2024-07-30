#include<iostream>
#include<vector>
#include<string>

using namespace std;

#define INF (~(0x1<<31))  //用来代表无穷大

struct EdgeData {
    char start;  //边的起点
    char end;   //边的终点
    int weight; //边的权重
    EdgeData() { }
    EdgeData(char s, char e, int w)
        :start(s), end(e), weight(w) { }
};

//邻接表
class ListUDG {
public:
    ListUDG() { }
    ListUDG(const vector<char>& vertexCollection,
      const vector<EdgeData*> edgeCollection);
    void initialEdge(int startVertexIndex, int endVertexIndex,
      int edgeWeight);

    void dijkstra(int startVertexIndex, vector<int>& prev,
      vector<int>& minDistanceCollection);

    void printGraph();
private:
    class EdgeNode {
        int vertexIndex;
        int weight;
        EdgeNode* nextEdge;
        friend class ListUDG;
    };
    class VertexNode {
        char data;
        EdgeNode* firstEdge;
        friend class ListUDG;
    };
private:
    int getVertexIndex(char vertex);
    void linkToEdgeLast(EdgeNode* edgeList, EdgeNode* newEdge);
    int getEdgeWeight(int startVertexIndex, int endVertexIndex);
private:
    int m_numberOfVertex;
    int m_numberOfEdge;
    vector<VertexNode> m_vertexCollection;
};

ListUDG::ListUDG(const vector<char>& vertexCollection,
  const vector<EdgeData*> edgeCollection) {
    char startVertex;
    char endVertex;
    int edgeWeight;
    int startVertexIndex;
    int endVertexIndex;

    m_numberOfVertex = vertexCollection.size();
    m_numberOfEdge = edgeCollection.size();
    for(int i = 0; i < m_numberOfVertex; i++) {
        VertexNode newVertexNode;
        newVertexNode.data = vertexCollection[i];
        newVertexNode.firstEdge = nullptr;
        m_vertexCollection.push_back(newVertexNode);
    }
    for(int i = 0; i < m_numberOfEdge; i++) {
        startVertex = edgeCollection[i]->start;
        endVertex = edgeCollection[i]->end;
        edgeWeight = edgeCollection[i]->weight;
        startVertexIndex = getVertexIndex(startVertex);
        endVertexIndex = getVertexIndex(endVertex);

        initialEdge(startVertexIndex, endVertexIndex, edgeWeight);
        initialEdge(endVertexIndex, startVertexIndex, edgeWeight);
    }
}

void ListUDG::initialEdge(int startVertexIndex,
  int endVertexIndex, int edgeWeight) {
    EdgeNode* newEdge = new EdgeNode();
    newEdge->vertexIndex = endVertexIndex;
    newEdge->weight = edgeWeight;
    if(m_vertexCollection[startVertexIndex].firstEdge == nullptr) {
        m_vertexCollection[startVertexIndex].firstEdge = newEdge;
    }
    else {
        linkToEdgeLast(m_vertexCollection[startVertexIndex].firstEdge,
          newEdge);
    }
}

void ListUDG::dijkstra(int startVertexIndex, vector<int>& prev,
  vector<int>& minDistanceCollection) {
    int minDistance;
    int newDistance;
    int minDistanceIndex;
    vector<int> flags(m_numberOfVertex, 0);
    for(int i = 0; i < m_numberOfVertex; i++) {
        minDistanceCollection[i] = getEdgeWeight(startVertexIndex, i);
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
            newDistance = getEdgeWeight(minDistanceIndex, j);
            newDistance = ((newDistance == INF) ? INF :
                           (minDistance + newDistance));
            if(flags[j] == 0 && (newDistance < minDistanceCollection[j])) {
                minDistanceCollection[j] = newDistance;
                prev[j] = minDistanceIndex;
            }
        }
    }
    //打印dijkstra最短路径的结果
    cout<<"dijkstra("<<m_vertexCollection[startVertexIndex].data<<"): "<<endl;
    for(int i = 0; i < m_numberOfVertex; i++) {
        cout<<"   shortest("<<m_vertexCollection[startVertexIndex].data<<
            ", "<<m_vertexCollection[i].data<<")="<<minDistanceCollection[i]<<endl;
    }
}

int ListUDG::getVertexIndex(char vertex) {
    for(int i = 0; i < m_numberOfVertex; i++) {
        if(m_vertexCollection[i].data == vertex) {
            return i;
        }
    }
    return -1;
}

void ListUDG::linkToEdgeLast(EdgeNode* edgeList, EdgeNode* newEdge) {
    EdgeNode* curEdgeNode = edgeList;
    while(curEdgeNode->nextEdge != nullptr) {
        curEdgeNode = curEdgeNode->nextEdge;
    }
    curEdgeNode->nextEdge = newEdge;
}

int ListUDG::getEdgeWeight(int startVertexIndex, int endVertexIndex) {
    EdgeNode* curEdgeNode;
    if(startVertexIndex == endVertexIndex) {
        return 0;
    }
    curEdgeNode = m_vertexCollection[startVertexIndex].firstEdge;
    while(curEdgeNode != nullptr) {
        if(endVertexIndex == curEdgeNode->vertexIndex) {
            return curEdgeNode->weight;
        }
        curEdgeNode = curEdgeNode->nextEdge;
    }
    return INF;
}

void ListUDG::printGraph() {
    EdgeNode* curEdgeNode;
    cout<<"List Graph: "<<endl;
    for(int i = 0; i < m_numberOfVertex; i++) {
        cout<<i<<"("<<m_vertexCollection[i].data<<"): ";
        curEdgeNode = m_vertexCollection[i].firstEdge;
        while(curEdgeNode != nullptr) {
            cout<<curEdgeNode->vertexIndex<<"("<<
                m_vertexCollection[curEdgeNode->vertexIndex].data<<
                ")"<<"--"<<curEdgeNode->weight<<"   ";
            curEdgeNode = curEdgeNode->nextEdge;
        }
        cout<<endl;
    }
}

void testDijkstra() {
    vector<char> vertexCollection = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G'
    };
    vector<EdgeData*> edgeCollection = {
               // 起点 终点 权
        new EdgeData('A', 'B', 12),
        new EdgeData('A', 'F', 16),
        new EdgeData('A', 'G', 14),
        new EdgeData('B', 'C', 10),
        new EdgeData('B', 'F',  7),
        new EdgeData('C', 'D',  3),
        new EdgeData('C', 'E',  5),
        new EdgeData('C', 'F',  6),
        new EdgeData('D', 'E',  4),
        new EdgeData('E', 'F',  2),
        new EdgeData('E', 'G',  8),
        new EdgeData('F', 'G',  9)
    };
    int numberOfVertex = vertexCollection.size();
    vector<int> prev(numberOfVertex, 0);
    vector<int> minDistanceCollection(numberOfVertex, 0);
    ListUDG* graph = new ListUDG(vertexCollection, edgeCollection);
    graph->printGraph();
    graph->dijkstra(3, prev, minDistanceCollection);
}

int main()
{
    testDijkstra();
}

