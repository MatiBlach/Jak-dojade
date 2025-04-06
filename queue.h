#pragma once

#include <iostream>

template<typename T>
class Queue {
private:
    struct Node {
        T m_data;
        Node* next;
        Node(const T& data) : m_data(data), next(nullptr) {}
    };

    Node* m_head;
    Node* m_tail;
    size_t m_size;

public:
    Queue() : m_head(nullptr), m_tail(nullptr), m_size(0) {}

    ~Queue() {
        while (!empty()) {
            pop();
        }
    }

    void push(const T& data) {
        Node* new_node = new Node(data);
        if (empty()) {
            m_head = new_node;
        }
        else {
            m_tail->next = new_node;
        }
        m_tail = new_node;
        m_size++;
    }

    void pop() {
        if (empty()) {
            return;
        }
        Node* node = m_head;
        m_head = m_head->next;
        delete node;
        m_size--;
    }

    T& front() {
        return m_head->m_data;
    }

    T& back() {
        return m_tail->m_data;
    }

    bool empty() const {
        return m_size == 0;
    }

    size_t size() const {
        return m_size;
    }
};
