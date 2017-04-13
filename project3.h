/*
 * project3.h - Declarations for class ThreadedBinarySearchTree - CPS222 Project 3
 *
 * Copyright(c) 1997, 1999, 2001, 2003, 2014 - Russell C. Bjork
 *
 */

#include <iostream>
#include <string>
using namespace std;

class ThreadedBinarySearchTree
  {
    /*
     * PURPOSE: Support storing and accessing of pairs consisting of a string
     *		and an integer in a binary search tree, with forward and 
     *		backward inorder, preorder, and postorder traversal possible 
     *		by using iterators.
     */    

    public:

	class Iterator;			    // Fully declared below

	/* Constructor, destructor */

	ThreadedBinarySearchTree();
	~ThreadedBinarySearchTree();

	/* Mutators */

	void insert(string key, int value);  // Add a new node
	void erase(Iterator iter);	     // Remove node pointed to by iter

	/* Accessors */

	bool empty() const;		     // Return true iff tree is empty
	
	Iterator lookup(string key) const;   // Iterator to node containing key
					     // or end() if no such node
	
	// The following return iterators that support forward and reverse 
	// traversal of the tree in the various orders by using operator ++/--
	// or appropriate __succ/__pred methods of the Iterator. 
	// The xxx_begin() form returns an iterator to the first node in the
	// specified order; the xxx_rbegin() returns an iterator to the last
	// node in the specified order - i.e. the first in reverse order.

	Iterator inorder_begin() const;
	Iterator inorder_rbegin() const;
	Iterator preorder_begin() const;
	Iterator preorder_rbegin() const;
	Iterator postorder_begin() const;
	Iterator postorder_rbegin() const;

	// The following value is returned by all iterators when advanced
	// past the last or backed up past the first node in the tree in
	// the respective order, and also by failed lookup() or parent()
	// operations
 
	Iterator end() const;
	
    private:

	/* Copy constructor, assignment */
	
	// Implementing these methods correctly would require a deep copy of the
	// entire tree.  Rather than doing this (which would be very time
	// consuming to carry out) we prohibit the use of these operations by
	// declaring the methods private.
	
	ThreadedBinarySearchTree(const ThreadedBinarySearchTree &);
	const ThreadedBinarySearchTree 
	    & operator = (const ThreadedBinarySearchTree &);
	    
	/* Incomplete declaration for class Node - needed by Iterator */
	
	class Node;

        void insertr(Node *root, string key, int value);
        
    public:

	/* Nested class Iterator */

	class Iterator
	  {
	    public:

		// Public Constructor
	
		Iterator();

		// Move forward and backward in tree in appropriate order,
		// and return the modified iterator
	
		Iterator & operator ++();	// Use order specified when
		Iterator & operator --();	// the Iterator was created

		Iterator operator ++(int);	// Postfix ++
		Iterator operator --(int);	// Postfix --

		Iterator & insucc();
		Iterator & inpred();
		Iterator & presucc();
		Iterator & prepred();
		Iterator & postsucc();
		Iterator & postpred();

		// Return iterator to parent of current node, or end() if none
	
		Iterator parent() const;

		// Access info in node selected by iterator

		string key() const;
		int value() const;

		// Compare to iterators - equal if at same node

		bool operator == (const Iterator & other) const;
		bool operator != (const Iterator & other) const;

		// Output info on node selected by iterator (used by tester)

		friend ostream & operator << (ostream & s, Iterator & it);

	    private:

		friend class ThreadedBinarySearchTree;

		// Iterators are available for three traversal orders

		enum Order { UNDEFINED, INORDER, PREORDER, POSTORDER };

		Iterator(const Node * header,
			 Node * ptr,
			 Order order);
			 
		// Header of the tree for which this is an iterator,
		// Node in the tree that iterator currently refers to,
		// Traversal order for this iterator

		const Node * 		_header;
		Node *			_ptr;
		Order			_order;
	  };
	

	friend class Iterator;
	
    private:

	// Nodes of this type are used to actually store the tree
	
	class Node
	  { 
	    public:

		Node()
		  { }
		Node(string key, int value, Node * lchild, Node * rchild)
		  : _key(key), _value(value), _lchild(lchild), _rchild(rchild)
		  { }
		~Node()
		  { }

		string	_key;
		int	_value;
		Node *	_lchild,
		     *	_rchild;

		// The following are implemented in the test driver - they
		// let the tester keep track of storage allocation/deallocation

		void * operator new(size_t size);
		void operator delete(void * ptr);
	  };

	// The following functions support threading the tree

	static bool isThread(Node * ptr);
	static Node * makeThread(Node * ptr);
	static Node * makePointer(Node * thread);

	friend ostream & operator << (ostream & s, Iterator & it);

	// The tree is accessed through a header node

	Node * _header;
  };
