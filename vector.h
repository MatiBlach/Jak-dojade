#pragma once 

#include <iostream>

template<typename T>
class Vector {
private:
    T* m_data; 
    size_t m_size;
    size_t m_capacity; //obecny 

public:
    Vector()
        : m_data(nullptr), m_size(0), m_capacity(0)
    {
    }

    Vector(size_t size, const T& value)
        : m_data(new T[size]), m_size(size), m_capacity(size)
    {
        for (size_t i = 0; i < size; i++) {
            m_data[i] = value;
        }
    }

    ~Vector()
    {
        delete[] m_data;
    }

    void reserve(size_t capacity)
    {
        if (capacity > m_capacity) {
            T* new_data = new T[capacity];

            for (size_t i = 0; i < m_size; i++) {
                new_data[i] = m_data[i];
            }

            delete[] m_data;

            m_data = new_data;
            m_capacity = capacity;
        }
    }

    void push_back(const T& value)
    {
        if (m_size >= m_capacity) {
            if (m_capacity == 0)
                reserve(1);
            else
                reserve(m_capacity * 2);
        }

        m_data[m_size] = value;
        m_size++;
    }

    void pop_back()
    {
        if (m_size > 0)
            m_size--;
    }

    size_t size() const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_capacity;
    }

    T& operator[](size_t index)
    {
        return m_data[index];
    }

    T* begin() const {
        return m_data;
    }

    T* end() const {
        return m_data + m_size;
    }

    Vector(const Vector<T>& other) {
        m_size = other.m_size;
        m_capacity = other.m_capacity;
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; i++) {
            if (i < m_capacity) {
                m_data[i] = other.m_data[i];
            }
        }
    }

    Vector<T>& operator=(const Vector<T>& other) {
        if (this != &other) {
            delete[] m_data;
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_data = new T[m_capacity];
            for (size_t i = 0; i < m_size; i++) {
                if (i < m_capacity)
                    m_data[i] = other.m_data[i];
            }
        }
        return *this;
    }
};
