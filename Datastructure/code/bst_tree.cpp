#include<iostream>
#include<algorithm>
#include<queue>
#include<vector>
#include<stack>
#include<climits>

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;
    TreeNode(int val): value{ val }, left{ nullptr }, right{ nullptr } { }
};

class BinaryTree {
public:
    BinaryTree(): root{ nullptr } {}
    ~BinaryTree() {
        deleteAllNodes();
        std::cout<<"Have delete all nodes...\n";
    }

    void insertNode(int toInsertNodeValue);
    void insertNodeHelper(TreeNode* rootOfTree, int toInsertNodeValue);
    void deleteAllNodes();
    void deleteAllNodesHelper(TreeNode* toDeleteNode);
    void deleteOneNode(int toDeleteNodeValue);
    TreeNode* deleteOneNodeHelper(TreeNode* rootOfTree, int toDeleteNodeValue);
    TreeNode* getMinvalueNode(TreeNode* rootOfTree) const;
    TreeNode* searchNode(int toSearchNodeValue) const;
    TreeNode* searchNodeRecursive(TreeNode* rootOfTree, int toSearchNodeValue) const;

    void preorderTraversal() const;
    void preorderTraversalRecursive(TreeNode* rootOfTree) const;
    void preorderTraversalIterate(TreeNode* rootOfTree) const;
    void midorderTraversal() const;
    void midorderTraversalRecursive(TreeNode* rootOfTree) const;
    void midorderTraversalIterate(TreeNode* rootOfTree) const;
    void postorderTraversal() const;
    void postorderTraversalRecursive(TreeNode* rootOfTree) const;
    void postorderTraversalIterate(TreeNode* rootOfTree) const;
    void levelTraversal() const;
private:
    TreeNode* root;
};

void BinaryTree::insertNode(int toInsertNodeValue) {
    if(root == nullptr) {
        root = new TreeNode(toInsertNodeValue);
    }
    else {
        insertNodeHelper(root, toInsertNodeValue);
    }
}

void BinaryTree::insertNodeHelper(TreeNode* rootOfTree, int toInsertNodeValue) {
    if(toInsertNodeValue < rootOfTree->value) {
        if(rootOfTree->left == nullptr) {
            rootOfTree->left = new TreeNode(toInsertNodeValue);
        }
        else {
            insertNodeHelper(rootOfTree->left, toInsertNodeValue);
        }
    }
    else if(toInsertNodeValue > rootOfTree->value) {
        if(rootOfTree->right == nullptr) {
            rootOfTree->right = new TreeNode(toInsertNodeValue);
        }
        else {
            insertNodeHelper(rootOfTree->right, toInsertNodeValue);
        }
    }
    else {
        //相同时不做处理
    }
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
    return rootOfTree;
}

TreeNode* BinaryTree::getMinvalueNode(TreeNode* rootOfTree) const {
    TreeNode* minTreeNode = rootOfTree;
    while(minTreeNode && minTreeNode->left) {
        minTreeNode = minTreeNode->left;
    }
    return minTreeNode;
}

TreeNode* BinaryTree::searchNode(int toSearchNodeValue) const {
    return searchNodeRecursive(root, toSearchNodeValue);
}

TreeNode* BinaryTree::searchNodeRecursive(TreeNode* rootOfTree, int toSearchNodeValue) const {
    if(rootOfTree == nullptr || rootOfTree->value == toSearchNodeValue) {
        return rootOfTree;
    }
    if(toSearchNodeValue < rootOfTree->value) {
        return searchNodeRecursive(rootOfTree->left, toSearchNodeValue);
    }
    else if(toSearchNodeValue > rootOfTree->value) {
        return searchNodeRecursive(rootOfTree->right, toSearchNodeValue);
    }
    return nullptr; //永远到不了这里
}

void BinaryTree::preorderTraversal() const {
    // preorderTraversalRecursive(root);
    preorderTraversalIterate(root);
    std::cout<<"\n\n";
}

