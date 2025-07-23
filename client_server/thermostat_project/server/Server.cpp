#include<boost/interprocess/managed_shared_memory.hpp>
#include<boost/interprocess/containers/vector.hpp>
#include<boost/interprocess/allocators/allocator.hpp>
#include<boost/interprocess/sync/named_mutex.hpp>
#include<iostream>
#include<thread>
#include<mutex>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<chrono>
#include<pthread.h>
using namespace boost::interprocess;
using namespace std;

mutex mtx;
typedef boost::interprocess::allocator<int, boost::interprocess::managed_shared_memory::segment_manager> Allocator;
typedef vector<int, Allocator> myVector;


managed_shared_memory segment(open_or_create, "EXOAnalysisSharedMemory", 65536);
const Allocator alloc(segment.get_segment_manager());
myVector *vec = segment.find_or_construct<myVector>("myVector")(alloc);
    

class AC
{
public:
    float roomTemperature;
    bool switcher=false;
    float desire;
    float ext;


    int Heating(float desire, float ext)
    {
        roomTemperature=20;

        if(roomTemperature<=desire)
        {
            switcher=true;// porneste switcherul
            cout<<"\nServerul porneste!\n";
        }

        while(roomTemperature<desire)
        {
            roomTemperature=roomTemperature+ 0.5;
            cout<<"desire temperature: "<<desire<<endl;
            cout<<"exterior temperature: "<<ext<<endl;
            cout<<"room temperature: "<<roomTemperature<<endl;
            this_thread::sleep_for(1000ms);
            cout<<"\n--------------\n";
        }

        if(roomTemperature==desire)
        {
            Reset();// opreste switcherul
            cout<<"\nServerul se opreste!\n";
        }

        return roomTemperature;

    }

    int Heating2(float desire, float ext)
    {

        if(!switcher)
        {
            while(ext>roomTemperature)
            {
                roomTemperature=roomTemperature+0.1;
                cout<<"exterior temperature: "<<ext<<endl;
                cout<<"desire temperature: "<<desire<<endl;
                cout<<"room temperature: "<<roomTemperature<<endl;
                this_thread::sleep_for(1000ms);
                cout<<"\n--------------\n";   
            }
        }

        
        return roomTemperature;
    }

    int Cooling(float desire, float ext)
    {
        roomTemperature=20;

        if(roomTemperature>=desire)
        {
            switcher=true;
            cout<<"\nServerul porneste!\n";
        }

        while(roomTemperature>desire)
        {
            roomTemperature=roomTemperature- 0.5;
            cout<<"desire temperature: "<<desire<<endl;
            cout<<"exterior temperature: "<<ext<<endl;
            cout<<"room temperature: "<<roomTemperature<<endl;
            this_thread::sleep_for(1000ms);// 1 second
            cout<<"\n--------------\n";
        }

        if(roomTemperature==desire)
        {
            Reset();
            cout<<"\nServerul se opreste!\n";
        }

        return roomTemperature;

    }

    int Cooling2(float desire, float ext)
    {

        if(!switcher)
        {
            while(ext<roomTemperature)
            {
                roomTemperature=roomTemperature-0.1;
                cout<<"exterior temperature: "<<ext<<endl;
                cout<<"desire temperature: "<<desire<<endl;
                cout<<"room temperature: "<<roomTemperature<<endl;
                this_thread::sleep_for(1000ms);
                cout<<"\n--------------\n";   
            }
        }

        
        return roomTemperature;
    }

    void DeltaTemp(float desire, float ext)
    {
        if((roomTemperature-desire)>=1)
        {
            Cooling(desire, ext);
            Reset();
            //Cooling2(desire, ext);
        }

        else
         if((desire-roomTemperature)>=1)
         {
            Heating(desire, ext);
            Reset();
            //Heating2(desire, ext);
        }

        else
        {
            cout<<"\n...\n";
        }
    }


    void PrintData()
    {
        
        cout << "roomTemp=" << roomTemperature << endl;
        cout << "desireTemp=" << desire << endl;
        cout << "extTemp=" << ext << endl;
        
    }

    bool Reset()
    {
        return switcher=false;
    }

    bool Set()
    {
        return switcher=true;
    }

};


/*int main()
{
    AC srv;
    int contor=1;
    float a,b;
    for(auto i=0;i<(*vec).size();i=i+2)
    {
        a=(*vec)[i];
        b=(*vec)[i+1];
        cout<<"\nProcesam informatiile de la clientul "<<contor<<endl;
        srv.DeltaTemp(a,b);
        cout<<"\nClientul "<<contor<<" e gata\n";
        cout<<"================"<<endl;
        contor++;
    }
    srv.Reset();
    contor=1;
    for(auto i=0;i<(*vec).size();i=i+2)
    {
        cout<<"\nNivelam temperatura din camera pentru clientul "<<contor<<endl;
        a=(*vec)[i];
        b=(*vec)[i+1];
        srv.roomTemperature=a;
        if(b>srv.roomTemperature)
        {
            srv.Heating2(a,b);
        }
        else
        {
            srv.Cooling2(a,b);
        }
        contor++;
    }
    
}*/


int main()
{
    AC srv;
    int contor;
    float a,b;
    int opt;

    do{
        cout<<"\nDati optiunea(1-continuare proces; 2-iesire din proces): ";cin>>opt;
        cout<<endl;

        switch (opt)
        {
            case 1:
                srv.Set();
                contor=1;
                for(auto i=0;i<(*vec).size();i=i+2)
                {
                    a=(*vec)[i];
                    b=(*vec)[i+1];

                    cout<<"\nProcesam informatiile de la clientul "<<contor<<endl;
                    srv.DeltaTemp(a,b);
                    cout<<"\nClientul "<<contor<<" e gata\n";
                    cout<<"================"<<endl;
                    contor++;
                }

                srv.Reset();

                contor=1;
                for(auto i=0;i<(*vec).size();i=i+2)
                {
                    cout<<"\nNivelam temperatura din camera pentru clientul "<<contor<<endl;
                    a=(*vec)[i];
                    b=(*vec)[i+1];
                    srv.roomTemperature=a;
                    if(b>srv.roomTemperature)
                    {
                        srv.Heating2(a,b);
                    }
                    else
                    {
                        srv.Cooling2(a,b);
                    }
                    contor++;
                }
                break;
            case 2:
                exit(0);
                break;
    
            default:
                cout<<"\noptiune gresita\n";
                break;
        }

    }while(opt!=2);
}