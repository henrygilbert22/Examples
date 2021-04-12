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
        PreCache();


    private:
        vector<vector<string>> storage;




};

#endif
