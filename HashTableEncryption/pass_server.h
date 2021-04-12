#ifndef PASS_SERVER_H
#define PASS_SERVER_H

#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include <functional>

#include "hashtable.h"
 
using namespace cop4530;

template <typename K, typename V>
class PassServer : public HashTable <K, V>
{
   public:
      explicit PassServer(size_t size = 101); 
      ~PassServer(); 
      bool load(const char *filename);
      bool addUser(std::pair<string, string> & kv);
      bool addUser(std::pair<string, string> && kv);
      bool removeUser(const string & k);
      bool changePassword(const pair<string, string> &p, const string & newpassword);
      bool find(const string & user);
      void dump();
      size_t size();
      bool write_to_file(const char *filename);
      

   private:
      string encrypt(const string & str); 
      HashTable<K,V> myTable;
};


#endif
