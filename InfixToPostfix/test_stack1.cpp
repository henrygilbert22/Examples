#include <iostream>
#include <cstdlib>
#include "stack.h"

using namespace std;
using namespace cop4530;

int main() {
	
	Stack<int> intstk;

	cout << "inserting 10 elements" << endl;
	for (unsigned int i = 0; i < 10; ++i) 
		intstk.push(i);

	cout << "Size: " << intstk.size() << endl;

	cout << "elements: " << intstk << endl;

	cout << "emptying the stack" << endl;
	while (!intstk.empty()) {
		cout << intstk.top() << " ";
		intstk.pop();
	}
	cout << endl;

	cout << "Size: " << intstk.size() << endl;

	cout << "inserting 10 elements" << endl;

	for (unsigned int i = 0; i < 10; ++i) 
		intstk.push(i);

	Stack<int> intstk1(intstk);
	Stack<int> intstk2;
	intstk2 = intstk;

	cout << intstk << endl;
	cout << intstk1 << endl;
	cout << intstk2 << endl;

	intstk.push(100);
	if (intstk2 <= intstk)
		cout << "unchanged stack smaller." << endl;
	else
		cout << "ERROR: longer stack is smaller " << endl;

	if (intstk1 == intstk2)
		cout << "Equal stacks" << endl;
	else
		cout << "ERROR: stacks are not equal" << endl;

	intstk1.pop();


	if (intstk1 == intstk2)
		cout << "Error: equal stacks" << endl;
	else
		cout << "Stacks are not equal" << endl;

	if (intstk1 <= intstk2)
		cout << "intstk1 is less than or equal to intstk2" << endl;
	else
		cout << "ERROR: wrong comparision" << endl;

        cout << intstk1 << endl;

	Stack<char> stack2;

	stack2.push('+');
	cout<<stack2.top();

	return(EXIT_SUCCESS);
}
