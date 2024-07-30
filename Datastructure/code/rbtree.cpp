#include<iostream>
#include<vector>
#include<string>
#include<algorithm>

using namespace std;

enum Color {
    RED, BLACK
};

struct RBTreeNode {
    Color color;
    int key;
    RBTreeNode* left;
    RBTreeNode* right;
    RBTreeNode* parent;
    RBTreeNode(int initialKey)
        :color(RED), key(initialKey),
         left(nullptr), right(nullptr), parent(nullptr) { }
};

class RBTree {
public:
    RBTree(): root(nullptr) { }
    ~RBTree() {
        deleteAllNodes();
    }

    void insertNode(RBTreeNode* toInsertNode);
    void deleteAllNodes();
    void deleteAllNodesHelper(RBTreeNode* rootOfTree);

    void leftRotate(RBTreeNode* rootOfTree);
    void rightRotate(RBTreeNode* rootOfTree);
    void fixupTree(RBTreeNode* toFixupNode);
    void changeColorXXr(RBTreeNode* toFixupNode, RBTreeNode* toFixupNodeUncle);
    void changeColorAndRotateLXb(RBTreeNode* toFixupNode);
    void changeColorAndRotateRXb(RBTreeNode* toFixupNode);

    void midorderTraversal() const;
    void midorderTraversalRecursive(RBTreeNode* rootOfTree) const;
    void preorderTraversal() const;
    void preorderTraversalRecursive(RBTreeNode* rootOfTree) const;
public:
    RBTreeNode* root;
};

void RBTree::insertNode(RBTreeNode* toInsertNode) {
    RBTreeNode* toInsertNodeParent = nullptr;
    RBTreeNode* currentTreeNode = root;
    while(currentTreeNode != nullptr) {
        toInsertNodeParent = currentTreeNode;
        if(toInsertNode->key < currentTreeNode->key) {
            currentTreeNode = currentTreeNode->left;
        }
        else if(toInsertNode->key > currentTreeNode->key) {
            currentTreeNode = currentTreeNode->right;
        }
        else {
            return; //不允许插入两个值相同的结点
        }
    }
    //调整toInsertNode的父结点
    toInsertNode->parent = toInsertNodeParent;
    if(toInsertNodeParent == nullptr) {
        //如果要插入结点的父结点为空，说明是根结点
        root = toInsertNode;
    }
    else if(toInsertNode->key < toInsertNodeParent->key) {
        toInsertNodeParent->left = toInsertNode;
    }
    else if(toInsertNode->key > toInsertNodeParent->key) {
        toInsertNodeParent->right = toInsertNode;
    }
    fixupTree(toInsertNode);
}

void RBTree::deleteAllNodes() {
    deleteAllNodesHelper(root);
}

void RBTree::deleteAllNodesHelper(RBTreeNode* rootOfTree) {
    if(rootOfTree == nullptr) {
        return;
    }
    deleteAllNodesHelper(rootOfTree->left);
    deleteAllNodesHelper(rootOfTree->right);
    std::cout<<"delete "<<rootOfTree->key<<"\n";
    delete rootOfTree;
    rootOfTree = nullptr;
}

//左旋：
//  1.当前结点的右子树会作为新树的根结点
//  2.当前结点会作为新树的根结点的左子树
//  3.如果，新的树根，原来有左子树，原来的左子树，
//    就作为旧根结点的右子树
void RBTree::leftRotate(RBTreeNode* rootOfTree) {
    RBTreeNode* newRoot = rootOfTree->right;
    RBTreeNode* rootOfTreeRL = rootOfTree->right->left;
    //调整rootOfTreeRL的父结点
    if(rootOfTreeRL != nullptr) {
        rootOfTreeRL->parent = rootOfTree;
    }
    rootOfTree->right = rootOfTreeRL;
    //调整newRoot的父结点
    newRoot->parent = rootOfTree->parent;
    if(rootOfTree->parent == nullptr) {
        //说明rootOfTree原来就是整个树的根
        root = newRoot;
    }
    else if(rootOfTree == rootOfTree->parent->left) {
        rootOfTree->parent->left = newRoot;
    }
    else if(rootOfTree == rootOfTree->parent->right) {
        rootOfTree->parent->right = newRoot;
    }
    //调整rootOfTree的父结点
    rootOfTree->parent = newRoot;
    newRoot->left = rootOfTree;
}

//右旋：
//  1.当前结点的左子树会作为新树的根结点
//  2.当前结点会作为新树的根结点的右子树
//  3.如果，新的树根，原来有右子树，原来的右子树，
//    就作为就根结点的左子树
void RBTree::rightRotate(RBTreeNode* rootOfTree) {
    RBTreeNode* newRoot = rootOfTree->left;
    RBTreeNode* rootOfTreeLR = rootOfTree->left->right;
    //修改rootOfTreeLR的父结点
    if(rootOfTreeLR != nullptr) {
        rootOfTreeLR->parent = rootOfTree;
    }
    rootOfTree->left = rootOfTreeLR;
    //修改newRoot的父结点
    newRoot->parent = rootOfTree->parent;
    if(rootOfTree->parent == nullptr) {
        //说明rootOfTree原来就是整个树的根
        root = newRoot;
    }
    else if(rootOfTree == rootOfTree->parent->left) {
        rootOfTree->parent->left = newRoot;
    }
    else if(rootOfTree == rootOfTree->parent->right) {
        rootOfTree->parent->right = newRoot;
    }
    //修改rootOfTree的父结点
    rootOfTree->parent = newRoot;
    newRoot->right = rootOfTree;
}

