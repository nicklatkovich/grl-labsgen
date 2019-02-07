#pragma once

template <class T>
class UnorderedArray {
private:
	unsigned int _min_length;
	unsigned int _data_length = 0;
	unsigned int _length = 0;
	T *_data;
public:
	UnorderedArray(const unsigned int min_length = 32);
	~UnorderedArray();
	T& push(const T value);
	T pop();
	const unsigned int get_length() const;
};
