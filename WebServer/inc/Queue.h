/*
 * Queue.h
 *
 *  Created on: 08/06/2015
 *      Author: sergio.ferreira
 */

#ifndef QUEUE_H_
#define QUEUE_H_

#define MAX_ELEMENTS 5
#include <LPC17xx.h>


template<class T>
class Queue {
public:
	Queue();
	virtual void Enqueue(T value);
	virtual T Dequeue();
	uint8_t IsEmpty();
	uint8_t IsFull();

private:
	T data[MAX_ELEMENTS];
protected:
	int i, j;
};

#endif /* QUEUE_H_ */
