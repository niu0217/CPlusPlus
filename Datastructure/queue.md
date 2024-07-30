# queue

## 1. 问题

## 2. 引入代码

```c++
#include<iostream>
#include<vector>
#include<stdexcept>

class Queue {
public:
    bool empty() const {
        return elements.empty();
    }
    size_t size() const {
        return elements.size();
    }
    void push(int element) {
        elements.push_back(element);
    }
    int pop() {
        if(empty()) {
            throw std::out_of_range("Queue is empty, cannot dequeue!");
        }
        int front = elements.front();
        elements.erase(elements.begin());
        return front;
    }
    int front() const {
        if(empty()) {
            throw std::out_of_range("Queue is empty, cannot dequeue!");
        }
        return elements.front();
    }
private:
    std::vector<int> elements;
};

int main()
{
    Queue queue;
    queue.push(1);
    queue.push(2);
    queue.push(3);
    queue.push(4);
    queue.push(5);
    queue.push(6);
    printf("%d\n", queue.pop());
    printf("%d\n", queue.front());
}

```

## 3. 两个栈实现队列

```c++
#include<iostream>
#include<stack>
#include<stdexcept>

class MyQueue {
public:
    MyQueue() { }
    void push(int element);  //入队
    int pop();  //出队
    int peek();  //取得队首元素
    bool empty() const {
        return inputStack.empty() && outputStack.empty();
    }
private:
    //将元素从inputStack移动到outputStack.
    //方便可以从outputStack弹出最旧的元素
    void moveElementsToOutputStack();
    std::stack<int> inputStack;
    std::stack<int> outputStack;
};

void MyQueue::moveElementsToOutputStack() {
    while(inputStack.size() > 0) {
        int topElement = inputStack.top();
        outputStack.push(topElement);
        inputStack.pop();
    }
}

void MyQueue::push(int element) {
    inputStack.push(element);
}

int MyQueue::pop() {
    //如果outputStack是空的，那么将inputStack赋值给它
    if(outputStack.empty()) {
        moveElementsToOutputStack();
    }
    //如果还是空的，那么就说明队列为空
    if(outputStack.empty()) {
        throw std::runtime_error("Queue is empty, cannot pop!");
    }
    int frontElement = outputStack.top();
    outputStack.pop();
    return frontElement;
}

int MyQueue::peek() {
    //如果outputStack是空的，那么将inputStack赋值给它
    if(outputStack.empty()) {
        moveElementsToOutputStack();
    }
    //如果还是空的，那么就说明队列为空
    if(outputStack.empty()) {
        throw std::runtime_error("Queue is empty, cannot pop!");
    }
    return outputStack.top();
}

int main()
{
    MyQueue queue;
    queue.push(1); // 队列现在是: [1]
    queue.push(2); // 队列现在是: [1, 2] (第一个元素是队列的前面)
    std::cout << queue.peek() << std::endl; // 返回 1，队列仍然是 [1, 2]
    std::cout << queue.pop() << std::endl;  // 返回 1，队列现在是 [2]
    std::cout << queue.empty() << std::endl; // 返回 false，队列现在不是空的
    queue.push(3); // 将3推入已经包含2的队列，队列现在是 [2, 3]
    std::cout << queue.peek() << std::endl; // 返回 2，队列仍然是 [2, 3]
    std::cout << queue.pop() << std::endl;  // 返回 2，队列现在是 [3]
    std::cout << queue.pop() << std::endl;  // 返回 3，队列现在是空的
    std::cout << queue.empty() << std::endl; // 返回 true，因为队列是空的
    return 0;
}
```

