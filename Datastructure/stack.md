# stack

## 1. 问题

## 2. 引入代码

```c++
#include<iostream>
#include<vector>
#include<stdexcept>

class Stack {
public:
    void push(int element);
    void pop();
    int top() const;
    bool isEmpty() const {
        return elements.empty();
    }
private:
    std::vector<int> elements;  //使用vector作为实现栈的底层容器
};

void Stack::push(int element) {
    elements.push_back(element);
}

void Stack::pop() {
    if(isEmpty()) {
        throw std::out_of_range("Stack<>::pop():empty Stack");
    }
    elements.pop_back();
}

int Stack::top() const {
    if(isEmpty()) {
        throw std::out_of_range("Stack<>::pop():empty Stack");
    }
    return elements.back();
}

int main()
{
    try {
        Stack stack;
        stack.push(7);
        std::cout << stack.top() << std::endl; // 输出：7
        stack.pop();
        std::cout << stack.isEmpty() << std::endl; // 输出：1（true）
        stack.top(); // 将会抛出std::out_of_range异常，因为栈是空的
    } catch (const std::exception& ex) {
        std::cerr << "Exception: " << ex.what() << std::endl; // 输出异常信息
        return -1; // 返回非零值表示程序异常终止
    }
    return 0; // 程序正常退出，返回0值
}
```

## 3. 用queue实现栈

```c++
#include<iostream>
#include<queue>
#include<stdexcept>

template<typename T>
class Stack {
public:
    void push(const T& element);
    T pop();
    T top() const;
    bool empty() const {
        return mainQueue.empty();
    }
private:
    void moveElementsExceptLastElement();
    std::queue<T> mainQueue;
    std::queue<T> helpQueue;
};

template<typename T>
void Stack<T>::moveElementsExceptLastElement() {
    while(mainQueue.size() > 1) {
        helpQueue.push(mainQueue.front());
        mainQueue.pop();
    }
}

template<typename T>
void Stack<T>::push(const T& element) {
    mainQueue.push(element);
}

template<typename T>
T Stack<T>::pop() {
    if(mainQueue.empty()) {
        throw std::out_of_range("Stack is empty!");
    }

    moveElementsExceptLastElement();

    T topElement = mainQueue.front();
    mainQueue.pop();
    std::swap(mainQueue, helpQueue);  //将mainQueue的数据恢复，除了第一个元素
    return topElement;
}

template<typename T>
T Stack<T>::top() const {
    if(mainQueue.empty()) {
        throw std::out_of_range("Stack is empty!");
    }
    std::queue<T> copyMainQueue = mainQueue;  //为了不改变mainQueue本身
    while(copyMainQueue.size() > 1) {
        copyMainQueue.pop();
    }
    return copyMainQueue.front();  //此时只剩最后一个元素
}

int main()
{
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);
    printf("%d\n", stack.top());
    printf("%d\n", stack.pop());
    printf("%d\n", stack.top());
}
```

