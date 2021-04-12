#include <iostream>
#include <string>
#include "List.h"

using namespace std;
using namespace cop4530;

// Function object types, for use with remove_if()
class isOdd {
   public:
      bool operator() (const int& value) { return (value%2)!=0; }
};

class LongerThanFive {
   public:
      bool operator() (const string& s) { return (s.size() > 5); }
};

int main() {
  
   List<int> s1;
   List<string> s2;
   const int num = 10;

   cout << "Testing list with integer values ..." << endl;
   cout << "pushing back " << num << " integer values" << endl;

   for (int i = 0; i < num; ++i) {
      s1.push_back(i);
   }
   cout << "size " << s1.size() << endl;
   cout<<s1<<endl;
   
   auto itr = s1.end();
  
  
   for (; itr != s1.end(); ++itr) {
     
     cout << " " << *itr;
   }
   cout << endl;

   cout << "pushing front " << num << " integer values" << endl;

   for (int i = 0; i < num; ++i) {
      s1.push_front(i);
   }

   cout << "size " << s1.size() << endl;

   itr = s1.begin();
   cout << *itr++;
  
   for (; itr != s1.end(); ++itr) {
      cout << " " << *itr;
   }
   cout << endl;

   cout << "pop front " << num/2 << " integer values" << endl;
   for (int i = 0; i < num/2; ++i) {
      s1.pop_front();
   }

   cout << "size " << s1.size() << endl;

   itr = s1.begin();
   cout << *itr++;
   for (; itr != s1.end(); ++itr) {
      cout << " " << *itr;
   }
   cout << endl;

   cout << "pop back " << num/2 << " integer values" << endl;
   for (int i = 0; i < num/2; ++i) {
      s1.pop_back();
   }

   cout << "size " << s1.size() << endl;

   itr = s1.begin();
   cout << *itr++;
   for (; itr != s1.end(); ++itr) {
      cout << " " << *itr;
   }
   cout << endl;

   cout << "removing 0" << endl;
   s1.remove(0);
   cout << s1 << endl;

   cout << "size " << s1.size() << endl;
   cout << s1 << endl;

   cout << "removing first 4 elements" << endl;
   itr = s1.begin();
   auto itr_t = itr;
   for (int i = 0; i < 4; ++i)
      ++itr_t;

   s1.erase(itr, itr_t);
   cout << "size " << s1.size() << endl;
   cout << s1 << endl;

   cout << "reverse list" << endl;
   s1.reverse();
   cout << "size " << s1.size() << endl;
   cout << s1 << endl;

   cout << "clearing list" << endl;
   s1.clear();

   if (s1.empty()) {
      cout << "all cleared" << endl;
   } else {
      cout << "wrong with clear() function" << endl;
   }

   cout << "testing other constructors" << endl;
  List<int> l_t1(8, 5);



  cout<<l_t1<<endl;
 
  List<int> l_t2(l_t1.begin(), l_t1.end());
   List<int> iList {1, 1, 3, 2, 3, 90, 4, 4, 5, 3, 7, 8, 90, 10, 2, 90, 80, 70, 60, 50};

   cout << l_t2 << endl;
   cout << iList << endl;

   cout << "deduplicate list" << endl;
   iList.deduplicate();
   cout << "size " << iList.size() << endl;
   cout << iList<< endl;

   cout << "testing init_list assignment operator" << endl;
   iList = {4, 8, 15, 16, 23, 42, 99, 100, 200, 211, 353, 400, 501, 503};
   cout << iList << endl;
   cout << "Removing odd elements of this list" << endl;
   iList.remove_if(isOdd());
   cout << iList << endl;

   cout << "testing comparison operators" << endl;
   if (l_t1 == l_t2) {
      cout << "they are the same" << endl;
   } else {
      cout << "wrong" << endl;
   }

   cout << "remove one element from l_t1" << endl;
   l_t1.pop_back();
   if (l_t1 == l_t2) {
      cout << "wrong" << endl;
   } else {
      cout << "they contain different values" << endl;
   }

   l_t2.push_back(10);
   // testing move
   List<int> l_t3 = std::move(l_t2);
   for (auto & x : l_t3) {
      cout << x << " ";
   }
   cout << endl;

   // check if l_t2 is empty now
   // NOTE that we should NOT access l_t2
   // this is just for the purpose of testing
   // the implementation of move constructor
   if (!l_t2.empty()) {
      cout << "wrong" << endl;
   } else {
      cout << "l_t2 is empty now" << endl;
   }

    //testing string list 
  
   cout << "Testing list with string values ..." << endl;
   cout << "pushing back " << num << " string values" << endl;

   for (int i = 1; i <= num; ++i) {
      string str(i, '0');
      s2.push_back(str);
   }
   cout << "size " << s2.size() << endl;

   auto itr2 = s2.begin();
   cout << *itr2++;
   for (; itr2 != s2.end(); ++itr2) {
      cout << " " << *itr2;
   }
   cout << endl;

   cout << "pushing front " << num << " integer values" << endl;

   for (int i = 1; i <= num; ++i) {
      string str(i, '1');
      s2.push_front(str);
   }

   cout << "size " << s2.size() << endl;

   itr2 = s2.begin();
   cout << *itr2++;
   for (; itr2 != s2.end(); ++itr2) {
      cout << " " << *itr2;
   }
   cout << endl;

   cout << "pop front " << num/2 << " string values" << endl;
   for (int i = 0; i < num/2; ++i) {
      s2.pop_front();
   }

   cout << "size " << s2.size() << endl;

   itr2 = s2.begin();
   cout << *itr2++;
   for (; itr2 != s2.end(); ++itr2) {
      cout << " " << *itr2;
   }
   cout << endl;

   cout << "pop back " << num/2 << " string values" << endl;
   for (int i = 0; i < num/2; ++i) {
      s2.pop_back();
   }

   cout << "size " << s2.size() << endl;

   itr2 = s2.begin();
   cout << *itr2++;
   for (; itr2 != s2.end(); ++itr2) {
      cout << " " << *itr2;
   }
   cout << endl;

   cout << "removing 0" << endl;
   s2.remove("0");
   cout << "size " << s2.size() << endl;
   cout << s2 << endl;

   cout << "removing first 4 elements" << endl;
   itr2 = s2.begin();
   auto itr2_t = itr2;
   for (int i = 0; i < 4; ++i)
      ++itr2_t;
   s2.erase(itr2, itr2_t);
   cout << "size " << s2.size() << endl;
   cout << s2 << endl;

   cout << "reverse list" << endl;
   s2.reverse();
   cout << "size " << s2.size() << endl;
   cout << s2 << endl;

   cout << "clearing list" << endl;
   s2.clear();

   if (s2.empty()) {
      cout << "all cleared" << endl;
   } else {
      cout << "wrong with clear() function" << endl;
   }

   cout << "testing other constructors" << endl;
   List<string> s2_t1(8, "2");
   List<string> s2_t2(s2_t1.begin(), s2_t1.end());

   cout << s2_t2 << endl;

   cout << "testing comparison operators" << endl;
   if (s2_t1 == s2_t2) {
      cout << "they are the same" << endl;
   } else {
      cout << "wrong" << endl;
   }

   cout << "remove one element from s2_t1" << endl;
   s2_t1.pop_back();
   if (s2_t1 == s2_t2) {
      cout << "wronng" << endl;
   } else {
      cout << "they contain different values" << endl;
   }

   List<string> sList {"Harry", "Ron", "Hermione", "Dumbledore", "McGonagall", "Snape", 
      "Luna", "Draco", "Neville", "Fred", "George", "Ginny"};

   cout << sList << endl;
   cout << "Removing names longer than 5 letters" << endl;
   sList.remove_if(LongerThanFive());
   cout << sList << endl;

   return 0;
}
