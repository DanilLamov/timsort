#include <iostream>
#include <cstdlib>
#include <fstream>
#include <ctime>

#include "timsort.h"

int main() {
	srand(time(NULL));

	int a[1000];
	for (int i = 0; i < 1000; ++i)
		a[i] = rand() % 10000;

	std::ofstream fout;
	fout.open("test.txt");
	for (int i = 0; i < 1000; ++i) {
		fout << a[i] << ' ';
		if (i % 10 == 9) fout << '\n';
	}
	fout.close();

	timsort(a, a + 1000);

	fout.open("answer.txt");
	for (int i = 0; i < 1000; ++i) {
		fout << a[i] << ' ';
		if (i % 10 == 9) fout << '\n';
	}
	fout.close();

	return 0;
}