#include<iostream>
#include<thread>
#include "CAS_stack.h"
#include<queue>
#include<stack>
lock_free_stack<int> st;

std::stack<int> st2;

void func(int a)
{
    std::cout << "push函数运行\n";
    st2.push(a);
}

void func1()
{
    int b = st2.top();
    st2.pop();
    std::cout << "b = " << b << "\n"; 
}

int main()
{
    //std::thread t(func1);
    std::thread t1(func, 3);
    std::thread t2(func, 2);
    std::thread t3(func, 1);
    std::thread t4(func1);
    std::thread t5(func1);    
    //t.join();
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();
    return 0;
}