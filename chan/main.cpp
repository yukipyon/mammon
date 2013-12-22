#include "../parser.hpp"
#include <iostream>

int main(int argc, char **argv) {

	if (argc < 3) {
		std::cout << "Input xpath and attribute." << std::endl;
		return 0;
	}

	const char *filename = "input.html";
	//const char *xpath = "//span[@class='reflink']/span/a";
	const char *xpath = argv[1];
	const char *att = argv[2];

	std::vector<std::string> res = parser::parse_path(filename, xpath, att);

	for (const auto &a : res) {
		if (!a.empty())
			std::cout << a << std::endl;
	}
}
