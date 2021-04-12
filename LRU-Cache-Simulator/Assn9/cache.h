#ifndef CACHE_H
#define CACHE_H

#include <vector>
#include <list>
#include <string>     // string.c_str(), string();
#include <algorithm>  // std::find
#include <functional> // std::hash
#include <iostream>   // std::cout, std::cerr
#include <fstream>    // ifstream and ofstream
#include <string.h>   // strcpy, strtok, 
#include <sstream>
#include <bitset>

using namespace std;

class Cache{

    public:
        Cache();
        ~Cache();
        void ReadData();
        int bitManip(int number, int k, int p);
        pair<bool,bool> cacheInserter(int index, int tag, char action);
        void loadCache();
        void print();
        void printResults();
        


    private:

        struct InnerCache {    

            bool v;
            int tag;
            int data;
            bool LRU;
            int doubleAccses;



            InnerCache(int vPass = false, int tagPass = 0, int dataPass=0, bool LRUPass = false)
            {v = vPass; tag = tagPass; data = dataPass; LRU = LRUPass;doubleAccses = 0;}

             
        };

        struct PreCache {    

            int tag;
            string address;
            int index;
            int offset;
            char action;


            PreCache(int tagPass, string addressPass, int indexPass, int offsetPass, char actionPass)
            {tag = tagPass; address = addressPass; index = indexPass; offset = offsetPass; action = actionPass;}
            
        };

        int tagAmount;
        int offSetAmount;
        int indexAmount;
        double missCount;
        double totalCount;
        vector<vector<InnerCache>> myCache;
        vector<PreCache> CacheStorage;
        vector<string> results;





};

#endif
