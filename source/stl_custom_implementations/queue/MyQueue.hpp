#include <iostream>
#pragma once

template <class T>

class MyQueue
{
private:

	struct Node
	{
	public:

		T data;
		Node* next = nullptr;
		Node* prev = nullptr;

		explicit Node(const T& data);

		explicit Node(const T& data, Node* next, Node* prev);

	};

	Node* head = nullptr;
	Node* tail = nullptr;
	size_t countOfNodes = 0;

	void free();

	void copyFrom(const MyQueue& other);

	void moveTo(MyQueue&& other);

public:

	MyQueue();

	MyQueue(const MyQueue& other);

	MyQueue(MyQueue&& other) noexcept;

	MyQueue& operator = (const MyQueue& other);

	MyQueue& operator = (MyQueue&& other) noexcept;

	~MyQueue();

	const T& front() const;

	void pop();

	void push(const T& element);

	bool empty() const;

	size_t size() const;

};

template<class T>

MyQueue<T>::Node::Node(const T& data) :
	data(data), next(nullptr), prev(nullptr) {}

template<class T>

MyQueue<T>::Node::Node(const T& data, Node* next, Node* prev) :
	data(data), next(next), prev(prev) {}

template <class T>

void MyQueue<T>::free()
{

	Node* current = head;

	while (current != nullptr)
	{

		Node* nextNode = current->next;
		delete current;
		current = nextNode;

	}

	head = nullptr;
	tail = nullptr;
	countOfNodes = 0;

}

template <class T>

void MyQueue<T>::copyFrom(const MyQueue<T>& other)
{

	free();

	Node* prev = nullptr;
	Node* current = other.head;

	head = nullptr;
	tail = nullptr;

	while (current != nullptr)
	{

		Node* newNode = new Node(current->data);
		newNode->prev = prev;
		newNode->next = nullptr;

		if (prev) prev->next = newNode;
		else head = newNode;

		prev = newNode;
		current = current->next;

	}

	tail = prev;
	countOfNodes = other.countOfNodes;

}

template <class T>

void MyQueue<T>::moveTo(MyQueue<T>&& other)
{

	this->head = other.head;
	this->tail = other.tail;
	this->countOfNodes = other.countOfNodes;

	other.head = nullptr;
	other.tail = nullptr;
	other.countOfNodes = 0;

}

template <class T>

MyQueue<T>::MyQueue()
{

	this->head = nullptr;
	this->tail = nullptr;
	this->countOfNodes = 0;

}

template <class T>

MyQueue<T>::MyQueue(const MyQueue<T>& other)
{

	copyFrom(other);

}

template <class T>

MyQueue<T>::MyQueue(MyQueue&& other) noexcept
{

	moveTo(std::move(other));

}

template <class T>

MyQueue<T>& MyQueue<T>::operator = (const MyQueue<T>& other)
{

	if (this != &other)
	{

		free();
		copyFrom(other);

	}

	return *this;

}

template <class T>

MyQueue<T>& MyQueue<T>::operator = (MyQueue<T>&& other) noexcept
{

	if (this != &other)
	{

		free();
		moveTo(std::move(other));

	}

	return *this;

}

template <class T>

MyQueue<T>::~MyQueue()
{

	free();

}

template <class T>

const T& MyQueue<T>::front() const
{

	if (empty()) throw std::runtime_error("Queue is empty");
	return head->data;

}

template <class T>

void MyQueue<T>::pop()
{

	if (empty()) throw std::runtime_error("Queue is empty");

	Node* iter = head;
	head = head->next;

	if (head == nullptr) tail = nullptr;
	else head->prev = nullptr;

	delete iter;
	countOfNodes -= 1;

}

template <class T>

void MyQueue<T>::push(const T& element)
{

	Node* newNode = new Node(element, nullptr, tail);

	if (tail == nullptr) head = newNode;
	else tail->next = newNode;

	tail = newNode;
	countOfNodes += 1;

}

template <class T>

bool MyQueue<T>::empty() const
{

	return countOfNodes == 0;

}

template <class T>

size_t MyQueue<T>::size() const
{

	return countOfNodes;

}
