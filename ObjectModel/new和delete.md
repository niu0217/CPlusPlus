# new和delete

## 1. malloc来分配0个字节

很糟糕的做法！！！！！！！！

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

void func() {
    void* p = malloc(0);
    // char* p = new char[0];

    int value;
    value = 1;
}

int main()
{
    func();
}
```

`p`中的内容修改必出错！！！！！

## 2. new内存分配细节分析

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
#include<string.h>
#include<stdio.h>
using namespace std;

void func() {
    char* ppoint = new char[10];
    memset(ppoint, 0, 10);
    delete []ppoint;
}

int main()
{
    func();
}
```

一块内存的回收，影响范围很广，远远不是10字节，而是一大片。

## 3. 重载类中的operator new/delete

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
using namespace std;

class A {
public:
    static void* operator new(size_t size);
    static void operator delete(void* phead);
public:
    A() {
        cout<<"A::A()\n";
    }
    ~A() {
        cout<<"A::~A()\n";
    }
};

void* A::operator new(size_t size) {
    A* ppoint = (A*)malloc(size);
    return ppoint;
}

void A::operator delete(void* phead) {
    free(phead);
}

void func() {
  	//先调用我们自己的operator new，然后调用A的构造函数
    A* pa = new A();
    //先调用A的析构函数，然后调用我们自己的operator delete
    delete pa;
}

void func2() {
    //调用编译器的operator new和operator delete
    A* pa = ::new A();
    ::delete pa;
}

int main()
{
    func();
    func2();
}
```

## 4. 重载类中的operator new[]/delete[]

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
using namespace std;

class A {
public:
    static void* operator new[](size_t size);
    static void operator delete[](void* phead);
public:
    A() {
        cout<<"A::A()\n";
    }
    ~A() {
        cout<<"A::~A()\n";
    }
};

void* A::operator new[](size_t size) {
    A* ppoint = (A*)malloc(size);
    return ppoint;
}

void A::operator delete[](void* phead) {
    free(phead);
}

void func() {
    //构造和析构函数被调用3次，
    //但是operator new[]和operator delete[]只被调用一次
    A* pa = new A[3];
    delete []pa;
}

int main()
{
    func();
}
```

调试：

```c++
In file: /home/ubuntu/Dev/C++ObjectModern/constructor/test.cpp
   21     }
   22 };
   23
   24 void* A::operator new[](size_t size) {
   25     A* ppoint = (A*)malloc(size);
 ► 26     return ppoint;
   27 }
   28
   29 void A::operator delete[](void* phead) {
   30     free(phead);
   31 }

pwndbg> p ppoint
$7 = (A *) 0x55555556aeb0
pwndbg> x/3gx 0x55555556aeb0
0x55555556aeb0:	0x0000000000000000	0x0000000000000000
0x55555556aec0:	0x0000000000000000
```

```c++
In file: /home/ubuntu/Dev/C++ObjectModern/constructor/test.cpp
   32
   33 void func() {
   34     //构造和析构函数被调用3次，
   35     //但是operator new[]和operator delete[]只被调用一次
   36     A* pa = new A[3];
 ► 37     delete []pa;
   38 }
   39
   40 int main()
   41 {
   42     func();
     
pwndbg> p pa
$8 = (A *) 0x55555556aeb8
pwndbg> x/3gx 0x55555556aeb0
0x55555556aeb0:	0x0000000000000003	0x0000000000000000
0x55555556aec0:	0x0000000000000000
```

从这里我们看到`operator new[]`申请的是`0x55555556aeb0`，但是返回的却是`0x55555556aeb8`，相差了8个字节，这8个字节用来记录大小`0x55555556aeb0:	0x0000000000000003`

## 5. 重载全局operator new/delete

不常用，影响范围太广；

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
#include<ctime>
using namespace std;

void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void* phead) {
    free(phead);
}

void operator delete[](void* phead) {
    free(phead);
}

class A {
public:
    A() {
        cout<<"A::A()\n";
    }
    ~A() {
        cout<<"A::~A()\n";
    }
};

void func() {
    int* pint = new int(12);
    delete pint;

    char* parr = new char[10];
    delete []parr;

    A* p = new A();
    delete p;

    A* pa = new A[3];
    delete []pa;

    return;
}

int main()
{
    func();
}
```

## 6. 重载全局的operator new[]/delete[]

不常用，影响范围太广；

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
#include<ctime>
using namespace std;

void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

void operator delete(void* phead) {
    free(phead);
}

void operator delete[](void* phead) {
    free(phead);
}

class A {
public:
    A() {
        cout<<"A::A()\n";
    }
    ~A() {
        cout<<"A::~A()\n";
    }
};

void func() {
    int* pint = new int(12);
    delete pint;

    char* parr = new char[10];
    delete []parr;

    A* p = new A();
    delete p;

    A* pa = new A[3];
    delete []pa;

    return;
}

int main()
{
    func();
}
```

## 7. 定位new (placement new)

有`placement new`，但是没有对应的`placement delete`。

功能：在已经分配的原始内存中初始化一个对象。

+ `placement new`并不分配内存，你需要提前将这个`placement new`要使用的内存分配出来；
+ 初始化一个对象（初始化一个对象的内存），可以理解为调用这个对象的构造函数；

总结：`placement new`就是能够在一个预先分配好的内存地址中构造一个对象。

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
#include<ctime>
using namespace std;

class A {
public:
    int m_a;
    A() : m_a{0} {

    }
    A(int value) : m_a{value} {

    }
    ~A() {
        cout<<"A::~A()\n";
    }
};

void func() {
    void* mymemPoint = (void*)new char[sizeof(A)]; //事先分配好内存
    A* pmyAobj1 = new (mymemPoint) A(); //特别注意：这里不会额外分配内存

    void* mymemPoint2 = (void*)new char[sizeof(A)];
    A* pmyAobj2 = new (mymemPoint2) A(12);

    //释放内存：方法一
    // delete pmyAobj1;
    // delete pmyAobj2;

    //释放内存：方法二
    pmyAobj1->~A();
    pmyAobj2->~A();
    delete[](void*)pmyAobj1;
    delete[](void*)pmyAobj2;
}

int main()
{
    func();
}
```

## 8. 重载多个版本的new

```c++
/* ************************************************************************
> File Name:     test.cpp
> Author:        niu0217
> Created Time:  Thu 08 Feb 2024 04:50:35 PM CST
> Description:
 ************************************************************************/

#include<iostream>
#include<ctime>
using namespace std;

class A {
public:
  	//operator new的第一个参数是固定的，编译器自动传入
    void* operator new(size_t size, int arg1, int arg2) {
        return NULL;
    }
};

void func() {
    A* pa = new (1234, 56) A();
}

int main()
{
    func();
}
```

注意：这个时候我们的自定义new不会调用A的构造函数！！！！！！！
