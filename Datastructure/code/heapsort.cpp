#include<iostream>
#include<vector>
#include<algorithm>

//调整堆结构
void heapify(std::vector<int>& elements, int minheapSize, int rootIndex) {
    int largest = rootIndex;  //初始化largest为根节点
    int left = 2 * rootIndex + 1;  //左子节点
    int right = 2 * rootIndex + 2;  //右子节点
    if(left < minheapSize && elements[left] > elements[largest]) {
        largest = left;
    }
    if(right < minheapSize && elements[right] > elements[largest]) {
        largest = right;
    }
    if(largest != rootIndex) {
        std::swap(elements[rootIndex], elements[largest]);
        heapify(elements, minheapSize, largest);
    }
}

//堆排序
void heapSort(std::vector<int>& elements) {
    int minheapSize = elements.size();
    //构建大顶堆，从最后一个非叶子节点开始调整堆结构
    for(int i = (minheapSize / 2) - 1; i >= 0; i--) {
        heapify(elements, minheapSize, i);
    }
    //从堆顶开始，将最大的元素交换到末尾，然后调整剩余元素
    //为堆结构，如此循环直到排好序
    for(int i = minheapSize - 1; i >= 0; i--) {
        std::swap(elements[0], elements[i]);
        heapify(elements, i, 0);
    }
}

int main()
{
    std::vector<int> elements = {
        12, 13, 11, 5, 6, 7
    };
    heapSort(elements);
    for(auto element : elements) {
        printf("%d ", element);
    }
    printf("\n");
}
