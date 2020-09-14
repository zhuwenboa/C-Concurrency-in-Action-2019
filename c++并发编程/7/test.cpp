#include "Lock.h"
#include<vector>
#include<unistd.h>
#include<iostream>
#include<thread>

LockFreeQueue<int> q;

void func1()
{
    std::vector<int> vec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    for(int i = 0; i < vec.size(); ++i)
        q.Enqueue(vec[i]);    
}

void func2()
{
    std::vector<int> vec{11, 12, 13, 14, 15, 16, 17, 18, 19, 20};
    for(int i = 0; i < vec.size(); ++i)
    {
        q.Enqueue(vec[i]);
    }
}

void func3()
{
    int a;
    for(int i = 0; i < 10; ++i)
    {
        q.Try_Dequeue(a);
        std::cout << "func3 a = " << a << "\n";
    }
}

void func4()
{
    int a;
    for(int i = 0; i < 10; ++i)
    {
        q.Try_Dequeue(a);
        std::cout << "func4 a = " << a << "\n";
    }
}

int main()
{
    std::thread t1(func1);
    std::thread t2(func2);
    std::thread t3(func3);
    std::thread t4(func4);
    t1.join();
    t2.join();
    t3.join();
    t4.join();
}