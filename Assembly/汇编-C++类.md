# 汇编-C++类

## 1. 案例一

代码：

```c++
#include<iostream>
#include<vector>

struct TreeNode {
    int value_;
    TreeNode* left_;
    TreeNode* right_;
    TreeNode(int value) {
        value_ = value;
        left_ = nullptr;
        right_ = nullptr;
    }
};

int main()
{
    TreeNode* node1 = new TreeNode(1);
    TreeNode* node2 = new TreeNode(2);
    TreeNode* node3 = new TreeNode(3);

    node1->left_ = node2;
    node1->right_ = node3;
    return 0;
}
```

分析：

```assembly
──────────────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]──────────────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/disassemby/test.cpp
   18     TreeNode* node2 = new TreeNode(2);
   19     TreeNode* node3 = new TreeNode(3);
   20
   21     node1->left_ = node2;
   22     node1->right_ = node3;
 ► 23     return 0;
   24 }
──────────────────────────────────────────────────────────────────────────────────[ STACK ]──────────────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe900 —▸ 0x7ffff7dd62e8 (__exit_funcs_lock) ◂— 0x0
01:0008│     0x7fffffffe908 —▸ 0x55555556aeb0 ◂— 0x1
02:0010│     0x7fffffffe910 —▸ 0x55555556aed0 ◂— 0x2
03:0018│     0x7fffffffe918 —▸ 0x55555556aef0 ◂— 0x3
04:0020│     0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│     0x7fffffffe928 —▸ 0x5555555552b0 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe930 ◂— 0x0
07:0038│     0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
────────────────────────────────────────────────────────────────────────────────[ BACKTRACE ]────────────────────────────────────────────────────────────────────────────────
 ► 0   0x555555555208 main+127
   1   0x7ffff7c09083 __libc_start_main+243
```

```bash
pwndbg> p node1
$4 = (TreeNode *) 0x55555556aeb0
pwndbg> p node2
$5 = (TreeNode *) 0x55555556aed0
pwndbg> p node3
$6 = (TreeNode *) 0x55555556aef0
pwndbg>
```

可以看到node之间相差了32个字节。我们查看一下node1开始的96个字节：

```bash
pwndbg> x/12gx 0x55555556aeb0
0x55555556aeb0:	0x0000000000000001	0x000055555556aed0
0x55555556aec0:	0x000055555556aef0	0x0000000000000021
0x55555556aed0:	0x0000000000000002	0x0000000000000000
0x55555556aee0:	0x0000000000000000	0x0000000000000021
0x55555556aef0:	0x0000000000000003	0x0000000000000000
0x55555556af00:	0x0000000000000000	0x000000000000f101
pwndbg>
```

node1的地址范围：0x55555556aeb0～0x55555556aed0

+ 0x55555556aeb0取8个字节：0x0000000000000001。而这个地址里面的前4个字节就代表value_的值。
+ 0x55555556aeb8取8个字节：0x000055555556aed0。这个地址的8个字节里面保存的就是left_的值。
+ 0x55555556aec0取8个字节：0x000055555556aef0。这个地址的8个字节里面保存的就是right_的值。
+ 0x55555556aec8取8个字节：0x0000000000000021。这个地址应该是为了内存对齐而设置的。

## 2. 案例二

代码：

```c++
#include<iostream>
#include<vector>

struct TreeNode {
    TreeNode* left_;
    int value_;
    TreeNode* right_;
    TreeNode(int value) {
        value_ = value;
        left_ = nullptr;
        right_ = nullptr;
    }
};

int main()
{
    TreeNode* node1 = new TreeNode(1);
    TreeNode* node2 = new TreeNode(2);
    TreeNode* node3 = new TreeNode(3);

    node1->left_ = node2;
    node1->right_ = node3;
    return 0;
}
```

分析：

```assembly
──────────────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]──────────────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/disassemby/test.cpp
   18     TreeNode* node2 = new TreeNode(2);
   19     TreeNode* node3 = new TreeNode(3);
   20
   21     node1->left_ = node2;
   22     node1->right_ = node3;
 ► 23     return 0;
   24 }
──────────────────────────────────────────────────────────────────────────────────[ STACK ]──────────────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe900 —▸ 0x7ffff7dd62e8 (__exit_funcs_lock) ◂— 0x0
01:0008│     0x7fffffffe908 —▸ 0x55555556aeb0 —▸ 0x55555556aed0 ◂— 0x0
02:0010│     0x7fffffffe910 —▸ 0x55555556aed0 ◂— 0x0
03:0018│     0x7fffffffe918 —▸ 0x55555556aef0 ◂— 0x0
04:0020│     0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│     0x7fffffffe928 —▸ 0x5555555552b0 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe930 ◂— 0x0
07:0038│     0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

```bash
pwndbg> p node1
$1 = (TreeNode *) 0x55555556aeb0
pwndbg> p node2
$2 = (TreeNode *) 0x55555556aed0
pwndbg> p node3
$3 = (TreeNode *) 0x55555556aef0
pwndbg>
```

可以看到node之间相差了32个字节。我们查看一下node1开始的96个字节：

```bash
pwndbg> x/12gx 0x55555556aeb0
0x55555556aeb0:	0x000055555556aed0	0x0000000000000001
0x55555556aec0:	0x000055555556aef0	0x0000000000000021
0x55555556aed0:	0x0000000000000000	0x0000000000000002
0x55555556aee0:	0x0000000000000000	0x0000000000000021
0x55555556aef0:	0x0000000000000000	0x0000000000000003
0x55555556af00:	0x0000000000000000	0x000000000000f101
pwndbg>
```

node1的地址范围：0x55555556aeb0～0x55555556aed0

+ 0x55555556aeb0取8个字节：0x000055555556aed0。这个地址里面的8个字节就代表left_的值。
+ 0x55555556aeb8取8个字节：0x0000000000000001。这个地址的4个字节里面保存的就是value_的值。
+ 0x55555556aec0取8个字节：0x000055555556aef0。这个地址的8个字节里面保存的就是right_的值。
+ 0x55555556aec8取8个字节：0x0000000000000021。这个地址应该是为了内存对齐而设置的。

需要注意的是，我们定义变量的顺序是：

```c++
struct TreeNode {
    TreeNode* left_;
    int value_;
    TreeNode* right_;
    TreeNode(int value) {
        value_ = value;
        left_ = nullptr;
        right_ = nullptr;
    }
};
```

这个刚好和内存中存放的方式联系起来了。