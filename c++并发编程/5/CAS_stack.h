#include<iostream>
#include<atomic>
#include<vector>

//实现一个无锁的线程安全栈

template<typename T>
class lock_free_stack
{
private: 
    struct node
    {
        T data;
        node *next;
        node(const T &a) : data(a) {}
    };
    std::atomic<node*> head; 
public:  
    void push(const T& data);
    T pop();
};

template<typename T>
void lock_free_stack<T>::push(const T& data)
{
    node *new_node = new node(data);
    new_node->next = head.load();
    /*
    *代码的亮点是使用“比较/交换”操作：返回false时，因为比较失败(例如，head被其他线程锁修改)，
    *会使用head中的内容更新new_node->next(第一个参数)的内容。循环中不需要每次都重新加载head指针，
    *因为编译器会完成这件事。
    */
    while(!head.compare_exchange_weak(new_node->next, new_node));
    std::cout << "push函数运行\n";
    /*
    * 就不需要这样子， 但是这样的代码看起来逻辑更清楚
    do
    {
        new_node->next = head.load();
    } while (!head.compare_exchange_weak(new_node->next, new_node));
    */
}

template<typename T> 
T lock_free_stack<T>::pop()
{
    node *p = head.load();
    //还要判断当栈为空的情况
    while(head.load() && !head.compare_exchange_weak(p, p->next));
    if(!p)
        return 0;
    T data = p->data;
    delete p;
    return data;
}

