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
#include <unistd.h>

#include "hashtable.h"
#include "pass_server.h"

using namespace std;
using namespace cop4530;
 
template <typename K, typename V>
size_t PassServer<K,V>::size()                                                                              // Just utilizing HashTable size function
{
   return myTable.size();
}

template <typename K, typename V>
PassServer<K,V>::PassServer(size_t size)                                                                    // Creaitng new HashTable based off input size
{
   myTable = HashTable<K,V>(size);
}

template <typename K, typename V>  
PassServer<K,V>::~PassServer()                                                                               // Clearning the hash table     
{
   myTable.clear();
}

template <typename K, typename V>
bool PassServer<K,V>::load(const char *filename)                                                               // Creaitng new pair based off input from file. Then encypting the data and pushing it into the hashTable
{

   ifstream InFile;
   InFile.open(filename);
   InFile.clear();

   K key;
   V value;
   pair<K,V> myPair;

   if(!InFile)
      return false;

   while(InFile)
   {
      InFile >> key >> value;
      myPair.first = key;
      myPair.second = encrypt(value);
      myTable.insert(myPair);
   }

   InFile.close();
   return true;
}

template <typename K, typename V>
bool PassServer<K,V>::addUser(std::pair<string, string> & kv)                                                  //Encrypting the data then pushing it onto the hashtable
{
   kv.second = encrypt(kv.second);
   return myTable.insert(kv);
}

template <typename K, typename V>
bool PassServer<K,V>::addUser(std::pair<string, string> && kv)                                                 //Same as above but with move
{
   kv.second = encrypt(kv.second);
   return myTable.insert(std::move(kv));
}

template <typename K, typename V>
bool PassServer<K,V>::removeUser(const string & k)                                                                //Using Hashtable remove function
{
  return myTable.remove(k);
}

template <typename K, typename V>
bool PassServer<K,V>::changePassword(const pair<string, string> &p, const string & newpassword)                  //Encrypting given data first because what it will be matched to is already encrypted. 
{                                                                                                                // Then error checking for if it contains or the password is not correct
   string pass1 = encrypt(p.second);                                                                             // Then creaing new pair and adding it into the hashTable
   string pass2 = encrypt(newpassword);

   if(!myTable.contains(p.first))
      return false;

   if(p.second == newpassword)
      return false;


   pair<K,V> pairToCompare;

   pairToCompare.first = p.first;
   pairToCompare.second = pass1;

   if(myTable.match(pairToCompare))
   {
      pair<K,V> newPair;

      newPair.first = p.first;
      newPair.second = pass2;

      myTable.insert(newPair);

      return true;
   }
   
      return false;
   
   

   

}

template <typename K, typename V>
bool PassServer<K,V>::find(const string & user)                                                                      // Using hashtable functon contains
{
   return myTable.contains(user);
}

template <typename K, typename V>
void PassServer<K,V>::dump()                                                                                         //Using hashtable function dump
{
   myTable.dump();
}

template <typename K, typename V>
bool PassServer<K,V>::write_to_file(const char *filename)                                                            //Using hashtable fucntion write to
{
   return myTable.write_to_file(filename);
}

template <typename K, typename V>
string PassServer<K,V>::encrypt(const string & str)                                                                  //Uisng given example on how to encrypt
{
   char salt[] = "$1$################";
   string key = str;

   char * password = new char [100];
   strcpy (password, crypt(key.c_str(), salt));
   password =  strtok (password, "$");
   password =  strtok (NULL, "$");
   password = strtok(NULL, "S");
   return password;

}






/* XXX: Please keep this line at the very end */
template class PassServer<string, string>;
