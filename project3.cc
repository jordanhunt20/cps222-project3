// Copyright 2016 Adam Vigneaux and Jordan Hunt

#include "project3.h"
#include <iomanip>
#include <iostream>
using namespace std;

/*
 * Methods of class ThreadedBinarySearchTree
 *
 */

typedef ThreadedBinarySearchTree::Iterator Iterator;

ThreadedBinarySearchTree::ThreadedBinarySearchTree()
: _header(new  Node())
{
    _header -> _lchild = makeThread(_header);
    _header -> _rchild = _header;
}

/**
 * Destructor: deletes all nodes from memory
 */
ThreadedBinarySearchTree::~ThreadedBinarySearchTree()
{
    for (Iterator i = this->inorder_begin(); i != this->end(); i++) {
        delete i._ptr;
    }

    delete _header;
}

/**
 * Recursive auxiliary function for node insertion
 * If left or right child of root is a thread AND key belongs to left
 * or right of root (respectively), then insert node
 * Otherwise, recursively look down tree to find the right place for
 * the node and insert it
 */
void ThreadedBinarySearchTree::insertr(Node *root, string key, int value) {
    // Base case: left child is thread, key belongs left of root
    if (isThread(root->_lchild) && key < root->_key) {

        Node *newNode = new Node(key, value, root->_lchild,
                                 makeThread(root));
        root->_lchild = newNode;
        
        
    // Base case: right child is thread, key belongs right of root
    } else if (isThread(root->_rchild) && key > root->_key) {
        
        Node *newNode = new Node(key, value, makeThread(root),
                                 root->_rchild);
        root->_rchild = newNode;
        
    // Recursive step
    } else {
        if (key < root->_key) {
            insertr(root->_lchild, key, value);
        } else {
            insertr(root->_rchild, key, value);
        }
    }
} 

/**
 * Insert a node into the tree
 * If tree is empty, create a new root
 * Otherwise, recursively find the place for the node to insert and
 * insert it
 */ 
void ThreadedBinarySearchTree::insert(string key, int value)
{
    // Base case
    if (empty()) {

        Node *newNode = new Node(key, value, makeThread(_header),
                                 makeThread(_header));
        _header->_lchild = newNode;
        
    // Recursive step
    } else {
        insertr(_header->_lchild, key, value);
    }
}

void ThreadedBinarySearchTree::erase(Iterator iter)
{ /* STUB - REPLACE WITH REAL CODE */ }

/**
 * Test if the tree is empty
 * @return True iff the tree is empty, false otherwise
 */
bool ThreadedBinarySearchTree::empty() const 
{
    return makePointer(_header->_lchild) == _header;
}

/**
 * Look for a node in tree by key
 * @param key Key of a node in the tree
 * @return    Found node or end()
 */
Iterator ThreadedBinarySearchTree::lookup(string key) const
{
    Iterator curr = this->inorder_begin();
    while (curr._ptr->_key != key && curr != end()) {
        curr = curr.insucc();
    }
    return curr;
}

Iterator ThreadedBinarySearchTree::inorder_begin() const
{ return Iterator(_header, _header, Iterator::INORDER).insucc(); }

Iterator ThreadedBinarySearchTree::inorder_rbegin()const
{ return Iterator(_header, _header, Iterator::INORDER).inpred(); }

Iterator ThreadedBinarySearchTree::preorder_begin() const
{ return Iterator(_header, _header, Iterator::PREORDER).presucc(); }

Iterator ThreadedBinarySearchTree::preorder_rbegin() const
{ return Iterator(_header, _header, Iterator::PREORDER).prepred(); }

Iterator ThreadedBinarySearchTree::postorder_begin() const
{ return Iterator(_header, _header, Iterator::POSTORDER).postsucc(); }

Iterator ThreadedBinarySearchTree::postorder_rbegin() const
{ return Iterator(_header, _header, Iterator::POSTORDER).postpred(); }

Iterator ThreadedBinarySearchTree::end() const
{ return Iterator(_header, _header, Iterator::UNDEFINED); }

/*
 * Thread manipulation methods
 *
 */

#define THREAD_FLAG_MASK 0x8000000000000000

bool ThreadedBinarySearchTree::isThread(Node * ptr)
{ return (((long) ptr) & THREAD_FLAG_MASK) != 0; }
 
ThreadedBinarySearchTree::Node * ThreadedBinarySearchTree::makeThread(Node * ptr)
{ return (Node *) (((long) ptr) | THREAD_FLAG_MASK); }

ThreadedBinarySearchTree::Node * ThreadedBinarySearchTree::makePointer(Node * thread)
{ return (Node *) (((long) thread) & ~THREAD_FLAG_MASK); }






/**************************************************************************************/
/*
 * Methods of class ThreadedBinarySearchTree::Iterator
 *
 */
/**************************************************************************************/
        
Iterator::Iterator()
  : _header(NULL), _ptr(NULL), _order(UNDEFINED)
{ }

Iterator & Iterator::operator ++()
{
    switch (_order)
    {
        case INORDER:   return insucc();
        case PREORDER:  return presucc();
        case POSTORDER: return postsucc();
        default:        return * this;
    }
}

Iterator & Iterator::operator --()
{
    switch (_order)
    {
        case INORDER:   return inpred();
        case PREORDER:  return prepred();
        case POSTORDER: return postpred();
        default:        return * this;
    }
}

Iterator Iterator::operator ++(int)
{
    Iterator result = * this;
    operator ++();
    return result;
}

Iterator Iterator::operator --(int)
{
    Iterator result = * this;
    operator --();
    return result;
}

