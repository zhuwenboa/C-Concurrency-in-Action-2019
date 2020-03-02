#include<iostream>
#include<atomic>

std::atomic_flag  f = ATOMIC_FLAG_INIT; // atomic_flag类型必须被ATOMIC_FLAG_INIT初始化。
 
class Spinlock_mutex
{
public: 
    Spinlock_mutex() = default;

    void lock()
    {
        while(flag.test_and_set(std::memory_order_acquire)); //此处为atomic设置标志，如果之前没有设置标志返回false，如果设置返回true。
    }
    void unlock()
    {
        flag.clear(std::memory_order_release);     //清除标志位
    }

private:  
    std::atomic_flag flag = ATOMIC_FLAG_INIT;

};

int main()
{
    std::atomic<bool> a;
    bool b = a.load(std::memory_order_acquire);  //load 加载操作，将atomic<bool>当前值返回为普通的bool类型的值
    std::cout << a << "\n";
    a.store(false);                              //store是一个存储操作
    std::cout << a << "\n";

    b = a.exchange(true, std::memory_order_acq_rel);
    std::cout << a << "\n";


    f.clear();
    bool x = f.test_and_set();
    std::cout << x << "\n";
}