#include<iostream>
#include<memory>
#include<thread>
#include<mutex>


std::shared_ptr<int> resource_prt;
std::once_flag resource_flag;


void init_resource()
{
    resource_prt.reset(new int(5));
    std::cout << *resource_prt << "\n";
}

void foo()
{
    std::call_once(resource_flag, init_resource);
}

int main()
{
    std::thread t1(foo);
    std::thread t2(foo);
    std::thread t3(foo);
    t1.join();
    t2.join();
    t3.join();
    return 0;
}