#include <iostream>
#include <string>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include "List.h"

using namespace std;
using namespace cop4530;

#define REPEATS 5

int main(int argc, char ** argv) {

   int cnt = 100;

for(int i=0; i<3; i++)
{
 List<int> nums[REPEATS]; 

   int range = cnt / 3;

   for (int j = 0; j < REPEATS; j++) {
      /* initialize a random seed;
       * generate a random number between 1 and range.  */
      srand (time(NULL));
      for (int i = 0; i < cnt; i++) {
         int item = rand() % range + 1;
         nums[j].push_back(item);
      }
   }

   auto r_start = chrono::steady_clock::now();
   for (int i = 0; i < REPEATS; i++) {
      nums[i].reverse();
   }
   auto r_end = chrono::steady_clock::now();

   int usec = chrono::duration_cast<chrono::microseconds>(r_end - r_start).count();
   cout << "reverse " <<cnt<<"-item list: " << usec/REPEATS<<" usec"<<endl;
   cnt = cnt*10;

}

cnt = 100;
for(int i=0; i<3; i++)
{
 List<int> nums[REPEATS]; 
 

   int range = cnt / 3;

   for (int j = 0; j < REPEATS; j++) {
      /* initialize a random seed;
       * generate a random number between 1 and range.  */
     
      srand (time(NULL));
      for (int i = 0; i < cnt; i++) {
         int item = rand() % range + 1;
         nums[j].push_back(item);
      }
   }

   auto r_start = chrono::steady_clock::now();
   for (int i = 0; i < REPEATS; i++) {
      nums[i].deduplicate();
   }
   auto r_end = chrono::steady_clock::now();

   int usec = chrono::duration_cast<chrono::microseconds>(r_end - r_start).count();
   cout << "deduplicate " <<cnt<<"-item list: " << usec/REPEATS<<" usec"<<endl;

   cnt = cnt*10;
}
  



   return 0;
}
