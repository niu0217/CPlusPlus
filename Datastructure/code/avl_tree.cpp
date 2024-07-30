#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<stack>
#include<climits>
#include<fstream>
#include<sstream>  //用于处理字符串流
#include<string>

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int initialValue)
        : value{ initialValue }, left{ nullptr }, right{ nullptr } { }
};

class BinaryTree {
public:
    BinaryTree() : root{ nullptr } { }
    ~BinaryTree() {
        deleteAllNodes();
    }

    void insertNode(int toInsertNodeValue);
    TreeNode* insertNodeHelper(TreeNode* rootOfTree, int toInsertNodeValue);
    void deleteAllNodes();
    void deleteAllNodesHelper(TreeNode* toDeleteNode);
    void deleteOneNode(int toDeleteNodeValue);
    TreeNode* deleteOneNodeHelper(TreeNode* rootOfTree, int toDeleteNodeValue);

    TreeNode* leftRotate(TreeNode* rootOfTree);
    TreeNode* rightRotate(TreeNode* rootOfTree);
    TreeNode* adjustTree(TreeNode* rootOfTree, int flags);

    TreeNode* getMinvalueNode(TreeNode* rootOfTree) const;
    int getTreeHeight(TreeNode* rootOfTree) const;
    int getTreeBalance(TreeNode* rootOfTree) const;
    TreeNode* searchNode(TreeNode* rootOfTree, int toSearchNodeValue) const;

    void levelTraversal() const;
private:
    TreeNode* root;
};

void BinaryTree::insertNode(int toInsertNodeValue) {
    root = insertNodeHelper(root, toInsertNodeValue);
}

TreeNode* BinaryTree::insertNodeHelper(TreeNode* rootOfTree, int toInsertNodeValue) {
    if(rootOfTree == nullptr) {
        return new TreeNode(toInsertNodeValue);
    }
    if(toInsertNodeValue < rootOfTree->value) {
        rootOfTree->left = insertNodeHelper(rootOfTree->left, toInsertNodeValue);
    }
    else if(toInsertNodeValue > rootOfTree->value) {
        rootOfTree->right = insertNodeHelper(rootOfTree->right, toInsertNodeValue);
    }
    else {
        //不做处理
        return rootOfTree;
    }
    return adjustTree(rootOfTree, 0);
}

void BinaryTree::deleteAllNodes() {
    deleteAllNodesHelper(root);
}

void BinaryTree::deleteAllNodesHelper(TreeNode* toDeleteNode) {
    if(toDeleteNode == nullptr) {
        return;
    }
    deleteAllNodesHelper(toDeleteNode->left);
    deleteAllNodesHelper(toDeleteNode->right);
    delete toDeleteNode;
    toDeleteNode = nullptr;
}

void BinaryTree::deleteOneNode(int toDeleteNodeValue) {
    root = deleteOneNodeHelper(root, toDeleteNodeValue);
}

TreeNode* BinaryTree::deleteOneNodeHelper(TreeNode* rootOfTree, int toDeleteNodeValue) {
    if(rootOfTree == nullptr) {
        return rootOfTree;
    }
    if(toDeleteNodeValue < rootOfTree->value) {
        rootOfTree->left = deleteOneNodeHelper(rootOfTree->left, toDeleteNodeValue);
    }
    else if(toDeleteNodeValue > rootOfTree->value) {
        rootOfTree->right = deleteOneNodeHelper(rootOfTree->right, toDeleteNodeValue);
    }
    else {
        if(rootOfTree->left == nullptr) {
            //只有一个右结点或者没有子结点
            TreeNode* afterDeleteSubtreeRoot = rootOfTree->right;
            delete rootOfTree;
            rootOfTree = nullptr;
            return afterDeleteSubtreeRoot;
        }
        else if(rootOfTree->right == nullptr) {
            //只有一个左结点或者没有子结点
            TreeNode* afterDeleteSubtreeRoot = rootOfTree->left;
            delete rootOfTree;
            rootOfTree = nullptr;
            return afterDeleteSubtreeRoot;
        }
        else {
            //有两个子结点，获取右子树中的最小值结点来替换
            //当前结点，并删除哪个最小值结点
            TreeNode* minSubtreeNode = getMinvalueNode(rootOfTree->right);
            rootOfTree->value = minSubtreeNode->value;
            rootOfTree->right = deleteOneNodeHelper(rootOfTree->right, minSubtreeNode->value);
        }
    }
    return adjustTree(rootOfTree, 1);
}

//左旋：
//  1.当前结点的右子树会作为新树的根结点
//  2.当前结点会作为新树的根结点的左子树
//  3.如果，新的树根，原来有左子树，原来的左子树，
//    就作为旧根结点的右子树.
TreeNode* BinaryTree::leftRotate(TreeNode* rootOfTree) {
    TreeNode* newRoot = rootOfTree->right;
    TreeNode* rootOfTreeRL = rootOfTree->right->left;
    newRoot->left = rootOfTree;
    rootOfTree->right = rootOfTreeRL;
    return newRoot;
}

