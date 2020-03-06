#include<iostream>
#include<atomic>
#include<thread>
#include<vector>
#include<algorithm>
#include<memory>

template<typename T>
class lock_free_queue
{
private:
    struct node
    {
        T data;
        node* next;

        node() : next(nullptr) {}
    };
    
    //支持原子操作的头指针和尾指针
    std::atomic<node*> head;
    std::atomic<node*> tail;

public:  
    lock_free_queue() : head(new node()), tail(head.load()) {}
    
    T pop_head()
    {
        T value;
        node*  old_head;
        node*  next;
        node*  old_tail;
        while(true)
        {
            old_head = head.load();
            next = old_head->next;
            old_tail = tail.load();
            //头结点不是最新的头结点，则重新获取头结点
            if(old_head != head.load())
                continue;
            
            //判断队列是否为空，首先要判断head == tail 还要判断head->next 为NULL。
            if(old_head == tail.load() && next == NULL)
            {
                return -1;
            }
            //如果tail指针落后
            if(old_head == tail.load() && next != NULL)
            {
                tail.compare_exchange_weak(old_tail, next);
                continue;
            }
            if(head.compare_exchange_weak(old_head, old_head->next))
            {
                value = old_head->data;
                break;
            }
        }
        delete(old_head);
        return value; 
        //return old_head;
    }

    void push(T new_value)
    {
        node* new_node;
        new_node->data = new_value;


        node*  old_tail;
        while(true)
        {
            old_tail = tail.load();
            std::atomic<node*> next(old_tail->next);
            //如果获取的尾指针不等于当前的尾指针，证明有别的线程操作了tail指针，所以需要重新获取tail指针
            if(tail != tail.load())
                continue;
            
            //如果尾指针的next不等于NULL，则fetch全局尾指针到next
            if(old_tail->next != NULL)
            {
                while(!tail.compare_exchange_weak(old_tail, old_tail->next));
                continue;
            }

            //将尾节点置于next指针
            if(next.compare_exchange_weak(old_tail->next, new_node))
            {
                node* temp = tail.load();
                std::cout << temp->next->data << "==\n";
                break;   
            }
        }
        //置尾结点
        tail.compare_exchange_weak(old_tail, new_node);
        std::cout << "插入成功\n";
    }
};