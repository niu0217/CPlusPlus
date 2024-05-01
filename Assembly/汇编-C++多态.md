# 汇编-C++多态

## 1. 案例1

代码：

```c++
#include<iostream>
using namespace std;

class Animal {
public:
    virtual void speak() {
        cout<<"I am base"<<endl;
    }
    virtual void run() {
        cout<<"Running"<<endl;
    }
};

class Cat: public Animal {
public:
    void speak() override {
        cout<<"I am a cat"<<endl;
    }
};

class Dog: public Animal {
public:
    void run() override {
        cout<<"Dog running"<<endl;
    }
};

int main()
{
    Cat cat_object;
    Dog dog_object;
    Animal *animal_object_ptr = new Animal();
    animal_object_ptr = &cat_object;
    animal_object_ptr->speak();
    animal_object_ptr->run();
}
```

### 步骤一

```assembly
─────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x555555555203 <main+26>    xor    eax, eax
   0x555555555205 <main+28>    lea    rax, [rip + 0x2b04]           <vtable for Cat+16>
   0x55555555520c <main+35>    mov    qword ptr [rbp - 0x30], rax
   0x555555555210 <main+39>    lea    rax, [rip + 0x2ad9]           <vtable for Dog+16>
   0x555555555217 <main+46>    mov    qword ptr [rbp - 0x28], rax
 ► 0x55555555521b <main+50>    mov    edi, 8
   0x555555555220 <main+55>    call   operator new(unsigned long)@plt                <operator new(unsigned long)@plt>

   0x555555555225 <main+60>    mov    rbx, rax
   0x555555555228 <main+63>    mov    qword ptr [rbx], 0
   0x55555555522f <main+70>    mov    rdi, rbx
   0x555555555232 <main+73>    call   Animal::Animal()                <Animal::Animal()>
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe910 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│     0x7fffffffe918 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│     0x7fffffffe920 ◂— 0x0
03:0018│     0x7fffffffe928 ◂— 0x6e306fcee6a5e600
04:0020│     0x7fffffffe930 —▸ 0x7fffffffea30 ◂— 0x1
05:0028│     0x7fffffffe938 —▸ 0x555555555410 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe940 ◂— 0x0
07:0038│     0x7fffffffe948 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe910**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe918**
+ 值：[rbp - 0x28] = **0x555555557cf0**

```assembly
pwndbg> info vtbl cat_object
vtable for 'Cat' @ 0x555555557d10 (subobject @ 0x7fffffffe910):
[0]: 0x55555555536c <Cat::speak()>
[1]: 0x555555555330 <Animal::run()>
pwndbg> info vtbl dog_object
vtable for 'Dog' @ 0x555555557cf0 (subobject @ 0x7fffffffe918):
[0]: 0x5555555552f4 <Animal::speak()>
[1]: 0x5555555553a8 <Dog::run()>
pwndbg>
```

分析：

+ 0x555555557d10是对象cat_object的虚表指针。后面紧跟的就是虚函数的函数指针地址。
+ 0x555555557cf0是对象dog_object的虚表指针。后面紧跟的就是虚函数的函数指针地址。

### 步骤二

```assembly
────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x55555555523b <main+82>     lea    rax, [rbp - 0x30]
   0x55555555523f <main+86>     mov    qword ptr [rbp - 0x20], rax
 ► 0x555555555243 <main+90>     mov    rax, qword ptr [rbp - 0x20]
   0x555555555247 <main+94>     mov    rax, qword ptr [rax]
   0x55555555524a <main+97>     mov    rdx, qword ptr [rax]
   0x55555555524d <main+100>    mov    rax, qword ptr [rbp - 0x20]
   0x555555555251 <main+104>    mov    rdi, rax
   0x555555555254 <main+107>    call   rdx

   0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20]
   0x55555555525a <main+113>    mov    rax, qword ptr [rax]
   0x55555555525d <main+116>    add    rax, 8
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function.cpp
   29 {
   30     Cat cat_object;
   31     Dog dog_object;
   32     Animal *animal_object_ptr = new Animal();
   33     animal_object_ptr = &cat_object;
 ► 34     animal_object_ptr->speak();
   35     animal_object_ptr->run();
   36 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rax rsp 0x7fffffffe910 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│         0x7fffffffe918 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│         0x7fffffffe920 —▸ 0x7fffffffe910 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
03:0018│         0x7fffffffe928 ◂— 0x6e306fcee6a5e600
04:0020│         0x7fffffffe930 —▸ 0x7fffffffea30 ◂— 0x1
05:0028│         0x7fffffffe938 —▸ 0x555555555410 (__libc_csu_init) ◂— endbr64
06:0030│ rbp     0x7fffffffe940 ◂— 0x0
07:0038│         0x7fffffffe948 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe910**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe918**
+ 值：[rbp - 0x28] = **0x555555557cf0**

对象animal_object_ptr：

+ 地址：rbp - 0x20 = **0x7fffffffe920**
+ 值：[rbp - 0x20] = **0x7fffffffe910**

### 步骤三

延续步骤二，分析调用过程`animal_object_ptr->speak();`

```assembly
0x555555555243 <main+90>     mov    rax, qword ptr [rbp - 0x20]
0x555555555247 <main+94>     mov    rax, qword ptr [rax]
0x55555555524a <main+97>     mov    rdx, qword ptr [rax]
0x55555555524d <main+100>    mov    rax, qword ptr [rbp - 0x20]
0x555555555251 <main+104>    mov    rdi, rax
0x555555555254 <main+107>    call   rdx
```

指令：`0x555555555243 <main+90>     mov    rax, qword ptr [rbp - 0x20]`

+ 含义：用寄存器rax保存对象cat_object的地址

指令：`0x555555555247 <main+94>     mov    rax, qword ptr [rax]`

+ 含义：用寄存器rax保存虚表指针

指令：`0x55555555524a <main+97>     mov    rdx, qword ptr [rax]`

+ 含义：用寄存器rdx保存要调用的虚函数的地址。

指令：`0x555555555254 <main+107>    call   rdx`

+ 含义：调用虚函数

### 步骤四

```assembly
─────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x55555555524d <main+100>    mov    rax, qword ptr [rbp - 0x20]
   0x555555555251 <main+104>    mov    rdi, rax
   0x555555555254 <main+107>    call   rdx

   0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20]
   0x55555555525a <main+113>    mov    rax, qword ptr [rax]
 ► 0x55555555525d <main+116>    add    rax, 8                        <vtable for Cat+16>
   0x555555555261 <main+120>    mov    rdx, qword ptr [rax]
   0x555555555264 <main+123>    mov    rax, qword ptr [rbp - 0x20]
   0x555555555268 <main+127>    mov    rdi, rax
   0x55555555526b <main+130>    call   rdx

   0x55555555526d <main+132>    mov    eax, 0
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function.cpp
   30     Cat cat_object;
   31     Dog dog_object;
   32     Animal *animal_object_ptr = new Animal();
   33     animal_object_ptr = &cat_object;
   34     animal_object_ptr->speak();
 ► 35     animal_object_ptr->run();
   36 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe910 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│     0x7fffffffe918 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│     0x7fffffffe920 —▸ 0x7fffffffe910 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
03:0018│     0x7fffffffe928 ◂— 0x6e306fcee6a5e600
04:0020│     0x7fffffffe930 —▸ 0x7fffffffea30 ◂— 0x1
05:0028│     0x7fffffffe938 —▸ 0x555555555410 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe940 ◂— 0x0
07:0038│     0x7fffffffe948 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

分析：`animal_object_ptr->run();`

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe910**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe918**
+ 值：[rbp - 0x28] = **0x555555557cf0**

对象animal_object_ptr：

+ 地址：rbp - 0x20 = **0x7fffffffe920**
+ 值：[rbp - 0x20] = **0x7fffffffe910**

指令：`0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20]`

+ 含义：用rax保存对象cat_object的地址

指令：`0x55555555525a <main+113>    mov    rax, qword ptr [rax]`

+ 含义：用rax保存虚表指针

指令：`0x55555555525d <main+116>    add    rax, 8 `

+ 含义：用rax保存需要调用的虚函数地址

指令：` 0x555555555261 <main+120>    mov    rdx, qword ptr [rax]`

+ 含义：用寄存器rdx保存要调用的虚函数的地址。

## 2. 案例2

代码：

```c++
#include<iostream>
using namespace std;