//右旋：
//  1.当前结点的左子树会作为新树的根结点
//  2.当前结点会作为新树的根结点的右子树
//  3.如果，新的树根，原来有右子树，原来的右子树，
//    就作为就根结点的左子树.
TreeNode* BinaryTree::rightRotate(TreeNode* rootOfTree) {
    TreeNode* newRoot = rootOfTree->left;
    TreeNode* rootOfTreeLR = rootOfTree->left->right;
    newRoot->right = rootOfTree;
    rootOfTree->left = rootOfTreeLR;
    return newRoot;
}

//调整树，使其平衡
//  flags: 为0代表插入结点的调整；为1代表删除结点的调整
TreeNode* BinaryTree::adjustTree(TreeNode* rootOfTree, int flags) {
    int currentTreeBalance = getTreeBalance(rootOfTree);
    int leftSubtreeBalance = getTreeBalance(rootOfTree->left);
    int rightSubtreeBalance = getTreeBalance(rootOfTree->right);
    //我们是否调整这个树，是看平衡因子是不是绝对值大于1
    //LL型失衡
    bool condition = false; //用于区别插入和删除结点时的不同处理条件
    if(flags == 1) {
        condition = leftSubtreeBalance >= 0;
    }
    else {
        condition = leftSubtreeBalance > 0;
    }
    if(currentTreeBalance > 1 && condition) {
        return rightRotate(rootOfTree);
    }
    //LR型失衡
    if(currentTreeBalance > 1 && leftSubtreeBalance < 0) {
        rootOfTree->left = leftRotate(rootOfTree->left);
        //此时已经变成了LL型失衡
        return rightRotate(rootOfTree);
    }
    //RR型失衡
    if(flags == 1) {
        condition = rightSubtreeBalance <= 0;
    }
    else {
        condition = rightSubtreeBalance < 0;
    }
    if(currentTreeBalance < -1 && condition) {
        return leftRotate(rootOfTree);
    }
    //RL型失衡
    if(currentTreeBalance < -1 && rightSubtreeBalance > 0) {
        rootOfTree->right = rightRotate(rootOfTree->right);
        //此时已经变成了RR型失衡
        return leftRotate(rootOfTree);
    }
    return rootOfTree;
}

TreeNode* BinaryTree::getMinvalueNode(TreeNode* rootOfTree) const {
    TreeNode* minTreeNode = rootOfTree;
    while(minTreeNode && minTreeNode->left) {
        minTreeNode = minTreeNode->left;
    }
    return minTreeNode;
}

int BinaryTree::getTreeHeight(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return 0; //空树的树高为0
    }
    int leftSubtreeHeight = getTreeHeight(rootOfTree->left);
    int rightSubtreeHeight = getTreeHeight(rootOfTree->right);
    return 1 + std::max(leftSubtreeHeight, rightSubtreeHeight);
}

int BinaryTree::getTreeBalance(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return 0;
    }
    return getTreeHeight(rootOfTree->left) - getTreeHeight(rootOfTree->right);
}

TreeNode* BinaryTree::searchNode(TreeNode* rootOfTree, int toSearchNodeValue) const {
    if(rootOfTree == nullptr || rootOfTree->value == toSearchNodeValue) {
        return rootOfTree;
    }
    if(toSearchNodeValue < rootOfTree->value) {
        return searchNode(rootOfTree->left, toSearchNodeValue);
    }
    else {
        return searchNode(rootOfTree->right, toSearchNodeValue);
    }
}

void BinaryTree::levelTraversal() const {
    if(root == nullptr) {
        return;
    }
    std::queue<TreeNode*> treeNodes;
    treeNodes.push(root);
    while(!treeNodes.empty()) {
        int size = treeNodes.size();
        for(int i = 0; i < size; i++) {
            TreeNode* treeNode = treeNodes.front();
            treeNodes.pop();
            std::cout<<treeNode->value<<" ";
            if(treeNode->left) {
                treeNodes.push(treeNode->left);
            }
            if(treeNode->right) {
                treeNodes.push(treeNode->right);
            }
        }
    }
    std::cout<<"\n\n";
}

void testBinaryTree() {
    std::vector<std::vector<int>> treeNodeDatas = {
        { 5, 4, 3 },
        { 5, 6, 7 },
        { 5, 3, 4 },
        { 5, 8, 6 },
        { 5, 6, 7, 8, 9, 10 },
        { 9, 8, 7, 6, 5, 4, 3, 2, 1}
    };
    for(const auto& treeNodeDataVec : treeNodeDatas) {
        BinaryTree* tree = new BinaryTree();
        for(const auto& treeNodeData : treeNodeDataVec) {
            tree->insertNode(treeNodeData);
        }
        tree->levelTraversal();
        delete tree;
        tree = nullptr;
    }
}

int main()
{
    testBinaryTree();
}

