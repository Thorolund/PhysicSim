#include <iostream>
#include "logger.h"
#include "vec.h"
#include <vector>
int main(int argc, char* argv[]) {
	vec a(5, 10);
	vec b(3, -3);

	std::vector<vec> results;

	results.push_back(a + b);
	results.push_back(a - b);
	results.push_back(a * b);
	results.push_back(a / 5);
	results.push_back(a / 0);
	results.push_back(b / 0);
	results.push_back(b.rnormal());
	results.push_back(b.lnormal());
	results.push_back(b.len1());

	for (vec res : results) {
		std::cout << "(" << res.getx() << ";" << res.gety() << ")" << std::endl;
	}
	return 0;
}
