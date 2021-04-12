#include <iostream>
#include <algorithm>
#include "List.h"

using namespace std;
using namespace cop4530;


int main()
{


    List<int> MyList(5,20);

    for(int i=0; i<10;i++)
    {
        if(i<5)
            {
                MyList.push_front(i);
            }
        else
        {
            MyList.push_back(i);
        }
      
    }

    MyList.print(cout);
    cout<<endl;

     List<int> copyList;
     copyList = MyList;

    cout<<"copy"<<endl;
    copyList.print(cout);


    MyList.reverse();
    cout<<endl;
    MyList.deduplicate();

    cout<<"print out new list: "<<endl;
    cout<<MyList<<endl;

    List<int>::const_iterator itr = MyList.begin();

    cout<<*itr<<endl;
  

  




   





return 0;
}