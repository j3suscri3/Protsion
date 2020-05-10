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
        Database        Database(Argument.getDatabase(), Argument.getDirectory(), Argument.getLevel());
	//Open the connection to the database
        if (Database.openDatabase()) {

		//Initialize the configuration with the executable arguments
		Configuration   Configuration(Database, Argument.getDatabase(), Argument.getDirectory(), Argument.getFilePid(), Argument.getLevel(), Argument.getStatus(), Argument.getUser(), Argument.getIpMode(), Argument.getInterface());
		//Load all configurations
		if (Configuration.loadConfiguration(Database)) {

			//Unload all configurations
			if (!Configuration.unloadConfiguration(Database))
				exit(EXIT_FAILURE);

			//Close the connection to the database
			Database.closeDatabase();

			exit(EXIT_SUCCESS);

		} else
			 exit(EXIT_FAILURE);

	} else
		 exit(EXIT_FAILURE);

}