class Animal {
public:
    virtual void speak() {
        cout<<"I am base"<<endl;
    }
    virtual void run() {
        cout<<"Running"<<endl;
    }
};

class Cat: public Animal {
public:
    void speak() override {
        cout<<"I am a cat"<<endl;
    }
    void run() override {
        cout<<"Cat running"<<endl;
    }
};

class Dog: public Animal {
public:
    void run() override {
        cout<<"Dog running"<<endl;
    }
};

int main()
{
    Cat cat_object;
    Dog dog_object;
    Animal *animal_object_ptr = new Animal();
    animal_object_ptr = &cat_object;
    animal_object_ptr->speak();
    animal_object_ptr->run();
}
```

### 步骤一

```assembly
─────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x555555555203 <main+26>    xor    eax, eax
   0x555555555205 <main+28>    lea    rax, [rip + 0x2b04]           <vtable for Cat+16>
   0x55555555520c <main+35>    mov    qword ptr [rbp - 0x30], rax
   0x555555555210 <main+39>    lea    rax, [rip + 0x2ad9]           <vtable for Dog+16>
   0x555555555217 <main+46>    mov    qword ptr [rbp - 0x28], rax
 ► 0x55555555521b <main+50>    mov    edi, 8
   0x555555555220 <main+55>    call   operator new(unsigned long)@plt                <operator new(unsigned long)@plt>

   0x555555555225 <main+60>    mov    rbx, rax
   0x555555555228 <main+63>    mov    qword ptr [rbx], 0
   0x55555555522f <main+70>    mov    rdi, rbx
   0x555555555232 <main+73>    call   Animal::Animal()                <Animal::Animal()>
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function_two.cpp
   30
   31 int main()
   32 {
   33     Cat cat_object;
   34     Dog dog_object;
 ► 35     Animal *animal_object_ptr = new Animal();
   36     animal_object_ptr = &cat_object;
   37     animal_object_ptr->speak();
   38     animal_object_ptr->run();
   39 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│     0x7fffffffe908 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│     0x7fffffffe910 ◂— 0x0
03:0018│     0x7fffffffe918 ◂— 0xd614087a287e0b00
04:0020│     0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│     0x7fffffffe928 —▸ 0x555555555440 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe930 ◂— 0x0
07:0038│     0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

```assembly
pwndbg> info vtbl cat_object
vtable for 'Cat' @ 0x555555557d10 (subobject @ 0x7fffffffe900):
[0]: 0x55555555536c <Cat::speak()>
[1]: 0x5555555553a8 <Cat::run()>
pwndbg> info vtbl dog_object
vtable for 'Dog' @ 0x555555557cf0 (subobject @ 0x7fffffffe908):
[0]: 0x5555555552f4 <Animal::speak()>
[1]: 0x5555555553e4 <Dog::run()>
```

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe900**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe908**
+ 值：[rbp - 0x28] = **0x555555557cf0**

### 步骤二

```assembly
─────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x55555555523b <main+82>     lea    rax, [rbp - 0x30]
   0x55555555523f <main+86>     mov    qword ptr [rbp - 0x20], rax
 ► 0x555555555243 <main+90>     mov    rax, qword ptr [rbp - 0x20]
   0x555555555247 <main+94>     mov    rax, qword ptr [rax]
   0x55555555524a <main+97>     mov    rdx, qword ptr [rax]
   0x55555555524d <main+100>    mov    rax, qword ptr [rbp - 0x20]
   0x555555555251 <main+104>    mov    rdi, rax
   0x555555555254 <main+107>    call   rdx

   0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20]
   0x55555555525a <main+113>    mov    rax, qword ptr [rax]
   0x55555555525d <main+116>    add    rax, 8
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function_two.cpp
   32 {
   33     Cat cat_object;
   34     Dog dog_object;
   35     Animal *animal_object_ptr = new Animal();
   36     animal_object_ptr = &cat_object;
 ► 37     animal_object_ptr->speak();
   38     animal_object_ptr->run();
   39 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rax rsp 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│         0x7fffffffe908 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│         0x7fffffffe910 —▸ 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
03:0018│         0x7fffffffe918 ◂— 0xd614087a287e0b00
04:0020│         0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│         0x7fffffffe928 —▸ 0x555555555440 (__libc_csu_init) ◂— endbr64
06:0030│ rbp     0x7fffffffe930 ◂— 0x0
07:0038│         0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe900**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe908**
+ 值：[rbp - 0x28] = **0x555555557cf0**

对象animal_object_ptr：

+ 地址：rbp - 0x20 = **0x7fffffffe910**
+ 值：[rbp - 0x20] = **0x7fffffffe900**

### 步骤三

分析：`animal_object_ptr->speak();`

```assembly
─────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x55555555523b <main+82>     lea    rax, [rbp - 0x30]
   0x55555555523f <main+86>     mov    qword ptr [rbp - 0x20], rax
   0x555555555243 <main+90>     mov    rax, qword ptr [rbp - 0x20]
   0x555555555247 <main+94>     mov    rax, qword ptr [rax]
   0x55555555524a <main+97>     mov    rdx, qword ptr [rax]
   0x55555555524d <main+100>    mov    rax, qword ptr [rbp - 0x20]
 ► 0x555555555251 <main+104>    mov    rdi, rax
   0x555555555254 <main+107>    call   rdx

   0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20]
   0x55555555525a <main+113>    mov    rax, qword ptr [rax]
   0x55555555525d <main+116>    add    rax, 8
   0x555555555261 <main+120>    mov    rdx, qword ptr [rax]
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function_two.cpp
   32 {
   33     Cat cat_object;
   34     Dog dog_object;
   35     Animal *animal_object_ptr = new Animal();
   36     animal_object_ptr = &cat_object;
 ► 37     animal_object_ptr->speak();
   38     animal_object_ptr->run();
   39 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rax rsp 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│         0x7fffffffe908 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│         0x7fffffffe910 —▸ 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
03:0018│         0x7fffffffe918 ◂— 0xfa55f58e67aba300
04:0020│         0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│         0x7fffffffe928 —▸ 0x555555555440 (__libc_csu_init) ◂— endbr64
06:0030│ rbp     0x7fffffffe930 ◂— 0x0
07:0038│         0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

寄存器rdx的值：

```assembly
 0x55555555523b <main+82>     lea    rax, [rbp - 0x30]   
 0x55555555523f <main+86>     mov    qword ptr [rbp - 0x20], rax
 0x555555555243 <main+90>     mov    rax, qword ptr [rbp - 0x20]  #rax=0x7fffffffe900
 0x555555555247 <main+94>     mov    rax, qword ptr [rax] #rax=0x555555557d10
 0x55555555524a <main+97>     mov    rdx, qword ptr [rax] #rax=[0x555555557d10]即0x55555555536c
```

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe900**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe908**
+ 值：[rbp - 0x28] = **0x555555557cf0**

对象animal_object_ptr：

+ 地址：rbp - 0x20 = **0x7fffffffe910**
+ 值：[rbp - 0x20] = **0x7fffffffe900**

### 步骤四

分析：`animal_object_ptr->run()`

```assembly
────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
   0x555555555254 <main+107>    call   rdx

   0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20]
   0x55555555525a <main+113>    mov    rax, qword ptr [rax]
   0x55555555525d <main+116>    add    rax, 8
   0x555555555261 <main+120>    mov    rdx, qword ptr [rax]
 ► 0x555555555264 <main+123>    mov    rax, qword ptr [rbp - 0x20]   <vtable for Cat+24>
   0x555555555268 <main+127>    mov    rdi, rax
   0x55555555526b <main+130>    call   rdx

   0x55555555526d <main+132>    mov    eax, 0
   0x555555555272 <main+137>    mov    rcx, qword ptr [rbp - 0x18]
   0x555555555276 <main+141>    xor    rcx, qword ptr fs:[0x28]
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function_two.cpp
   33     Cat cat_object;
   34     Dog dog_object;
   35     Animal *animal_object_ptr = new Animal();
   36     animal_object_ptr = &cat_object;
   37     animal_object_ptr->speak();
 ► 38     animal_object_ptr->run();
   39 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
01:0008│     0x7fffffffe908 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x5555555552f4 (Animal::speak()) ◂— endbr64
02:0010│     0x7fffffffe910 —▸ 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x55555555536c (Cat::speak()) ◂— endbr64
03:0018│     0x7fffffffe918 ◂— 0x75b001600e6cbd00
04:0020│     0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│     0x7fffffffe928 —▸ 0x555555555440 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe930 ◂— 0x0
07:0038│     0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```

对象cat_object：

+ 地址：rbp - 0x30 = **0x7fffffffe900**
+ 值：[rbp - 0x30] = **0x555555557d10**

对象dog_object：

+ 地址：rbp - 0x28 = **0x7fffffffe908**
+ 值：[rbp - 0x28] = **0x555555557cf0**

对象animal_object_ptr：

+ 地址：rbp - 0x20 = **0x7fffffffe910**
+ 值：[rbp - 0x20] = **0x7fffffffe900**

寄存器rdx的值：

```assembly
0x555555555256 <main+109>    mov    rax, qword ptr [rbp - 0x20] #rax=0x7fffffffe900
0x55555555525a <main+113>    mov    rax, qword ptr [rax]  #rax=0x555555557d10
0x55555555525d <main+116>    add    rax, 8  #rax=0x555555557d18
0x555555555261 <main+120>    mov    rdx, qword ptr [rax]  #[0x555555557d18]即0x5555555553a8
```

## 3. 案例3

代码：

```c++
#include<iostream>
using namespace std;

class Animal {
public:
    virtual void speak() {
        cout<<"I am base"<<endl;
    }
    virtual void run() {
        cout<<"Running"<<endl;
    }
    void sleeping() {
        cout<<"Animal sleeping"<<endl;
    }
};

class Cat: public Animal {
public:
    void speak() override {
        cout<<"I am a cat"<<endl;
    }
    void run() override {
        cout<<"Cat running"<<endl;
    }
};

class Dog: public Animal {
public:
    void run() override {
        cout<<"Dog running"<<endl;
    }
};

int main()
{
    Cat cat_object;
    Dog dog_object;
    Animal *animal_object_ptr = new Animal();
    animal_object_ptr = &cat_object;
    animal_object_ptr->speak();
    animal_object_ptr->run();
    animal_object_ptr->sleeping();
}
```

分析：` animal_object_ptr->sleeping();`

```assembly
─────────────────────────────────────────────────────────────[ DISASM / x86-64 / set emulate on ]──────────────────────────────────────────────────────────────
 ► 0x55555555526d <main+132>    mov    rax, qword ptr [rbp - 0x20]   <std::cout@@GLIBCXX_3.4>
   0x555555555271 <main+136>    mov    rdi, rax
   0x555555555274 <main+139>    call   Animal::sleeping()                <Animal::sleeping()>

   0x555555555279 <main+144>    mov    eax, 0
   0x55555555527e <main+149>    mov    rcx, qword ptr [rbp - 0x18]
   0x555555555282 <main+153>    xor    rcx, qword ptr fs:[0x28]
   0x55555555528b <main+162>    je     main+169                <main+169>

   0x55555555528d <main+164>    call   __stack_chk_fail@plt                <__stack_chk_fail@plt>

   0x555555555292 <main+169>    add    rsp, 0x28
   0x555555555296 <main+173>    pop    rbx
   0x555555555297 <main+174>    pop    rbp
───────────────────────────────────────────────────────────────────────[ SOURCE (CODE) ]───────────────────────────────────────────────────────────────────────
In file: /home/ubuntu/Dev/Test/polymorphism_function_three.cpp
   37     Dog dog_object;
   38     Animal *animal_object_ptr = new Animal();
   39     animal_object_ptr = &cat_object;
   40     animal_object_ptr->speak();
   41     animal_object_ptr->run();
 ► 42     animal_object_ptr->sleeping();
   43 }
───────────────────────────────────────────────────────────────────────────[ STACK ]───────────────────────────────────────────────────────────────────────────
00:0000│ rsp 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x5555555553b4 (Cat::speak()) ◂— endbr64
01:0008│     0x7fffffffe908 —▸ 0x555555557cf0 (vtable for Dog+16) —▸ 0x555555555300 (Animal::speak()) ◂— endbr64
02:0010│     0x7fffffffe910 —▸ 0x7fffffffe900 —▸ 0x555555557d10 (vtable for Cat+16) —▸ 0x5555555553b4 (Cat::speak()) ◂— endbr64
03:0018│     0x7fffffffe918 ◂— 0x5023dd909123e300
04:0020│     0x7fffffffe920 —▸ 0x7fffffffea20 ◂— 0x1
05:0028│     0x7fffffffe928 —▸ 0x555555555490 (__libc_csu_init) ◂— endbr64
06:0030│ rbp 0x7fffffffe930 ◂— 0x0
07:0038│     0x7fffffffe938 —▸ 0x7ffff7c09083 (__libc_start_main+243) ◂— mov edi, eax
```



