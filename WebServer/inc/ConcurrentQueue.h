/*
 * ConcurrentQueue.h
 *
 *  Created on: 08/06/2015
 *      Author: sergio.ferreira
 */

#ifndef CONCURRENTQUEUE_H_
#define CONCURRENTQUEUE_H_

#include <Queue.h>
#include <Mutex.h>
#include <AutomaticResetEvent.h>

template<class T>
class ConcurrentQueue : public Queue<T> {
public:
	ConcurrentQueue();
	void Enqueue(T value);
	T Dequeue();
private:
	Mutex mutex;
	AutomaticResetEvent resetEvent;
};

#endif /* CONCURRENTQUEUE_H_ */
