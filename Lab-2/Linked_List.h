#pragma once

template <class T>

class LinkedList {
public:

    LinkedList();
    LinkedList(const T* items, int count);
    LinkedList(const LinkedList<T>& other);
    LinkedList<T>& operator=(const LinkedList<T>& other);
    ~LinkedList();

    T GetFirst() const;
    T GetLast() const;
    T Get(int index) const;

    int GetLength() const;
    void Append(const T& item);
    void Prepend(const T& item);
    void InsertAt(const T& item, int index);

    LinkedList<T> Concat(const LinkedList<T>& list) const;
    LinkedList<T> GetSubList(int startIndex, int endIndex) const;

private:
    struct Node;
    Node* head_;
    Node* tail_;
    int length_;

    Node* GetNode(int index) const;
    void CheckIndex(int index) const;
    void Clear();
};

#include "Linked_List.tpp"

