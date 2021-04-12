#include <math.h>
#include <iostream>
#include "string.h"
#include "opnum.h"
#include "stack.h"
#include <sstream>

using namespace cop4530;

using namespace std;



void stackEvaluator(Stack<string> stack)
{
   Stack<float> evaluator;
   while(!stack.empty())
   {
      if(stack.top().compare("+")!=0 && stack.top().compare("-")!=0  && stack.top().compare("*")!=0  && stack.top().compare("/")!=0 )              //Checking if the top is not an operator
      {                                                                                                                                            //if not, changing the string into a float and pushing it onto new stack

         stringstream floatVal(stack.top());
         float number = 0;
         floatVal >> number;     
         evaluator.push(number);
      }
      else
      {
         
         float valOne = evaluator.top();                                                                                                           //If we hit an operator then pop the top two values
         evaluator.pop();
         float valTwo = evaluator.top();
         evaluator.pop();

         if(stack.top().compare("*")==0)
         {
            if(ceilf(valOne)!=valOne || ceilf(valTwo)!=valTwo)                                                                                     //If its a float, evaluate and push, if not cast to an int then evaluate and push
            {  
               evaluator.push((valOne * valTwo));
            }
            else
            {
               int newOne = floor(valOne);
               int newTwo = floor(valTwo);

               evaluator.push((newOne * newTwo));
            }
            
            
         }

         if(stack.top().compare("/")==0)
         {
             if(ceilf(valOne)!=valOne || ceilf(valTwo)!=valTwo)
            {
               evaluator.push((valTwo / valOne));
            }
            else
            {
               int newOne = floor(valOne);
               int newTwo = floor(valTwo);
               evaluator.push((newTwo / newOne));
            }
         }

         if(stack.top().compare("+")==0)
         {
             if(ceilf(valOne)!=valOne || ceilf(valTwo)!=valTwo)
            {
               evaluator.push((valTwo + valOne));
            }
            else
            {
               int newOne = floor(valOne);
               int newTwo = floor(valTwo);
               evaluator.push((newTwo + newOne));
            }
         }

         if(stack.top().compare("-")==0)
         {
              if(ceilf(valOne)!=valOne || ceilf(valTwo)!=valTwo)
            {
               evaluator.push((valTwo - valOne));
            }
            else
            {
               int newOne = floor(valOne);
               int newTwo = floor(valTwo);
               evaluator.push((newTwo - newOne));
            }
         }
            
         
      }
      

      stack.pop();
   }

   cout<<endl<<"RESULT: "<<evaluator.top()<<endl;
}

void infixToPostfix(int argc, char **argv)
{
    int retval = SYM_NULL;
   set_input(argc, argv);                                                                                                                                               //Defining all my stacks and variables
   Stack<string> stack;
   Stack<string> otherStack;
   Stack<string> ogString;
   bool id = false;



    do {                                                                                                                                                                 //Looping through based on flex

       char * opnum = get_opnum(&retval);
       if (retval) {
          switch (retval)
          {
             case SYM_NAME:                                                                                                                                              //If its a name just output but store the value in ogString to evaluate it
                otherStack.push(string(opnum));
                id = true;
                ogString.push(string(opnum));
                break;

             case SYM_INTEG:                                                                                                                                            //If int, output and store value
                otherStack.push(string(opnum));
                ogString.push(string(opnum));

                break;

             case SYM_FLOAT: 
                otherStack.push(string(opnum));                                                                                                                         //Same as abaove for flaot
                ogString.push(string(opnum));
                break;

             case SYM_ADD:                                                                                                                                              //While the top is an operand, output the values then push + onto the stack

             ogString.push("+");
               if(!stack.empty())
               {
                  while(stack.top().compare("+")==0 || stack.top().compare("-")==0  || stack.top().compare("*")==0  || stack.top().compare("/")==0 )
                  {
                     otherStack.push(stack.top());
                     stack.pop();
                     if(stack.empty())
                        break;
                  } 
                  stack.push("+");
               }
               else
               {
                     stack.push("+");
               }
                break;
            
             case SYM_SUB:                                                                                                                                            //While the top is an operand, output the values then push - onto the stack
             ogString.push("-");                
                if(!stack.empty())
               {
                  while(stack.top().compare("+")==0 || stack.top().compare("-")==0  || stack.top().compare("*")==0  || stack.top().compare("/")==0 )
                  {
                     otherStack.push(stack.top());
                     stack.pop();
                     if(stack.empty())
                        break;
                  } 
                  stack.push("-");
               }
               else
               {
                     stack.push("-");
               }
                break;

             case SYM_MUL:                                                                                                                                              //If the top is a mutli or div operand then pop and store multi onot stack

             ogString.push("*");                      
               if(!stack.empty())   
               {
                  if(stack.top().compare("*")==0  || stack.top().compare("/")==0 )
                     {
                     otherStack.push(stack.top());
                     stack.pop();
                     stack.push("*");
                     }
                     else
                     {
                      stack.push("*");
                     }
                     
               }    
               else
               {
                  stack.push("*");
               }

                break;

             case SYM_DIV:                                                                                                                                             //If the top is a mutli or div operand then pop and store multi onot stack
             ogString.push("/");
             if(!stack.empty())
               {
                  if(stack.top().compare("*")==0  || stack.top().compare("/")==0 )
                     {
                     otherStack.push(stack.top());
                     stack.pop();
                     stack.push("/");
                     }
                     else
                     {
                      stack.push("/");
                     }
                     
               }    
               else
               {
                  stack.push("/");
               }

               break;

             case SYM_OPEN:                                                                                                                                                 //Just push the open
             ogString.push("(");
                 stack.push("(");
                break;

             case SYM_CLOSE:                                                                                                                                               //Making sure the brackets work then if they do, pop everything until the corresposning (
               ogString.push(")");
                  if(!stack.contains("(") || stack.top().compare("(")==0)
                  {
                     cout<<"ERROR: Mismatching braces"<<endl;
                     id=true;
                     break;
                  }

                 while(stack.top().compare("(")!=0)
                 {
                    otherStack.push(stack.top());
                    stack.pop();
                 }
                 stack.pop();
                break;

             case SYM_INVAL:                                                                                                                                      // Outputting that token in invalid
                 cout<<"invalid : " <<opnum<<endl;
                break;

              case END_LINE: 

               while(!stack.empty())                                                                                                                              //outputting the stored stack so that we now have outputed the correct infix form
                 {
                     otherStack.push(stack.top());
                     stack.pop();
                 }
             
              ogString.reverse();   
                                                                                                                                                                  //After reversing this now contains the orgional string
               if(!ogString.Validator())                                                                                                                           //Validating it to make sure it passes all tests
               {
                  id=true;
                  ogString.reverse();
                  cout<<"Found in input: " << ogString<<endl;

               }

               if(id)
                  cout<< otherStack<<endl;
         
              if(id==false)                                                                                                                                       //If its all good, then go ahead and evaluate it 
              {
               otherStack.reverse();        
               stackEvaluator(otherStack);
              }
               id=false;  
               otherStack.clear();                                                                                                                               //Clearing all my stacks for the next string
               ogString.clear();
               cout<<endl<<endl<<endl;

               break;
 
             default:
               
              break; 
              
          }
       }

    } while (retval > SYM_NULL);

   


}


int main(int argc, char **argv)
{
  infixToPostfix(argc,argv);

}