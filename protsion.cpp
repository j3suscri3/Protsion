#include <iostream>

#include "argument.h"
#include "configuration.h"

int main(int argc, char *argv[]) {

	Argument 	Arg;
	Configuration 	Conf;

	std::cout << "daemonise :" << Conf.getStatus() << std::endl;
	Arg.parsing(argc, argv);

	return 0;

}
