#include "core/cargument.hpp"
#include "core/cconfiguration.hpp"
#include "core/cdatabase.hpp"

int main(int argc, char *argv[]) {

	//Parse the executable arguments
	CArgument Arguments(argc, argv);
	//Initialize the configuration with the executable arguments
	CConfiguration Configurations(Arguments.getting());
	//Initialize the database configuration
	CDatabase Database(Configurations.getting("Database"));

	//Opening the database session
	if(Database.opening()) {

		//Load all configurations
        	if(!Configurations.loading(Database))
                	exit(EXIT_FAILURE);
/*
        	//Unload all configurations
        	if(!Configurations.unloading())
                	exit(EXIT_FAILURE);
*/
	}

	exit(EXIT_SUCCESS);

}
