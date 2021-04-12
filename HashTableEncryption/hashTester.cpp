#include <vector>
#include <list>
#include <string>     // string.c_str(), string();
#include <algorithm>  // std::find
#include <functional> // std::hash
#include <iostream>   // std::cout, std::cerr
#include <fstream>    // ifstream and ofstream
#include <string.h>   // strcpy, strtok, 
#include <unistd.h>   // crypt()
#include <stdlib.h>   

#include "hashtable.h"
#include "pass_server.h"

using namespace std;
using namespace cop4530;
 
 int main()
 {
     cout<<"made it"<<endl;

     HashTable<string,string> myTable;

     myTable.load("sample_users");

     myTable.dump();

     myTable.write_to_file("outputText2");

     return 0;
 }
