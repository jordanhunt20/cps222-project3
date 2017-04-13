/*
 * Test driver for CPS222 Project 3
 *
 * Copyright (c) 1997, 2001, 2002, 2003, 2013  - Russell C. Bjork
 *
 */

#include "project3.h"
#include <iomanip>
#include <cstdio>
#include <sys/stat.h>
using namespace std;

/*
 * operator << for Iterator
 *
 */

ostream & operator << (ostream & stream, ThreadedBinarySearchTree::Iterator & iter)
{ 
        char buffer[20];
        
        sprintf(buffer, "%-18p ", (void *) iter._ptr);
        stream << buffer;
        if (iter._ptr == NULL)
                stream << "*** NULL - THIS IS A VERY BAD THING! ***";
        else if (iter._ptr == iter._header)
                stream << "*** HEADER ***";
        else if (ThreadedBinarySearchTree::isThread(iter._ptr))
                stream << "*** THREAD ***";
        else
        {
                stream << iter._ptr -> _key
                           << setw((iter._ptr -> _key.length() > 14) ? 1 
                                                                         : 14 - iter._ptr -> _key.length()) << " "
                           << " - value: " <<  setw(3) << iter._ptr -> _value;
                stream << " - children: ";

                if (ThreadedBinarySearchTree::isThread(iter._ptr -> _lchild))
                        if (ThreadedBinarySearchTree::makePointer(iter._ptr -> _lchild) == iter._header)
                                stream << "thr header            ";
                        else
                        {
                                sprintf(buffer, "thr %-18p", (void *) 
                                   ThreadedBinarySearchTree::makePointer(iter._ptr -> _lchild));
                                stream << buffer;
                        }
                else
                        if (iter._ptr -> _lchild == iter._header)
                                stream << "    header            ";
                        else
                        {
                                sprintf(buffer, "    %-18p", (void *) iter._ptr -> _lchild);
                                stream << buffer;
                        }

                stream << " ";
                
                if (ThreadedBinarySearchTree::isThread(iter._ptr -> _rchild))
                        if (ThreadedBinarySearchTree::makePointer(iter._ptr -> _rchild) == iter._header)
                                stream << "thr header            ";
                        else
                        {
                                sprintf(buffer, "thr %-18p", (void *) 
                                   ThreadedBinarySearchTree::makePointer(iter._ptr -> _rchild));
                                stream << buffer;
                        }
                else
                        if (iter._ptr -> _rchild == iter._header)
                                stream << "    header            ";
                        else
                        {
                                sprintf(buffer, "    %-18p", (void *) iter._ptr -> _rchild);
                                stream << buffer;
                        }
        }
        return stream;
}

/*
 * Storage allocation/deallocation for node
 *
 */

static int newCount = 0;
static int deleteCount = 0;
static void * lastNodeAllocated = NULL;

void * ThreadedBinarySearchTree::Node::operator new(size_t size)
{
        newCount ++;
        lastNodeAllocated = ::operator new(size);
        return lastNodeAllocated;
}

void ThreadedBinarySearchTree::Node::operator delete(void * ptr)
{
        deleteCount ++;
        ::operator delete(ptr);
}

/*
 * Main test driver program
 *
 */

// Possible traversal orders

#define TRAVERSE_INORDER 1
#define TRAVERSE_PREORDER 2
#define TRAVERSE_POSTORDER 4

