磕磕绊绊 加油!

```cpp
/*
实现一个list
1.构造
2.pushfront
3.pushback
4.迭代器类 包括运算符重载 前置++ 后置++  指针操作->   * ==  !=
5.begin 返回迭代器 end 返回迭代器
 */
#include <iostream>
using namespace std;
// 假设左边是:front prev   右边end next
template <typename T>
class Node
{
public:
    // 三种情况第一次 第一次插数据 前插数据  后插数据
    Node(T val, Node *head, Node *tail) : val(val), next(head), prev(tail) {}

    T val;
    Node *next;
    Node *prev;
};
template <typename T>
class List
{
public:
    class iterator // 迭代器的本质就是经过封装的指向list每一个节点的指针
    {
    public:
        iterator operator++(int)
        {
            iterator temp;
            temp = *this;
            itr = itr->next;
            return temp;
        }
        iterator &operator++()
        {
            itr = itr->next;
            return *this;
        }
        T operator*()
        {
            return itr->val;
        }
        bool operator!=(const iterator &i)
        {
            if (this->itr != i.itr)
            {
                return 1;
            }
            return 0;
        }
        int operator==(iterator &i)
        {
            if (this->itr == i.itr)
            {
                return 1;
            }
            return 0;
        }

        // private:
        Node<T> *itr;
    };
    List()
    {
        this->head = NULL;
        this->tail = NULL;
    }
    iterator list_begin()
    {
        iterator temp;
        temp.itr = this->head;
        return temp;
    }
    iterator list_end()
    {
        iterator temp;
        temp.itr = this->tail->next;
        return temp;
    }

    void push_front(T value)
    {
        if (!this->head)
        {
            Node<T> *temp = new Node<T>(value, NULL, NULL);
            this->head = temp;
            this->tail = temp;
        }
        else
        {
            Node<T> *temp = new Node<T>(value, head, NULL);
            head->prev = temp;
            head = temp;
        }
    }
    void push_back(T value)
    {
        if (!this->tail)
        {
            Node<T> *temp = new Node<T>(value, NULL, NULL);
            this->head = temp;
            this->tail = temp;
        }
        else
        {
            Node<T> *temp = new Node<T>(value, NULL, tail);
            tail->next = temp;
            tail = temp;
        }
    }

private:
    Node<T> *head;
    Node<T> *tail;
};
int main()
{
    List<int> list;
    list.push_back(1);
    list.push_front(2);
    list.push_front(4);
    list.push_front(3);
    list.push_front(11);
    list.push_back(23);
    for (List<int>::iterator it = list.list_begin(); it != list.list_end(); it++)
    {
        cout << *it << endl;
    }
    return 0;
}

```
