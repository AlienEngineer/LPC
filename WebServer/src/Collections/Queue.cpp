/*
 * Queue.cpp
 *
 *  Created on: 08/06/2015
 *      Author: sergio.ferreira
 */

#include <Queue.h>

template<class T>
Queue<T>::Queue() {
	this->i = this->j = 0;
}

template<class T>
void Queue<T>::Enqueue(T value){
	this->data[this->i] = value;
	this->i++;

	if (this->i > MAX_ELEMENTS) {
		this->i = 0;
	}
}

template<class T>
T Queue<T>::Dequeue() {
	if (this->IsEmpty()) {
		return (T)0;
	}

	T value = this->data[this->j];
	this->j++;

	if (this->j > MAX_ELEMENTS) {
		this->j = 0;
	}
	return value;
}

template<class T>
uint8_t Queue<T>::IsEmpty() {
	return (this->i == this->j);
}

template<class T>
uint8_t Queue<T>::IsFull() {
	return (this->i == this->j+1);
}

template class Queue<uint8_t>;