int main()
{
        {
                // Wrap the bulk of the test driver in a block, so that the destructor
                // for theTree will be called before we check to see if all nodes
                // created have been deleted
                
                ThreadedBinarySearchTree theTree;
                ThreadedBinarySearchTree::Iterator iter;

                // Find out whether our input stream is interactive by seeing if device
                // 0 is a character special device.  interactive is true if it is.
                
                struct stat s;
                fstat(0, & s);
                bool interactive = S_ISCHR(s.st_mode);
                
                // Commands and arguments inputted by the user
                
                char command;
                string key;
                int value;
                int order;
                string comment;

                do
                {
                        if (interactive)
                                cout << endl << "Command? ";
                        cin >> command;
                        if (cin.eof())
                                command = 'Q';
                        
                        switch(command)
                        {
                                case '?':
                                
                                        // Test of empty() method

                                        cout << "Testing empty() - result is: " 
                                                << (theTree.empty() ? "true" : "false") << endl;
                                                
                                        break;
                                        
                                case 'I': case 'i':
                                {
                                        // Test of insert() method
                                        
                                        cin >> key >> value;
                                        theTree.insert(key, value);
                                        
                                        char buffer[20];
                                        cout << "Inserted " << key << " with value " << value << " at ";
                                        sprintf(buffer, "%-18p", lastNodeAllocated);
                                        cout << buffer << endl;
                                        
                                        break;
                                }
                                
                                case 'T' : case 't':

                                        cin >> order;
                                        
                                        // Test of forward traversal(s)

                                        if (order & TRAVERSE_INORDER)
                                        {
                                                cout << "Doing inorder traversal: " << endl << endl;
                                                for (iter = theTree.inorder_begin(); iter != theTree.end(); iter ++)
                                                        cout << iter << endl;
                                                cout << endl;
                                        }
                                        
                                        if (order & TRAVERSE_PREORDER)
                                        {
                                                cout << "Doing preorder traversal: " << endl << endl;
                                                for (iter = theTree.preorder_begin(); iter != theTree.end(); iter ++)
                                                        cout << iter << endl;
                                                cout << endl;
                                        }
                        
                                        if (order & TRAVERSE_POSTORDER)
                                        {
                                                cout << "Doing postorder traversal: " << endl << endl;
                                                for (iter = theTree.postorder_begin(); iter != theTree.end(); iter ++)
                                                        cout << iter << endl;
                                                cout << endl;
                                        }
                                        
                                        break;
                                        
                                case 'R': case 'r':
                                
                                        cin >> order;
                                        
                                        // Test of reverse traversal(s)
                        
                                        if (order & TRAVERSE_INORDER)
                                        {
                                                cout << "Doing reverse inorder traversal: " << endl << endl;
                                                for (iter = theTree.inorder_rbegin(); iter != theTree.end(); iter --)
                                                        cout << iter << endl;
                                                cout << endl;
                                        }
                                        
                                        if (order & TRAVERSE_PREORDER)
                                        {
                                                cout << "Doing reverse preorder traversal: " << endl << endl;
                                                for (iter = theTree.preorder_rbegin(); iter != theTree.end(); iter --)
                                                        cout << iter << endl;
                                                cout << endl;
                                        }
                                        
                                        if (order & TRAVERSE_POSTORDER)
                                        {
                                                cout << "Doing reverse postorder traversal: " << endl << endl;
                                                for (iter = theTree.postorder_rbegin(); iter != theTree.end();iter --)
                                                        cout << iter << endl;
                                                cout << endl;
                                        }
                                        
                                        break;
                                                                        
                                case 'L': case 'l':
                                
                                        // Test of lookup
                                        
                                        cin >> key;
                        
                                        cout << "Looking up: " << key;
                                        iter = theTree.lookup(key);
                                        if (iter == theTree.end())
                                                cout << " - not found" << endl;
                                        else
                                                cout << " - got:" << endl << iter << endl;
                                                
                                        break;
                                        
                                case 'P': case 'p':
                                
                                        // Test of parent - must look up first
                                        
                                        cin >> key;
                        
                                        cout << "Looking up: " << key;
                                        iter = theTree.lookup(key);
                                        if (iter == theTree.end())
                                                cout << " - key not found" << endl;
                                        else
                                                cout << " - key was found" << endl;
                                        cout << "Calling parent() on result";
                                        iter = iter.parent();
                                        cout << " - got:" << endl << iter << endl;
                                        
                                        break;

                                case 'E': case 'e':
                                
                                        // Test of erase

                                        cin >> key;

                                        cout << "Looking up: " << key;
                                        iter = theTree.lookup(key);
                                        if (iter == theTree.end())
                                                cout << " - not found" << endl;
                                        else
                                        {
                                                cout << " - found - erasing it" << endl;
                                                theTree.erase(iter);
                                        }
                                        
                                        break;
                                        
                                case 'Q': case 'q':
                                
                                        // Quit
                                        
                                        cout << "Quitting" << endl;
                                        
                                        break;
                                        
                                case '#':
                                
                                        // Comment
                                        
                                        getline(cin, comment);
                                        cout << "#" << comment << endl;
                                        
                                        break;
                                        
                                default:
                                
                                        cout << "Unrecognized command: " << command << endl;
                                        
                                case 'H': case 'h':
                                
                                        cout << 
"Recognized commands are: ?(isempty); I(nsert) key value; T(raverse) order;" << endl;
                                        cout << 
"                         R(everse traverse) order; L(ookup) key; P(arent) key;" << endl;
                                        cout <<
"                         E(rase) key; H(elp); Q(uit) [or eof]"<< endl;
                        }
                        
                } while (command != 'Q' && command != 'q');
                
// Erroneously included here - does not belong!                return 0;
   }

   cout << "You created a total of: " << newCount << " nodes" << endl;
   cout << "You deleted a total of: " << deleteCount << " nodes" << endl;
   if (newCount == deleteCount)
                cout << "You are a good steward of nodes!" << endl;
   else if (newCount < deleteCount)
                cout << "WARNING: YOU DELETED SOME NODE(S) MORE THAN ONCE!" << endl;
   else
                cout << "LITTERBUG!" << endl;
                
   return 0;
 }
