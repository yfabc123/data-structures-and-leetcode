踩坑日记 对"*"运算符重载如果不返回引用会导致double free 
因为不加引用那么返回的是一个临时对象 是当前调用者的副本 或者说是this的副本 共享str 然后他调用show方法虽然返回了正确的输出结果但是之后立马被析构 由于 这个副本和test1  test2 他们仨共享str 所以之后这两个副本析构调用~Test析构时也出现了问题 
   
    /* 重载 * */
    T &operator*()
    {
        return *ptr;
    }
![image](https://github.com/yfabc123/data-structures-and-leetcode/assets/103840107/b6027038-3230-4ba9-8e3c-68fd09c91d54)

```c++
#include <iostream>
#include <string.h>
using namespace std;
// 模拟智能指针
template <typename T>
class my_shared_ptr
{
public:
    /* 有参构造 */
    my_shared_ptr(T *obj = nullptr) : ptr(obj), count(new int(1))
    {
        cout << "有参构造" << endl;
        if (obj == nullptr)
        {
            *count = 0;
            ptr = nullptr;
        }
    }
    /* 拷贝构造 */

    my_shared_ptr(my_shared_ptr<T> &obj) : ptr(obj.ptr), count(obj.count)
    {
        cout << "拷贝构造" << endl;
        (*count)++;
    }
    /* 重载赋值符号 */
    my_shared_ptr<T> &operator=(my_shared_ptr<T> &obj)
    {
        cout << "调用重载=" << endl;
        if (this != &obj)
        {
            this->release();
            this->ptr = obj.ptr;
            this->count = obj.count;
            (*count)++;
        }
        return *this;
    }

    /* 重载 * */
    T &operator*()
    {
        return *ptr;
    }
    /* 重载 -> */ // 此处有点迷糊...比起*因为不是对自身操作 不知道返回值是什么..
    T *operator->()
    {
        return ptr;
    }
    /* 控制引用计数器 */
    void release()
    {
        (*count)--;
        if (*count == 0)
        {
            if (ptr != nullptr)
            {
                delete ptr;
                cout << "my_shared_ptr析构" << endl;
            }
            else
            {
                cout << "ptr 为空 my_shared_ptr不析构" << endl;
            }
            delete count;
        }
    }

    /* 析构 */
    ~my_shared_ptr()
    {
        release();
    }

    T *ptr;
    int *count;
};

// 模拟string类
class Test
{
public:
    Test(const char *str = nullptr)
    {
        size_t len = sizeof(str);
        this->str = new char[len];
        strcpy(this->str, str);
    }
    ~Test()
    {
        if (str != nullptr)
        {
            cout << "Test析构" << endl;

            delete[] str;
        }
        else
        {
            cout << "Test为空不用析构" << endl;
        }
    }
    void show()
    {
        cout << str << endl;
    }

private:
    char *str;
};
int main()
{
    my_shared_ptr<Test> test1(new Test("hello"));
    my_shared_ptr<Test> test2;
    cout << *(test2.count) << endl;
    test2 = test1;
    (*test2).show();
    test2->show();
    test2.operator->()->show();

    return 0;
}

```
