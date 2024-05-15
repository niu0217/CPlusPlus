# Linux下C++的OBJ文件解析

## 1. 案例一

### 1.1 准备工作

**代码：**

```c++
//constructor.cpp

#include<iostream>

class GrandFather {
public:
};

class FirstFather : virtual public GrandFather {
public:
};

class SecondFather : virtual public GrandFather {
public:
};

class Child : public FirstFather, public SecondFather {
public:
};

int main()
{
    Child object;
}
```

**生成obj文件：**

```bash
g++ -c constructor.cpp

objdump -x constructor.o > constructor.txt
```

### 1.2 分析obj文件

#### 1.2.1 片段一

```bash
RELOCATION RECORDS FOR [.eh_frame]:
OFFSET           TYPE              VALUE
0000000000000020 R_X86_64_PC32     .text._ZN11FirstFatherC2Ev
0000000000000040 R_X86_64_PC32     .text._ZN12SecondFatherC2Ev
0000000000000060 R_X86_64_PC32     .text._ZN5ChildC1Ev
0000000000000080 R_X86_64_PC32     .text
00000000000000a0 R_X86_64_PC32     .text+0x0000000000000042
00000000000000c0 R_X86_64_PC32     .text+0x000000000000008f
```

在`.eh_frame`段中的RELOCATION RECORDS（重定位记录）与C++异常处理框架有关，用于描述函数的异常表信息。这个段通常包含用于支持语言级别异常处理（如C++ try/catch块）的数据结构，例如CIE (Common Information Entry) 和 FDE (Frame Description Entry)。

以下是您给出的部分`.eh_frame`段重定位记录的解读：

- `OFFSET`：在`.eh_frame`段内需要进行重定位的具体偏移位置。
- `TYPE`：均为 `R_X86_64_PC32` 类型，表示这些地址是相对PC（程序计数器）的32位重定位，用于指向异常处理框架中引用的函数或代码区域。
- `VALUE`：
  - `.text._ZN11FirstFatherC2Ev` 指向类`FirstFather`构造函数的地址。
  - `.text._ZN12SecondFatherC2Ev` 指向类`SecondFather`构造函数的地址。
  - `.text._ZN5ChildC1Ev` 指向类`Child`构造函数的地址。
  - `.text` 表示对`.text`代码段本身的引用，可能是指向某个特殊处理例程的起点。
  - `.text+0x0000000000000042` 和 `.text+0x000000000000008f` 分别指向`.text`段内的特定偏移地址，这两个地址可能对应着与异常处理相关的重要指令或标记点。

这些重定位确保当运行时发生异常时，能正确地遍历堆栈并执行适当的清理工作（析构函数调用等），同时找到合适的catch块来处理异常。

#### 1.2.2 片段二

```bash
RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE              VALUE
0000000000000023 R_X86_64_PLT32    _ZN5ChildC1Ev-0x0000000000000004
000000000000003c R_X86_64_PLT32    __stack_chk_fail-0x0000000000000004
0000000000000066 R_X86_64_PC32     .bss-0x0000000000000004
000000000000006b R_X86_64_PLT32    _ZNSt8ios_base4InitC1Ev-0x0000000000000004
0000000000000072 R_X86_64_PC32     __dso_handle-0x0000000000000004
0000000000000079 R_X86_64_PC32     .bss-0x0000000000000004
0000000000000080 R_X86_64_REX_GOTPCRELX  _ZNSt8ios_base4InitD1Ev-0x0000000000000004
0000000000000088 R_X86_64_PLT32    __cxa_atexit-0x0000000000000004
```

这些RELOCATION RECORDS（重定位记录）是在链接阶段对`.text`段中符号引用进行修正的条目，确保最终生成的可执行文件中的地址指向正确的目标。以下是部分重定位记录的具体含义：

- `OFFSET`：在`.text`代码段内的偏移地址，需要进行重定位。

