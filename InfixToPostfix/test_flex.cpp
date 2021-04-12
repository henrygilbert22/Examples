#include <math.h>
#include <iostream>

#include "string.h"
#include "opnum.h"

using namespace std;

int main(int argc, char **argv)
{
   int retval = SYM_NULL;
   set_input(argc, argv);

    do {
       char * opnum = get_opnum(&retval);
       if (retval) {
          switch (retval)
          {
             case SYM_NAME: cout<<"ID: "; break;
             case SYM_INTEG: cout<<"Integer: "; break;
             case SYM_FLOAT: cout<<"Float: "; break;
             case SYM_ADD: cout<<"Op: "; break;
             case SYM_SUB: cout<<"Op: "; break;
             case SYM_MUL: cout<<"Op: "; break;
             case SYM_DIV: cout<<"Op: "; break;
             case SYM_OPEN: cout<<"left Par: "; break;
             case SYM_CLOSE: cout<<"Right Par: "; break;
             case SYM_INVAL: cout<<"Invalid: "; break;
             default: break; 
          }
          cout <<opnum<<endl;
       }
    } while (retval > SYM_NULL);
}
