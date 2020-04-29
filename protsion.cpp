#include <iostream>
#include <string>

#include <limits.h>
#include <unistd.h>

#include "argument.h"
#include "configuration.h"

using namespace std;

int main(int argc, char *argv[]) {
	Argument 	Arg;
	Arg.parse(argc, argv);

	Configuration   Conf(Arg.getDirectory(), Arg.getFileConf(), Arg.getFilePid(), Arg.getLevel(), Arg.getStatus(), Arg.getUser());
	cout << "fileConf : " << Conf.getFileConf() << endl;
	cout << "filePid : " << Conf.getFilePid() << endl;

	char result[ PATH_MAX ];
  	ssize_t count = readlink( "/proc/self/exe", result, PATH_MAX );
	cout << std::string( result, (count > 0) ? count : 0 ) << endl;

	return 0;
}