/**
 * Find next node in inorder traversal
 * If the node has a thread for a right child, then the thread points
 * to the inorder successor; else, we go left as far as possible in
 * the right subtree.
 * @return iterator
 */
Iterator & Iterator::insucc()
{
    if (isThread(_ptr -> _rchild))
        _ptr = makePointer(_ptr -> _rchild);
    else {
        _ptr = _ptr -> _rchild;
        while (! isThread(_ptr -> _lchild)) // not null
            _ptr = _ptr -> _lchild;    
    }
    
    return *this;
}

/**
 * Find next node in reverse inorder traversal
 * If the node has a thread for a left child, then the thread points
 * to the inorder successor; else, we go right as far as possible in
 * the left subtree.
 * @return iterator
 */
Iterator & Iterator::inpred()
{
    if (isThread(_ptr -> _lchild))
        _ptr = makePointer(_ptr -> _lchild);
    else {
        _ptr = _ptr -> _lchild;
        while (! isThread(_ptr -> _rchild))
            _ptr = _ptr -> _rchild;    
    }
    return *this;
}

/**
 * Find next node in preorder traversal
 * @return iterator
 */
Iterator & Iterator::presucc()
{
    if (!isThread(_ptr->_lchild)) { // if node has an lchild return lchild
        _ptr = makePointer(_ptr->_lchild);
    } else { // else no children
	while (isThread(_ptr->_rchild)) { // while no rchild
	    _ptr = makePointer(_ptr->_rchild); // follow rchild thread    
	} 
         _ptr = _ptr->_rchild; // return rchild
    }
    return *this;
}

/**
 * Find next node in reverse preorder traversal
 * @return iterator
 */
Iterator & Iterator::prepred()
{
    Iterator parent = this->parent();
    if ((parent._ptr->_lchild == _ptr) || 
	((parent._ptr->_rchild == _ptr) && isThread(parent._ptr->_lchild))) { 
        _ptr = parent._ptr;
    } else {
        *this = parent;
	_ptr = parent._ptr->_lchild;
        while (! isThread(_ptr->_lchild)) {
	    if (! isThread(_ptr->_rchild)) {
                _ptr = _ptr->_rchild;
            } else {
                _ptr = _ptr->_lchild;
	    }
	}
    }
    
    return *this;
}

/**
 * Find next node in postorder traversal
 * @return iterator
 */
Iterator & Iterator::postsucc()
{
    Iterator parent = this->parent();
    cout << 1 << endl;

    // If node is right child or left and only child, return parent
    if ((makePointer(parent._ptr)->_rchild == _ptr) || 
	((makePointer(parent._ptr)->_lchild == _ptr) &&
         isThread(makePointer(parent._ptr)->_rchild))) { 
        
        _ptr = parent._ptr;

    // Else node is left child and parent has right child
    } else {
        cout << 3 << endl;

        cout << (parent._ptr->_rchild == _header) << endl;

        // Get root of tree if parent is header
        if (parent._ptr->_rchild == _header) {
            _ptr = _header->_lchild;

        // Start with first node in right subtree
        } else {
            _ptr = parent._ptr->_rchild;    
        }

        cout << 4 << endl;

        // While not at bottom of tree
        while (!isThread(_ptr->_lchild) ||
                 !isThread(_ptr->_rchild)) {
            cout << 5 << endl;

	    if (!isThread(_ptr->_lchild)) {
                cout << 6 << endl;
                _ptr = _ptr->_lchild;
                cout << 7 << endl;
            } else {

                cout << 8 << endl;
                _ptr = makePointer(_ptr)->_rchild;
                cout << 9 << endl;
	    }
        }
    }

    return * this;
}

/**
 * Find next node in reverse postorder traversal
 * @return iterator
 */
Iterator & Iterator::postpred()
{
    if (! isThread(_ptr->_rchild)) {
        _ptr = _ptr->_rchild;
    } else if (! isThread(_ptr->_lchild)) {
        _ptr = _ptr->_lchild;  
    } else {
        while(isThread(_ptr->_lchild)) {
	    _ptr = _ptr->_lchild;
        }
        _ptr = _ptr->_lchild; 
    }
    return * this;
}

/**
 * Get parent of current node
 * @return iterator
 */
Iterator Iterator::parent() const
{
    
    Node *child = _ptr;
    Node *curr = _header->_lchild; // Start at root
    bool found = false;

    // If current node is root, return header
    if (makePointer(_header->_lchild) == child) { 
        return Iterator(_header, (Node *)_header, Iterator::UNDEFINED);
    }

    // Search for parent
    while (!found) {

	// If root is parent
        if (makePointer(curr->_lchild)->_key == key() ||
	    makePointer(curr->_rchild)->_key == key()) {
	    found = true;
	
	// Look in left side of tree
	} else if (key() < curr->_key) {
	    curr = curr->_lchild;

	// Look in right side of tree
	} else {
            curr = curr->_rchild; 
	}
    }

    return Iterator(_header, curr, _order);
}

string Iterator::key() const
{ return _ptr -> _key; }

int Iterator::value() const
{ return _ptr -> _value; }

bool Iterator::operator == (const Iterator & other) const
{ return _ptr == other._ptr && _ptr != NULL; }

bool Iterator::operator != (const Iterator & other) const
{ return _ptr != other._ptr || _ptr == NULL; }

Iterator::Iterator(const Node * header,
                   Node * ptr, 
                   Order order)
  : _header(header), _ptr(ptr), _order(order)
{ }

// ostream operator << is in a separate file
