// 命名规范

// 文件的编码最好用UTF-8，怕乱码

// 头文件的命名规范
// 系统自带的用<>，自己写的用""
#include <iostream>
#include "include/Example.h"

// 宏定义的命名规范
// 一般用大写字母，多个单词用下划线连接
#define MAX_COUNT 10
#define THIS_IS_MY_MARCO_EXAMPLE 1

// 常量的命名规范
// 一般用大写字母，多个单词用下划线连接
const int MAX_COUNT = 10;
const int THIS_IS_MY_CONST_EXAMPLE = 1;

// 全局变量的命名规范
// 一般用g开头，大驼峰
int gCount = 10;
int gThisIsMyGlobalExample = 1;

// 类型的命名规范
// 大驼峰
typedef int MyType;
typedef int MyTypeExample;

// 函数的命名规范
// 名称大驼峰，参数小驼峰
void MyFunction();
void MyFunctionExample(int test, int testExample);

// 变量的命名规范
// 小驼峰
int myCount = 10;
int myThisIsMyExample = 1;

// 结构体的命名规范
// 大驼峰
struct MyStruct
{
    int count;
    int thisIsMyExample;
};

// 枚举的命名规范
// 大驼峰
enum MyEnum
{
    COUNT,
    THIS_IS_MY_EXAMPLE
};

// 类的命名规范
// 大驼峰
class MyClass
{
public:
    MyClass();
    ~MyClass();

    // 公有变量的命名规范
    // 小驼峰
public:
    int count;
    int thisIsMyExample;

    // 公有函数的命名规范
    // 大驼峰
public:
    void MyFunction();
    void MyFunctionExample();

    // 私有变量的命名规范
    // 小驼峰，前面加下划线
private:
    int _count;
    int _thisIsMyExample;

    // 私有函数的命名规范
    // 大驼峰，前面加下划线
private:
    void _MyFunction();
    void _MyFunctionExample();
};

// 函数内部的命名规范
void MyFunctionExample(int test, int testExample)
{
    // 局部变量的命名规范
    // 小驼峰，前面加m_
    int m_count = 10;
    int m_thisIsMyExample = 1;

    // 其他的就是前面多加一个m_就行了
}