void BinaryTree::preorderTraversalRecursive(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    std::cout<<rootOfTree->value<<" ";
    preorderTraversalRecursive(rootOfTree->left);
    preorderTraversalRecursive(rootOfTree->right);
}

void BinaryTree::preorderTraversalIterate(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    std::stack<TreeNode*> treeNodes;
    treeNodes.push(rootOfTree);
    while(!treeNodes.empty()) {
        TreeNode* treeNode = treeNodes.top();
        treeNodes.pop();
        std::cout<<treeNode->value<<" ";
        if(treeNode->right) {
            treeNodes.push(treeNode->right);
        }
        if(treeNode->left) {
            treeNodes.push(treeNode->left);
        }
    }
}

void BinaryTree::midorderTraversal() const {
    // midorderTraversalRecursive(root);
    midorderTraversalIterate(root);
    std::cout<<"\n\n";
}

void BinaryTree::midorderTraversalRecursive(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    midorderTraversalRecursive(rootOfTree->left);
    std::cout<<rootOfTree->value<<" ";
    midorderTraversalRecursive(rootOfTree->right);
}

void BinaryTree::midorderTraversalIterate(TreeNode* rootOfTree) const {
    std::stack<TreeNode*> treeNodes;
    TreeNode* currentTreeNode = rootOfTree;
    while(currentTreeNode != nullptr || !treeNodes.empty()) {
        //左
        while(currentTreeNode != nullptr) {
            treeNodes.push(currentTreeNode);
            currentTreeNode = currentTreeNode->left;
        }
        currentTreeNode = treeNodes.top();
        treeNodes.pop();
        //中
        std::cout<<currentTreeNode->value<<" ";
        //右
        currentTreeNode = currentTreeNode->right;
    }
}

void BinaryTree::postorderTraversal() const {
    // postorderTraversalRecursive(root);
    postorderTraversalIterate(root);
    std::cout<<"\n\n";
}

void BinaryTree::postorderTraversalRecursive(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    postorderTraversalRecursive(rootOfTree->left);
    postorderTraversalRecursive(rootOfTree->right);
    std::cout<<rootOfTree->value<<" ";
}

void BinaryTree::postorderTraversalIterate(TreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    std::stack<TreeNode*> treeNodes;
    std::stack<TreeNode*> treeNodesPrint;
    TreeNode* treeNode = nullptr;
    treeNodes.push(rootOfTree);
    while(!treeNodes.empty()) {
        treeNode = treeNodes.top();
        treeNodes.pop();
        treeNodesPrint.push(treeNode);
        if(treeNode->left) {
            treeNodes.push(treeNode->left);
        }
        if(treeNode->right) {
            treeNodes.push(treeNode->right);
        }
    }
    while(!treeNodesPrint.empty()) {
        std::cout<<treeNodesPrint.top()->value<<" ";
        treeNodesPrint.pop();
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
    BinaryTree* tree = new BinaryTree();
    tree->insertNode(5);
    tree->insertNode(10);
    tree->insertNode(6);
    tree->insertNode(12);
    tree->insertNode(11);
    tree->insertNode(14);
    std::cout<<"前序遍历：\n";
    tree->preorderTraversal();
    std::cout<<"中序遍历：\n";
    tree->midorderTraversal();
    std::cout<<"后序遍历：\n";
    tree->postorderTraversal();
    std::cout<<"层次遍历：\n";
    tree->levelTraversal();

    std::cout<<"查找值为6的结点：\n";
    TreeNode* resultNode = tree->searchNode(6);
    if(resultNode) {
        std::cout<<"找到了..\n\n";
    }
    else {
        std::cout<<"咩有找到..\n\n";
    }

    std::cout<<"删除值为10的结点: \n";
    tree->deleteOneNode(10);
    tree->levelTraversal();

    delete tree;
    tree = nullptr;
}

int main()
{
    testBinaryTree();
}

