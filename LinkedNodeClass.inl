// James Zhu
// 11-30-2024
// Implementation of the LinkedNodeClass as a templated inline file.
// This file contains the implementation of the LinkedNodeClass, 
// which represents a node in a doubly-linked list. Each node 
// contains an integer value, and pointers to the previous and 
// next nodes in the list.

#include<iostream>
#include "LinkedNodeClass.h"

template <class T>
LinkedNodeClass<T>::LinkedNodeClass(LinkedNodeClass<T> *inPrev, 
const T &inVal, LinkedNodeClass<T> *inNext)
    : prevNode(inPrev), nodeVal(inVal), nextNode(inNext) {}

template <class T>
T LinkedNodeClass<T>::getValue() const {
    return nodeVal;
}

template <class T>
LinkedNodeClass<T>* LinkedNodeClass<T>::getNext() const {
    return nextNode;
}

template <class T>
LinkedNodeClass<T>* LinkedNodeClass<T>::getPrev() const {
    return prevNode;
}

template <class T>
void LinkedNodeClass<T>::setNextPointerToNull() {
    nextNode = NULL;
}

template <class T>
void LinkedNodeClass<T>::setPreviousPointerToNull() {
    prevNode = NULL;
}

template <class T>
void LinkedNodeClass<T>::setBeforeAndAfterPointers() {
    if (prevNode) {
        prevNode->nextNode = this;
    }
    if (nextNode) {
        nextNode->prevNode = this;
    }
}
