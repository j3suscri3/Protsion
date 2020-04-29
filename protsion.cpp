#include <iostream>
#include <string>

#include "argument.h"
#include "configuration.h"

using namespace std;

int main(int argc, char *argv[]) {
	Argument 	Arg;
	//Parse the executable arguments
	Arg.parse(argc, argv);
	//Initialize the configuration with the executable arguments
	Configuration   Conf(Arg.getDirectory(), Arg.getFileConf(), Arg.getFilePid(), Arg.getLevel(), Arg.getStatus(), Arg.getUser());
	//Load all configurations
	Conf.loadConfiguration();
	cout << "fileConf : " << Conf.getFileConf() << endl;
	cout << "filePid : " << Conf.getFilePid() << endl;
	cout << "directory : " << Conf.getDirectory() << endl;

	

	return 0;
}