- `TYPE`：
  - `R_X86_64_PLT32`：表示针对x86-64架构的Procedure Linkage Table (PLT)条目的32位重定位，用于间接调用外部函数。
  - `R_X86_64_PC32`：相对PC（程序计数器）的32位重定位，常见于跳转和调用指令。
  - `R_X86_64_REX_GOTPCRELX`：一种特殊的x86-64重定位类型，使用了REX前缀并通过GOT表（全局偏移表）进行间接寻址，通常用于全局变量或函数调用。

- `VALUE`：
  - `_ZN5ChildC1Ev-0x0000000000000004`：表示类`Child`的构造函数地址，并有一个负偏移量。
  - `__stack_chk_fail-0x0000000000000004`：指向栈溢出检测失败处理函数`__stack_chk_fail`的地址。
  - `.bss-0x0000000000000004`：指向`.bss`段，可能是指向未初始化数据区域的指针调整。
  - `_ZNSt8ios_base4InitC1Ev-0x0000000000000004`：指向`std::ios_base`类的构造函数地址。
  - `__dso_handle-0x0000000000000004`：指向动态共享对象(DSO)句柄，与运行时库的初始化和终止有关。
  - `_ZNSt8ios_base4InitD1Ev-0x0000000000000004`：指向`std::ios_base`类的析构函数地址。
  - `__cxa_atexit-0x0000000000000004`：指向`__cxa_atexit`函数的地址，该函数用于注册在程序结束时需要调用的清理函数。

每个重定位记录都指示链接器在生成可执行文件时如何修改指定位置的地址，以确保它们正确链接到相应的函数、全局变量或其他必要的内存区域。

#### 1.2.3 片段三

```bash
RELOCATION RECORDS FOR [.text._ZN5ChildC1Ev]:
OFFSET           TYPE              VALUE
0000000000000017 R_X86_64_PC32     _ZTT5Child+0x0000000000000004
0000000000000022 R_X86_64_PLT32    _ZN11FirstFatherC2Ev-0x0000000000000004
0000000000000031 R_X86_64_PC32     _ZTT5Child+0x000000000000000c
000000000000003c R_X86_64_PLT32    _ZN12SecondFatherC2Ev-0x0000000000000004
0000000000000043 R_X86_64_PC32     _ZTV5Child+0x0000000000000014
0000000000000051 R_X86_64_PC32     _ZTV5Child+0x000000000000002c
```

这些RELOCATION RECORDS是针对`.text._ZN5ChildC1Ev`段的，这个段通常对应于类`Child`构造函数的机器码实现。以下是各个记录的具体解释：

- `OFFSET`：在`.text._ZN5ChildC1Ev`（即`Child`类构造函数）内部的偏移地址。

- `TYPE`：
  - `R_X86_64_PC32`：相对PC的32位重定位，用于调整相对于当前指令地址的跳转或引用。
  - `R_X86_64_PLT32`：同上文所述，用于间接调用外部函数的PLT条目。

- `VALUE`：
  - `_ZTT5Child+0x0000000000000004`：指向类`Child`的完整类型信息(`typeinfo`)表（`vtable for type_info`）的一个偏移量处。这里可能是在构造函数中初始化RTTI（运行时类型信息）相关部分。
  
  - `_ZN11FirstFatherC2Ev-0x0000000000000004`：指向基类`FirstFather`的构造函数地址，并有一个负偏移量，说明在构造`Child`对象时会先调用`FirstFather`的构造函数。
  
  - `_ZTT5Child+0x000000000000000c`：再次指向`Child`类的类型信息表的另一个偏移位置，可能与不同的RTTI特性相关联。

  - `_ZN12SecondFatherC2Ev-0x0000000000000004`：类似地，指向基类`SecondFather`的构造函数地址，表明`Child`的构造过程中也会调用`SecondFather`的构造函数。

  - `_ZTV5Child+0x0000000000000014` 和 `_ZTV5Child+0x000000000000002c`：指向`Child`类虚函数表（vtable）的不同偏移位置。在构造函数中可能会初始化对象的虚函数指针以指向正确的vtable，确保多态行为正确执行。

通过这些重定位记录，链接器将在程序加载和执行期间修正地址引用，使得`Child`类构造函数能够正确初始化其基类成员以及与RTTI相关的数据结构。

