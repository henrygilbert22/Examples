
using namespace std;
using namespace cop4530;

//************************************************************************************************************************
// CONST_ITERATOR
//************************************************************************************************************************
template <typename T>
List<T>::const_iterator::const_iterator() : current{nullptr}
{ }

template <typename T>
List<T>::const_iterator::const_iterator( Node *p ) : current {p}
{ }

template <typename T>
const T & List<T>::const_iterator::operator*() const
{ return retrieve();}

template <typename T>
typename List<T>::const_iterator & List<T>::const_iterator::operator++()
{
    current = current->next;
    return *this;
}

template <typename T>
 typename List<T>::const_iterator List<T>::const_iterator::operator++(int)
{
   const_iterator old = *this;
   ++(*this);
   return old;
}

template <typename T>
typename List<T>::const_iterator & List<T>::const_iterator::operator--()
{
   current = current->prev;
    return *this;
}

template <typename T>
 typename List<T>::const_iterator List<T>::const_iterator::operator--(int)
{
   const_iterator old = *this;
   --(*this);
   return old;
}

template <typename T>
 bool List<T>::const_iterator::operator==(const const_iterator & rhs) const
{
    return current == rhs.current;
}

template <typename T>
 bool List<T>::const_iterator::operator!=(const const_iterator & rhs) const
{
    return !(*this == rhs);
}

template <typename T>
T & List<T>::const_iterator::retrieve() const
{
    return current->data;
}

//************************************************************************************************************************
//ITERATOR
//************************************************************************************************************************
template <typename T>
List<T>::iterator::iterator()
{

}

template <typename T>
 T & List<T>::iterator::operator*() 
{
    return const_iterator::retrieve();
}

template <typename T>
const T & List<T>::iterator::operator*() const
{
        return const_iterator::operator*();
}

template <typename T>
typename List<T>::iterator & List<T>::iterator::operator++()
{
    this->current = this->current->next;
    return *this;
}

template <typename T>
 typename List<T>::iterator List<T>::iterator::operator++(int)
{
   iterator old = *this;
   ++(*this);
   return old;
}

template <typename T>
typename List<T>::iterator & List<T>::iterator::operator--()
{
   this->current = this->current->prev;
    return *this;
}

template <typename T>
 typename List<T>::iterator List<T>::iterator::operator--(int)
{
    iterator old = *this;
   --(*this);
   return old;
}

template <typename T>
List<T>::iterator::iterator( Node *p ): const_iterator{p}
{  }

//************************************************************************************************************************
//LIST
//************************************************************************************************************************

template <typename T>
List<T>::List()
{
    init();
}

template<typename T>
List<T>::~List<T>()
{
    clear();
    delete head;
    delete tail;
}

template<typename T>
 List<T>::List(int num, const T& val)
{
    init();
    for(int i=0; i<num;i++)
    {
                push_front(val);
    }

}




template<typename T>
List<T>::List(const List &rhs)
{
    init();
    for(auto &x : rhs)
        push_back(x);
}

template<typename T>
List<T>::List(List && rhs): theSize{rhs.theSize}, head{rhs.head}, tail{rhs.tail}
{
        init();
   rhs.theSize = 0;
   rhs.head = nullptr;
   rhs.tail = nullptr; 

}

template <typename T>
List<T>::List(const_iterator start, const_iterator end)             //STIL NEEED TO TEST
{
    init();
  while(start != end)
  {
      push_front(start.retrieve());
      start++;
  }
}

template <typename T>
T & List<T>::front()
{
    return *begin();
}

template <typename T>
const T & List<T>::front() const
{
    return *begin();
}


template <typename T>
T & List<T>::back()
{
    return *--end();
}


template <typename T>
const T & List<T>::back() const
{
    return *--end();
}

template<typename T>
typename List<T>::iterator List<T>::begin()
{
    iterator itr(head->next);
    return itr; 
}

template<typename T>
typename List<T>::const_iterator List<T>::begin() const
{
    const_iterator itr(head->next);
    return itr;
}

template<typename T>
typename List<T>::iterator List<T>::end()
{
    iterator itr(tail);
    return itr;
}

template<typename T>
typename List<T>::const_iterator List<T>::end() const
{
    const_iterator itr(tail);
    return itr;
}
template <typename T>
void List<T>::push_front(const T &val)
{
    iterator itr = begin();
    insert(itr,val);
}

template <typename T>
void List<T>::push_front( T &&val)
{
    iterator itr = begin();
    insert(itr,std::move(val));
}

