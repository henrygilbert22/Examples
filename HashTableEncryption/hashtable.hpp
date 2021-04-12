using namespace cop4530;

// returns largest prime number <= n or zero if input is too large
// This is likely to be more efficient than prime_above(), because
// it only needs a vector of size n
template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below (unsigned long n)                                                   // Fucntion given by proffesor
{
   if (n > max_prime)
   {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
   }
   if (n == max_prime)
   {
      return max_prime;
   }
   if (n <= 1)
   {
      std::cerr << "** input too small \n";
      return 0;
   }

   // now: 2 <= n < max_prime
   std::vector <unsigned long> v (n+1);
   setPrimes(v);
   while (n > 2)
   {
      if (v[n] == 1)
         return n;
      --n;
   }

   return 2;
}

//Sets all prime number indexes to 1. Called by method prime_below(n) 
template <typename K, typename V>
void HashTable<K, V>::setPrimes(std::vector<unsigned long>& vprimes)                                           //Function given by proffesor
{
   int i = 0;
   int j = 0;

   vprimes[0] = 0;
   vprimes[1] = 0;
   int n = vprimes.capacity();

   for (i = 2; i < n; ++i)
      vprimes[i] = 1;

   for( i = 2; i*i < n; ++i)
   {
      if (vprimes[i] == 1)
         for(j = i + i ; j < n; j += i)
            vprimes[j] = 0;
   }
}

template <typename K, typename V>
size_t HashTable<K,V>::size() const                                                                            //Returning current size
{
   return currentSize;
}

template <typename K, typename V>
HashTable<K,V>::HashTable(size_t size)                                                                         //Calling resize function based on size
{ 
   myTable.resize(prime_below(size));
}

template <typename K, typename V>
HashTable<K,V>::~HashTable()                                                                                   //Calling vector clear 
{
   myTable.clear();
}

template <typename K, typename V>
bool HashTable<K,V>::contains(const K & x)                                                                     //Getting supposed list then checking the list to see if it was there
{
   auto & currentList = myTable[myhash(x)];

   if(!currentList.empty())
   {
      for(auto i = currentList.begin(); i != currentList.end(); i++)
      {
         if(i->first == x)
            return true;
      }
      return false;
      
   }
   else
   {
         return false;
   }
   


}

template <typename K, typename V>
bool HashTable<K,V>::match(const std::pair<K, V> & kv)                                                            // Seeing if the list contains the given pair
{
   auto & currentList = myTable[myhash(kv.first)];
   return find(begin(currentList), end(currentList), kv) != end(currentList);
}

template <typename K, typename V>
bool HashTable<K,V>::insert(const std::pair<K, V> & kv)                                                           //Finding the correct list, then looping through the list to see if value already exists 
{                                                                                                                 //If the k value exists, then change its v, if not, add new pair based on given input and increment size
   auto & currentList = myTable[myhash(kv.first)];
    if(find(begin(currentList), end(currentList), kv) != end(currentList))
      return false;
    
   for(auto i = currentList.begin(); i != currentList.end(); i++)
   {
      if(i->first == kv.first)
         {
            i->second = kv.second;
            return true;
         }
   }
    currentList.push_back(kv);

    if(++currentSize > myTable.size())
      rehash();

    return true;

}

template <typename K, typename V>
bool HashTable<K,V>::insert (std::pair<K, V> && kv)                                                                  //Same as above, just move
{
 auto & currentList = myTable[myhash(kv.first)];
    if(find(begin(currentList), end(currentList), kv) != end(currentList))
      return false;
   

   for(auto i = currentList.begin(); i != currentList.end(); i++)
   {
      if(i->first == kv.first)
         {
            i->second = move(kv.second);
            return true;
         }
   }
    currentList.push_back(std::move(kv));

    if(++currentSize > myTable.size())
      rehash();
      
    return true;
}

template <typename K, typename V>
bool HashTable<K,V>::remove(const K & k)                                                                             //Getting supposed list then checking if list actually contains the value and removing correct pair if it does
{
   auto & currentList = myTable[myhash(k)];
   if(!currentList.empty())
   {
      for(auto i = currentList.begin(); i != currentList.end(); i++)
         {
         if(i->first == k)
            {
               currentList.erase(i);
               currentSize--;
               return true;
            }
         }
      return false;
   }

   return false;

}

template <typename K, typename V>
void HashTable<K,V>::clear()                                                                                         //Calling makeempty function
{
   makeEmpty();
}

template <typename K, typename V>
bool HashTable<K,V>::load(const char * filename)                                                                     //Looping through file, creating pairs based on inputs then pushign ito hashtable
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
      myPair.second = value;

      insert(myPair);
   }

   InFile.close();
   return true;


}

template <typename K, typename V>
void HashTable<K,V>::dump()                                                                                             //Looping through all lists then all positons in list to print out hashtable
{
   cout<<"------Beginning the Dump--------"<<endl;
   for(auto & thisList: myTable)
   {
      if(!thisList.empty())
      {
         for(auto & item: thisList)
         cout << item.first << " "<< item.second << "|";

         cout<<endl;
      }
      
   }
   cout<<"------Ending the Dump--------"<<endl;
      
}


template <typename K, typename V>
bool HashTable<K,V>::write_to_file(const char *filename)                                                             //Same thing as dump exept writing to a file
{
   ofstream myfile;
   myfile.open (filename);
   myfile.clear();

   if(!myfile)
      return false;

   for(auto & thisList: myTable)
   {
      if(!thisList.empty())
      {
         for(auto & item: thisList)
         myfile << item.first << " "<< item.second << "|";

         myfile<<endl;
      }
      
   }

   myfile.close();
   return true;

}

template <typename K, typename V>
void HashTable<K,V>::makeEmpty()                                                                                        //Clearing all the lists
{
   for(auto & thisList: myTable)
      thisList.clear();
}

template <typename K, typename V>
void HashTable<K,V>::rehash()                                                                                           //Creating hashtable to store old hashtable
{
   vector<list<pair<K,V>>> oldTable = myTable;                                                                          //Creating new hashtable and sie 2*prime and inptting old hashtable into it 

   myTable.resize(prime_below(2*myTable.size()));

   for(auto & thisList: myTable)
      thisList.clear();
   
   currentSize = 0;
   pair<K,V> myPair;

   for(auto & thisList: oldTable)
      for(auto & item: thisList)
         insert(std::move(item));


   
}

template <typename K, typename V>
size_t HashTable<K,V>::myhash(const K & x)                                                                           //My hashfuntion derived from the textbook
{
   static hash<K>  ht;
   return ht(x)% myTable.size();
}


