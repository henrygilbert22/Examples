#ifndef BET_H
#define BET_H

#include <iostream>
#include <initializer_list>
#include <vector> 
#include "string.h"
#include "token.h"
#include <algorithm>


template <typename T>
class Bet{


    public:
        Bet();                                                              //Zero param constructor
        Bet(const list<Token> & postfix);                                   //Single param constructor
        Bet(const Bet &);
        ~Bet();                                                             //Delete constructor
        bool buildFromPostfix(const list<Token> & postfix);                 //Bool for succesfull tree creating
        const Bet & operator= (const Bet<T> &);                                //Deep copy construcotr
         
        void printInfixExpression();                                        //Print infix expression (use private function)
        void printPostfixExpression();                                      //Print postfix expression (use private function)
        size_t size();                                                      //Number of nodes, using private function
        size_t leaf_nodes();                                                //Number of lead nodes
        bool empty();                                                       //Self explanatory


    private:
        struct BinaryNode {    

             T element;   
             BinaryNode *left;    
             BinaryNode *right;    

             BinaryNode(const T &theElement = T{ }, BinaryNode *lt = nullptr, BinaryNode *rt= nullptr)
             : element{theElement}, left{lt}, right{rt} {}    
             BinaryNode(T && theElement, BinaryNode *lt = nullptr, BinaryNode *rt = nullptr)
             : element{std::move(theElement)}, left{lt}, right{rt} {}};

        BinaryNode * root;

        void printInfixExpression(BinaryNode *t);                           //Prints infix, need to inlcude appropriate brackets (recursive)
        void makeEmpty(BinaryNode* &t);                                     //Delete all nodes of t's subtree (recursive)
        BinaryNode * clone(BinaryNode *t);                                  //Clone all nodes  pointred to by t (recurssive)
        void printPostfixExpression(BinaryNode *n);                         //Print out corresponsing postfix (revusive)
        size_t size(BinaryNode *t);                                         //Number of nodes pointed to by t (recursive)
        size_t leaf_nodes(BinaryNode *t);                                   //Number of leaves pointed to, (recursive)
        bool presedance(BinaryNode *t1, BinaryNode *t2);                    //Checking presedance
        bool presedance2(BinaryNode *t1, BinaryNode *t2);                   //Checking for mutiple operators

    





};

#include "bet.cpp"
#endif