template <typename T>
void List<T>::push_back(const T &val)
{
    iterator itr = end();
    insert(itr,val);
}

template <typename T>
void List<T>::push_back( T &&val)
{
    iterator itr = end();
    insert(itr,std::move(val));
}

template<typename T>
void List<T>::pop_back()
{
   erase(tail->prev);
}


template<typename T>
void List<T>::pop_front()
{
    erase(head->next);
}

template<typename T>
void List<T>::remove(const T &val)
{
   iterator itr = begin();

   while(itr != tail)
   {
        if(itr.retrieve()==val)
            itr = erase(itr);
        else
            itr++;
   }
       
        
}

template<typename T>
int List<T>::size() const
{
   return theSize;
}

template<typename T>
bool List<T>::empty() const
{
    return(theSize==0);
}


template<typename T>
void List<T>::clear()
{
   while(!empty())
        pop_front();
}

template<typename T>
typename List<T>::iterator List<T>::insert(iterator itr, const T& val)
{
      Node *p = itr.current;
      theSize++;
      return {p->prev = p->prev->next = new Node{val,p->prev,p}};

}

template<typename T>
typename List<T>::iterator List<T>::insert(iterator itr, T&& val)
{ 
       Node *p = itr.current;
      theSize++;
      return {p->prev = p->prev->next = new Node{std::move(val),p->prev,p}};
}

template<typename T>
typename List<T>::iterator List<T>::erase(iterator itr)
{
   Node *p = itr.current;
   theSize--;
   p->prev->next = p->next;
   p->next->prev = p->prev;

    iterator newItr = p->next;
   delete p;

   return newItr;
   }


template<typename T>
typename List<T>::iterator List<T>::erase(iterator start, iterator end)
{
   for(iterator itr = start; itr !=end;)
        itr = erase(itr);
    return end;

    }

template<typename T>
void List<T>::init()
{
    theSize = 0;
   
    head = new Node();
    tail = new Node();

    head->next = tail;
    tail->prev = head;
   
}

template<typename T>
void List<T>::reverse()
{
    Node *search = tail->prev->prev;
   
   while(search != head)
   {     
    Node *temp = search;
    search = search->prev;   
    temp->next->prev = temp->prev;
    temp->prev->next = temp->next;

    temp->prev = tail->prev;
    tail->prev->next = temp;
    
    temp->next = tail;
    tail->prev = temp;
   }

}

template<typename T>
void List<T>::deduplicate()
{
  Node *search = head->next;
    List<T>::iterator itr = begin();

    while(search!=tail)
    {
        itr++;
       while(itr.current!=tail)
       {
           if(itr.current->data == search->data)
           {
            itr = erase(itr);  
           }
           else
                itr++;
       }
       itr = search->next;
       search = search->next;
    }
}

template<typename T>
List<T> & List<T>::operator=( const List & rhs )
{
   List copy = rhs;
   std::swap(*this, copy);
   return *this;
}

template<typename T>
List<T> & List<T>::operator=(  List && rhs )
{
  std::swap(theSize, rhs.theSize);
  std::swap(head,rhs.head);
  std::swap(tail,rhs.tail);

  return *this;
}

template<typename T>
template <typename PREDICATE>
void List<T>::remove_if(PREDICATE pred)
{
      iterator itr = begin();

    while(itr.current != tail)
    {
        if(pred(itr.retrieve()))
            itr = erase(itr);
        else
           itr++;      

    }
}

 template<typename T>
 bool operator==(const List<T> & lhs, const List<T> &rhs)
{
    if(lhs.size() != rhs.size())
        return false;

    auto itrL = lhs.begin();
    auto itrR = rhs.begin();

    while(itrL!= lhs.end())
    {
        if(*itrL != *itrR)
            return false;

        itrL++;
        itrR++;
    }
    return true;
   
}

template<typename T>
 bool operator!=(const List<T> & lhs, const List<T> &rhs)
{
   return !(lhs==rhs);
}

template<typename T>
std::ostream & operator<<(std::ostream &os, const List<T> &l)
{
    l.print(cout);
    return os;
}


template<typename T>
List<T> & List<T>::operator=(initializer_list<T> iList)
{
   for(const auto& itr: iList) 
       push_back(itr);

    return *this;
}


template<typename T>
List<T>::List(std::initializer_list<T> iList)
{
    init();
    for(const auto& itr: iList) 
       push_back(itr);
}



template<typename T>
void List<T>::print(std::ostream& os, char ofc) const
{
    auto itr = begin();

    while(itr!= end())
    {
        os<<*itr<<ofc;
        itr++;
    }
}

