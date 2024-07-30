#include<iostream>
#include<vector>
#include<queue>
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

    void dfsTraversal();
    void dfsTraversalRecursive(int vertexIndex,
      vector<int>& visitedVertexIndexs);
    void bfsTraversal();

    void dijkstra(int startVertexIndex, vector<int>& prev,
      vector<int>& minDistanceCollection);
    void prim(int vertexIndex);
    void kruskal();

    void printGraph();
    EdgeData* getAllEdges();
    void sortAllEdges(EdgeData* edges);
    int getVertexEnd(vector<int>& vends, int vertexIndex);
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

void ListUDG::dfsTraversal() {
    vector<int> visitedVertexIndexs(m_numberOfVertex, 0);
    cout<<"DFS: ";
    for(int vertexIndex = 0; vertexIndex < m_numberOfVertex; vertexIndex++) {
        if(!visitedVertexIndexs[vertexIndex]) {
            dfsTraversalRecursive(vertexIndex, visitedVertexIndexs);
        }
    }
    cout<<endl;
}

void ListUDG::dfsTraversalRecursive(int vertexIndex,
  vector<int>& visitedVertexIndexs) {
    EdgeNode* currentEdgeNode;
    visitedVertexIndexs[vertexIndex] = 1;
    cout<<m_vertexCollection[vertexIndex].data<<" ";
    currentEdgeNode = m_vertexCollection[vertexIndex].firstEdge;
    while(currentEdgeNode != nullptr) {
        if(!visitedVertexIndexs[currentEdgeNode->vertexIndex]) {
            dfsTraversalRecursive(currentEdgeNode->vertexIndex, visitedVertexIndexs);
        }
        currentEdgeNode = currentEdgeNode->nextEdge;
    }
}

void ListUDG::bfsTraversal() {
    queue<int> vertexIndexQueue;
    vector<int> visitedVertexIndexs(m_numberOfVertex, 0);
    EdgeNode* currentEdgeNode = nullptr;
    cout<<"BFS: ";
    vertexIndexQueue.push(0);
    while(!vertexIndexQueue.empty()) {
        int size = vertexIndexQueue.size();
        for(int i = 0; i < size; i++) {
            int vertexIndex = vertexIndexQueue.front();
            vertexIndexQueue.pop();
            if(!visitedVertexIndexs[vertexIndex]) {
                visitedVertexIndexs[vertexIndex] = 1;
                cout<<m_vertexCollection[vertexIndex].data<<" ";
            }
            currentEdgeNode = m_vertexCollection[vertexIndex].firstEdge;
            while(currentEdgeNode != nullptr) {
                if(!visitedVertexIndexs[currentEdgeNode->vertexIndex]) {
                    visitedVertexIndexs[currentEdgeNode->vertexIndex] = 1;
                    cout<<m_vertexCollection[currentEdgeNode->vertexIndex].data<<" ";
                    vertexIndexQueue.push(currentEdgeNode->vertexIndex);
                }
                currentEdgeNode = currentEdgeNode->nextEdge;
            }
        }
    }
    cout<<endl;
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

void ListUDG::prim(int vertexIndex) {
    int minDistance;
    int sum = 0;
    int primsIndex = 0;
    vector<char> prims(m_numberOfVertex, 0);
    vector<int> weights(m_numberOfVertex, 0);

    //生成最小生成树
    prims[primsIndex++] = m_vertexCollection[vertexIndex].data;
    for(int i = 0; i < m_numberOfVertex; i++) {
        weights[i] = getEdgeWeight(vertexIndex, i);
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
        prims[primsIndex++] = m_vertexCollection[k].data;
        weights[k] = 0;
        for(j = 0; j < m_numberOfVertex; j++) {
            int newDistance = getEdgeWeight(k, j);
            if(weights[j] != 0 && newDistance < weights[j]) {
                weights[j] = newDistance;
            }
        }
    }

    //计算最小生成树
    for(int i = 1; i < primsIndex; i++) {
        minDistance = INF;
        int endVertexIndex = getVertexIndex(prims[i]);
        for(int j = 0; j < i; j++) {
            int startVertexIndex = getVertexIndex(prims[j]);
            int edgeWeight = getEdgeWeight(startVertexIndex, endVertexIndex);
            if(edgeWeight < minDistance) {
                minDistance = edgeWeight;
            }
        }
        sum += minDistance;
    }

    //打印最小生成树
    cout<<"PRIM("<<m_vertexCollection[vertexIndex].data<<")="<<sum<<": ";
    for(int i = 0; i < primsIndex; i++) {
        cout<<prims[i]<<" ";
    }
    cout<<endl;
}

void ListUDG::kruskal() {
    int startVertexIndex;
    int endVertexIndex;
    vector<int> vends(m_numberOfVertex, 0);
    vector<EdgeData> minEdgeCollection;

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
    EdgeNode* edgeNode;
    if(startVertexIndex == endVertexIndex) {
        return 0;
    }
    edgeNode = m_vertexCollection[startVertexIndex].firstEdge;
    while(edgeNode != nullptr) {
        if(endVertexIndex == edgeNode->vertexIndex) {
            return edgeNode->weight;
        }
        edgeNode = edgeNode->nextEdge;
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

EdgeData* ListUDG::getAllEdges() {
    int edgeIndex = 0;
    EdgeData* edges;
    EdgeNode* edgeNode;
    edges = new EdgeData[m_numberOfEdge];
    for(int i = 0; i < m_numberOfVertex; i++) {
        edgeNode = m_vertexCollection[i].firstEdge;
        while(edgeNode != nullptr) {
            if(edgeNode->vertexIndex > i) {
                edges[edgeIndex].start = m_vertexCollection[i].data;
                edges[edgeIndex].end = m_vertexCollection[edgeNode->vertexIndex].data;
                edges[edgeIndex].weight = edgeNode->weight;
                edgeIndex++;
            }
            edgeNode = edgeNode->nextEdge;
        }
    }
    return edges;
}

void ListUDG::sortAllEdges(EdgeData* edges) {
    for(int i = 0; i < m_numberOfEdge; i++) {
        for(int j = i + 1; j < m_numberOfEdge; j++) {
            if(edges[i].weight > edges[j].weight) {
                swap(edges[i], edges[j]);
            }
        }
    }
}

int ListUDG::getVertexEnd(vector<int>& vends, int vertexIndex) {
    while(vends[vertexIndex] != 0) {
        vertexIndex = vends[vertexIndex];
    }
    return vertexIndex;
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
    graph->prim(0);
    graph->kruskal();

    graph->dfsTraversal();
    graph->bfsTraversal();
}

int main()
{
    testDijkstra();
}
