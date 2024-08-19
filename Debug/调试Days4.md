# 调试Days4

## 今日任务

【使用gdb排查c++程序死锁的方法-哔哩哔哩】 https://b23.tv/JEkBGry



## 代码

```c++
/* ************************************************************************
> File Name:     deadlock.cpp
> Author:        niu0217
> Created Time:  Mon 19 Aug 2024 02:59:38 PM CST
> Description:   
 ************************************************************************/

#include <iostream>
#include <mutex>
#include <unistd.h>
#include <thread>
using namespace std;

mutex mtx1;
mutex mtx2;

void thread_func1()
{
  cout << "Thread 1 run.\n";
  lock_guard<mutex> lck1(mtx1);
  sleep(1);
  lock_guard<mutex> lck2(mtx2);
  cout << "Thread 1 exit.\n";
}

void thread_func2()
{
  cout << "Thread 2 run.\n";
  lock_guard<mutex> lck2(mtx2);
  sleep(1);
  lock_guard<mutex> lck1(mtx1);
  cout << "Thread 2 exit.\n";
}

int main()
{
  thread t1(thread_func1);
  thread t2(thread_func2);

  sleep(1);

  t1.join();
  t2.join();
}
```

