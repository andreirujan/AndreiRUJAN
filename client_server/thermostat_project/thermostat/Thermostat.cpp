#include "boost/algorithm/string.hpp"
#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/containers/vector.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

#include <iostream>
#include<mutex>
using namespace std;
using namespace boost::interprocess;

typedef boost::interprocess::allocator<int, boost::interprocess::managed_shared_memory::segment_manager> Allocator;
typedef boost::interprocess::vector<int, Allocator> myVector;
mutex mtx;


managed_shared_memory segment(open_or_create, "EXOAnalysisSharedMemory", 65536);
const Allocator alloc_inst (segment.get_segment_manager());
 myVector *vec = segment.find_or_construct<myVector>("myVector")(alloc_inst);


class Client
{
public:
    float desireTemp;
    float extTemp;
    float roomTemp;
    int clienti=0;
    bool switcher = true;
    string iesire;
    
public:

    void SendData()
    {
        if(switcher)
        {
            ReadData();
        }
    }


    void ReadData()
    {
        roomTemp=20;
        cout<<"Introduceti date? (yes/no) : ";cin>>iesire;
        if(iesire=="no")
        {
            exit(0);
        }

        else
        {
            if(switcher)
            {

                clienti++;
                cout<<"\nClientul "<<clienti<<" scrie date in termostat\n";
                cout<<"desireTemp=";cin>>desireTemp;
                cout<<"extTemp=";cin>>extTemp;
                cout<<"roomTemp="<<roomTemp;
                cout<<endl;
                CreateVector();
            }
        }

    }

    void CreateVector()
    {
        vec->push_back(desireTemp);
        vec->push_back(extTemp);

        for(auto i=0;i<(*vec).size();i++)
        {
            cout<<(*vec)[i]<<endl;
        } 
    }

    void ClearVector()
    {
        vec->clear();
    }

    bool Reset()
    {
        return switcher=false;
    }
};

int main()
{
    Client client;
    
    client.ClearVector();

    while(client.iesire!="no")
    {
        client.SendData();
    }

    client.Reset();
    
    return 0;
}
