#pragma once
#include <cstddef>
template<class T>
class Node{
    private:
        T data;
        int priority;
        Node<T> *next;
    public:
       Node(void);
       Node(T &data);
       Node(T &data, int prio);
       void set_data(T &new_data);
       void set_next(Node<T> *next_node);
       void set_priority(int prio);
       T get_data(void);
       Node<T> *get_next(void);
       int get_prio(void);
};

template<class T>
Node<T>::Node()
{
    data = 0;
    priority = 0;
    next = NULL;
}

template<class T>
Node<T>::Node(T &new_data)
{
    next = NULL;
    priority = 0;
    data = new_data;
}

template<class T>
Node<T>::Node(T &new_data, int prio)
{
    next = NULL;
    data = new_data;
    priority = prio;
}

template<class T>
void Node<T>::set_data(T &new_data)
{
    data = new_data;
}

template<class T>
T Node<T>::get_data()
{
    return data;
}

template<class T>
void Node<T>::set_next(Node<T> *next_node_ptr)
{
   next = next_node_ptr;
}


template<class T>
Node<T> *Node<T>::get_next()
{
    return next;
}

template<class T>
int Node<T>::get_prio(void)
{
    return priority;
}

template<class T>
void Node<T>::set_priority(int prio)
{
    priority = prio;
}