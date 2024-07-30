#include<iostream>
#include<vector>
#include<stdexcept>

class MinHeap {
public:
    size_t size() const;
    bool empty() const;
    void push(int element);
    int pop();
    int top() const;
private:
    std::vector<int> elements;  //用于保存堆数据

    //私有辅助函数
    void heapifyDown(size_t parent);  //向下调整堆
    void heapifyUp(size_t parent);  //向上调整堆
};

void MinHeap::heapifyDown(size_t parent) {
    size_t child = 2 * parent + 1;  //左孩子索引
    while(child < elements.size()) {
        //如果有右孩子并且右孩子比左孩子大，则选择右孩子
        if(child + 1 < elements.size() && elements[child + 1] > elements[child]) {
            child++;
        }
        //如果父结点大于孩子的值，则无需调整
        if(elements[parent] >= elements[child]) {
            break;
        }
        //否则，交换父节点和子节点
        std::swap(elements[parent], elements[child]);
        //继续向下调整
        parent = child;
        child = 2 * parent + 1;
    }
}

void MinHeap::heapifyUp(size_t child) {
    size_t parent = (child - 1) / 2;  //计算得到父节点的索引
    //如果子节点比父节点大，则交换它们并继续向上调整
    while(child > 0 && elements[child] > elements[parent]) {
        std::swap(elements[parent], elements[child]);
        child = parent;
        parent = (child - 1) / 2;
    }
}

size_t MinHeap::size() const {
    return elements.size();
}

bool MinHeap::empty() const {
    return elements.empty();
}

void MinHeap::push(int value) {
    elements.push_back(value);
    heapifyUp(elements.size() - 1);
}

int MinHeap::pop() {
    if(empty()) {
        throw std::runtime_error("Heap is empty, connot pop.");
    }
    int topElement = elements[0];
    elements[0] = elements.back();
    elements.pop_back();
    heapifyDown(0);
    return topElement;
}

int MinHeap::top() const {
    if(empty()) {
        throw std::runtime_error("Heap is empty.");
    }
    return elements[0];
}

int main()
{
    MinHeap minHeap;
    minHeap.push(5);
    minHeap.push(2);
    minHeap.push(9);
    minHeap.push(10);
    minHeap.push(23);
    minHeap.push(3);
    int size = minHeap.size();
    for(int i = 0; i < size; i++) {
        int topElement = minHeap.top();
        minHeap.pop();
        printf("%d ", topElement);
    }
    printf("\n");
}

