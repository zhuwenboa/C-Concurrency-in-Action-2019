#include<iostream>
#include<algorithm>
#include<vector>
#include<mutex>
#include<condition_variable>
#include<thread>

//单生产者单消费者模型

class ItemRepository
{
public:
    ItemRepository(int size) : capacity(size), write_pos(0), read_pos(0)
    {
        buf.resize(size);
    }
    std::mutex& get_mutex() {return mutex_;}
    std::condition_variable& get_repo_not_full() {return repo_not_full;}
    std::condition_variable& get_repo_not_empty() {return repo_not_empty;}
public:  
    std::vector<int> buf; //存放任务容器
    int capacity; //容量
    size_t read_pos; //消费者读取产品的位置
    size_t write_pos; //消费者写入产品的位置
    std::mutex mutex_;
    std::condition_variable repo_not_full; //条件变量，指示缓冲区不为满
    std::condition_variable repo_not_empty; //指示缓冲区不为空
};

void product(ItemRepository* ir, int item)
{
    std::unique_lock<std::mutex> lk(ir->mutex_);
    //容器已放满任务，等待产品缓冲区不为满这一条件发生
    while( ( (ir->write_pos + 1) % ir->capacity) == ir->read_pos)
    {
        std::cout << "产品缓冲区满\n";
        (ir->repo_not_full).wait(lk);
    }    
    ir->buf[ir->write_pos] = item;
    ir->write_pos++;
    if(ir->write_pos == ir->capacity)
        ir->write_pos = 0;
    (ir->repo_not_empty).notify_one(); //唤醒
    lk.unlock(); ///释放锁
}

int ConsumeItem(ItemRepository *ir)
{
    int data;
    std::unique_lock<std::mutex> lk(ir->mutex_);
    // item buffer is empty, just wait here.
    while(ir->write_pos == ir->read_pos) {
        std::cout << "Consumer is waiting for items...\n";
        (ir->repo_not_empty).wait(lk); // 消费者等待"产品库缓冲区不为空"这一条件发生.
    }

    data = (ir->buf)[ir->read_pos]; // 读取某一产品
    (ir->read_pos)++; // 读取位置后移

    if (ir->read_pos >= ir->capacity) // 读取位置若移到最后，则重新置位.
        ir->read_pos = 0;

    (ir->repo_not_full).notify_all(); // 通知消费者产品库不为满.
    lk.unlock(); // 解锁.

    return data; // 返回产品.
}







int main()
{
    
}