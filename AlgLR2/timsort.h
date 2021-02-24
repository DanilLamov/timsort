#ifndef TIMSORT_H
#define TIMSORT_H

#include <stack>
#include <functional>

//сортировка вставками
template <typename T> inline void insertionsort(T* begin, T* end) {
	T* i = begin + 1, * j = i;
	for (; i < end; ++i, j = i) {
		while (j > begin && *(j - 1) > *j) {
			std::swap(*(j - 1), *j);
			--j;
		}
	}
}

//вычисление размера массива run
inline int getMinrun(size_t size) {
	int r = 0;
	while (size >= 64) {
		r |= size & 1;
		size >>= 1;
	}
	return size + r;
}

template <typename T> inline void merge(T* begin1, T* begin2, T* end2, T* temp) {
	T* iter1 = begin1, * iter2 = begin2, * iter3 = temp;
	size_t cur = 0;
	while (iter1 < begin2 && iter2 < end2) {
		if (*iter1 < *iter2) *(iter3++) = *(iter1++);
		else *(iter3++) = *(iter2++);
	}

	while (iter1 < begin2) *(iter3++) = *(iter1++);
	while (iter2 < end2) *(iter3++) = *(iter2++);

	iter3 = temp;
	for (T* i = begin1; i < end2; ++i)
		*i = *(iter3++);
}

template <typename T> inline void _timsort(T* begin, T* end, T* temp) {
	size_t size = end - begin;
	if (size <= 64) {
		insertionsort(begin, end);
		return;
	}

	int minrun = getMinrun(size);
	T* iter = begin;
	std::stack<std::pair<size_t, T*>> s;
	while (iter < end) {
		T* iter1 = iter, * iter2 = iter;
		//находит последний отсортированный по возрастанию элемент
		while (iter1 < end - 1 && *iter1 <= *(iter1 + 1)) ++iter1;
		//находит послежний отсортированный по убыванию элемент
		while (iter2 < end - 1 && *iter2 >= *(iter2 + 1)) ++iter2;

		if (iter1 >= iter2) {
			iter1 = std::max(iter1, iter + minrun - 1);
			iter1 = std::min(iter1, end - 1);
			insertionsort(iter, iter1 + 1);
			s.push({ iter1 - iter + 1, iter });
			iter = iter1 + 1;
		}
		else {
			iter2 = std::max(iter2, iter + minrun - 1);
			iter2 = std::min(iter2, end - 1);
			std::reverse(iter, iter2 + 1);
			insertionsort(iter, iter2 + 1);
			s.push({ iter2 - iter + 1, iter });
			iter = iter2 + 1;
		}

		while (s.size() >= 3) {
			std::pair<size_t, T*> x = s.top();
			s.pop();
			std::pair<size_t, T*> y = s.top();
			s.pop();
			std::pair<size_t, T*> z = s.top();
			s.pop();
			if (z.first >= x.first + y.first && y.first >= x.first) {
				s.push(z);
				s.push(y);
				s.push(x);
				break;
			}
			else if (z.first >= x.first + y.first) {
				merge(y.second, x.second, x.second + x.first, temp);
				s.push(z);
				s.push({ x.first + y.first, y.second });
			}
			else {
				merge(z.second, y.second, y.second + y.first, temp);
				s.push({ z.first + y.first, z.second });
				s.push(x);
			}
		}
	}

	while (s.size() != 1) {
		std::pair<size_t, T*> x = s.top();
		s.pop();
		std::pair<size_t, T*> y = s.top();
		s.pop();
		if (x.second < y.second) std::swap(x, y);
		merge(y.second, x.second, x.second + x.first, temp);
		s.push({ y.first + x.first, y.second });
	}
}

template <typename T> inline void timsort(T* begin, T* end) {
	T* temp = new T[end - begin];
	_timsort(begin, end, temp);
	delete[] temp;
}

#endif