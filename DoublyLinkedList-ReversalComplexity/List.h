#ifndef LIST_H
#define LIST_H

#include <iostream>
#include <initializer_list>

namespace cop4530 {
    template <typename T>
    class List {
      private:    
	// The basic doubly linked list node.
	// Nested as private inside of List. 
	struct Node
	{
	    T       data;
	    Node   *prev;
	    Node   *next;

	    Node( const T & d = T{ }, Node * p = nullptr, Node * n = nullptr )
	      : data{ d }, prev{ p }, next{ n } { }
	    Node( T && d, Node * p = nullptr, Node * n = nullptr )
	      : data{ std::move( d ) }, prev{ p }, next{ n } { }
	};

      public:
        //nested const_iterator class
	class const_iterator
	{
	  public:
	    // Public constructor for const_iterator.
	    const_iterator( );

	    // Return the object stored at the current position.
	    // For const_iterator, this is an accessor with a
	    // const reference return type.
	    const T & operator* ( ) const;
	    
	    // increment/decrement operators
	    const_iterator & operator++ ( );
	    const_iterator operator++ ( int );
	    const_iterator & operator-- ( );
	    const_iterator operator-- ( int );
		
	    // comparison operators
	    bool operator== ( const const_iterator & rhs ) const;
	    bool operator!= ( const const_iterator & rhs ) const;

	  protected:
	    Node *current;              // pointer to node in List

	    // Protected helper in const_iterator that returns the object
	    // stored at the current position. Can be called by all
	    // three versions of operator* without any type conversions.
	    T & retrieve( ) const;

	    // Protected constructor for const_iterator.
	    // Expects a pointer that represents the current position.
	    const_iterator( Node *p );
	    
	    friend class List<T>;
	};

        // nested iterator class
	class iterator : public const_iterator
	{
	  public:

	    // Public constructor for iterator.
	    // Calls the base-class constructor.
	    // Must be provided because the private constructor
	    // is written; otherwise zero-parameter constructor
	    // would be disabled.
	    iterator( );
	    T & operator* ( );

	    // Return the object stored at the current position.
	    // For iterator, there is an accessor with a
	    // const reference return type and a mutator with
	    // a reference return type. The accessor is shown first.
	    const T & operator* ( ) const;

	    // increment/decrement operators
	    iterator & operator++ ( );
	    iterator operator++ ( int );
	    iterator & operator-- ( );
	    iterator operator-- ( int );

	  protected:
	    // Protected constructor for iterator.
	    // Expects the current position.
	    iterator( Node *p );

	    friend class List<T>;
	};

      public:
        // constructor, desctructor, copy constructor
        List();                  // default zero parameter constructor
        List(const List &rhs);   // copy constructor
        List(List && rhs);       // move constructor
       
	// num elements with value of val
	explicit List(int num, const T& val = T{});

	// constructs with elements [start, end)
	List(const_iterator start, const_iterator end);

	// constructs with a copy of each of the elements in the initalizer_list
	List (std::initializer_list<T> iList);

	~List( );

        // copy assignment operator
	List & operator= ( const List & rhs );
        // move assignment operator
	List & operator= ( List && rhs );
	// sets list to the elements of the initializer_list
	List & operator= (std::initializer_list<T> iList);
	
        // Accessor functions
        int size() const;       // number of elements
        bool empty() const;     // check if list is empty
        void clear();           // delete all elements
     
	// front, back, push_front, push_back, pop_front, and pop_back
	// are the basic double-ended queue operations.

        T & front();             // reference to the first element
	const T & front( ) const;
        T & back();              // reference to the last element
	const T & back( ) const;

        void push_front(const T & val); // insert to the beginning
        void push_front(T && val);      // move version of insert
        void push_back(const T & val);  // insert to the end
        void push_back(T && val);       // move version of insert
        void pop_front();               // delete first element
        void pop_back();                // delete last element
        void remove(const T &val);      // remove all elements with value = val

	template <typename PREDICATE>
	   void remove_if(PREDICATE pred);	// remove all elements for which Predicate pred
	//  returns true. pred can take in a function object

	// print out all elements. ofc is deliminitor
	void print(std::ostream& os, char ofc = ' ') const;

	// Return iterator representing beginning of list.
	// Mutator version is first, then accessor version.
        iterator begin();               // iterator to first element
        const_iterator begin() const;

	// Return iterator representing endmarker of list.
	// Mutator version is first, then accessor version.
        iterator end();                 // end marker iterator
        const_iterator end() const; 

        iterator insert(iterator itr, const T& val);  // insert val ahead of itr
        iterator insert(iterator itr, T && val);      // move version of insert
        iterator erase(iterator itr);                 // erase one element
        iterator erase(iterator start, iterator end); // erase [start, end)

        void reverse();         // reverse the order of all elements
        void deduplicate();     // remove the duplicates of any element

      private:
	int   theSize;
	Node *head;
	Node *tail;

	void init( );
    };

   // overloading comparison operators
   template <typename T>
      bool operator==(const List<T> & lhs, const List<T> &rhs);

   template <typename T>
      bool operator!=(const List<T> & lhs, const List<T> &rhs);

   // overloading output operator
   template <typename T>
      std::ostream & operator<<(std::ostream &os, const List<T> &l);

   // include the implementation file here
#include "List.hpp"

} // end of namespace cop4530

#endif