#### 1.2.4 片段四

```bash
RELOCATION RECORDS FOR [.data.rel.ro.local._ZTV5Child]:
OFFSET           TYPE              VALUE
0000000000000010 R_X86_64_64       _ZTI5Child
0000000000000028 R_X86_64_64       _ZTI5Child
```

这些RELOCATION RECORDS是针对`.data.rel.ro.local._ZTV5Child`段的，这个段通常包含类`Child`的虚函数表（vtable）以及与运行时类型信息（RTTI）相关的数据。以下是各个记录的具体解释：

- `OFFSET`：
  - `0x0000000000000010` 和 `0x0000000000000028`：在`.data.rel.ro.local._ZTV5Child`段中需要进行重定位的偏移地址。

- `TYPE`：
  - `R_X86_64_64`：这是x86-64架构下的一种重定位类型，用于调整64位地址引用。

- `VALUE`：
  - `_ZTI5Child`：指向类`Child`的类型信息（typeinfo）结构体。在C++运行时类型识别（RTTI）中，类型信息包含了关于类的一些元数据，例如类名和基类的信息。

两个重定位条目都指向同一个符号`_ZTI5Child`，说明在`.data.rel.ro.local._ZTV5Child`段中有两个位置需要链接到类`Child`的类型信息结构体地址。这样的布局常见于C++中的虚函数表（vtable），其中可能包括一个指向类型信息（typeinfo）的指针，以支持运行时类型识别和动态类型转换。

#### 1.2.5 片段五

```bash
RELOCATION RECORDS FOR [.data.rel.ro.local._ZTT5Child]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTV5Child+0x0000000000000018
0000000000000008 R_X86_64_64       _ZTC5Child0_11FirstFather+0x0000000000000018
0000000000000010 R_X86_64_64       _ZTC5Child8_12SecondFather+0x0000000000000018
0000000000000018 R_X86_64_64       _ZTV5Child+0x0000000000000030
```

这些RELOCATION RECORDS是针对`.data.rel.ro.local._ZTT5Child`段的，这个段通常包含了类`Child`的类型通配符表（typeinfo name for type）以及其他与RTTI相关的数据。以下是各个记录的具体解释：

- `OFFSET`：
  - `0x0000000000000000`、`0x0000000000000008`、`0x0000000000000010` 和 `0x0000000000000018`：在`.data.rel.ro.local._ZTT5Child`段中需要进行重定位的不同偏移地址。

- `TYPE`：
  - `R_X86_64_64`：这是x86-64架构下的一种重定位类型，用于调整64位地址引用。

- `VALUE`：
  - `_ZTV5Child+0x0000000000000018`：指向类`Child`虚函数表的一个特定偏移量位置。
  - `_ZTC5Child0_11FirstFather+0x0000000000000018`：指向从基类`FirstFather`到派生类`Child`的部分虚函数表或者与继承层次结构相关的RTTI信息的一个特定偏移量位置。
  - `_ZTC5Child8_12SecondFather+0x0000000000000018`：同上，但指向从基类`SecondFather`到派生类`Child`的部分虚函数表或RTTI信息的特定偏移量位置。
  - `_ZTV5Child+0x0000000000000030`：再次指向类`Child`虚函数表的另一个特定偏移量位置。

这些重定位记录说明了`.data.rel.ro.local._ZTT5Child`段中的内容如何与类`Child`及其基类`FirstFather`和`SecondFather`的虚函数表相关联。这有助于C++运行时支持动态类型识别和多态行为。

#### 1.2.6 片段六

```bash
RELOCATION RECORDS FOR [.data.rel.ro.local._ZTC5Child0_11FirstFather]:
OFFSET           TYPE              VALUE
0000000000000010 R_X86_64_64       _ZTI11FirstFather


RELOCATION RECORDS FOR [.data.rel.ro.local._ZTC5Child8_12SecondFather]:
OFFSET           TYPE              VALUE
0000000000000010 R_X86_64_64       _ZTI12SecondFather
```

