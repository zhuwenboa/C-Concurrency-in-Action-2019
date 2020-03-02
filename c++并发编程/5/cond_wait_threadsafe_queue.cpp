#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>

template<typename T>  
class threadsafe_queue
{
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    std::mutex head_mutex;
    std::unique_ptr<node> head;
    node* tail;
    std::mutex tail_mutex;
    std::condition_variable data_cond;

    node* get_tail()
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        return tail;
    }
    std::unique_ptr<node> pop_head()
    {
    }

    std::unique_ptr<node> wait_for_data()
    {
        std::unique_lock<std::mutex> head_lock(head_mutex);
        data_cond.wait(head_lock, [&]{return head.get() != get_tail();});
        std::unique_ptr<node> old_head = std::move(head);
        head = std::move(old_head->next);
        head_lock.unlock();
        return old_head;
    }


public:  
    threadsafe_queue() : head(new node), tail(head.get()) {}
    //删除拷贝构造和拷贝赋值函数
    threadsafe_queue(const threadsafe_queue&) = delete;
    threadsafe_queue& operator = (const threadsafe_queue&) = delete;
    
    std::shared_ptr<T> try_pop();
    bool try_pop(T &value);
    std::shared_ptr<T> wait_and_pop();
    void wait_and_pop(T &value);
    void push(T new_value);
    bool empty();
};


template<typename T> 
void threadsafe_queue<T>::push(T new_value)
{
    std::shared_ptr<T> new_data(std::make_shared<T>(std::move(new_value)));
    std::unique_ptr<T> p(new node);
    {
        std::lock_guard<std::mutex> tail_lock(tail_mutex);
        tail->data = new_data;
        node* const new_tail = p.get();
        tail->next = new_tail;
        tail = new_tail;
    }   
    data_cond.notify_one();
}
