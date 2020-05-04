	#include <iostream>
#include <string>

#include "common/argument.hpp"
#include "common/configuration.hpp"
#include "common/database.hpp"

using namespace std;

int main(int argc, char *argv[]) {

	Argument 	Argument;

	//Parse the executable arguments
	Argument.parse(argc, argv);

	//Initialize the database connection
        Database        Data(Argument.getDatabase(), Argument.getDirectory(), Argument.getLevel());
	//Open the connection to the database
        if(Data.openDatabase()) {

		Data.logDatabase("test");

		//Initialize the configuration with the executable arguments
		Configuration   Conf(Data, Argument.getDatabase(), Argument.getDirectory(), Argument.getFilePid(), Argument.getLevel(), Argument.getStatus(), Argument.getUser());
		//Load all configurations
		if(Conf.loadConfiguration(Data)) {

			Data.logDatabase("test");

			//Close the connection to the database
			Data.closeDatabase();

			exit(EXIT_SUCCESS);

		} else
			 exit(EXIT_FAILURE);

	} else
		 exit(EXIT_FAILURE);

}