这些RELOCATION RECORDS分别针对`.data.rel.ro.local._ZTC5Child0_11FirstFather`和`.data.rel.ro.local._ZTC5Child8_12SecondFather`段，这两个段通常包含了类`Child`从基类`FirstFather`和`SecondFather`继承的虚函数表部分以及其他与RTTI（运行时类型信息）相关的数据。

对于每个段：

- `OFFSET`：
  - `0x0000000000000010`：在对应的段中需要进行重定位的偏移地址。

- `TYPE`：
  - `R_X86_64_64`：这是x86-64架构下的一种重定位类型，用于调整64位地址引用。

- `VALUE`：
  - `_ZTI11FirstFather`：指向基类`FirstFather`的类型信息（typeinfo）结构体。
  - `_ZTI12SecondFather`：指向基类`SecondFather`的类型信息（typeinfo）结构体。

这些重定位记录说明了类`Child`从其两个基类继承的部分虚函数表或RTTI相关数据如何关联到相应的基类类型信息。这样，在C++运行时系统处理多态行为、动态类型转换以及异常处理等操作时，可以正确识别和使用这些类型的元数据。

#### 1.2.7 片段七

```bash
RELOCATION RECORDS FOR [.data.rel.ro._ZTI5Child]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS5Child
0000000000000018 R_X86_64_64       _ZTI11FirstFather
0000000000000028 R_X86_64_64       _ZTI12SecondFather
```

这些RELOCATION RECORDS是针对`.data.rel.ro._ZTI5Child`段的，这个段通常包含了类`Child`的类型信息（typeinfo）结构体。以下是各个记录的具体解释：

- `OFFSET`：
  - `0x0000000000000000`、`0x0000000000000008`、`0x0000000000000018` 和 `0x0000000000000028`：在`.data.rel.ro._ZTI5Child`段中需要进行重定位的不同偏移地址。

- `TYPE`：
  - `R_X86_64_64`：这是x86-64架构下的一种重定位类型，用于调整64位地址引用。

- `VALUE`：
  - `_ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010`：指向一个特定类型的虚函数表指针或与RTTI相关的内部结构体的一个特定偏移量位置。这里具体指向的是C++ ABI库提供的一个通用类类型信息结构体的一个偏移。
  - `_ZTS5Child`：指向类`Child`的类型名字符串（typeid(Child).name()会返回这个值）。
  - `_ZTI11FirstFather`：指向基类`FirstFather`的类型信息（typeinfo）结构体。
  - `_ZTI12SecondFather`：指向基类`SecondFather`的类型信息（typeinfo）结构体。

这些重定位记录表明了`.data.rel.ro._ZTI5Child`段中的内容如何与其他RTTI相关数据（包括类`Child`及其基类`FirstFather`和`SecondFather`的类型信息结构体以及可能的ABI库内部结构体）关联起来。这有助于C++运行时支持动态类型识别、多态行为、异常处理等功能。

#### 1.2.8 片段八

```bash
RELOCATION RECORDS FOR [.data.rel.ro._ZTI12SecondFather]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS12SecondFather
0000000000000018 R_X86_64_64       _ZTI11GrandFather


RELOCATION RECORDS FOR [.data.rel.ro._ZTI11FirstFather]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS11FirstFather
0000000000000018 R_X86_64_64       _ZTI11GrandFather


RELOCATION RECORDS FOR [.data.rel.ro._ZTI11GrandFather]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv117__class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS11GrandFather
```

这些RELOCATION RECORDS分别对应于类`SecondFather`、`FirstFather`和`GrandFather`的类型信息（typeinfo）结构体所在的内存段。以下是每个记录的具体解释：

- 对于`.data.rel.ro._ZTI12SecondFather`：
  - `0x0000000000000000`：指向C++ ABI库中用于多继承类的通用类类型信息结构体的一个偏移量位置。
  - `0x0000000000000008`：指向类`SecondFather`的类型名字符串。
  - `0x0000000000000018`：指向基类`GrandFather`的类型信息结构体。

