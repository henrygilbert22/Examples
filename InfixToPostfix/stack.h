#ifndef STACK_H
#define STACK_H


#include <iostream>
#include <initializer_list>
#include <vector> 
#include "string.h"
#include <algorithm>


namespace cop4530 {
    template <typename T>
    class Stack {

        public:
            explicit Stack(int initSize = 0);
            ~Stack();                                                       //Destructor;
           
            Stack (const Stack<T>&);                                        //copy constructor
            Stack(Stack<T> &&);                                             //move constructor 
            Stack<T>& operator= (const Stack <T>&);                         //copy assignment operator
            Stack<T> & operator=(Stack<T> &&);                              //move assingment operator
            void reverse();

            bool contains(T x) const;
            bool Validator();
            bool empty() const;                                             //returns true if no elements
            void clear();                                                   //Delete all elements from stack
            int size() const;                                               //number of elements in stack
            void push(const T& x);                                          //adds x onto stack
            void push(T && x);                                              //adds x onto stack - move
            void pop();                                                     //removes top postion
            T& top();                                                       //returns refrence to top position
            const T& top() const;                                           //accessor for top position
            void print(std::ostream& os, char ofc = ' ') const;             //Print function
            
        private:
            std::vector<T> currStack;



    };

    template <typename T>
    std::ostream& operator<< (std::ostream& os, const Stack<T>& a);
       
    template <typename T>
    bool operator== (const Stack<T>&, const Stack <T>&);

    template <typename T>
    bool operator!= (const Stack<T>&, const Stack <T>&);

    template <typename T>
    bool operator<= (const Stack<T>& a, const Stack <T>& b);

    #include "stack.hpp"

}
#endif
