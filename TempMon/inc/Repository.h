/*
 * Repository.h
 *
 *  Created on: 23/05/2015
 *      Author: sergio.ferreira
 */

#ifndef REPOSITORY_H_
#define REPOSITORY_H_

template<class T>
class Repository {
public:
	Repository();
	void Save(T * data);
	T * GetAll();
};

#endif /* REPOSITORY_H_ */
