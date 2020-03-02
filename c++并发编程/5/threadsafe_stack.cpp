#include<iostream>
#include<thread>
#include<stack>
#include<mutex>
#include<memory>

struct empty_stack : std::exception
{
    const char* what() const throw();
};


template<typename T>
class threadsafe_stack
{
public:  
    threadsafe_stack() = default;
    threadsafe_stack(const threadsafe_stack& other)
    {
        std::lock_guard<std::mutex> lock(other.m);  //锁住赋值变量的锁，防止其栈在赋值期间被竞争
        data = other.data;
    }
    threadsafe_stack& operator=(const threadsafe_stack&) = delete; //删除赋值函数

    void push(T new_value);
    std::shared_ptr<T> pop();
    void pop(T &value);
    bool empty() const;

private:  
    std::stack<T> data;
    mutable std::mutex m;

};

template<typename T>
void threadsafe_stack<T>::push(T new_value)
{
    std::lock_guard<std::mutex> lock(m);
    data.push(new_value);
}

template<typename T>
std::shared_ptr<T> threadsafe_stack<T>::pop()
{
    std::lock_guard<std::mutex> lock(m);
    if(data.empty())
        throw empty_stack();
    std::shared_ptr<T> const res(std::make_shared<T>(std::move(data.top())));
    data.pop();
    return res;   
}

template<typename T>
void threadsafe_stack<T>::pop(T& value)
{
    std::lock_guard<std::mutex> lock(m);
    if(data.empty())
        throw empty_stack();
    value = data.top();
    data.pop();
}

template<typename T>
bool threadsafe_stack<T>::empty() const
{
    std::lock_guard<std::mutex> lock(m);
    return data.empty();
}