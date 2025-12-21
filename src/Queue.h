#ifndef QUEUE_H
#define QUEUE_H

#include <stdexcept>
#include <iostream>

template <typename T>
class Queue {
private:
     struct Node {
          T data;
          Node* next;
          Node(T value ) : data(value), next(nullptr){}
};
     Node* frontptr;
     Node* backptr;
     int count;
public:
     Queue();
     ~Queue();
     void enqueue(T item);
     T dequeue();
     T getFront() const;
     bool isEmpty() const;
     int getSize() const;
     void display() const;
};
template <typename T>
Queue<T>::Queue() : frontptr(nullptr), backptr(nullptr), count(0) {}

template <typename T>
Queue<T>::~Queue() {
     while (!isEmpty()) {
          dequeue();
     }
}

template <typename T>
void Queue<T>::enqueue(T item) {
    Node* newNode = new Node(item);
    
    if (isEmpty()) {
        frontptr = newNode;
        backptr = newNode;
    } else {
        backptr->next = newNode;
        backptr = newNode;
    }
    count++;
}


template <typename T>
T Queue<T>::dequeue() {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty! Cannot dequeue.");
    }
    
    Node* temp = frontptr;
    T data = temp->data;

    frontptr = frontptr->next;

    if (frontptr == nullptr) {
        backptr = nullptr;
    }
    
    delete temp;
    count--;
    return data;
}


template <typename T>
T Queue<T>::getFront() const {
    if (isEmpty()) {
        throw std::runtime_error("Queue is empty! Cannot get front.");
    }
    return frontptr->data;
}


template <typename T>
bool Queue<T>::isEmpty() const {
    return frontptr == nullptr;
}


template <typename T>
int Queue<T>::getSize() const {
    return count;
}


template <typename T>
void Queue<T>::display() const {
    if (isEmpty()) {
        std::cout << "Queue is empty." << std::endl;
        return;
    }

    Node* current = frontptr;
    std::cout << "Queue contents (front to back):" << std::endl;
    while (current != nullptr) {
        current->data.display();
        current = current->next;
    }
}

#endif



