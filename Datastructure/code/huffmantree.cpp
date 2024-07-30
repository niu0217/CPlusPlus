#include<iostream>
#include<vector>
#include<queue>
#include<map>
#include<string>
#include<sstream>

struct HuffmanTreeNode {
    char data;
    int weight;
    HuffmanTreeNode* left;
    HuffmanTreeNode* right;
    HuffmanTreeNode(char initialData, int initialWeight,
            HuffmanTreeNode* initialLeft = nullptr,
            HuffmanTreeNode* initialRight = nullptr)
        : data{ initialData }, weight{ initialWeight},
          left{ initialLeft }, right{ initialRight } { }
};

struct Compare {
    bool operator()(const HuffmanTreeNode* leftTreeNode,
                    const HuffmanTreeNode* rightTreeNode) const {
        return leftTreeNode->weight > rightTreeNode->weight;
    }
};

class HuffmanTree {
public:
    HuffmanTree() : root{ nullptr } { }
    ~HuffmanTree() {
        deleteAllNodes();
    }

    void buildHuffmanTree(const std::map<char, int>& treeDatas);
    void generateHuffmanCodes(std::string code,
      std::map<char, std::string>& huffmanCodes) const;
    void generateHuffmanCodesHelper(HuffmanTreeNode* rootOfTree,
      std::string code, std::map<char, std::string>& huffmanCodes) const;

    void deleteAllNodes();
    void deleteAllNodesHelper(HuffmanTreeNode* rootOfTree);
private:
    HuffmanTreeNode* root;
};

void HuffmanTree::buildHuffmanTree(const std::map<char, int>& treeDatas) {
    std::priority_queue<HuffmanTreeNode*, std::vector<HuffmanTreeNode*>, Compare> minheapPQ;
    for(const auto& treeData : treeDatas) {
        minheapPQ.push(new HuffmanTreeNode(treeData.first, treeData.second));
    }
    while(minheapPQ.size() > 1) {
        HuffmanTreeNode* leftTreeNode = minheapPQ.top();
        minheapPQ.pop();
        HuffmanTreeNode* rightTreeNode = minheapPQ.top();
        minheapPQ.pop();
        int sumOfWeight = leftTreeNode->weight + rightTreeNode->weight;
        //0代表这是一个非叶子结点
        minheapPQ.push(new HuffmanTreeNode(0, sumOfWeight, leftTreeNode, rightTreeNode));
    }
    root = minheapPQ.top();
    minheapPQ.pop();
}

void HuffmanTree::generateHuffmanCodes(std::string code,
  std::map<char, std::string>& huffmanCodes) const {
    generateHuffmanCodesHelper(root, code, huffmanCodes);
}

void HuffmanTree::generateHuffmanCodesHelper(HuffmanTreeNode* rootOfTree,
  std::string code, std::map<char, std::string>& huffmanCodes) const {
    if(rootOfTree == nullptr) {
        return;
    }
    if(rootOfTree->data != 0) {
        //叶子结点
        huffmanCodes[rootOfTree->data] = code;
    }
    else {
        //非叶子结点
        generateHuffmanCodesHelper(rootOfTree->left, code + "1", huffmanCodes);
        generateHuffmanCodesHelper(rootOfTree->right, code + "0", huffmanCodes);
    }
}

void HuffmanTree::deleteAllNodes() {
    deleteAllNodesHelper(root);
}
void HuffmanTree::deleteAllNodesHelper(HuffmanTreeNode* rootOfTree) {
    if(rootOfTree == nullptr) {
        return;
    }
    deleteAllNodesHelper(rootOfTree->left);
    deleteAllNodesHelper(rootOfTree->right);
    std::cout<<"delete "<<rootOfTree->data<<"\n";
    delete rootOfTree;
    rootOfTree = nullptr;
}

int main()
{
    std::map<char, int> treeDatas = {
        {'a', 5}, {'b', 9}, {'c', 12}, {'d', 13}, {'e', 16}, {'f', 45}
    };
    HuffmanTree* huffmanTree = new HuffmanTree();

    huffmanTree->buildHuffmanTree(treeDatas);

    std::map<char, std::string> huffmanCodes;
    huffmanTree->generateHuffmanCodes("", huffmanCodes);
    for(const auto& data : huffmanCodes) {
        std::cout<<data.first<<": "<<data.second<<"\n";
    }

    delete huffmanTree;
    huffmanTree = nullptr;
}
