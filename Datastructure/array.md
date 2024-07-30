# array

## 1. 问题

## 2. 引入代码

### 2.1 代码

```c++
#include <cstddef>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <initializer_list>

template <typename T, std::size_t N>
class MyArray {
public:
    // 类型定义
    using value_type = T;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // 构造函数
    constexpr MyArray() noexcept { }
    explicit MyArray(const T& value) { fill(value); }
    MyArray(std::initializer_list<T> init) {
        if (init.size() > N) throw std::length_error("Initializer list too long");
        std::copy_n(init.begin(), init.size(), begin());
        if (init.size() < N) std::fill(begin() + init.size(), end(), T());
    }

    // 拷贝构造函数、拷贝赋值运算符、移动构造函数和移动赋值运算符可以默认
    MyArray(const MyArray&) = default;
    MyArray& operator=(const MyArray&) = default;
    MyArray(MyArray&&) = default; // 添加移动构造函数
    MyArray& operator=(MyArray&&) = default; // 添加移动赋值运算符

    // 访问元素
    reference at(size_type pos) {
        if (pos >= size()) throw std::out_of_range("Index out of range");
        return data[pos];
    }
    const_reference at(size_type pos) const {
        if (pos >= size()) throw std::out_of_range("Index out of range");
        return data[pos];
    }
    reference operator[](size_type pos) { return data[pos]; }
    const_reference operator[](size_type pos) const { return data[pos]; }
    reference front() { return data[0]; }
    const_reference front() const { return data[0]; }
    reference back() { return data[N - 1]; }
    const_reference back() const { return data[N - 1]; }

    // 迭代器相关
    iterator begin() noexcept { return data; }
    const_iterator begin() const noexcept { return data; }
    iterator end() noexcept { return data + N; }
    const_iterator end() const noexcept { return data + N; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const noexcept { return const_reverse_iterator(begin()); }
    const_iterator cbegin() const noexcept { return begin(); } // 添加const版本的begin和end
    const_iterator cend() const noexcept { return end(); }

    // 容量相关
    constexpr bool empty() const noexcept { return N == 0; }
    constexpr size_type size() const noexcept { return N; }
    constexpr size_type max_size() const noexcept { return N; } // 由于是固定大小，max_size就是N

    // 修改操作
    void fill(const T& value) { std::fill(begin(), end(), value); }
    void swap(MyArray& other) noexcept { std::swap_ranges(begin(), end(), other.begin()); }

    // 添加一些常用算法作为成员函数（可选）
    iterator find(const T& value) { return std::find(begin(), end(), value); }
    const_iterator find(const T& value) const { return std::find(cbegin(), cend(), value); }
    bool contains(const T& value) const { return find(value) != end(); } // C++20中std::array没有这个函数，但添加它很有用。
    size_t count(const T& value) const { return std::count(cbegin(), cend(), value); } // 计算某个值出现的次数。
    iterator erase(const_iterator pos) { // 注意：这个实现不支持删除多个元素，因为数组大小是固定的。仅用于模拟接口。
        if (pos == end()) throw std::out_of_range("Cannot erase end position");
        std::move(pos + 1, end(), pos); // 将后面的元素向前移动一个位置。注意这不是一个高效的操作。
        end()[-1] = T(); // 将最后一个元素设置为默认值。注意这可能会导致不必要的对象构造/析构。
        return pos; // 返回被删除元素的下一个位置。注意这不是标准std::array的erase行为。
    } // 在实际应用中，通常不会为固定大小的容器实现erase函数。这里只是为了展示如何模拟接口。
    // 注意：对于固定大小的数组，通常不会实现插入操作，因为会涉及到移动所有后续元素。这里没有提供insert函数。

private:
    T data[N]; // 固定大小的数组作为内部存储结构
};

// 示例用法：输出数组元素、查找、包含检查和修改数组元素等操作。
int main()
{
    MyArray<int, 5> arr{1, 2, 3, 4, 5}; // 使用初始化列表构造数组并初始化元素。
    for (int num : arr) std::cout << num << " "; // 输出: 1 2 3 4 5，然后换行。
    std::cout << '\n'; // 输出换行符以保持输出整洁。
    auto it = arr.find(3); // 查找元素3，并返回它的迭代器。这里会输出数组下标2对应的元素（如果找到的话）。注意下标是从0开始的。
    if (it != arr.end()) std::cout << "Found " << *it << " at index " << (it - arr.begin()) << '\n';
}
```

### 2.2 解释

#### 2.2.1 code1

（1）`constexpr MyArray() noexcept { }`

在C++中，`constexpr`、`noexcept`和默认构造函数都是与类和对象构造相关的特性。下面我将逐一解释这些特性，并以一个简单的`MyArray`类为例来说明它们是如何工作的。

1. **constexpr**：
   `constexpr`是一个类型修饰符，它指定一个表达式的值或对象必须在编译时就能计算出来。这通常用于优化性能，因为它允许编译器在编译时执行某些计算，而不是在运行时。当用于构造函数时，它意味着这个构造函数可以创建一个在编译时就能初始化的对象。