- 对于`.data.rel.ro._ZTI11FirstFather`：
  - `0x0000000000000000`：同样指向C++ ABI库中用于多继承类的通用类类型信息结构体的一个偏移量位置。
  - `0x0000000000000008`：指向类`FirstFather`的类型名字符串。
  - `0x0000000000000018`：也指向基类`GrandFather`的类型信息结构体。

- 对于`.data.rel.ro._ZTI11GrandFather`：
  - `0x0000000000000000`：指向C++ ABI库中用于非多继承类的基础类类型信息结构体的一个偏移量位置，这里使用的是`__class_type_info`。
  - `0x0000000000000008`：指向类`GrandFather`的类型名字符串。

综上所述，这些重定位记录确保了在运行时，能够正确链接到各个类及其基类的类型信息，以便支持动态类型识别、RTTI以及异常处理等功能。

#### 1.2.9 片段九

```bash
RELOCATION RECORDS FOR [.init_array]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       .text+0x000000000000008f
```

在`.init_array`段中的RELOCATION RECORDS（重定位记录）通常与程序初始化有关。这个特定的记录表示：

- `OFFSET`：`0x0000000000000000`，这是`.init_array`节内的偏移地址。

- `TYPE`：`R_X86_64_64`，这是一个64位地址空间的重定位类型，用于调整64位地址引用。

- `VALUE`：`.text+0x000000000000008f`，这个值表示在`.text`代码段中一个函数或指令的地址加上偏移量`0x000000000000008f`。当程序启动时，`.init_array`节中的函数指针将被依次调用执行其初始化操作。

因此，在程序启动时，`.init_array`的第一个元素（地址为`0x0000000000000000`）将会跳转到`.text`段内地址为`0x000000000000008f`的位置执行初始化代码。这种机制主要用于全局对象的构造以及其他需要在主程序开始前执行的静态初始化任务。

#### 1.2.10 完整obj文件

