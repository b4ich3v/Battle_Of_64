#include <iostream>
#pragma once

template <class T>

class Vector 
{
private:

public:

	T* data = nullptr;
	size_t sizeOfData = 0;
	size_t capacity = 8;

	void free();

	void copyFrom(const Vector& other);

	void moveTo(Vector&& other);

	void resize(size_t newCapacity);

public:

	Vector();

	Vector(const T* data, size_t sizeOfData);

	Vector(const Vector& other);

	Vector(Vector&& other) noexcept;

	Vector& operator = (const Vector& other);

	Vector& operator = (Vector&& other) noexcept;

	~Vector();

	const T& operator [] (size_t index) const;

	T& operator [] (size_t index);

        template<class U>

	friend std::ostream& operator << (std::ostream& os, const Vector<U>& v);

        template<class U>

	friend std::istream& operator >> (std::istream& is, Vector<U>& v);

	void pop_back();

	void push_back(const T& element);

	bool empty() const;

	size_t size() const;

};

template <class T>

void Vector<T>::free()
{

    delete[] data;
    data = nullptr;
    sizeOfData = 0;
    capacity = 8;

}

template <class T>

void Vector<T>::copyFrom(const Vector& other)
{

    data = new T[other.capacity];
    sizeOfData = other.sizeOfData;
    capacity = other.capacity;

    for (size_t i = 0; i < sizeOfData; i++) data[i] = other.data[i];
        
}

template <class T>

void Vector<T>::moveTo(Vector&& other)
{

    data = other.data;
    sizeOfData = other.sizeOfData;
    capacity = other.capacity;

    other.data = nullptr;
    other.sizeOfData = 0;
    other.capacity = 8;

}

template <class T>

void Vector<T>::resize(size_t newCapacity) 
{

    if (newCapacity < 1) newCapacity = 1;

    T* newData = new T[newCapacity];

    for (size_t i = 0; i < sizeOfData; i++) newData[i] = std::move(data[i]);

    delete[] data;
    data = newData;
    capacity = newCapacity;

}

template <class T>

Vector<T>::Vector():
    data(new T[capacity]), sizeOfData(0) {}

template <class T>

Vector<T>::Vector(const T* arr, size_t size)
    : data(nullptr), sizeOfData(size), capacity(size > 8 ? size : 8) 
{

    data = new T[capacity];
    for (size_t i = 0; i < sizeOfData; i++) data[i] = arr[i];
        
}

template <class T>

Vector<T>::Vector(const Vector& other) 
{

    copyFrom(other);

}

template <class T>

Vector<T>::Vector(Vector&& other) noexcept
{

    moveTo(std::move(other));

}

template <class T>

Vector<T>& Vector<T>::operator=(const Vector& other)
{

    if (this != &other) 
    {

        free();
        copyFrom(other);

    }

    return *this;

}

template <class T>

Vector<T>& Vector<T>::operator = (Vector&& other) noexcept 
{

    if (this != &other) 
    {

        free();
        moveTo(std::move(other));

    }

    return *this;

}

template <class T>

Vector<T>::~Vector()
{

    delete[] data;

}

template <class T>

const T& Vector<T>::operator[](size_t index) const 
{

    if (index < 0 || index >= sizeOfData) throw std::out_of_range("Vector index out of range");
    return data[index];

}

template <class T>

T& Vector<T>::operator[](size_t index)
{

    if (index < 0 || index >= sizeOfData) throw std::out_of_range("Vector index out of range");
    return data[index];

}

template <class T>

void Vector<T>::pop_back() 
{

    if (empty()) throw std::out_of_range("pop_back() on empty Vector");
    sizeOfData -= 1;
    
}

template <class T>

void Vector<T>::push_back(const T& element)
{

    if (sizeOfData >= capacity) resize(capacity * 2);
    data[sizeOfData++] = element;

}

template <class T>

bool Vector<T>::empty() const 
{

    return sizeOfData == 0;

}

template <class T>

size_t Vector<T>::size() const 
{

    return sizeOfData;

}

template <class T>

std::ostream& operator << (std::ostream& os, const Vector<T>& v) 
{

    os << '[';

    for (size_t i = 0; i < v.sizeOfData; i++)
    {

        os << v.data[i];
        if (i + 1 < v.sizeOfData) os << ", ";

    }

    os << ']';

    return os;

}

template <class T>

std::istream& operator >> (std::istream& is, Vector<T>& v)
{

    size_t newSize;
    if (!(is >> newSize)) return is;

    v.free();

    if (newSize > v.capacity) v.resize(newSize);
    v.sizeOfData = newSize;

    for (size_t i = 0; i < newSize; i++) is >> v.data[i];
     
    return is;

}