2. **noexcept**：
   `noexcept`是一个指定函数是否抛出异常的修饰符。如果一个函数被标记为`noexcept`，那么它就不能抛出任何异常。如果它违反了这一规则，程序会立即调用`std::terminate()`来结束执行。这对于性能优化和保证API的异常安全性很重要。
3. **默认构造函数**：
   默认构造函数是一个没有参数的构造函数，或者所有参数都有默认值的构造函数。当创建类的对象时没有提供任何初始化参数时，就会调用默认构造函数。

现在，让我们来看一个使用这些特性的`MyArray`类的例子：

```c++
class MyArray {  
public:  
    // constexpr默认构造函数，标记为noexcept  
    constexpr MyArray() noexcept {  
        // 这里不能有任何可能抛出异常的代码  
        // 也不能有任何只能在运行时计算的表达式（如果要保持constexpr）  
        // 通常这里会做一些简单的成员初始化工作  
    }  
  
    // 其他成员函数和成员变量...  
};
```

在这个例子中，`MyArray`类有一个被标记为`constexpr`和`noexcept`的默认构造函数。这意味着：

- 这个构造函数不会在运行时抛出异常（因为它被标记为`noexcept`）。
- 这个构造函数可以用于在编译时创建和初始化`MyArray`对象（因为它被标记为`constexpr`）。但是，请注意，要使一个对象真正在编译时被初始化，还需要满足其他条件，比如类的所有成员变量也必须能被静态初始化等。

在实际应用中，如果你的类只是简单地初始化其成员变量，并且这些初始化不可能引发异常，那么将构造函数标记为`constexpr`和`noexcept`是一个好习惯。这有助于提高代码的性能和可靠性。

#### 2.2.2 code2

（2）fill函数

`std::fill` 是 C++ 标准库 `<algorithm>` 中的一个函数，它提供了一种方便的方式来将指定值赋给一个范围内的所有元素。该函数接受三个参数：

1. **ForwardIterator first**: 指向要填充范围起始位置的迭代器。
2. **ForwardIterator last**: 指向要填充范围结束位置的下一个位置的迭代器（即，不包括这个位置的元素）。
3. **const T& value**: 要分配给范围内每个元素的值的引用，其中 `T` 是容器元素的类型。

函数原型如下：

```c++
template <class ForwardIt, class T>
constexpr void std::fill(ForwardIt first, ForwardIt last, const T& value);
```

```c++
#include <algorithm>
#include <vector>

int main() {
    std::vector<int> vec(8); // 创建包含8个整数的空向量

    // 使用 fill 函数将所有元素初始化为 42
    std::fill(vec.begin(), vec.end(), 42);

    // 现在 vec 中的所有元素都是 42
    for (const auto& element : vec) {
        std::cout << element << " "; // 输出: 42 42 42 42 42 42 42 42
    }

    return 0;
}
```

在这个例子中，`std::fill` 将向量 `vec` 中的所有元素都设置为了整数 `42`。

#### 2.2.3 code3

```c++
reference operator[](size_type pos) { return data[pos]; }
const_reference operator[](size_type pos) const { return data[pos]; }
```

**不同之处在哪里？**

在C++中，这两个`operator[]`函数确实构成了重载（overloading），尽管它们的参数列表看起来相同。重载的概念允许我们有多个同名函数，只要它们的参数列表（包括参数类型、数量或顺序）不同，或者一个是const而另一个不是。

在这个例子中，有两个`operator[]`函数：

1. 第一个是非const版本的`operator[]`，它返回`reference`（通常是对类内部数据的引用），并且没有标记为`const`。这意味着它可以在非const对象上调用，并且可以用来修改对象的状态。
2. 第二个是const版本的`operator[]`，它返回`const_reference`（通常是对类内部数据的常量引用），并且被标记为`const`。这意味着它可以在const对象上调用，但不能用来修改对象的状态。

这两个函数的关键区别在于它们的const属性。在C++中，const成员函数承诺不会修改调用它的对象的状态。因此，如果你有一个const对象或者一个通过const引用或指针访问的对象，你只能调用它的const成员函数。

这里是一个简化的例子来说明这个概念：

```c++
class MyContainer {  
public:  
    typedef int value_type;  
    typedef value_type& reference;  
    typedef const value_type& const_reference;  
    typedef size_t size_type;  
    // ... 其他成员，包括data数组 ...  
  
    // 非const版本的operator[]  
    reference operator[](size_type pos) {  
        // 假设data是一个内部数组  
        return data[pos]; // 返回引用，允许修改元素值  
    }  
  
    // const版本的operator[]  
    const_reference operator[](size_type pos) const {  
        // 假设data是一个内部数组  
        return data[pos]; // 返回常量引用，不允许修改元素值  
    }  
};
```

在这个例子中，如果你有一个非const的`MyContainer`对象，你可以使用非const版本的`operator[]`来读取或修改元素。如果你有一个const的`MyContainer`对象或者通过const引用或指针访问一个`MyContainer`对象，你只能使用const版本的`operator[]`来读取元素。