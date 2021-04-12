using namespace std;

template <typename T>
Bet<T>::Bet()
{
    root = nullptr;
} 

template<typename T>
Bet<T>::Bet(const Bet &t)                                                                                       //Simple copy constructor by cloning passed root
{
  root=  clone(t.root);
}

template <typename T>
Bet<T>::Bet(const list<Token> & postfix)                                                                        // Create from postfix by calling postfix function
{
    root = nullptr;
    buildFromPostfix(postfix);

} 

template <typename T>                                                                                           //Delete constructor by calling makeEmpty on the root
Bet<T>::~Bet()
{
    makeEmpty(root);
}

template <typename T>
bool Bet<T>::buildFromPostfix(const list<Token> & postfix)                                                      //Building from postfix functionn
{
        int numOperators = 0;                                                                                   //Counting number of operands and operators for more specific error checking
        int numOperands = 0;
          vector<BinaryNode*> myVector;                                                                         //Declaring a vector to hold binary nodes because I'm stuip and forgot about stacks 

 for(auto itr = postfix.begin(); itr != postfix.end(); itr++)                                                   //Looping through postfix list
    {       

        if(itr->getType() == 1 || itr->getType() == 2 || itr->getType() == 3 )                                  //Checking if its an operand, if so, creating node and pushing onto stack
        {
            BinaryNode *newNode = new BinaryNode(itr->getValue(),nullptr,nullptr);
            myVector.push_back(newNode);
            numOperands++;
        }
        else                                                                                                    //Otherwise creating operator node and making the left and right nodes the two node pointers that are in the vector
        {
                if(myVector.size()>=2)                                                                          //Checking that size is >= 2 for errors
                {
                numOperators++;
                BinaryNode *newNode = new BinaryNode(itr->getValue(),nullptr,nullptr);

                newNode->right = myVector[myVector.size() - 1];
                myVector.pop_back();

                newNode->left = myVector[myVector.size() - 1];
                myVector.pop_back();

                myVector.push_back(newNode);
                }
                else                                                                                             //If size fails, running through possible combos of operator numbers to operands and outputting appropraite errors
                {
                    if(numOperators == numOperands)
                    {
                        cout<< "Error: Unpaired opcode " <<myVector[0]->element.getValue()  <<endl;                    }
                    if(numOperators < numOperands)
                    {
                        cout<< "Error: Operator " <<myVector[0]->element.getValue()  << " has only one operand " <<endl;
                    }
                    if(numOperators > numOperands)
                    {
                        cout<< "Other error " <<myVector[0]->element.getValue()  <<endl;
                    }
                return false;
                }

        }


    }
        if(myVector.size() > 1)                                                                                 //Also error checking for if size is greaer than 1
            {
                if(numOperators == numOperands)
                    {
                        cout<< "Error: Operator " <<myVector[0]->element.getValue()  << " has only one operand" <<endl;
                    }
                    if(numOperators < numOperands)
                    {
                       cout<< "Error: Unpaired opcode " <<myVector[0]->element.getValue()  <<endl; 
                    }
                    if(numOperators > numOperands)
                    {
                        cout<< "Other error " <<myVector[0]->element.getValue()  <<endl;
                    }
                return false;

            }
        else if(!myVector.empty())                                                                                //Emptying root out after if it was false to clear space
                {
                root = myVector[0];
                myVector.empty();
                return true;
                }

    return false;
} 


template <typename T>
const Bet<T> & Bet<T>::operator= (const Bet<T> & x)                                                                 //Just calling clone and returning current object
{
    makeEmpty(root);
    root = clone(x.root);
    return *this;
} 

template <typename T>
void Bet<T>::printInfixExpression()                                                                                 //Calling relevant function
{
    printInfixExpression(root);
} 

template <typename T>
void Bet<T>::printPostfixExpression()                                                                               //Calling relevant function
{
    printPostfixExpression(root);
} 

template <typename T>                                                                                               //Calling relevant function
size_t Bet<T>::size()
{
    return size(root);
} 

template <typename T>                                                                                               //Calling relevant function
size_t Bet<T>::leaf_nodes()
{
    return leaf_nodes(root);
} 

template <typename T>                                                                                               //Checking if root is nullptr
bool Bet<T>::empty()
{
    return (root == nullptr);                                                                                       
} 

template <typename T>
void Bet<T>::printInfixExpression(BinaryNode *t)                                                                    //It was a challenge writing this, too so much time to figure out proper algorithim, ended up comparing presendences and number of operators
{
    if(t->left !=nullptr)
    {
        if(presedance(t,t->left))
        {
            cout<<"(";
            printInfixExpression(t->left);
            cout<<")";
        }
        else
        {
            printInfixExpression(t->left);
        }
    }

    cout << t->element.getValue() << " ";


     if(t->right !=nullptr)
     {
         if(presedance(t,t->right) || presedance2(t,t->right))
            {
                cout<<"(";
                printInfixExpression(t->right);
                cout<<")";
            }  
        else
        {
            printInfixExpression(t->right);
        }
     }
} 

template <typename T>
void Bet<T>::makeEmpty(BinaryNode* &t)                                                                                  //Recursivley calling itself on all nodes then deleting       
{
    if(t!=nullptr)
    {
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }
    t = nullptr; 
    
}

template <typename T>
typename Bet<T>::BinaryNode * Bet<T>::clone(BinaryNode *t)                                                               //Just creating new node for all nodes in passed tree
{
    if(t==nullptr)
        return nullptr;
    else
        return new BinaryNode(t->element,clone(t->left), clone(t->right));
    

    
} 

template <typename T>
void Bet<T>::printPostfixExpression(BinaryNode *n)                                                                          //Just recursivley calling to print out nodes
{
    if(n != nullptr)
    {
    printPostfixExpression(n->left);
    printPostfixExpression(n->right);
    cout<< n->element.getValue() << " ";
    }
    

} 

template <typename T>
size_t Bet<T>::size(BinaryNode *t)                                                                                          //Recursilvye calling itself for left and right and adding one each time
{
   if(t==nullptr)
    return 0;
    else
    {
        return 1+ size(t->left) + size(t->right);
    }
}

template <typename T>
size_t Bet<T>::leaf_nodes(BinaryNode *t)                                                                                    //Recursivleyc alling its self for right and left and only returning one if right and left are nullptr
{
    if(t==nullptr)
    {
        return 0;
    }
        
    else if(t->left== nullptr && t->right == nullptr)
    {
        return 1;
    }
    else 
    {
        return leaf_nodes(t->left) + leaf_nodes(t->right);
    }
}






template <typename T>
bool Bet<T>::presedance(BinaryNode *t1, BinaryNode *t2)                                                             //Literaral just checking nodes are combo of any two operators
{
    if((t1->element.getValue() == "*" || t1->element.getValue() == "/") && (t2->element.getValue() == "+" || t2->element.getValue() == "-"))
    {
        return true;
    } 
    else 
    {
                return false;
    }
}

template <typename T>                                                                                               // Checking if they are certain combo of + and -
bool Bet<T>::presedance2(BinaryNode *t1, BinaryNode *t2)
{
   if(t1->element.getValue() == t2->element.getValue())
        return true;
    if(t1->element.getValue()=="+" && t2->element.getValue()=="-" )
        return true;
    if(t1->element.getValue()=="-" && t2->element.getValue()=="+" )
        return true;
    
    return false;
    
}




