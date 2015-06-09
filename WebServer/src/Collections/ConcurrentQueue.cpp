/*
 * ConcurrentQueue.cpp
 *
 *  Created on: 08/06/2015
 *      Author: sergio.ferreira
 */

#include <ConcurrentQueue.h>
#include <Mutex.h>

template<class T>
ConcurrentQueue<T>::ConcurrentQueue() {
	this->i = this->j = 0;
}

template<class T>
void ConcurrentQueue<T>::Enqueue(T value) {
	mutex.Enter();

	Queue<T>::Enqueue(value);
	resetEvent.Signal();

	mutex.Leave();
}

template<class T>
T ConcurrentQueue<T>::Dequeue() {
	T value;

	mutex.Enter();

	while (this->IsEmpty())
	{
		mutex.Leave();
		resetEvent.Wait();
		mutex.Enter();
	}

	value = Queue<T>::Dequeue();

	mutex.Leave();

	return value;
}

template class ConcurrentQueue<uint8_t>;
