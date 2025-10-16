// James Zhu
// 11/30/2024
// Implementation of the FIFOQueueClass template class. This class
// represents a FIFO queue using a doubly linked list.

#include <iostream>
#include "FIFOQueueClass.h"
#include "LinkedNodeClass.h"

// Constructor
template <class T>
FIFOQueueClass<T>::FIFOQueueClass() : head(NULL), tail(NULL) {}

// Destructor
template <class T>
FIFOQueueClass<T>::~FIFOQueueClass() {
    clear();
}

// Enqueue a new item
template <class T>
void FIFOQueueClass<T>::enqueue(const T &newItem) {
    // Create a new node to hold the value
    LinkedNodeClass<T>* newNode = new LinkedNodeClass<T>(tail, 
    newItem, NULL);

    // If the queue is empty, set head to the new node
    if (head == NULL) {
        head = newNode;
    } 
    // link the new node to the current tail
    else {
        newNode->setBeforeAndAfterPointers();
    }
    // Update the tail to point to the new node
    tail = newNode;
}

// Dequeue and item
template <class T>
bool FIFOQueueClass<T>::dequeue(T &outItem) {
    if (!head) {
        return false; 
    }
    outItem = head->getValue();
    LinkedNodeClass<T> *temp = head;
    head = head->getNext();
    if (head) {
        head->setPreviousPointerToNull();
    } else {
        tail = NULL; 
    }
    delete temp;
    return true;
}

// Print the queue contents
template <class T>
void FIFOQueueClass<T>::print() const {
    LinkedNodeClass<T> *current = head;
    while (current) {
        std::cout << current->getValue() << " ";
        current = current->getNext();
    }
    std::cout << std::endl;
}

// Get the number of elements
template <class T>
int FIFOQueueClass<T>::getNumElems() const {
    int count = 0;
    LinkedNodeClass<T> *current = head;
    while (current) {
        count++;
        current = current->getNext();
    }
    return count;
}

// Clear the queue
template <class T>
void FIFOQueueClass<T>::clear() {
    T temp;
    while (dequeue(temp));
}