void RBTree::fixupTree(RBTreeNode* toFixupNode) {
    while((toFixupNode->parent != nullptr) &&
          (toFixupNode->parent->color == RED)) {
        if(toFixupNode->parent == toFixupNode->parent->parent->left) {
            //LXx
            RBTreeNode* toFixupNodeUncle = toFixupNode->parent->parent->right;
            if((toFixupNodeUncle != nullptr) &&
               (toFixupNodeUncle->color == RED)) {
                //LXr
                changeColorXXr(toFixupNode, toFixupNodeUncle);
            }
            else if((toFixupNodeUncle == nullptr) ||
                    (toFixupNodeUncle->color = BLACK)) {
                //LXb
                changeColorAndRotateLXb(toFixupNode);
            }
        }
        else if(toFixupNode->parent == toFixupNode->parent->parent->right) {
            //RXx
            RBTreeNode* toFixupNodeUncle = toFixupNode->parent->parent->left;
            if((toFixupNodeUncle != nullptr) &&
               (toFixupNodeUncle->color == RED)) {
                //RXr
                changeColorXXr(toFixupNode, toFixupNodeUncle);
            }
            else if((toFixupNodeUncle == nullptr) ||
                    (toFixupNodeUncle->color = BLACK)) {
                //RXb
                changeColorAndRotateRXb(toFixupNode);
            }
        }
    }
    root->color = BLACK;
}

void RBTree::changeColorXXr(RBTreeNode* toFixupNode, RBTreeNode* toFixupNodeUncle) {
    //1.toFixupNode的父亲结点变为黑色
    //2.toFixupNode的叔叔结点变为黑色
    //3.toFixupNode的祖父结点变为红色
    //4.将toFixupNode更新为toFixupNodeUncle的祖父结点，继续向上检查
    toFixupNode->parent->color = BLACK;
    toFixupNodeUncle->color = BLACK;
    toFixupNode->parent->parent->color = RED;
    toFixupNode = toFixupNode->parent->parent;
}

void RBTree::changeColorAndRotateLXb(RBTreeNode* toFixupNode) {
    if(toFixupNode == toFixupNode->parent->right) {
        //LRb
        //1.toFixupNodeParent更新为toFixupNode
        //2.对toFixupNode子树进行左旋
        toFixupNode = toFixupNode->parent;
        leftRotate(toFixupNode);
    }
    //LLb
    //1.toFixupNode的父亲结点变为黑色
    //2.toFixupNode的祖父结点变为红色
    //3.对toFixupNode的祖父结点为根的子树进行右旋
    toFixupNode->parent->color = BLACK;
    toFixupNode->parent->parent->color = RED;
    rightRotate(toFixupNode->parent->parent);
}

void RBTree::changeColorAndRotateRXb(RBTreeNode* toFixupNode) {
    if(toFixupNode == toFixupNode->parent->left) {
        //RLb
        //1.toFixupNodeParent更新为toFixupNode
        //2.对toFixupNode子树进行右旋
        toFixupNode = toFixupNode->parent;
        rightRotate(toFixupNode);
    }
    //RRb
    //1.toFixupNode的父亲结点变为黑色
    //2.toFixupNode的祖父结点变为红色
    //3.对toFixupNode的祖父结点为根的子树进行左旋
    toFixupNode->parent->color = BLACK;
    toFixupNode->parent->parent->color = RED;
    leftRotate(toFixupNode->parent->parent);
}

void RBTree::midorderTraversal() const {
    midorderTraversalRecursive(root);
    std::cout<<"\n\n";
}

void RBTree::midorderTraversalRecursive(RBTreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    midorderTraversalRecursive(rootOfTree->left);
    printf("key:%d, color:%d\n", rootOfTree->key, rootOfTree->color);
    midorderTraversalRecursive(rootOfTree->right);
}

void RBTree::preorderTraversal() const {
    preorderTraversalRecursive(root);
    std::cout<<"\n\n";
}

void RBTree::preorderTraversalRecursive(RBTreeNode* rootOfTree) const {
    if(rootOfTree == nullptr) {
        return;
    }
    printf("key:%d, color:%d\n", rootOfTree->key, rootOfTree->color);
    preorderTraversalRecursive(rootOfTree->left);
    preorderTraversalRecursive(rootOfTree->right);
}

int main()
{
    vector<int> keyDatas = {
        10, 50, 60, 62, 65, 70
    };
    RBTree* rbtree = new RBTree();
    RBTreeNode* node = nullptr;
    for(auto key : keyDatas) {
        node = new RBTreeNode(key);
        rbtree->insertNode(node);
    }
    rbtree->midorderTraversal();
    rbtree->preorderTraversal();

    delete rbtree;
    rbtree = nullptr;
}

