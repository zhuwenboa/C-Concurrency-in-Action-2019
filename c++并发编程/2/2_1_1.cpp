#include<iostream>
#include<thread>

using namespace std;

class backfun
{
public:  
    void operator() () const
    {
        cout << "backfun start to run\n";
    }
};

int main()
{
    backfun f;
    //thread my_thread(f);
    thread my_thread( backfun{} );
    my_thread.join();
}