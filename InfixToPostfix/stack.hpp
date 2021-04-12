using namespace std;
using namespace cop4530;

template <typename T>
Stack<T>::Stack(int initSize)
{
    currStack.reserve(initSize);
}

template <typename T>
Stack<T>::~Stack()
{
    currStack.clear();
}

template <typename T>
Stack<T>::Stack (const Stack<T>& s)
{
    for(int i =0; i<s.size(); i++)
        currStack.push_back(s.currStack[i]);
}

template <typename T>
Stack<T>::Stack(Stack<T> && s)
{
  for(int i =0; i<s.size(); i++)
        currStack.push_back(std::move(s.currStack[i]));

}

template <typename T>
Stack<T> & Stack<T>::operator=(const Stack <T>& s)
{
    
    for(int i =0; i<s.size(); i++)
        currStack.push_back(s.currStack[i]);

    return *this;
}



template <typename T>
Stack<T> & Stack<T>::operator=(Stack<T> && s)
{
    for(int i =0; i<s.size(); i++)
        currStack.push_back(std::move(s.currStack[i]));

    return *this;

}


template <typename T>
bool Stack<T>::empty() const
{
    return currStack.empty();
}


template <typename T>
void Stack<T>::clear()
{

     currStack.clear();
}


template <typename T>
int Stack<T>::size() const
{
    return currStack.size();
}


template <typename T>
void Stack<T>::push(const T& x)
{
    currStack.push_back(x);
}


template <typename T>
void Stack<T>::push(T && x)
{
    currStack.push_back(std::move(x));
}


template <typename T>
void Stack<T>::pop()
{
    currStack.pop_back();
}


template <typename T>
T & Stack<T>::top()
{
    return currStack[currStack.size()-1];
}


template <typename T>
const T& Stack<T>::top() const
{
    return currStack[currStack.size()-1];
}


template <typename T>
void Stack<T>::print(std::ostream& os, char ofc) const
{    
    Stack<T> newStack = *this;

    newStack.reverse();
    while(!newStack.empty())
    {
        os<<newStack.top()<<ofc;
        newStack.pop();
    }
}

template <typename T>                                                                                   //Pretty simple operator function
std::ostream& operator<< (std::ostream& os, const Stack<T>& a)
{
    a.print(cout);
    return os;
}
       
template <typename T>                                                                                  //Pretty simple operator function
bool operator== (const Stack<T>& s1, const Stack <T>& s2)
{
    if(s1.size()!=s2.size())
        return false;

    T a1,a2;
    Stack<T> new1(s1);
    Stack<T> new2(s2);

    for(int i=0; i<s1.size();i++)
        {
            a1 = new1.top();
            a2 = new2.top();

            if(a1 != a2)
                return false;

            new1.pop();
            new2.pop();
        }
    return true;
}

template <typename T>                                                                                 //Pretty simple operator function
bool operator!= (const Stack<T>& s1, const Stack <T>& s2)
{
    return !(s1==s2);
}

template <typename T>
bool operator<= (const Stack<T>& s1, const Stack <T>& s2)                                               //Pretty simple operator function
{
      T a1,a2;
    Stack<T> new1(s1);
    Stack<T> new2(s2);

    for(int i=0; i<s1.size();i++)
        {
            a1 = new1.top();
            a2 = new2.top();

            if(a1 > a2)
                return false;

            new1.pop();
            new2.pop();
        }
    return true;
}

template<typename T>    
void Stack<T>::reverse()                                                                                //Function to reverse a stack
{

    
   std::vector<T> newVector;  

   while(!empty())
   {
      newVector.push_back(top());
      pop();
   }

clear();
int i =0;
   while(i < newVector.size())
   {
       push(newVector[i]);
       i++;
   }
}

template<typename T>
bool Stack<T>::contains(T x) const                                                                        //Function that tests whether stack contains an element (use for validaton)
{
    if(std::find(currStack.begin(), currStack.end(), x) != currStack.end()) 
        return true;
    else
        return false;
}

template<typename T>
bool Stack<T>::Validator()                                                                                  //Validator function, very ineffective but catches all the nesseary cases
{
    if(currStack.front() == "+" || currStack.front() == "-" || currStack.front() == "*" ||currStack.front() == "/")
    {
        cout<<"Error: Extra ending operator"<<endl;
        return false;
    }

    if(currStack.back() == "+" || currStack.back() == "*" ||currStack.back() == "/")
    {
        cout<<"Error: Extra beggining operator"<<endl;
        return false;
    }
   
    if(count(currStack.begin(),currStack.end(),")") != count(currStack.begin(),currStack.end(),"("))
        {
            cout<<"Error: Mismatching braces"<<endl;
            return false;
        }
   
   
   
    int i = size()-1;
    while(i > 0)
    {
    if((currStack[i] != "+" && currStack[i] != "-" && currStack[i] != "/" && currStack[i] != "*") && (currStack[i-1] != "+" && currStack[i-1] != "-" && currStack[i-1] != "/" && currStack[i-1] != "*"))
        {
            if(currStack[i]!="(" && currStack[i-1]!=")")
            {
                cout<<"Error: Consecutive operands : "<< currStack[i] <<" and: "<<currStack[i-1]<<endl;
                return false;
            }
            
        }
        if((currStack[i] == "+" || currStack[i] == "-" || currStack[i] == "/" || currStack[i] == "*") && (currStack[i-1] == "+" || currStack[i-1] == "-" || currStack[i-1] == "/" || currStack[i-1] == "*"))
        {
            cout<<"Error: consecutive operators: " << currStack[i] <<" and: "<<currStack[i-1]<<endl;
            return false;
        }
        
        i--;
    }
        
       
    return true;
}
