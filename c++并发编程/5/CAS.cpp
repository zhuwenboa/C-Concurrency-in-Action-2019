#include<iostream>
#include<atomic>
#include<unistd.h>
void test_func1()
{
    bool expected = false;
    std::atomic<bool> b;
    std::cout << b << "\n";
    std::cout << b.compare_exchange_weak(expected, true) << "\n";
    /*
    while(!b.compare_exchange_weak(expected, true) && !expected)
    {
        std::cout << "赋值失败\n";
        sleep(3);
    }
    */
    std::cout << b << "\n";
} 

int main()
{
    test_func1();

}