#include "cache.h"

Cache::Cache()
{
    ifstream InFile;                                                                                //Opening trace.config file
    InFile.open("trace.config");
    InFile.clear();
    string myString;
    vector<int> myVector;

    if(!InFile)
    {
        cout<<"ERROR: File does not exist"<<endl;
        return;
    }


     while(getline(InFile, myString))                                                               //Getting file contents
    {
       stringstream myToken(myString);

       while(getline(myToken, myString, ' '))
       {
       }

    myVector.push_back(stoi(myString));                                                             //Really dumb way of parsing the string
   }

    int numSets = myVector[0];                                                                      //Getting values
    int setSize = myVector[1];
    int lineSize = myVector[2];

    myCache.resize(numSets);                                                                        //Setting up my cache based on input
    for(int i=0; i< numSets; i++)
        myCache[i].resize(setSize);


    indexAmount = log2(numSets);                                                                    //Getting correct relative numbrs based off log2
    offSetAmount = log2(lineSize);
    tagAmount = 32-indexAmount-offSetAmount;

    for(int i=0; i< myCache.size(); i++)
        myCache[i][0].LRU = true;

    cout<< "Ref     Access      Address     Tag     Index       Offset      Result      Memrefs" << endl;   //Printing out header
    cout<<"----    ------     --------   -------   -----       ------      ------      -------"  << endl;

}

Cache::~Cache()                                                                                     //Clearing all the vectors
{
    myCache.clear();
    CacheStorage.clear();
    results.clear();
}

void Cache::ReadData()
{
    int size;                                                                                       //Creating all the variables I need to create
    int num;
    int offSet;
    int tag;
    int index;
    char action1;
    vector<string> storage;
     

    string line = " ";
    int counter = 1;
    while (!cin.eof())                                                                              //Reading until end of file
    {   
        getline(cin,line);

        
        stringstream myToken(line);     

        while(getline(myToken, line, ':'))                                                          //Parsing data
            {
                storage.push_back(line);
            }
    
        action1 = storage[0][0];                                                                    //Setting correct variables
        size = stoi(storage[1]);
        num = stoi(storage[2], 0, 16);

        
        if(size != 1 && size !=2 && size !=4 && size !=8 )                                          //Error checking for allignment and size constraits
        {
            cerr<<"Invalid size: " << size<< " on line "<< counter << endl;
        } 
        else if(num % size != 0)
        {
            cerr<<"Invalid line " << " on line "<< counter <<endl;
        }
        else
        {
            offSet = bitManip(num, offSetAmount, 1);                                            //Setting my offset, index and tag using my bitManip function
            index = bitManip(num, indexAmount, offSetAmount + 1);
            tag = bitManip(num, tagAmount, offSetAmount + indexAmount + 1);
        
            PreCache newPreCache(tag,storage[2], index, offSet, action1);                           //Creating new preCache struct to hold data
            CacheStorage.push_back(newPreCache);                                                    //Loading it into my Cach storage
        }   

        
        loadCache();                                                                                //Calling load cache then clearing all variables
        CacheStorage.clear();
        storage.clear();
        counter++;
    }

    printResults();                                                                                 //After end of file. print all results

}

int Cache::bitManip(int number, int k, int p) 
{ 
    return (((1 << k) - 1) & (number >> (p - 1)));                                                  //Getting the bit values between two indexs
} 

pair<bool,bool> Cache::cacheInserter(int index, int tag, char action)
{
    pair<bool,bool> myPair;                                                                         //Creating a pair because I want to return two different bool values

     for(int i =0; i<myCache[index].size(); i++)
        {
            if(myCache[index][i].v==true && myCache[index][i].tag == tag)                           //First checking if anything matches the input and v is already set
                {
                    myPair.first = true;
                    myPair.second = false;
                    return myPair;
                }
        }
        for(int i =0; i<myCache[index].size(); i++)                                                 // Then finding the position that is LRU
        {
            if(myCache[index][i].LRU == true)
            {
                myCache[index][i].LRU = false;                                                      //Setting it's LRU to false as it is now being accsees
                if(i+1 < myCache[index].size())                                                     //Setting next avaible spot as LRU
                {       
                    myCache[index][i+1].LRU = true;
                }
                else
                {
                    myCache[index][0].LRU = true;
                }

                    myPair.first = false;                                                           //Setting it to a miss

                    myCache[index][i].v = true;                                                     //Initializing the v and setting the tag
                    myCache[index][i].tag = tag;

                    if(action == 'W')                                                               //Incrementing the amount of times its been written too
                        myCache[index][i].doubleAccses ++;

                    if(myCache[index][i].doubleAccses > 1)
                        myPair.second = true;     

                    return myPair;       

            }
        }
    

        return myPair;

}

void Cache::loadCache()
{
    double internalMissCount =0;                                                                    //Initializing count varaibles
    double Total =0;

    int counter =1;
    for(auto preCache: CacheStorage)                                                                //Looping through my storage vector
    {
        pair<bool,bool> myPair = cacheInserter(preCache.index, preCache.tag, preCache.action );     //Getting the correct output of hit or miss when inserting
        string action;
        if(preCache.action == 'W')
            action = "write";
        else
            action = "read";
        
        if(myPair.first)                                                                            //Just printing out the data here
            results.push_back(to_string(counter) + "        " +  action + "         " + preCache.address + "          " + to_string(preCache.tag) +"          " + to_string(preCache.index) + "         " + to_string(preCache.offset) + "           hit             0");
        else
        {
            if(myPair.second)
                results.push_back(to_string(counter) + "        " +  action + "         " + preCache.address + "          " + to_string(preCache.tag) +"          " + to_string(preCache.index) + "         " + to_string(preCache.offset) + "          miss           2");
            else
                results.push_back(to_string(counter) + "        " +  action + "         " + preCache.address + "          " + to_string(preCache.tag) +"          " + to_string(preCache.index) + "         " + to_string(preCache.offset) + "          miss           1");
            
            missCount++;

        }
        Total++;
    
        counter++;
    }
    totalCount += Total;
    missCount += internalMissCount;
    
    for(auto s: results)
        cout<< s<<endl;

    results.clear();


}



void Cache::print()
{
    for(auto subCache: myCache)
    {
        for(auto myInner: subCache)
        {
            cout<< " v: " << myInner.v << " tag: " << myInner.tag << " Data: " << myInner.data << " LRU: " << myInner.LRU << "      ";
        }
        cout<<endl;
    }
}

void Cache::printResults()
{
    cout<<"Simulation Summary Statistics"<<endl;
    cout<<"------------------------------"<<endl;
    cout<<"Total hits       :" << totalCount - missCount <<endl;
    cout<<"Total misses     :" << missCount << endl;
    cout<<"Total accses     :" << totalCount <<endl;
    cout<<"Hit ratio        :" << (totalCount-missCount)/totalCount <<endl;
    cout<<"Miss ratio       :" << missCount/totalCount <<endl;
}