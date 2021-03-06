/* $Id: list.cpp,v 1.6 2011-03-01 18:24:31-08 dmfrank - $
 * Derek Frank, dmfrank@ucsc.edu
 *
 * NAME
 *    list - implementation file
 */

#include <cstdlib>
#include <exception>
#include <stdexcept>

using namespace std;

#include "util.h"

//
// ctor
//    Basic constructor setting all to 0 or NULL.
//
template<typename T>
list<T>::list (): length (0) {
   front = NULL;
   back = NULL;
}

//
// ctor
//    Copy constructor.
//
template<typename T>
list<T>::list (const list<T> &that): length(0) {
   front = NULL;
   back = NULL;
      
   node* curr = that.back;
   for (int i = 1; i <= that.size(); ++i) {
      this->push ( curr->getElem() );
      curr = curr->getPrev();
   }
}

//
// operator=
//    Sets this list equal to that list.
//
template<typename T>
list<T> &list<T>::operator= (const list<T> &that) {
   if (this == &that) return *this;

   delete this;
   node* curr = that.back;
   for (int i = 1; i <= that.size(); ++i) {
      this->push ( curr->getElem() );
      curr = curr->getPrev();
   }
   return *this;
}

//
// dtor
//    Destructor for the list class.  Deletes every node.
//
template<typename T>
list<T>::~list () {
   if ( this->isempty() ) return;

   node* curr = front;
   while ( !this->isempty() ) {
      front = front->getNext();
      delete curr;
      curr = front;
      --this->length;
   }

   front = NULL;
   back = NULL;
}

//
// size
//    Returns an int of the size of this list.
//
template<typename T>
int list<T>::size () const {
   return this->length;
}

//
// isempty
//    Returns true if this list is empty.  False otherwise.
//
template<typename T>
bool list<T>::isempty () const {
   if (length == 0) return true;
   return false;
}

//
// contains_elem
//    Compares that with every element in this list.  If that element
//    exists, returns true.  Returns false if that element is not in
//    this list.
//
template<typename T>
bool list<T>::contains_elem (const T &that) const {
   if ( this->isempty() ) return false;

   node* curr = front;
   for (int i = 1; i <= this->size(); ++i) {
      if ( curr->getElem() == that ) return true;
      curr = curr->getNext();
   }
   return false;
}

//
// push
//    Inserts the given element at the front.
//
template<typename T>
void list<T>::push (T that) {
   node* N = new node(that);
   if ( this->isempty() ) {
      front = N;
      back = N;
   }else {
      N->setNext (front);
      front->setPrev (N);
      front = N;
   }
   ++length;
}

//
// pop
//    Removes and returns the front.
//
template<typename T>
T list<T>::pop () {
   if ( this.isempty() )
      throw runtime_error ("list error: calling pop() on empty list");

   T elem = front->getElem();
   node* temp = front;
   front = front->getNext();
   front->setPrev (NULL);
   delete temp;
   temp = NULL;
   --length;
   return elem;
}

//
// parent
//    Returns the array index of the parent of the given index.
//
template<typename T>
int list<T>::parent (int i) const {
   if (i < 2)
      throw runtime_error ("list error: calling parent() on root");
   
   return (i/2);
}

//
// left
//    Returns the array index of the left leaf/sub-tree of the given
//    index.
//
template<typename T>
int list<T>::left (int i) const {
   return (2*i);
}

//
// right
//    Returns the array index of the right leaf/sub-tree of the given
//    index.
//
template<typename T>
int list<T>::right (int i) const {
   return ((2*i) + 1);
}

//
// getHeapSize
//    Returns an int of the current heapsize.  For use only with
//    Heapsort algorithm.
//
template<typename T>
int list<T>::getHeapSize () const {
   return this->heapSize;
}

//
// heapify
//    Sub-function to the Heapsort algorithm.  Uses recursion.
//
template<typename T>
void list<T>::heapify (int i) {
   int largest = 0;
   int l = left (i);
   int r = right (i);
   
   // Get value of List[i]
   node* pI = front;
   for (int j = 1; j < i; ++j) {
      pI = pI->getNext();
   }
   // Get value of List[l]
   node* pL = NULL;
   if ( l <= this->getHeapSize() ) {
      pL = front;
      for (int j = 1; j < l; ++j) {
         pL = pL->getNext();
      }
   }
   // Get value of List[r] only if it exists.
   node* pR = NULL;
   if ( r <= this->getHeapSize() ) {
      pR = front;
      for (int j = 1; j < r; ++j) {
         pR = pR->getNext();
      }
   }

   // Compare values of indexes largest and i.  Set smaller.
   if ( l <= this->getHeapSize() &&  pL->getElem() > pI->getElem() ) {
      largest = l;
   } else {
      largest = i;
   }
   
   // Get value of List[largest]
   node* pS = NULL;
   if (largest == l) { pS = pL; }
   else if (largest == r) { pS = pR; }
   else { pS = pI; }

   // Compare values of indexes right and largest.  Set smaller.
   if ( r <= getHeapSize() && pR->getElem() > pS->getElem() ) {
      largest = r;
      pS = pR;
   }

   // If values at indexes largest and i do not match, then exchange
   // node positions.
   if (largest != i) {
      T temp = pI->getElem();
      pI->setElem ( pS->getElem() );
      pS->setElem (temp);
      this->heapify (largest);
   }
}

//
// buildHeap
//    Sub-function to the Heapsort algorithm.  Makes te given list a
//    special binary tree.
//
template<typename T>
void list<T>::buildHeap () {
   heapSize = this->size();
   for (int i = (this->size() / 2); i >= 1; --i) {
      this->heapify (i);
   }
}

//
// sort
//   Sorts this list in ascending order of T.  Uses the Heapsort
//   algorithm
//
template<typename T>
void list<T>::sort () {
   if ( this->isempty() || this->size() == 1 ) return;

   this->buildHeap();
   typename list<T>::node* pI = back;
   for (int i = this->size(); i >= 2; --i) {
      T temp = pI->getElem();
      pI->setElem ( front->getElem() );
      front->setElem (temp);
      --heapSize;
      this->heapify (1);
      pI = pI->getPrev();
   }
}

//
// operator<<
//    Override the << operator to print to std out every element in a
//    list on its own line.  Tells the user the list is empty if it
//    contains no elements.
//
template<typename T>
ostream &operator<< (ostream &out, const list<T> &that) {
   if ( that.isempty() ) return out << endl;

   typename list<T>::node* curr = that.front;
   for (int i = 1; i <= that.size(); ++i) {
      out << curr->getElem() << endl;
      curr = curr->getNext();
   }
      
   return out;
}
