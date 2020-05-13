#include "core/cargument.hpp"
#include "core/cconfiguration.hpp"

int main(int argc, char *argv[]) {

	//Parse the executable arguments
	CArgument Argument(argc, argv);

	//Initialize the configuration with the executable arguments
	CConfiguration Configuration(Argument.getting());

	//Load all configurations
        if(!Configuration.loading())
		exit(EXIT_FAILURE);
/*
        //Unload all configurations
        if(!Configuration.unloading())
        	exit(EXIT_FAILURE);
*/
	exit(EXIT_SUCCESS);

}
