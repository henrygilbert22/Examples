#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <vector>
#include <list>
#include <string>     // string.c_str(), string();
#include <algorithm>  // std::find
#include <functional> // std::hash
#include <iostream>   // std::cout, std::cerr
#include <fstream>    // ifstream and ofstream
#include <string.h>   // strcpy, strtok, 

using namespace std;

// max_prime is used by the helpful functions provided to you.
static const unsigned int max_prime = 1301081;
 
// SeparateChaining Hash table class
namespace cop4530 {
   template <typename K, typename V>
      class HashTable
      {
         public:
            explicit HashTable(size_t size = 101 ); 
            ~HashTable();
            bool contains(const K & x);
            bool match(const std::pair<K, V> & kv);
            bool insert(const std::pair<K, V> & kv);
            bool insert (std::pair<K, V> && kv);
            bool remove(const K & k);
            void clear();
            bool load(const char * filename);
            void dump();
            bool write_to_file(const char *filename);
	         size_t size() const;

         private:
            vector<list<pair<K,V>>> myTable;
            size_t currentSize;
            void makeEmpty();
            void rehash();
            size_t myhash(const K & x);
            unsigned long prime_below (unsigned long);
            void setPrimes(vector<unsigned long>&);
      };

} // end of namespace cop4530

#include "hashtable.hpp"

#endif
