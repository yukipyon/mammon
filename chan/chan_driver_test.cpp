#include "desuchan_driver.hpp"
#include "shanachan_driver.hpp"
#include "secchan_driver.hpp"
#include "wakachan_driver.hpp"
#include "../kyukon/kyukon.hpp"
#include <fstream>
#include <signal.h>
#include <iostream>

static chan_driver *driver;

void quit(int) 
{
	std::cout << "SIGINT recieved, finishing surrent operations"
	"and shutting down the system." << std::endl;
	driver->quit();
}

int main(int argc, char **argv) {

	assert(argc > 1);

	std::ifstream ifs;
	ifs.open("proxies");

	assert(ifs.good());

	std::vector<std::pair<std::string, bool>> proxies;
	std::string tmp;

	while(std::getline(ifs, tmp)) {
		
		assert(!tmp.empty());
		proxies.push_back(std::pair<std::string, bool>(tmp, true));
	}

	assert(!proxies.empty());

	kyukon::init(proxies);
	ifs.close();

	ifs.open("desuchan_boards");
	std::vector<std::string> boards;

	while(std::getline(ifs, tmp))
		boards.push_back(tmp);

	if (!strcmp(argv[1], "desu")) {
		driver = new desuchan_driver(std::move(boards));
	} else if (!strcmp(argv[1], "waka")) {
		driver = new wakachan_driver(std::move(boards));
	} else if (!strcmp(argv[1], "shana")) {
		driver = new shanachan_driver(std::move(boards));
	} else if (!strcmp(argv[1], "sec")) {
		driver = new secchan_driver(std::move(boards));
	} else {
		printf("Invalid domain %s\n", argv[1]);
		exit(1);
	}

	signal(SIGINT, &quit);
	while(1);
}
