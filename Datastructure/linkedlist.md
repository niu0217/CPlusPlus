# linkedlist

## 1. 问题

## 2. 引入代码

```c++
#include<iostream>

struct LinkedNode {
    int nodeData;
    LinkedNode* nextNode;
    LinkedNode(int initialNodeData)
        : nodeData{ initialNodeData }, nextNode{ nullptr } { }
};

class LinkedList {
public:
    LinkedList()
        : headNode{ nullptr }, tailNode{ nullptr }, linkedlistSize{ 0 } { }
    ~LinkedList() {
        clearLinkedList();
        std::cout<<"Delete all LinkedNode...."<<std::endl;
    }
    void insertToEndOfLinkedList(int newNodeData);
    void insertToIndex(int index, int newNodeData);
    void removeToIndex(int index);
    int findLinkedNode(int findNodeData) const;
    void clearLinkedList();
    void printLinkedList() const;

    bool checkIndex(int index);
private:
    LinkedNode* headNode;
    LinkedNode* tailNode;
    int linkedlistSize;
};

void LinkedList::insertToEndOfLinkedList(int newNodeData) {
    LinkedNode* newNode = new LinkedNode(newNodeData);
    if(headNode == nullptr) {
        headNode = tailNode = newNode;
    }
    else {
        tailNode->nextNode = newNode;
        tailNode = newNode;
    }
    linkedlistSize++;
}

void LinkedList::insertToIndex(int index, int newNodeData) {
    if(!checkIndex(index)) {
        return;
    }
    if(index == 0) {
        LinkedNode* newNode = new LinkedNode(newNodeData);
        newNode->nextNode = headNode;
        headNode = newNode;
    }
    else {
        LinkedNode* prevNodeOfInsertPosition = headNode;
        for(int i = 0; i < index - 1; i++) {
            prevNodeOfInsertPosition = prevNodeOfInsertPosition->nextNode;
        }
        LinkedNode* newNode = new LinkedNode(newNodeData);
        newNode->nextNode = prevNodeOfInsertPosition->nextNode;
        prevNodeOfInsertPosition->nextNode = newNode;
        if(prevNodeOfInsertPosition == tailNode) {
            tailNode = newNode;
        }
    }
    linkedlistSize++;
}

void LinkedList::removeToIndex(int index) {
    if(!checkIndex(index)) {
        return;
    }
    if(index == 0) {
        LinkedNode* headNextNode = headNode->nextNode;
        delete headNode;
        headNode = headNextNode;
        if(headNode == nullptr) {
            tailNode = nullptr;
        }
    }
    else {
        LinkedNode* prevNodeOfDeletePosition = headNode;
        for(int i = 0; i < index - 1; i++) {
            prevNodeOfDeletePosition = prevNodeOfDeletePosition->nextNode;
        }
        LinkedNode* toDeleteNode = prevNodeOfDeletePosition->nextNode;
        prevNodeOfDeletePosition->nextNode = toDeleteNode->nextNode;
        if(toDeleteNode == tailNode) {
            tailNode = prevNodeOfDeletePosition;
        }
        delete toDeleteNode;
    }
    linkedlistSize--;
}

int LinkedList::findLinkedNode(int findNodeData) const {
    LinkedNode* currentNode = headNode;
    for(int i = 0; i < linkedlistSize; i++) {
        if(currentNode->nodeData == findNodeData) {
            return i;
        }
        currentNode = currentNode->nextNode;
    }
    return -1;
}

void LinkedList::clearLinkedList() {
    LinkedNode* toDeleteNode = headNode;
    LinkedNode* nextToDeleteNode = nullptr;
    while(toDeleteNode != nullptr) {
        nextToDeleteNode = toDeleteNode->nextNode;
        delete toDeleteNode;
        toDeleteNode = nextToDeleteNode;
    }
    headNode = tailNode = nullptr;
    linkedlistSize = 0;
}

void LinkedList::printLinkedList() const {
    LinkedNode* toPrintNode = headNode;
    while(toPrintNode != nullptr) {
        std::cout<<toPrintNode->nodeData<<" ";
        toPrintNode = toPrintNode->nextNode;
    }
    std::cout<<std::endl<<std::endl;
}

bool LinkedList::checkIndex(int index) {
    if(index < 0 || index > linkedlistSize) {
        std::cout<<"Index out of range."<<std::endl;
        return false;
    }
    return true;
}

int main()
{
    LinkedList* linkedList = new LinkedList();
    linkedList->insertToEndOfLinkedList(10);
    linkedList->insertToEndOfLinkedList(20);
    linkedList->insertToEndOfLinkedList(30);
    linkedList->insertToEndOfLinkedList(40);
    linkedList->insertToEndOfLinkedList(50);
    linkedList->insertToEndOfLinkedList(60);
    linkedList->insertToEndOfLinkedList(70);
    linkedList->insertToEndOfLinkedList(80);
    linkedList->insertToEndOfLinkedList(90);
    linkedList->insertToEndOfLinkedList(100);
    std::cout<<"初始链表：\n";
    linkedList->printLinkedList();

    std::cout<<"在链表的index=5的位置插入数据999: \n";
    linkedList->insertToIndex(5, 999);
    linkedList->printLinkedList();

    std::cout<<"删除链表中的index=7的节点：\n";
    linkedList->removeToIndex(7);
    linkedList->printLinkedList();

    std::cout<<"查找节点数据为40的节点：\n";
    if(linkedList->findLinkedNode(40)) {
        std::cout<<"Find.....\n"<<std::endl;
    }
    else {
        std::cout<<"Not find...\n"<<std::endl;
    }

    std::cout<<"删除所有节点：\n";
    delete linkedList;
}
```



