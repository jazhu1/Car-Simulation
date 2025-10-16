// James Zhu
// 11-30-2024
// Implementation of the SortedListClass as a templated inline file
// that maintains elements in sorted order. 

#include <iostream>
#include "SortedListClass.h"

// Default Constructor
template <class T>
SortedListClass<T>::SortedListClass() : head(NULL), tail(NULL) {}

// Copy constructor
template <class T>
SortedListClass<T>::SortedListClass(const SortedListClass<T> &rhs)
 : head(NULL), 
tail(NULL) {
    LinkedNodeClass<T> *current = rhs.head;
    while (current != NULL) {
        insertValue(current->getValue());
        current = current->getNext();
    }
}

// Destructor
template <class T>
SortedListClass<T>::~SortedListClass() {
    clear();
}

// Assignment operator
template <class T>
SortedListClass<T>& SortedListClass<T>::operator=(
    const SortedListClass<T> &rhs) {
    if (this != &rhs) {
        clear();
        LinkedNodeClass<T> *current = rhs.head;
        while (current != NULL) {
            insertValue(current->getValue());
            current = current->getNext();
        }
    }
    return *this;
}

// Clears the list completely
template <class T>
void SortedListClass<T>::clear() {
    T temp;
    while (removeFront(temp));
}

template <class T>
void SortedListClass<T>::insertValue(const T &valToInsert) {
    // Case 1: Empty list
    if (head == NULL) {
        LinkedNodeClass<T>* insertNode = 
        new LinkedNodeClass<T>(NULL, valToInsert, NULL);
        tail = head = insertNode;
    }
    else {
        LinkedNodeClass<T>* current = head;

        // Traverse to find the correct insertion point
        while (current != NULL && current->getValue() <= valToInsert) {
            current = current->getNext();
        }

        // Case 2: Insert at the head
        if (current == head) {
            LinkedNodeClass<T>* insertNode = new LinkedNodeClass<T>(NULL, 
            valToInsert, head);
            insertNode->setBeforeAndAfterPointers();
            head = insertNode;
        }
        // Case 3: Insert at the tail
        else if (current == NULL) {
            LinkedNodeClass<T>* insertNode = new LinkedNodeClass<T>(tail, 
            valToInsert, NULL);
            insertNode->setBeforeAndAfterPointers();
            tail = insertNode;
        }
        // Case 4: Insert in the middle
        else {
            LinkedNodeClass<T>* insertNode = 
            new LinkedNodeClass<T>(current->getPrev(), valToInsert, 
            current);
            insertNode->setBeforeAndAfterPointers();
        }
    }
}

// Prints the list from head to tail
template <class T>
void SortedListClass<T>::printForward() const {
    LinkedNodeClass<T> *current = head;
    std::cout << "Forward List Contents Follow:" << std::endl;
    while (current != NULL) {
        std::cout << "  " << current->getValue() << std::endl;
        current = current->getNext();
    }
    std::cout << "End Of List Contents" << std::endl;
}

// Prints the list from tail to head
template <class T>
void SortedListClass<T>::printBackward() const {
    LinkedNodeClass<T> *current = tail;
    std::cout << "Backward List Contents Follow:" << std::endl;
    while (current != NULL) {
        std::cout << "  " << current->getValue() << std::endl;
        current = current->getPrev();
    }
    std::cout << "End Of List Contents" << std::endl;
}

// Removes the front node and retrieves its value
template <class T>
bool SortedListClass<T>::removeFront(T &theVal) {
    if (head == NULL) {
        return false;
    }
    LinkedNodeClass<T> *temp = head;
    theVal = head->getValue();
    head = head->getNext();
    delete temp;

    if (head != NULL) {
        head->setPreviousPointerToNull();
    } else {
        tail = NULL; 
    }
    return true;
}

// Removes the last node and retrieves its value
template <class T>
bool SortedListClass<T>::removeLast(T &theVal) {
    if (tail == NULL) {
        return false;
    }
    LinkedNodeClass<T> *temp = tail;
    theVal = tail->getValue();
    tail = tail->getPrev();
    delete temp;

    if (tail != NULL) {
        tail->setNextPointerToNull();
    } else {
        head = NULL; 
    }
    return true;
}

// Gets the number of elements in the list
template <class T>
int SortedListClass<T>::getNumElems() const {
    int count = 0;
    LinkedNodeClass<T> *current = head;
    while (current != NULL) {
        count++;
        current = current->getNext();
    }
    return count;
}

// Retrieves the value at a specific index
template <class T>
bool SortedListClass<T>::getElemAtIndex(
    const int index, T &outVal) const {
    if (index < 0) {
        return false;
    }
    int count = 0;
    LinkedNodeClass<T> *current = head;
    while (current != NULL && count < index) {
        count++;
        current = current->getNext();
    }
    if (current == NULL) {
        return false;
    }
    outVal = current->getValue();
    return true;
}