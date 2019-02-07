#include "UnorderedArray.hpp"
#include "Point.hpp"
#include <cstdlib>

template <class T>
UnorderedArray<T>::UnorderedArray(const unsigned int min_length)
	: _min_length(min_length)
	, _data_length(min_length)
{
	_data = new T[this->_min_length];
}

template <class T>
UnorderedArray<T>::~UnorderedArray()
{
	delete this->_data;
}

template <class T>
T& UnorderedArray<T>::push(const T value)
{
	if (this->_length == this->_min_length) {
		this->_data_length *= 2;
		T* new_data = new T[this->_data_length];
		for (unsigned int i = 0; i < this->_length; i++) {
			new_data[i] = this->_data[i];
		}
		delete this->_data;
		this->_data = new_data;
	}
	this->_data[this->_length] = value;
	this->_length++;
}

template <class T>
T UnorderedArray<T>::pop()
{
	unsigned int index = rand() % this->_length;
	T result = this->_data[index];
	this->_length--;
	this->_data[index] = this->_data[this->_length];
	if (this->_length * 2 == this->_data_length && this->_data_length > this->_min_length) {
		this->_data_length /= 2;
		T* new_data = new T[this->_data_length];
		for (unsigned int i = 0; i < this->_length; i++) {
			new_data[i] = this->_data[i];
		}
		delete this->_data;
		this->_data = new_data;
	}
	return result;
}

template <class T>
const unsigned int UnorderedArray<T>::get_length() const
{
	return this->_length;
}

template class UnorderedArray<unsigned int>;
template class UnorderedArray<Point>;
