#include <iostream>
#include "Cell.h"
#include "NSequence.h"
#include "FunClassTemp.h"

using namespace std;

int main( )
{
    NSequence <int>     v1 (0); /* one item added for an empty sequence */
    NSequence<IntCell> v2(0);

    for( int i = 0; i < v1.getSize(); i++ ) {
        v1[i] = i+10;
        v2[i].write(i+10);
        }
    for( int i = 0; i < 15; i++ ) {
        v1.push_back(7*i+7);
        v2.push_back(IntCell(7*i+7));
        }
    for( int i = 0; i < v1.getSize(); i+=2 ) {
        v1.remove(i);
        v2.remove(i);
    }
    for( int i = 0; i < v1.getSize(); i+=3 ) {
        v1.insert(i, 8*i + 8);
        v2.insert(i,IntCell(8*i + 8));
        }



	int begin;
	int sub;
    begin = 0;
    sub = longestSubseq_Linear<IntCell>(v2, IntCell(30), 20, &begin);
    cout << "Linear func found "<< sub << " items:";
    v1.printout(begin, begin+sub -1 );

    begin = 0;
    sub = longestSubseq_Quad<IntCell>(v2, IntCell(30), 20, &begin);
    cout << "Quad func found "<< sub << " items:";
    v1.printout(begin, begin+sub -1 );

    begin = 0;
    sub = longestSubseq_Cubic<IntCell>(v2, IntCell(30), 20, &begin);
    cout << "Cubic func found "<< sub << " items:";
    v1.printout(begin, begin+sub -1 );

return 0;
}
