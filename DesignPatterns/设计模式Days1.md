# 设计模式Days1

## 今日任务

学习单例

练习地址：https://kamacoder.com/problempage.php?pid=1074

代码地址：https://github.com/youngyangyang04/kama-DesignPattern/blob/main/DesignPattern/1-%E5%8D%95%E4%BE%8B%E6%A8%A1%E5%BC%8F.md



## 单例

**什么是单例？**

单例模式是一种**创建型设计模式**， 它的核心思想是保证**一个类只有一个实例，并提供一个全局访问点来访问这个实例。**

- 只有一个实例的意思是，在整个应用程序中，只存在该类的一个实例对象，而不是创建多个相同类型的对象。
- 全局访问点的意思是，为了让其他类能够获取到这个唯一实例，该类提供了一个全局访问点（通常是一个静态方法），通过这个方法就能获得实例。



**为什么使用单例？**

+ 全局控制，对唯一实例的受控访问。
+ 节省资源，避免多次创建不必要的对象。



**如何设计单例？**

- 私有的构造函数：防止外部代码直接创建类的实例。
- 私有的静态实例变量：保存该类的唯一实例。
- 公有的静态方法：通过公有的静态方法来获取类的实例。



**什么时候考虑使用单例？**

+ 资源共享：多个模块共享某个资源的时候，可以使用单例模式，比如说应用程序需要一个全局的配置管理器来存储和管理配置信息、亦或是使用单例模式管理数据库连接池。
+ 只有一个实例：当系统中某个类只需要一个实例来协调行为的时候，可以考虑使用单例模式， 比如说管理应用程序中的缓存，确保只有一个缓存实例，避免重复的缓存创建和管理，或者使用单例模式来创建和管理线程池。
+ 懒加载：如果对象创建本身就比较消耗资源，而且可能在整个程序中都不一定会使用，可以使用单例模式实现懒加载。



## 代码

```c++
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

class ShoppingCartManager
{
public:
  static ShoppingCartManager& getInstance()
  {
    static ShoppingCartManager instance;
    return instance;
  }

  void addCart(string itemName, int itemCnt)
  {
    if (m_cart.find(itemName) == m_cart.end())
    {
      m_order.emplace_back(itemName);
    }
    m_cart[itemName] += itemCnt;
  }

  void showCart()
  {
    for (const string &itemName : m_order)
    {
      cout << itemName << " " << m_cart[itemName] << endl;
    }
  }

private:
  ShoppingCartManager()
  {
  }

private:
  unordered_map<string, int> m_cart;
  vector<string> m_order;  // 保持输出的顺序
};

int main()
{
  string itemName;
  int itemCnt;
  
  while (cin >> itemName >> itemCnt)
  {
    ShoppingCartManager::getInstance().addCart(itemName, itemCnt);
  }

  ShoppingCartManager::getInstance().showCart();
}
```

