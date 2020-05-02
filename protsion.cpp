#include <iostream>
#include <string>

#include "common/argument.hpp"
#include "common/configuration.hpp"
#include "common/database.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	Argument 	Arg;

	//Parse the executable arguments
	Arg.parse(argc, argv);

	//Initialize the configuration with the executable arguments
	Configuration   Conf(Arg.getDirectory(), Arg.getFilePid(), Arg.getLevel(), Arg.getStatus(), Arg.getUser());

	//Load all configurations
	Conf.loadConfiguration();

	return 0;
}