```bash
constructor.o:     file format elf64-x86-64
constructor.o
architecture: i386:x86-64, flags 0x00000011:
HAS_RELOC, HAS_SYMS
start address 0x0000000000000000

Sections:
Idx Name          Size      VMA               LMA               File off  Algn
  0 .group        00000008  0000000000000000  0000000000000000  00000040  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  1 .group        00000008  0000000000000000  0000000000000000  00000048  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  2 .group        0000000c  0000000000000000  0000000000000000  00000050  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  3 .group        00000024  0000000000000000  0000000000000000  0000005c  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  4 .group        0000000c  0000000000000000  0000000000000000  00000080  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  5 .group        00000008  0000000000000000  0000000000000000  0000008c  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  6 .group        0000000c  0000000000000000  0000000000000000  00000094  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  7 .group        00000008  0000000000000000  0000000000000000  000000a0  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  8 .group        0000000c  0000000000000000  0000000000000000  000000a8  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
  9 .group        00000008  0000000000000000  0000000000000000  000000b4  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
 10 .group        0000000c  0000000000000000  0000000000000000  000000bc  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
 11 .group        00000008  0000000000000000  0000000000000000  000000c8  2**2
                  CONTENTS, READONLY, GROUP, LINK_ONCE_DISCARD
 12 .text         000000a8  0000000000000000  0000000000000000  000000d0  2**0
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
 13 .data         00000000  0000000000000000  0000000000000000  00000178  2**0
                  CONTENTS, ALLOC, LOAD, DATA
 14 .bss          00000001  0000000000000000  0000000000000000  00000178  2**0
                  ALLOC
 15 .rodata       00000001  0000000000000000  0000000000000000  00000178  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 16 .text._ZN11FirstFatherC2Ev 00000021  0000000000000000  0000000000000000  0000017a  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 17 .text._ZN12SecondFatherC2Ev 00000021  0000000000000000  0000000000000000  0000019c  2**1
                  CONTENTS, ALLOC, LOAD, READONLY, CODE
 18 .text._ZN5ChildC1Ev 00000060  0000000000000000  0000000000000000  000001be  2**1
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, CODE
 19 .data.rel.ro.local._ZTV5Child 00000030  0000000000000000  0000000000000000  00000220  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 20 .data.rel.ro.local._ZTT5Child 00000020  0000000000000000  0000000000000000  00000250  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 21 .data.rel.ro.local._ZTC5Child0_11FirstFather 00000018  0000000000000000  0000000000000000  00000270  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 22 .data.rel.ro.local._ZTC5Child8_12SecondFather 00000018  0000000000000000  0000000000000000  00000288  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 23 .data.rel.ro._ZTI5Child 00000038  0000000000000000  0000000000000000  000002a0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 24 .rodata._ZTS5Child 00000007  0000000000000000  0000000000000000  000002d8  2**0
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 25 .data.rel.ro._ZTI12SecondFather 00000028  0000000000000000  0000000000000000  000002e0  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 26 .rodata._ZTS12SecondFather 0000000f  0000000000000000  0000000000000000  00000308  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 27 .data.rel.ro._ZTI11FirstFather 00000028  0000000000000000  0000000000000000  00000318  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 28 .rodata._ZTS11FirstFather 0000000e  0000000000000000  0000000000000000  00000340  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 29 .data.rel.ro._ZTI11GrandFather 00000010  0000000000000000  0000000000000000  00000350  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 30 .rodata._ZTS11GrandFather 0000000e  0000000000000000  0000000000000000  00000360  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 31 .init_array   00000008  0000000000000000  0000000000000000  00000370  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, DATA
 32 .comment      0000002c  0000000000000000  0000000000000000  00000378  2**0
                  CONTENTS, READONLY
 33 .note.GNU-stack 00000000  0000000000000000  0000000000000000  000003a4  2**0
                  CONTENTS, READONLY
 34 .note.gnu.property 00000020  0000000000000000  0000000000000000  000003a8  2**3
                  CONTENTS, ALLOC, LOAD, READONLY, DATA
 35 .eh_frame     000000d8  0000000000000000  0000000000000000  000003c8  2**3
                  CONTENTS, ALLOC, LOAD, RELOC, READONLY, DATA
SYMBOL TABLE:
0000000000000000 l    df *ABS*	0000000000000000 constructor.cpp
0000000000000000 l    d  .text	0000000000000000 .text
0000000000000000 l    d  .data	0000000000000000 .data
0000000000000000 l    d  .bss	0000000000000000 .bss
0000000000000000 l    d  .rodata	0000000000000000 .rodata
0000000000000000 l     O .rodata	0000000000000001 _ZStL19piecewise_construct
0000000000000000 l     O .bss	0000000000000001 _ZStL8__ioinit
0000000000000000 l    d  .text._ZN11FirstFatherC2Ev	0000000000000000 .text._ZN11FirstFatherC2Ev
0000000000000000 l    d  .text._ZN12SecondFatherC2Ev	0000000000000000 .text._ZN12SecondFatherC2Ev
0000000000000000 l    d  .text._ZN5ChildC1Ev	0000000000000000 .text._ZN5ChildC1Ev
0000000000000000 l    d  .data.rel.ro.local._ZTV5Child	0000000000000000 .data.rel.ro.local._ZTV5Child
0000000000000000 l    d  .data.rel.ro.local._ZTT5Child	0000000000000000 .data.rel.ro.local._ZTT5Child
0000000000000000 l    d  .data.rel.ro.local._ZTC5Child0_11FirstFather	0000000000000000 .data.rel.ro.local._ZTC5Child0_11FirstFather
0000000000000000 l    d  .data.rel.ro.local._ZTC5Child8_12SecondFather	0000000000000000 .data.rel.ro.local._ZTC5Child8_12SecondFather
0000000000000000 l    d  .data.rel.ro._ZTI5Child	0000000000000000 .data.rel.ro._ZTI5Child
0000000000000000 l    d  .rodata._ZTS5Child	0000000000000000 .rodata._ZTS5Child
0000000000000000 l    d  .data.rel.ro._ZTI12SecondFather	0000000000000000 .data.rel.ro._ZTI12SecondFather
0000000000000000 l    d  .rodata._ZTS12SecondFather	0000000000000000 .rodata._ZTS12SecondFather
0000000000000000 l    d  .data.rel.ro._ZTI11FirstFather	0000000000000000 .data.rel.ro._ZTI11FirstFather
0000000000000000 l    d  .rodata._ZTS11FirstFather	0000000000000000 .rodata._ZTS11FirstFather
0000000000000042 l     F .text	000000000000004d _Z41__static_initialization_and_destruction_0ii
0000000000000000 l    d  .data.rel.ro._ZTI11GrandFather	0000000000000000 .data.rel.ro._ZTI11GrandFather
0000000000000000 l    d  .rodata._ZTS11GrandFather	0000000000000000 .rodata._ZTS11GrandFather
000000000000008f l     F .text	0000000000000019 _GLOBAL__sub_I_main
0000000000000000 l    d  .init_array	0000000000000000 .init_array
0000000000000000 l    d  .note.GNU-stack	0000000000000000 .note.GNU-stack
0000000000000000 l    d  .note.gnu.property	0000000000000000 .note.gnu.property
0000000000000000 l    d  .eh_frame	0000000000000000 .eh_frame
0000000000000000 l    d  .comment	0000000000000000 .comment
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000 l    d  .group	0000000000000000 .group
0000000000000000  w    F .text._ZN11FirstFatherC2Ev	0000000000000021 _ZN11FirstFatherC2Ev
0000000000000000  w    F .text._ZN12SecondFatherC2Ev	0000000000000021 _ZN12SecondFatherC2Ev
0000000000000000  w    F .text._ZN5ChildC1Ev	0000000000000060 _ZN5ChildC1Ev
0000000000000000  w    O .data.rel.ro.local._ZTT5Child	0000000000000020 _ZTT5Child
0000000000000000  w    O .data.rel.ro.local._ZTV5Child	0000000000000030 _ZTV5Child
0000000000000000 g     F .text	0000000000000042 main
0000000000000000         *UND*	0000000000000000 _GLOBAL_OFFSET_TABLE_
0000000000000000         *UND*	0000000000000000 __stack_chk_fail
0000000000000000  w    O .data.rel.ro._ZTI5Child	0000000000000038 _ZTI5Child
0000000000000000  w    O .data.rel.ro.local._ZTC5Child0_11FirstFather	0000000000000018 .hidden _ZTC5Child0_11FirstFather
0000000000000000  w    O .data.rel.ro.local._ZTC5Child8_12SecondFather	0000000000000018 .hidden _ZTC5Child8_12SecondFather
0000000000000000  w    O .data.rel.ro._ZTI11FirstFather	0000000000000028 _ZTI11FirstFather
0000000000000000  w    O .data.rel.ro._ZTI12SecondFather	0000000000000028 _ZTI12SecondFather
0000000000000000         *UND*	0000000000000000 _ZTVN10__cxxabiv121__vmi_class_type_infoE
0000000000000000  w    O .rodata._ZTS5Child	0000000000000007 _ZTS5Child
0000000000000000  w    O .rodata._ZTS12SecondFather	000000000000000f _ZTS12SecondFather
0000000000000000  w    O .data.rel.ro._ZTI11GrandFather	0000000000000010 _ZTI11GrandFather
0000000000000000  w    O .rodata._ZTS11FirstFather	000000000000000e _ZTS11FirstFather
0000000000000000         *UND*	0000000000000000 _ZNSt8ios_base4InitC1Ev
0000000000000000         *UND*	0000000000000000 .hidden __dso_handle
0000000000000000         *UND*	0000000000000000 _ZNSt8ios_base4InitD1Ev
0000000000000000         *UND*	0000000000000000 __cxa_atexit
0000000000000000         *UND*	0000000000000000 _ZTVN10__cxxabiv117__class_type_infoE
0000000000000000  w    O .rodata._ZTS11GrandFather	000000000000000e _ZTS11GrandFather


RELOCATION RECORDS FOR [.text]:
OFFSET           TYPE              VALUE
0000000000000023 R_X86_64_PLT32    _ZN5ChildC1Ev-0x0000000000000004
000000000000003c R_X86_64_PLT32    __stack_chk_fail-0x0000000000000004
0000000000000066 R_X86_64_PC32     .bss-0x0000000000000004
000000000000006b R_X86_64_PLT32    _ZNSt8ios_base4InitC1Ev-0x0000000000000004
0000000000000072 R_X86_64_PC32     __dso_handle-0x0000000000000004
0000000000000079 R_X86_64_PC32     .bss-0x0000000000000004
0000000000000080 R_X86_64_REX_GOTPCRELX  _ZNSt8ios_base4InitD1Ev-0x0000000000000004
0000000000000088 R_X86_64_PLT32    __cxa_atexit-0x0000000000000004


RELOCATION RECORDS FOR [.text._ZN5ChildC1Ev]:
OFFSET           TYPE              VALUE
0000000000000017 R_X86_64_PC32     _ZTT5Child+0x0000000000000004
0000000000000022 R_X86_64_PLT32    _ZN11FirstFatherC2Ev-0x0000000000000004
0000000000000031 R_X86_64_PC32     _ZTT5Child+0x000000000000000c
000000000000003c R_X86_64_PLT32    _ZN12SecondFatherC2Ev-0x0000000000000004
0000000000000043 R_X86_64_PC32     _ZTV5Child+0x0000000000000014
0000000000000051 R_X86_64_PC32     _ZTV5Child+0x000000000000002c


RELOCATION RECORDS FOR [.data.rel.ro.local._ZTV5Child]:
OFFSET           TYPE              VALUE
0000000000000010 R_X86_64_64       _ZTI5Child
0000000000000028 R_X86_64_64       _ZTI5Child


RELOCATION RECORDS FOR [.data.rel.ro.local._ZTT5Child]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTV5Child+0x0000000000000018
0000000000000008 R_X86_64_64       _ZTC5Child0_11FirstFather+0x0000000000000018
0000000000000010 R_X86_64_64       _ZTC5Child8_12SecondFather+0x0000000000000018
0000000000000018 R_X86_64_64       _ZTV5Child+0x0000000000000030


RELOCATION RECORDS FOR [.data.rel.ro.local._ZTC5Child0_11FirstFather]:
OFFSET           TYPE              VALUE
0000000000000010 R_X86_64_64       _ZTI11FirstFather


RELOCATION RECORDS FOR [.data.rel.ro.local._ZTC5Child8_12SecondFather]:
OFFSET           TYPE              VALUE
0000000000000010 R_X86_64_64       _ZTI12SecondFather


RELOCATION RECORDS FOR [.data.rel.ro._ZTI5Child]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS5Child
0000000000000018 R_X86_64_64       _ZTI11FirstFather
0000000000000028 R_X86_64_64       _ZTI12SecondFather


RELOCATION RECORDS FOR [.data.rel.ro._ZTI12SecondFather]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS12SecondFather
0000000000000018 R_X86_64_64       _ZTI11GrandFather


RELOCATION RECORDS FOR [.data.rel.ro._ZTI11FirstFather]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv121__vmi_class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS11FirstFather
0000000000000018 R_X86_64_64       _ZTI11GrandFather


RELOCATION RECORDS FOR [.data.rel.ro._ZTI11GrandFather]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       _ZTVN10__cxxabiv117__class_type_infoE+0x0000000000000010
0000000000000008 R_X86_64_64       _ZTS11GrandFather


RELOCATION RECORDS FOR [.init_array]:
OFFSET           TYPE              VALUE
0000000000000000 R_X86_64_64       .text+0x000000000000008f


RELOCATION RECORDS FOR [.eh_frame]:
OFFSET           TYPE              VALUE
0000000000000020 R_X86_64_PC32     .text._ZN11FirstFatherC2Ev
0000000000000040 R_X86_64_PC32     .text._ZN12SecondFatherC2Ev
0000000000000060 R_X86_64_PC32     .text._ZN5ChildC1Ev
0000000000000080 R_X86_64_PC32     .text
00000000000000a0 R_X86_64_PC32     .text+0x0000000000000042
00000000000000c0 R_X86_64_PC32     .text+0x000000000000008f
```

